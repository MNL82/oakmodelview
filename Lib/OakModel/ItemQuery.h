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

    int count(const Item &refItem);
    std::vector<Item> itemList(const Item &refItem);

    bool hasChildQuery() const;

    ItemQuery *childQuery();

protected:
    void add(ItemQueryUPtr query);

    virtual Item first(const Item &refItem) const;
    virtual Item last(const Item &refItem) const;
    virtual Item next(const Item &refItem, const Item &cItem) const;
    virtual Item previous(const Item &refItem, const Item &cItem) const;

protected:
    Item m_refItem;
    Item m_currentItem;
    ItemQueryUPtr m_childQueryUPtr = ItemQueryUPtr();


    // Iterator navigation implementation
public:
    class Iterator {

    public:
        Iterator(const ItemQuery & query);

        virtual ~Iterator();

        bool isValid() const;
//        Iterator &operator++(int);
//        Iterator &operator--(int);
        bool next();
        bool previous();

        const Item & item() const;

        bool first(const Item &refItem);
        bool last(const Item &refItem);

    protected:
        const ItemQuery *m_query;
        Item m_refItem;
        Item m_currentItem;

        Iterator *m_childIterator;

        friend class ItemQuery;
    };
    typedef std::unique_ptr<Iterator> IteratorUPtr;

    IteratorUPtr begin(const Item &refItem) const;
    IteratorUPtr rBegin(const Item &refItem) const;


    friend class QueryBuilder;
};

} // namespace Model
} // namespace Oak
