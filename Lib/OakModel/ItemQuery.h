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

class ItemQuery;
typedef std::unique_ptr<ItemQuery> ItemQueryUPtr;

// =============================================================================
// Class definition
// =============================================================================
class ItemQuery
{
public:
    ItemQuery();
    ItemQuery(const ItemQuery& copy);
    ItemQuery(ItemQuery&& move);

    virtual ~ItemQuery();

    int count(const Item &refItem);
    std::vector<Item> itemList(const Item &refItem);

    bool hasChildQuery() const;
    const ItemQuery *childQuery() const;

    const NodeDef * nodeDef(const NodeDef *nDef) const;

    virtual bool canInsertItem(const Item &refItem, int &index) const;
    virtual Item insertItem(const Item &refItem, int index) const;

    virtual bool canCloneItem(const Item &refItem, int &index, const Item &cloneItem) const;
    virtual Item cloneItem(const Item &refItem, int &index, const Item &cloneItem) const;

    virtual bool canMoveItem(const Item &refItem, int &index, const Item &moveItem) const;
    virtual Item moveItem(const Item &refItem, int &index, const Item &moveItem) const;

    virtual bool canRemoveItem(const Item &refItem, int index) const;
    virtual bool removeItem(Item &refItem, int index);

protected:
    void addChildQuery(ItemQueryUPtr query);

    virtual Item first(const Item &refItem) const;
    virtual Item last(const Item &refItem) const;
    virtual Item next(const Item &refItem, const Item &cItem) const;
    virtual Item previous(const Item &refItem, const Item &cItem) const;

    virtual const NodeDef * _nodeDef(const NodeDef *nDef) const;

protected:
    ItemQueryUPtr m_childQueryUPtr = ItemQueryUPtr();


    // Iterator navigation implementation
public:
    class Iterator {

    public:
        Iterator(const ItemQuery & query, const Item *refItem = nullptr);

        virtual ~Iterator();

        bool isValid() const;
        bool next();
        bool previous();

        const Item &item() const;

        bool first(const Item &refItem);
        bool last(const Item &refItem);

    protected:
        const ItemQuery *m_query;
        const Item *m_refItem;
        Item m_currentItem;

        Iterator *m_childIterator;

        friend class ItemQuery;
    };
    typedef std::unique_ptr<Iterator> IteratorUPtr;

    IteratorUPtr iterator(const Item &refItem) const;
//    IteratorUPtr begin(const Item &refItem) const;
//    IteratorUPtr rBegin(const Item &refItem) const;


    friend class QueryBuilder;
};

} // namespace Oak::Model

