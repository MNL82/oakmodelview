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
    std::vector<std::string>::iterator it;
    std::vector<std::string> list;
}

// =============================================================================
// (public)
int ItemQuery::count(const Item &refItem)
{
    auto it = begin(refItem);
    int count = 0;
    while (it->isValid()) {
        count++;
        it->next();
    }
    return count;
}

// =============================================================================
// (public)
std::vector<Item> ItemQuery::itemList(const Item &refItem)
{
    std::vector<Item> itemList;
    if (!m_childQueryUPtr) { return itemList; }

    auto it = begin(refItem);
    while (it->isValid()) {
        itemList.push_back(it->item());
        it->next();
    }

    return itemList;
}

// =============================================================================
// (public)
Item ItemQuery::previous(const Item &refItem, const Item &cItem) const
{
    UNUSED(refItem);
    UNUSED(cItem);
    return Item();
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

// =============================================================================
// (protected)
bool ItemQuery::hasChildQuery() const
{
    return static_cast<bool>(m_childQueryUPtr);
}

// =============================================================================
// (protected)
Item ItemQuery::first(const Item &refItem) const
{
    return refItem;
}

// =============================================================================
// (protected)
Item ItemQuery::last(const Item &refItem) const
{
    return refItem;
}

// =============================================================================
// (protected)
Item ItemQuery::next(const Item &refItem, const Item &cItem) const
{
    UNUSED(refItem);
    UNUSED(cItem);
    return Item();
}

// =============================================================================
// (public)
ItemQuery::IteratorUPtr ItemQuery::begin(const Item &refItem) const
{
    IteratorUPtr it(new Iterator(*this));
    it->first(refItem);
    return it;
}

// =============================================================================
// (public)
ItemQuery::IteratorUPtr ItemQuery::rBegin(const Item &refItem) const
{
    IteratorUPtr it(new Iterator(*this));
    it->last(refItem);
    return it;
}

// =============================================================================
// Iterator functions
// =============================================================================

// =============================================================================
// (public)
ItemQuery::Iterator::Iterator(const ItemQuery &query)
{
    m_query = &query;
    if (m_query && m_query->hasChildQuery()) {
        m_childIterator = new Iterator(*m_query->m_childQueryUPtr.get());
    } else {
        m_childIterator = nullptr;
    }
}

// =============================================================================
// (public)
ItemQuery::Iterator::~Iterator()
{
    if (m_childIterator) {
        delete m_childIterator;
        m_childIterator = nullptr;
    }
}

// =============================================================================
// (public)
bool ItemQuery::Iterator::isValid() const
{
    return !m_currentItem.isNull();
}

// =============================================================================
// (public)
bool ItemQuery::Iterator::next()
{
    if (!m_childIterator) { // No child query
        // Move next
        if (m_query) {
            m_currentItem = m_query->next(m_refItem, m_currentItem);
        } else {
            m_currentItem.clear();
        }
        return isValid();
    }

    // Child query move next
    m_childIterator->next();

    while (!m_childIterator->isValid()) { // Child query not valid
        // Move next
        if (!m_query) { // Query is empty
            m_currentItem.clear();
            return isValid();
        }
        m_currentItem = m_query->next(m_refItem, m_currentItem);
        if (m_currentItem.isNull()) { // End of Query
            return isValid();
        } else {
            // Initialize child query
            m_childIterator->first(m_currentItem);
        }
    }
    return isValid();
}

// =============================================================================
// (public)
bool ItemQuery::Iterator::previous()
{
    if (!m_childIterator) { // No child query
        // Move previous
        if (m_query) {
            m_currentItem = m_query->previous(m_refItem, m_currentItem);
        } else {
            m_currentItem.clear();
        }
        return isValid();
    }

    // Child query move previous
    m_childIterator->previous();

    while (!m_childIterator->isValid()) { // Child query not valid
        // Move previous
        if (m_query) { // Query is empty
            m_currentItem.clear();
            return isValid();
        }
        m_currentItem = m_query->previous(m_refItem, m_currentItem);
        if (m_currentItem.isNull()) { // End of Query
            return isValid();
        } else {
            // Initialize child query
            m_childIterator->last(m_currentItem);
        }
    }
    return isValid();
}

// =============================================================================
// (public)
const Item &ItemQuery::Iterator::item() const
{
    if (m_childIterator) {
        return m_childIterator->item();
    }
    return m_currentItem;
}

// =============================================================================
// (public)
bool ItemQuery::Iterator::first(const Item &refItem)
{
    m_refItem = refItem;

    if (m_query) {
        m_currentItem = m_query->first(m_refItem);
    } else {
        m_currentItem = m_refItem;
    }

    if (m_currentItem.isNull()) { return false; }

    if (!m_childIterator) { return true; }

    if (m_childIterator->first(m_currentItem)) { return true; }

    m_currentItem.clear();

    return false;
}

// =============================================================================
// (public)
bool ItemQuery::Iterator::last(const Item &refItem)
{
    m_refItem = refItem;

    if (m_query) {
        m_currentItem = m_query->last(m_refItem);
    } else {
        m_currentItem = m_refItem;
    }

    if (m_currentItem.isNull()) { return false; }

    if (!m_childIterator) { return true; }

    if (m_childIterator->last(m_currentItem)) { return true; }

    m_currentItem.clear();

    return false;
}

} // namespace Model
} // namespace Oak
