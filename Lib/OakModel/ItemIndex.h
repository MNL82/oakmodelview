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

namespace Oak {
namespace Model {

class ItemIndex;
typedef std::unique_ptr<ItemIndex> ItemIndexUPtr;

// =============================================================================
// Class definition
// =============================================================================
class ItemIndex
{
public:
    ItemIndex();
    ~ItemIndex();

    const std::string &name() const;
    int index() const;

    int depth() const;

    bool contains(const ItemIndex &itemIndex);

    Item item(const Item &rootItem, int depth = -1) const;

    const ItemIndex &childItemIndex(int depth) const;

    static ItemIndexUPtr create(const Item &_item, bool namedIndex = true);

protected:
    std::string m_name;
    int m_index;

    ItemIndex *m_childIndex;
};

} // namespace Model
} // namespace Oak
