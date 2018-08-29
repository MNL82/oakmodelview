/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "TableQuery.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
TableQuery::TableQuery()
{

}

// =============================================================================
// (public)
TableQuery::TableQuery(ItemQueryUPtr itemQuery)
{
    if (itemQuery) {
        m_itemQuery = std::move(itemQuery);
    }
}

// =============================================================================
// (public)
void TableQuery::setItemQuery(ItemQueryUPtr itemQuery)
{
    m_itemQuery = std::move(itemQuery);
}

// =============================================================================
// (public)
int TableQuery::columnCount() const
{
    return static_cast<int>(m_valueList.size());
}

// =============================================================================
// (public)
void TableQuery::addValueQuery(ValueQuerySPtr valueQuery)
{
    m_valueList.push_back(valueQuery);
}

// =============================================================================
// (public)
void TableQuery::reset(const Item &refItem)
{
    assert(m_itemQuery);
    m_itemQuery->reset(refItem);
}

// =============================================================================
// (public)
bool TableQuery::moveNext()
{
    assert(m_itemQuery);
    return m_itemQuery->moveNext();
}

// =============================================================================
// (public)
std::string TableQuery::columnName() const
{
    return std::string();
}

// =============================================================================
// (public)
void TableQuery::getValue(int index, UnionValue value) const
{
    assert(m_itemQuery);
    assert(index >= 0);
    assert(index < static_cast<int>(m_valueList.size()));
    m_valueList[static_cast<size_t>(index)]->getValue(m_itemQuery->current(), 0, value);
}

// =============================================================================
// (public)
const ItemValue &TableQuery::itemValue(int index)
{
    assert(m_itemQuery);
    assert(index >= 0);
    assert(index < static_cast<int>(m_valueList.size()));
    return m_valueList[static_cast<size_t>(index)]->itemValue(m_itemQuery->current(), 0);
}

// =============================================================================
// (public)
int TableQuery::count(const Item &item)
{
    assert(m_itemQuery);
    return m_itemQuery->count(item);
}

} // namespace Model
} // namespace Oak
