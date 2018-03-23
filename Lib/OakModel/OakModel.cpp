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
    return isDefNull() || isNodeNull();
}

// =============================================================================
// (public)
bool OakModel::isDefNull() const
{
    return m_rootItem.isDefNull();
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
    if (m_rootItem.isDefNull()) {
        return false;
    }

    if (backendType == Node::Type::XML) {
        m_xmlDoc.clear();
        Node documentNode(m_xmlDoc.appendChild(m_rootItem.def()->tagName()));
        m_rootItem.def()->onNodeInserted(documentNode);
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
    return m_rootItem.def()->name();
}

// =============================================================================
// (public)
const NodeDef* OakModel::rootNodeDef() const
{
    return m_rootItem.def();
}

// =============================================================================
// (public)
void OakModel::setRootNodeDef(NodeDefSPtr def)
{
    m_def = def; // Saved only to keep the definition alive (Smart Pointer)
    setRootNodeDef(def.get());
}

// =============================================================================
// (public)
void OakModel::setRootNodeDef(const NodeDef *def)
{
    if (m_rootItem.def() != def) {
        Node currentNode = m_currentItem.node();
        // Clear the current item before it becomes invalid
        setCurrentItem(Item());

        // Update the root definition and trigger event
        m_rootItem = Item(def, m_rootItem.node(), this);
        notifier_rootNodeDefChanged.trigger();

        // Set the current item to the same node as before
        // Otherwise sent the root node
        const NodeDef * currentItemDef = findNodeDef(currentNode);
        if (currentItemDef) {
            setCurrentItem(Item(currentItemDef, currentNode, this));
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
        m_rootItem = Item(m_rootItem.def(), node, this);

        notifier_rootNodeChanged.trigger();
        setCurrentItem(rootItem());
    }
}

// =============================================================================
// (public)
void OakModel::setRootItem(const Item& item)
{
    setRootNodeDef(item.def());
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
const NodeDef* OakModel::findNodeDef(Node node) const
{
    if (m_rootItem.isDefNull() || node.isNull()) { return nullptr; }

    // Check if the root definition match
    const NodeDef* def = m_rootItem.def()->getDerivedAny(node);
    if (def) { return def; }

    std::list<const NodeDef*> defList;
    std::vector<const NodeDef*> ignoreList;
    defList.push_back(m_rootItem.def());

    // Recursive check if any child definition match the node
    const NodeDef * currentDef;
    while (!defList.empty()) {
        currentDef = defList.front();
        defList.remove(currentDef);

        // Check if on of the child definitions match
        def = currentDef->containerGroup().containerDef(node);
        if (def) { return def; }
        ignoreList.push_back(currentDef);

        // Add child definitions to the list
        auto cList = currentDef->containerList();
        for(const ContainerDef* c: cList)
        {
            // Only add the definition if it is not in the ignore list
            //  to avoid checking an definition twice and enter an infinete loop
            if (std::find(ignoreList.begin(), ignoreList.end(), c->containerDef()) == ignoreList.end()) {
                defList.push_back(c->containerDef());
            }
        }
    }
    return nullptr;
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
    if (item.def()->derivedIdValueDefIndex() == valueIndex) {
        const NodeDef* def = findNodeDef(item.node());
        assert(def);
        Item newItem(def, item.node(), this);

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
