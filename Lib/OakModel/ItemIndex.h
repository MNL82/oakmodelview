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

#include "Item.h"


namespace Oak::Model {

class ItemIndex;
typedef std::unique_ptr<ItemIndex> ItemIndexUPtr;

// =============================================================================
// Class definition
// =============================================================================
class ItemIndex
{
public:
    ItemIndex();
    ItemIndex(int index);
    ItemIndex(const std::string &name, int index);
    ItemIndex(const ItemIndex &itemIndex);
    ~ItemIndex();

    bool isNull() const;
    bool isNamed(bool recursive = false) const;
    bool isUnnamed(bool recursive = false) const;

    bool equal(const ItemIndex &itemIndex) const;
    int depthWhereEqual(const ItemIndex &itemIndex) const;

    const std::string &name() const;
    int index() const;

    // Returns how many levels down the ItemIndex points
    int depth() const;

    bool contains(const ItemIndex &itemIndex) const;

    Item item(const Item &rootItem, int depth = -1) const;
    Item itemParent(const Item &rootItem) const;

    bool hasChildItemIndex() const;

    const ItemIndex &childItemIndex(int depth = 1) const;
    ItemIndex &childItemIndex(int depth = 1);
    const ItemIndex &lastItemIndex() const;
    ItemIndex &lastItemIndex();

    static ItemIndex &emptyItemIndex();

    static ItemIndexUPtr create(const Item &_item, bool namedIndex = true);

    // Manual creation of ItemIndex
    void setChildItemIndex(ItemIndex *itemIndex);

    // ************* Convert index to grouped of seperate format *************
    int convertIndexToUnnamed(const Item &_item) const;
    int convertIndexToNamed(const Item &_item, std::string &name) const;

protected:
    std::string m_name;
    int m_index;

    ItemIndex *m_childIndex;

    static ItemIndex s_itemIndex;
};

} // namespace Oak::Model

