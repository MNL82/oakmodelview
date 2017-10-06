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

class TreeDataModel;

// =============================================================================
// Class definition
// =============================================================================
class Item
{
public:
    Item();
    Item(const NodeDefinition* definition, Node node, const TreeDataModel* model = nullptr);
    Item(const Item& copy);
    Item(Item&& move);

    Item& operator=(const Item& copy);
    Item& operator=(Item&& move);

    bool operator==(const Item& _item) const;
    bool operator!=(const Item& _item) const;

    const ItemValue& operator()(VariantCRef valueId) const;
    Item operator[](int index) const;


    bool isNull() const;
    bool isDefinitionNull() const;
    bool isNodeNull() const;

    Node node() const { return m_node; }
    const NodeDefinition* definition() const { return m_definition; }
    const TreeDataModel* model() const;

    void setCurrent();

    std::vector<VariantCRef> valueIdList() const;
    std::vector<VariantCRef> childPrimaryKeyList() const;

    // ************* ItemValue access *************
    int valueCount() const;
    int valueIndex(const ItemValue& value) const;
    const ItemValue& valueAt(int index) const;

    const ItemValue& value(VariantCRef valueId) const;

    typedef std::vector<ItemValue>::const_iterator ValueIterator;

    ValueIterator valueBegin() const;
    ValueIterator valueEnd() const;

    bool hasKey() const;
    const ItemValue& valueKey() const;

    bool hasDerivedId() const;
    const ItemValue& valueDerivedId() const;

    // ************* Child Item access *************
    int childCount() const;
    int childCount(VariantCRef primaryKey) const;

    int childIndex(const Item &refChild) const;
    int childIndex(VariantCRef primaryKey, const Item &refChild) const;

    Item childAt(int index) const;
    Item childAt(VariantCRef primaryKey, int index) const;

    Item firstChild() const;
    Item firstChild(VariantCRef primaryKey) const;

    Item lastChild() const;
    Item lastChild(VariantCRef primaryKey) const;

    Item nextChild(const Item& refChild) const;
    Item nextChild(VariantCRef primaryKey, const Item& refChild) const;

    Item previousChild(const Item& refChild) const;
    Item previousChild(VariantCRef primaryKey, const Item& refChild) const;

    // ************* Parent Item access *************
    Item parent() const;

    // ************* Child Item edit *************
    //bool canInsertChild(int &index) const;
    bool canInsertChild(VariantCRef primaryKey, int &index) const;

    //Item insertChild(int &index) const;
    Item insertChild(VariantCRef primaryKey, int &index) const;

    bool canCloneChild(int &index, Item cloneItem) const;
    bool canCloneChild(VariantCRef primaryKey, int &index, Item cloneItem) const;

    Item cloneChild(int &index, Item cloneItem) const;
    Item cloneChild(VariantCRef primaryKey, int &index, Item cloneItem) const;

    bool canMoveChild(int &index, Item moveItem) const;
    bool canMoveChild(VariantCRef primaryKey, int &index, Item moveItem) const;

    Item moveChild(int &index, Item moveItem) const;
    Item moveChild(VariantCRef primaryKey, int &index, Item moveItem) const;

    bool canRemoveChild(int index) const;
    bool canRemoveChild(VariantCRef primaryKey, int index) const;

    bool removeChild(int index) const;
    bool removeChild(VariantCRef primaryKey, int index) const;

protected:
    inline void initItemValueList() const;

protected:
    const NodeDefinition* m_definition;
    Node m_node;
    const TreeDataModel* m_model;

    mutable std::vector<ItemValue> m_itemValueList;
};

} // namespace Model
} // namespace Oak
