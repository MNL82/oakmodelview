/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#pragma once

#include "Node.h"
#include "NodeDefinition.h"
#include "ItemValue.h"

namespace Oak {
namespace Model {

class OakModel;

// =============================================================================
// Class definition
// =============================================================================
class Item
{
public:
    Item();
    Item(const NodeDefinition* definition, Node node, const OakModel* model = nullptr);
    Item(const Item& copy);
    Item(Item&& move);

    Item& operator=(const Item& copy);
    Item& operator=(Item&& move);

    bool operator==(const Item& _item) const;
    bool operator!=(const Item& _item) const;

    template<typename T>
    const ItemValue& operator()(const T &valueId) const;

    Item operator[](int index) const;

    bool isNull() const;
    bool isDefinitionNull() const;
    bool isNodeNull() const;

    Node node() const { return m_node; }
    const NodeDefinition* definition() const { return m_definition; }
    const OakModel* model() const;

    void setCurrent();

//    std::vector<VariantCRef> valueIdList() const;
//    std::vector<VariantCRef> childPrimaryKeyList() const;

    // ************* ItemValue access *************
    int valueCount() const;
    int valueIndex(const ItemValue& value) const;
    const ItemValue& valueAt(int index) const;

    template<typename T>
    const ItemValue& value(const T &valueId) const;

    typedef std::vector<ItemValue>::const_iterator ValueIterator;

    ValueIterator valueBegin() const;
    ValueIterator valueEnd() const;

    bool hasKey() const;
    const ItemValue& valueKey() const;

    bool hasDerivedId() const;
    const ItemValue& valueDerivedId() const;

    // ************* Child Item access *************
    int childCount() const;
    template<typename T>
    int childCount(const T &primaryKey) const;
    int childIndex(const Item &refChild) const;
    template<typename T>
    int childIndex(const T &primaryKey, const Item &refChild) const;
    Item childAt(int index) const;
    template<typename T>
    Item childAt(const T &primaryKey, int index) const;
    Item firstChild() const;
    template<typename T>
    Item firstChild(const T &primaryKey) const;

    Item lastChild() const;
    template<typename T>
    Item lastChild(const T &primaryKey) const;

    Item nextChild(const Item& refChild) const;
    template<typename T>
    Item nextChild(const T &primaryKey, const Item& refChild) const;

    Item previousChild(const Item& refChild) const;
    template<typename T>
    Item previousChild(const T &primaryKey, const Item& refChild) const;

    // ************* Parent Item access *************
    Item parent() const;

    // ************* Child Item edit *************
    //bool canInsertChild(int &index) const;
    template<typename T>
    bool canInsertChild(const T &primaryKey, int &index) const;

    //Item insertChild(int &index) const;
    template<typename T>
    Item insertChild(const T &primaryKey, int &index) const;

    bool canCloneChild(int &index, Item cloneItem) const;
    template<typename T>
    bool canCloneChild(const T &primaryKey, int &index, Item cloneItem) const;

    Item cloneChild(int &index, Item cloneItem) const;
    template<typename T>
    Item cloneChild(const T &primaryKey, int &index, Item cloneItem) const;

    bool canMoveChild(int &index, Item moveItem) const;
    template<typename T>
    bool canMoveChild(const T &primaryKey, int &index, Item moveItem) const;

    Item moveChild(int &index, Item moveItem) const;
    template<typename T>
    Item moveChild(const T &primaryKey, int &index, Item moveItem) const;

    bool canRemoveChild(int index) const;
    template<typename T>
    bool canRemoveChild(const T &primaryKey, int index) const;

    bool removeChild(int index) const;
    template<typename T>
    bool removeChild(const T &primaryKey, int index) const;

protected:
    inline void initItemValueList() const;

    void onItemInserted(const Item& parentItem, int index) const;
    void onItemMoved(const Item& sourceParentItem, int sourceIndex, const Item& targetParentItem, int targetIndex) const;
    void onItemCloned(const Item& sourceParentItem, int sourceIndex, const Item& targetParentItem, int targetIndex) const;
    void onItemRemoved(const Item& parentItem, int index) const;

protected:
    const NodeDefinition* m_definition;
    Node m_node;
    const OakModel* m_model;

    mutable std::vector<ItemValue> m_itemValueList;
};

// =============================================================================
// (public)
template<typename T>
const ItemValue& Item::operator()(const T &valueId) const
{
    return value(valueId);
}

// =============================================================================
// (public)
template<typename T>
const ItemValue& Item::value(const T &valueId) const
{
    initItemValueList();
    for (const ItemValue& iv: m_itemValueList)
    {
        if (iv.valueId() == valueId) {
            return iv;
        }
    }
    return ItemValue::emptyItemValue();
}

// =============================================================================
// (public)
template<typename T>
int Item::childCount(const T &primaryKey) const
{
    return m_definition->container(primaryKey).nodeCount(m_node);
}

// =============================================================================
// (public)
template<typename T>
int Item::childIndex(const T &primaryKey, const Item &refChild) const
{
    return m_definition->container(primaryKey).nodeIndex(m_node, refChild.node());
}

// =============================================================================
// (public)
template<typename T>
Item Item::childAt(const T &primaryKey, int index) const
{
    const NodeDefinition* childeNodeDefinition;
    Node childNode = m_definition->container(primaryKey).node(m_node, index, &childeNodeDefinition);
    return Item(childeNodeDefinition, childNode, m_model);
}

// =============================================================================
// (public)
template<typename T>
Item Item::firstChild(const T &primaryKey) const
{
    const NodeDefinition* childNodeDefinition;
    Node childNode = m_definition->container(primaryKey).firstNode(m_node, &childNodeDefinition);
    return Item(childNodeDefinition, childNode, m_model);
}

// =============================================================================
// (public)
template<typename T>
Item Item::lastChild(const T &primaryKey) const
{
    const NodeDefinition* childNodeDefinition;
    Node childNode = m_definition->container(primaryKey).lastNode(m_node, &childNodeDefinition);
    return Item(childNodeDefinition, childNode, m_model);
}

// =============================================================================
// (public)
template<typename T>
Item Item::nextChild(const T &primaryKey, const Item& refChild) const
{
    const NodeDefinition* childNodeDefinition;
    Node childNode = m_definition->container(primaryKey).nextNode(refChild.m_node, &childNodeDefinition);
    return Item(childNodeDefinition, childNode, m_model);
}

// =============================================================================
// (public)
template<typename T>
Item Item::previousChild(const T &primaryKey, const Item& refChild) const
{
    const NodeDefinition* childNodeDefinition;
    Node childNode = m_definition->container(primaryKey).previousNode(refChild.m_node, &childNodeDefinition);
    return Item(childNodeDefinition, childNode, m_model);
}

// =============================================================================
// (public)
template<typename T>
bool Item::canInsertChild(const T &primaryKey, int &index) const
{
    return m_definition->container(primaryKey).canInsertNode(m_node, index);
}

// =============================================================================
// (public)
template<typename T>
Item Item::insertChild(const T &primaryKey, int &index) const
{
    const auto& container = m_definition->container(primaryKey);
    Item childItem(container.containerDefinition(), container.insertNode(m_node, index), m_model);
    if (m_model && !childItem.isNull()) {
        onItemInserted(*this, childIndex(childItem));
    }
    return std::move(childItem);
}

// =============================================================================
// (public)
template<typename T>
bool Item::canCloneChild(const T &primaryKey, int &index, Item cloneItem) const
{
    return m_definition->container(primaryKey).canCloneNode(m_node, index, cloneItem.m_node);
}

// =============================================================================
// (public)
template<typename T>
Item Item::cloneChild(const T &primaryKey, int &index, Item cloneItem) const
{
    if (m_model) {
        // Cash data needed to notify change
        Item sourceParentItem = cloneItem.parent();
        int sourceIndex = sourceParentItem.childIndex(cloneItem);
        // Perform the cloneing
        Item item = Item(cloneItem.m_definition, m_definition->container(primaryKey).cloneNode(m_node, index, cloneItem.m_node), m_model);
        // Notify everyone if cloning did not fail
        if (!item.isNull()) {
            onItemCloned(sourceParentItem, sourceIndex, *this, childIndex(item));
        }
        return std::move(item);
    } else {
        return Item(cloneItem.m_definition, m_definition->container(primaryKey).cloneNode(m_node, index, cloneItem.m_node), m_model);
    }
}

// =============================================================================
// (public)
template<typename T>
bool Item::canMoveChild(const T &primaryKey, int &index, Item moveItem) const
{
    return m_definition->container(primaryKey).canMoveNode(m_node, index, moveItem.m_node);
}

// =============================================================================
// (public)
template<typename T>
Item Item::moveChild(const T &primaryKey, int &index, Item moveItem) const
{
    if (m_model) {
        // Cash data needed to notify change
        Item sourceParentItem = moveItem.parent();
        int sourceIndex = sourceParentItem.childIndex(moveItem);
        // Perform the cloneing
        Item item = Item(moveItem.m_definition, m_definition->container(primaryKey).moveNode(m_node, index, moveItem.m_node), m_model);
        // Notify everyone if cloning did not fail
        if (!item.isNull()) {
            onItemMoved(sourceParentItem, sourceIndex, *this, childIndex(item));
        }
        return std::move(item);
    } else {
        return Item(moveItem.m_definition, m_definition->container(primaryKey).moveNode(m_node, index, moveItem.m_node), m_model);
    }
}

// =============================================================================
// (public)
template<typename T>
bool Item::canRemoveChild(const T &primaryKey, int index) const
{
    return m_definition->container(primaryKey).canRemoveNode(m_node, index);
}

// =============================================================================
// (public)
template<typename T>
bool Item::removeChild(const T &primaryKey, int index) const
{
    int index2 = childIndex(childAt(primaryKey, index));
    if (m_definition->container(primaryKey).removeNode(m_node, index)) {
        if (m_model) {
            onItemRemoved(*this, index2);
        }
        return true;
    }
    return false;
}

} // namespace Model
} // namespace Oak
