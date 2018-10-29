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

#include "../ServiceFunctions/Trace.h"
#include "ItemServiceFunctions.h"
#include "ObserverInterface.h"

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

    ASSERT(false);
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

        clearObservers();

        // Update the root definition and trigger event
        m_rootItem = Item(def, m_rootItem.node(), this);

        createObservers();

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
const ItemIndex &OakModel::currentItemIndex() const
{
    if (m_currentItemIndex) {
        return *m_currentItemIndex.get();
    } else {
        return ItemIndex::emptyItemIndex();
    }
}

// =============================================================================
// (public)
void OakModel::setCurrentItem(Item item, bool forceUpdate) const
{
    if (m_currentItem != item || forceUpdate) {
        m_currentItem = item;
        m_currentItemIndex = ItemIndex::create(m_currentItem);
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

#endif // XML_BACKEND

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
// (public)
ItemIndexUPtr OakModel::convertItemIndexToNamed(const ItemIndex &itemIndex) const
{
    ASSERT(!m_rootItem.isNull());
    ASSERT(!itemIndex.isNull());

    ItemIndex * newRootItemIndex = nullptr;
    const ItemIndex * sItemIndex = &itemIndex;
    ItemIndex *tItemIndex;
    Item item = m_rootItem;

    while (true) {
        if (sItemIndex->isNamed(true)) { // The all of the remaining item index is already named
            tItemIndex = new ItemIndex(*sItemIndex); // Copy the item index
            break;
        }
        if (sItemIndex->isNamed()) {
            item = sItemIndex->item(item);
            tItemIndex = new ItemIndex(sItemIndex->name(), sItemIndex->index());
        } else {
            // Item index needs to be converted from unnamed to named
            int index = sItemIndex->index();
            Item childItem = sItemIndex->item(item, 1);
            std::string name = childItem.def()->name();
            if (item.def()->containerCount() > 1) {
                // Index needs to be updated if more than one container
                index = item.childIndex(name, childItem);
            }
            item = childItem;
            tItemIndex = new ItemIndex(name, index);
        }

        if (newRootItemIndex == nullptr) {
            newRootItemIndex = tItemIndex;
        } else {
            newRootItemIndex->lastItemIndex().setChildItemIndex(tItemIndex);
        }

        if (!sItemIndex->hasChildItemIndex()) {
            // Conversion is complete
            break;
        }
        sItemIndex = &sItemIndex->childItemIndex();
    }

    return ItemIndexUPtr(newRootItemIndex);
}

// =============================================================================
// (public)
ItemIndexUPtr OakModel::convertItemIndexToUnnamed(const ItemIndex &itemIndex) const
{
    ASSERT(!m_rootItem.isNull());
    ASSERT(!itemIndex.isNull());

    ItemIndex * newRootItemIndex = nullptr;
    const ItemIndex * sItemIndex = &itemIndex;
    ItemIndex *tItemIndex;
    Item item = m_rootItem;

    while (true) {
        if (sItemIndex->isUnnamed(true)) { // The all of the remaining item index is already unnamed
            tItemIndex = new ItemIndex(*sItemIndex); // Copy the item index
            break;
        }
        if (sItemIndex->isUnnamed()) {
            item = sItemIndex->item(item);
            tItemIndex = new ItemIndex(sItemIndex->index());
        } else {
            // Item index needs to be converted from named to unnamed
            int index = sItemIndex->index();
            Item childItem = sItemIndex->item(item, 1);
            if (item.def()->containerCount() > 1) {
                // Index needs to be updated if more than one container
                index = item.childIndex(childItem);
            }
            item = childItem;
            tItemIndex = new ItemIndex(index);
        }

        if (newRootItemIndex == nullptr) {
            newRootItemIndex = tItemIndex;
        } else {
            newRootItemIndex->lastItemIndex().setChildItemIndex(tItemIndex);
        }

        if (!sItemIndex->hasChildItemIndex()) {
            // Conversion is complete
            break;
        }
        sItemIndex = &sItemIndex->childItemIndex();
    }

    return ItemIndexUPtr(newRootItemIndex);
}

// =============================================================================
// (protected)
void OakModel::onItemRemoveBefore(const ItemIndex &itemIndex) const
{
    notifier_itemRemoveBefore.trigger(itemIndex);
}

// =============================================================================
// (protected)
void OakModel::onItemInserteAfter(const ItemIndex &itemIndex) const
{
    notifier_itemInserteAfter.trigger(itemIndex);
}

// =============================================================================
// (protected)
void OakModel::onItemMoveAfter(const ItemIndex &sourceItemIndex, const ItemIndex &targetItemIndex) const
{
    notifier_itemMoveAfter.trigger(sourceItemIndex, targetItemIndex);

    // Check if the current item have moved and update it if so
    ItemIndexUPtr currentItemIndex = ItemIndex::create(m_currentItem);
    if (!currentItemIndex) {
        setCurrentItem(targetItemIndex.item(m_rootItem));
    } else if (!m_currentItemIndex->equal(*currentItemIndex.get())) {
        setCurrentItem(m_currentItem, true);
    }
}

// =============================================================================
// (protected)
void OakModel::onItemMoveBefore(const ItemIndex &sourceItemIndex, const ItemIndex &targetItemIndex) const
{
    notifier_itemMoveBefore.trigger(sourceItemIndex, targetItemIndex);
}

// =============================================================================
// (protected)
void OakModel::onItemCloneAfter(const ItemIndex &sourceItemIndex, const ItemIndex &targetItemIndex) const
{
    notifier_itemCloneAfter.trigger(sourceItemIndex, targetItemIndex);

    // Change the current item to the clone if it was the one cloned
    if (m_currentItemIndex->equal(sourceItemIndex)) {
        setCurrentItem(targetItemIndex.item(m_rootItem));
    }
}

// =============================================================================
// (protected)
void OakModel::onItemRemoveAfter(const ItemIndex &itemIndex) const
{
    // Notify the view
    notifier_itemRemoveAfter.trigger(itemIndex);

    // Check if the current item is removed and update it if so
    if (m_currentItem.isNull()) { return; }

    if (m_currentItemIndex->contains(itemIndex)) {
        // The current item is no longer valid
        if (m_currentItemIndex->depth() == itemIndex.depth()) {
            // The deleted item is the current item
            Item parentItem = itemIndex.itemParent(m_rootItem);
            const ItemIndex &lastItemIndex = itemIndex.lastItemIndex();
            int count = parentItem.childCount(lastItemIndex.name());
            if (count > lastItemIndex.index()) {
                // Set the next sibling as the current item
                setCurrentItem(parentItem.childAt(lastItemIndex.name(), lastItemIndex.index()));
            } else if (count > 0) {
                // Set the previous sibling as the current item
                setCurrentItem(parentItem.childAt(lastItemIndex.name(), count-1));
            } else {
                // Set the parent item as the current item
                setCurrentItem(parentItem);
            }
        } else {
            // The parent item is removed so clear the current item
            setCurrentItem(Item());
        }

        return;
    }
}

// =============================================================================
// (protected)
void OakModel::onEntryChangeAfter(const ItemIndex &itemIndex, int valueIndex) const
{
    notifier_entryChangeAfter.trigger(itemIndex, valueIndex);


    // TODO: Check for references to the changed value

}

// =============================================================================
// (protected)
void OakModel::onEntryTypeChangeAfter(const ItemIndex &itemIndex) const
{
    Item item = itemIndex.item(m_rootItem);
    const NodeDef* def = findNodeDef(item.node());
    ASSERT(def);
    Item newItem(def, item.node(), this);

    notifier_entryTypeChangeAfter.trigger(itemIndex);
    if (item == m_currentItem) {
        setCurrentItem(newItem);
    }
}

// =============================================================================
// (protected)
void OakModel::onEntryKeyChangeAfter(const ItemIndex &itemIndex) const
{
    notifier_entryKeyChangeAfter.trigger(itemIndex);
}

// =============================================================================
// (public)
void OakModel::createObservers()
{
    // Insert code to create new observers here (Do not connect)

    /******************* Create Option Observers ******************/
    // Find all option queries in the node definition tree
    std::vector<NodeEntryQuery> queryList;
    std::vector<NodeEntryQuery> queryExcludedList;
    findOptionQueries(m_rootItem.def(), queryList, queryExcludedList, true);

    //

    for (ObserverInterfaceUPtr &observer: m_observerList)
    {
        observer->connect();
    }
}

// =============================================================================
// (public)
void OakModel::clearObservers()
{
    for (ObserverInterfaceUPtr &observer: m_observerList)
    {
        observer->disconnect();
    }
    m_observerList.clear();
}

} // namespace Model
} // namespace Oak
