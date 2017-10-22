/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "Query.h"

#include <utility>

namespace Oak {
namespace Model {

// =============================================================================
// (public)
Query::Query()
{

}

// =============================================================================
// (public)
Query::~Query()
{
    if (m_childQuery) {
        delete m_childQuery;
        m_childQuery = nullptr;
    }
}

// =============================================================================
// (public)
void Query::reset(Item refItem)
{
    m_refItem = std::move(refItem);
    if (!m_currentItem.isNull()) {
        m_currentItem.clear();
        if (m_childQuery) {
            m_childQuery->reset(m_currentItem);
        }
    }
}

// =============================================================================
// (public)
bool Query::moveNext()
{
    assert(!m_refItem.isNull());

    // Skip recursive iteration if there are no child query
    if (!m_childQuery) {
        return moveCurrentNext();
    }

    // Start from the beginning if current item is null
    if (m_currentItem.isNull()) {
        // First item
        if (moveCurrentNext()) {
            m_childQuery->reset(m_currentItem);
        } else {
            return false;
        }
    }

    // Find the next child current item
    while (!m_childQuery->moveNext()) {
        if (moveCurrentNext()) {
            m_childQuery->reset(m_currentItem);
        } else {
            // There are no more items in the query
            return false;
        }
    }
    return true;
}

// =============================================================================
// (public)
const Item& Query::current() const
{
    if (m_childQuery) {
        return m_childQuery->current();
    }
    return m_currentItem;
}

// =============================================================================
// (public)
void Query::add(Query *query)
{
    if (m_childQuery) {
        m_childQuery->add(query);
    } else {
        m_childQuery = query;
    }
}

} // namespace Model
} // namespace Oak
