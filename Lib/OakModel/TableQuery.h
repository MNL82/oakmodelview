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

#include "EntryQuery.h"

namespace Oak {
namespace Model {

class TableQuery;
typedef std::shared_ptr<TableQuery> TableQuerySPtr;
typedef std::weak_ptr<TableQuery> TableQueryWPtr;

// =============================================================================
// Class definition
// =============================================================================
class TableQuery
{
public:
    TableQuery();
    TableQuery(ItemQueryUPtr itemQuery);

    void setItemQuery(ItemQueryUPtr itemQuery);

    int columnCount() const;
    void addValueQuery(EntryQuerySPtr valueQuery);

    int count(const Item &item) const;

    ItemQuery &itemQuery();
    const ItemQuery &itemQuery() const;

protected:
    ItemQueryUPtr m_itemQuery;
    std::vector<EntryQuerySPtr> m_entryList; // Should be a valueRef (to be entryRef)

public:
    // Iterator navigation implementation
    class Iterator : public ItemQuery::Iterator {

    public:
        Iterator(const TableQuery &tableQuery, const Item *refItem = nullptr);

        virtual ~Iterator() override;

        const Entry &entry(int index) const;
        void getValue(int index, UnionValue value) const;

        template<typename T>
        T value(int index);

    protected:
        const TableQuery *m_tableQuery;
        std::vector<EntryQuery::Iterator*> m_entryIteratorList;

        friend class TableQuery;
    };
    typedef std::unique_ptr<Iterator> IteratorUPtr;

    IteratorUPtr iterator(const Item &refItem) const;
//    IteratorUPtr begin(const Item &refItem) const;
//    IteratorUPtr rBegin(const Item &refItem) const;
};

// =============================================================================
// (public)
template<typename T>
T TableQuery::Iterator::value(int index)
{
    const Entry &e = entry(index);
    if (e.isNull()) { T(); }
    return e.value<T>();
}

} // namespace Model
} // namespace Oak

