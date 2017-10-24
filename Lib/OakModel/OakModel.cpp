/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "OakModel.h"

#include <list>
#include <QDebug>

namespace Oak {
namespace Model {

// =============================================================================
// (public)
OakModel::OakModel()
{
    //qDebug() << "OakModel()";
}

// =============================================================================
// (public)
OakModel::~OakModel()
{
    //qDebug() << "~OakModel()";
    notifier_destroyed.trigger();
}

// =============================================================================
// (public)
bool OakModel::isNull() const
{
    return isDefinitionNull() || isNodeNull();
}

// =============================================================================
// (public)
bool OakModel::isDefinitionNull() const
{
    return m_rootItem.isDefinitionNull();
}

// =============================================================================
// (public)
bool OakModel::isNodeNull() const
{
    return m_rootItem.isNodeNull();
}

// =============================================================================
// (public)
bool OakModel::createNewRootDocument(Node::Type backendType, bool setAsCurrent)
{
    if (m_rootItem.isDefinitionNull()) {
        return false;
    }

    if (backendType == Node::Type::XML) {
        m_xmlDoc.clear();
        Node documentNode(m_xmlDoc.appendChild(m_rootItem.definition()->tagName()));
        m_rootItem.definition()->onNodeInserted(documentNode);
        setXMLRootNode(documentNode, setAsCurrent);
        return true;
    }

    assert(false);
    return false;
}

// =============================================================================
// (public)
void OakModel::clearRoot()
{
    setRootNode(Node());
}

// =============================================================================
// (public)
const Item &OakModel::rootItem() const
{
    return m_rootItem;
}

// =============================================================================
// (public)
const std::string &OakModel::rootDefName() const
{
    return m_rootItem.definition()->name();
}

// =============================================================================
// (public)
const NodeDefinition* OakModel::rootNodeDefinition() const
{
    return m_rootItem.definition();
}

// =============================================================================
// (public)
void OakModel::setRootNodeDefinition(NodeDefinitionSPtr definition)
{
    m_definition = definition; // Saved only to keep the definition alive (Smart Pointer)
    setRootNodeDefinition(definition.get());
}

// =============================================================================
// (public)
void OakModel::setRootNodeDefinition(const NodeDefinition *definition)
{
    if (m_rootItem.definition() != definition) {
        Node currentNode = m_currentItem.node();
        // Clear the current item before it becomes invalid
        setCurrentItem(Item());

        // Update the root definition and trigger event
        m_rootItem = Item(definition, m_rootItem.node(), this);
        notifier_rootNodeDefinitionChanged.trigger();

        // Set the current item to the same node as before
        // Otherwise sent the root node
        const NodeDefinition * currentItemDefinition = findNodeDefinition(currentNode);
        if (currentItemDefinition) {
            setCurrentItem(Item(currentItemDefinition, currentNode, this));
        } else {
            setCurrentItem(rootItem());
        }
    }
}

// =============================================================================
// (public)
void OakModel::setRootNode(Node node)
{
    if (m_rootItem.node() != node) {
        setCurrentItem(Item());
        m_rootItem = Item(m_rootItem.definition(), node, this);

        notifier_rootNodeChanged.trigger();
        setCurrentItem(rootItem());
    }
}

// =============================================================================
// (public)
void OakModel::setRootItem(const Item& item)
{
    setRootNodeDefinition(item.definition());
    setRootNode(item.node());
}

// =============================================================================
// (public)
const Item& OakModel::currentItem() const
{
    return m_currentItem;
}

// =============================================================================
// (public)
void OakModel::setCurrentItem(Item item) const
{
    if (m_currentItem != item) {
        m_currentItem = item;
        notifier_currentItemChanged.trigger();
    }
}

#ifdef XML_BACKEND
// =============================================================================
// (public)
void OakModel::setXMLRootNode(Node rootNode, bool setAsCurrent)
{
    m_XMLRootNode = rootNode;

    if (setAsCurrent) { setRootNode(m_XMLRootNode); }
}

// =============================================================================
// (public)
bool OakModel::loadXMLRootNode(const std::string& filePath, bool setAsCurrent)
{
    if (!m_xmlDoc.load(filePath)) { return false; }

    m_xmlDocFilePath = filePath;

    setXMLRootNode(m_xmlDoc.documentElement(), setAsCurrent);

    return true;
}

// =============================================================================
// (public)
bool OakModel::saveXMLRootNode(const std::string& filePath)
{
    if (m_xmlDoc.isNull()) {
        // Implement this
        return false;
    }
    if (filePath.empty()) {
        if (m_xmlDocFilePath.empty()) { return false; }
        return m_xmlDoc.save(m_xmlDocFilePath);
    } else {
        if (m_xmlDoc.save(filePath)) {
            m_xmlDocFilePath = filePath;
            return true;
        }
        return false;
    }
}

// =============================================================================
// (public)
const NodeDefinition* OakModel::findNodeDefinition(Node node) const
{
    if (m_rootItem.isDefinitionNull() || node.isNull()) { return 0; }

    // Check if the root definition match
    const NodeDefinition* definition = m_rootItem.definition()->getDerivedAny(node);
    if (definition) { return definition; }

    std::list<const NodeDefinition*> definitionList;
    std::vector<const NodeDefinition*> ignoreList;
    definitionList.push_back(m_rootItem.definition());

    // Recursive check if any child definition match the node
    const NodeDefinition * currentDefinition;
    while (!definitionList.empty()) {
        currentDefinition = definitionList.front();
        definitionList.remove(currentDefinition);

        // Check if on of the child definitions match
        definition = currentDefinition->containerGroup().containerDefinition(node);
        if (definition) { return definition; }
        ignoreList.push_back(currentDefinition);

        // Add child definitions to the list
        auto cList = currentDefinition->containerList();
        for(const ContainerDefinition* c: cList)
        {
            // Only add the definition if it is not in the ignore list
            //  to avoid checking an definition twice and enter an infinete loop
            if (std::find(ignoreList.begin(), ignoreList.end(), c->containerDefinition()) == ignoreList.end()) {
                definitionList.push_back(c->containerDefinition());
            }
        }
    }
    return 0;
}

// =============================================================================
// (protected)
void OakModel::onItemInserted(const Item &parentItem, int index) const
{
    notifier_itemInserted.trigger(parentItem, index);
}

// =============================================================================
// (protected)
void OakModel::onItemMoved(const Item &sourceParentItem, int sourceIndex, const Item &targetParentItem, int targetIndex) const
{
    // Notify the view
    notifier_itemMoved.trigger(sourceParentItem, sourceIndex, targetParentItem, targetIndex);

    Item movedItem = targetParentItem.childAt(targetIndex);
    if (sourceParentItem == targetParentItem) {
        m_currentItem = targetParentItem.childAt(targetIndex);
        notifier_currentItemChanged.trigger();
        return;
    }

    // TODO: Check if this works when drag and drop move is implemented
    Item item = m_currentItem;
    while (!item.isNull()) {
        if (movedItem == item) {
            // The current item has been moved
            notifier_currentItemChanged.trigger();
            return;
        }
        item = item.parent();
    }
}

// =============================================================================
// (protected)
void OakModel::onItemCloned(const Item &sourceParentItem, int sourceIndex, const Item &targetParentItem, int targetIndex) const
{
    // Notify the view
    notifier_itemCloned.trigger(sourceParentItem, sourceIndex, targetParentItem, targetIndex);

    // Change the current item to the clone if it was the one cloned
    if (sourceParentItem.childAt(sourceIndex) == m_currentItem) {
        setCurrentItem(targetParentItem.childAt(targetIndex));
    }
}

// =============================================================================
// (protected)
void OakModel::onItemRemoved(const Item &parentItem, int index) const
{
    // Notify the view
    notifier_itemRemoved.trigger(parentItem, index);

    if (m_currentItem.isNull()) { setCurrentItem(m_rootItem); }

    // The current Item needs to be updated if it has been removed
    // Check if is has been removed
    Item item = m_currentItem;
    Item pItem = item.parent();
    while(!pItem.isNull()) {
        // The parent of the removed item has been updated to not contain the removed item
        if (pItem.childIndex(item) < 0) {
            // The current item has been removed
            // Now it finds the best candidate to replace it

            // First the next sibling
            item = parentItem.childAt(index);
            if (!item.isNull()) {
                setCurrentItem(item);
                return;
            }
            // Secound the previous sibling
            item = parentItem.childAt(index-1);
            if (!item.isNull()) {
                setCurrentItem(item);
                return;
            }
            // Thrid the parent
            setCurrentItem(parentItem);
            return;
        }
        item = pItem;
        pItem = item.parent();
    }
}

// =============================================================================
// (protected)
void OakModel::onItemValueChanged(const Item &item, int valueIndex) const
{
    if (item.definition()->derivedIdValueDefIndex() == valueIndex) {
        const NodeDefinition* definition = findNodeDefinition(item.node());
        assert(definition);
        Item newItem(definition, item.node(), this);

        notifier_itemValueChanged.trigger(newItem, valueIndex);
        if (item == m_currentItem) {
            setCurrentItem(newItem);
        }
    } else {
        notifier_itemValueChanged.trigger(item, valueIndex);
    }
}

#endif // XML_BACKEND

} // namespace Model
} // namespace Oak
