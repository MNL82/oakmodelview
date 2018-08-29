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

#include "ValueQuery.h"

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
    void addValueQuery(ValueQuerySPtr valueQuery);

    void reset(const Item &refItem);
    bool moveNext();

    std::string columnName() const;

    void getValue(int index, UnionValue value) const;

    const ItemValue &itemValue(int index);

    template<typename T>
    T value(int index);

    int count(const Item &item);

protected:
    ItemQueryUPtr m_itemQuery;
    std::vector<ValueQuerySPtr> m_valueList; // Should be a valueRef (to be entryRef)
};

// =============================================================================
// (public)
template<typename T>
T TableQuery::value(int index)
{
    assert(m_itemQuery);
    assert(index >= 0);
    assert(index < static_cast<int>(m_valueList.size()));
    return m_valueList[index]->value<T>(m_itemQuery->current(), 0);
}

} // namespace Model
} // namespace Oak

