/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ItemQuery.h"

#include "ItemQueryChildren.h"
#include "ItemQueryParent.h"
#include "ItemQueryIgnoreSelf.h"

#include <utility>

namespace Oak {
namespace Model {

// =============================================================================
// (public)
ItemQuery::ItemQuery()
{

}

// =============================================================================
// (public)
ItemQuery::~ItemQuery()
{
}

// =============================================================================
// (public)
void ItemQuery::reset(const Item &refItem)
{
    m_refItem = refItem;
    if (!m_currentItem.isNull()) {
        m_currentItem.clear();
        if (m_childQueryUPtr) {
            m_childQueryUPtr->reset(m_currentItem);
        }
    }
}

// =============================================================================
// (public)
bool ItemQuery::moveNext()
{
    assert(!m_refItem.isNull());

    // Skip recursive iteration if there are no child query
    if (!m_childQueryUPtr) {
        return moveCurrentNext();
    }

    // Start from the beginning if current item is null
    if (m_currentItem.isNull()) {
        // First item
        if (moveCurrentNext()) {
            m_childQueryUPtr->reset(m_currentItem);
        } else {
            return false;
        }
    }

    // Find the next child current item
    while (!m_childQueryUPtr->moveNext()) {
        if (moveCurrentNext()) {
            m_childQueryUPtr->reset(m_currentItem);
        } else {
            // There are no more items in the query
            return false;
        }
    }
    return true;
}

// =============================================================================
// (public)
const Item& ItemQuery::current(bool recursive) const
{
    if (recursive && m_childQueryUPtr) {
        return m_childQueryUPtr->current(recursive);
    }
    return m_currentItem;
}

// =============================================================================
// (public)
int ItemQuery::count(const Item &item)
{
    reset(item);
    int count = 0;
    while(moveNext()) {
        count++;
    }
    return count;
}

// =============================================================================
// (public)
std::vector<Item> ItemQuery::itemList(const Item &item)
{
    std::vector<Item> itemList;
    if (!m_childQueryUPtr) { return itemList; }

    m_childQueryUPtr->reset(item);
    while(m_childQueryUPtr->moveNext()) {
        itemList.push_back(m_childQueryUPtr->current());
    }

    return itemList;
}

// =============================================================================
// (public)
ItemQuery *ItemQuery::childQuery()
{
    return m_childQueryUPtr.get();
}

// =============================================================================
// (protected)
void ItemQuery::add(ItemQueryUPtr query)
{
    if (m_childQueryUPtr) {
        m_childQueryUPtr->add(std::move(query));
    } else {
        m_childQueryUPtr = std::move(query);
    }
}

} // namespace Model
} // namespace Oak
