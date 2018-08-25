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

class ItemQuery;
typedef std::unique_ptr<ItemQuery> ItemQueryUPtr;

// =============================================================================
// Class definition
// =============================================================================
class ItemQuery
{
public:
    ItemQuery();
    virtual ~ItemQuery();

    void reset(const Item &refItem);
    virtual bool moveNext();
    const Item &current(bool recursive = true) const;

    int count(const Item &item);
    std::vector<Item> itemList(const Item &item);

    ItemQuery *childQuery();

protected:
    void add(ItemQueryUPtr query);
    virtual bool moveCurrentNext() = 0;

protected:
    Item m_refItem;
    Item m_currentItem;
    ItemQueryUPtr m_childQueryUPtr = ItemQueryUPtr();

    friend class QueryBuilder;
};

} // namespace Model
} // namespace Oak
