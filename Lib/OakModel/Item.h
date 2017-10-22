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

    const ItemValue& operator()(const std::string &name) const;

    Item operator[](int index) const;

    bool isNull() const;
    bool isDefinitionNull() const;
    bool isNodeNull() const;

    void clear();

    Node node() const { return m_node; }
    const NodeDefinition* definition() const { return m_definition; }
    const OakModel* model() const;

    void setCurrent();

    std::vector<std::string> valueNameList() const;
    std::vector<std::string> childNameList() const;

    // ************* ItemValue access *************
    int valueCount() const;
    int valueIndex(const ItemValue& value) const;
    const ItemValue& valueAt(int index) const;

    const ItemValue& value(const std::string &valueName) const;

    typedef std::vector<ItemValue>::const_iterator ValueIterator;

    ValueIterator valueBegin() const;
    ValueIterator valueEnd() const;

    bool hasKey() const;
    const ItemValue& valueKey() const;

    bool hasDerivedId() const;
    const ItemValue& valueDerivedId() const;

    // ************* Child Item access *************
    int childCount() const;
    int childCount(const std::string &name) const;

    int childIndex(const Item &refChild) const;
    int childIndex(const std::string &name, const Item &refChild) const;

    Item childAt(int index) const;
    Item childAt(const std::string &name, int index) const;

    Item firstChild() const;
    Item firstChild(const std::string &name) const;

    Item lastChild() const;
    Item lastChild(const std::string &name) const;

    Item nextChild(const Item& refChild) const;
    Item nextChild(const std::string &name, const Item& refChild) const;

    Item previousChild(const Item& refChild) const;
    Item previousChild(const std::string &name, const Item& refChild) const;

    // ************* Parent Item access *************
    Item parent() const;

    // ************* Child Item edit *************
    bool canInsertChild(const std::string &name, int &index) const;

    Item insertChild(const std::string &name, int &index) const;

    bool canCloneChild(int &index, Item cloneItem) const;
    bool canCloneChild(const std::string &name, int &index, Item cloneItem) const;

    Item cloneChild(int &index, Item cloneItem) const;
    Item cloneChild(const std::string &name, int &index, Item cloneItem) const;

    bool canMoveChild(int &index, Item moveItem) const;
    bool canMoveChild(const std::string &name, int &index, Item moveItem) const;

    Item moveChild(int &index, Item moveItem) const;
    Item moveChild(const std::string &name, int &index, Item moveItem) const;

    bool canRemoveChild(int index) const;
    bool canRemoveChild(const std::string &name, int index) const;

    bool removeChild(int index) const;
    bool removeChild(const std::string &name, int index) const;

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

} // namespace Model
} // namespace Oak
