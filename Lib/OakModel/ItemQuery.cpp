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
#include "QueryBuilder.h"

#include <utility>
#include <iterator>

namespace Oak::Model {

// =============================================================================
// (public)
ItemQuery::ItemQuery()
{

}

// =============================================================================
// (public)
ItemQuery::ItemQuery(const ItemQuery& copy)
{
    m_childQueryUPtr = QueryBuilder::duplicate(copy.m_childQueryUPtr);
}

// =============================================================================
// (public)
ItemQuery::ItemQuery(ItemQuery&& move)
{
    m_childQueryUPtr = std::move(move.m_childQueryUPtr);
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
    auto it = iterator(refItem);
    int count = 0;
    while (it->next()) {
        count++;
    }
    return count;
}

// =============================================================================
// (public)
std::vector<Item> ItemQuery::itemList(const Item &refItem)
{
    std::vector<Item> itemList;
    if (!m_childQueryUPtr) { return itemList; }

    auto it = iterator(refItem);
    while (it->next()) {
        itemList.push_back(it->item());
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
const ItemQuery *ItemQuery::childQuery() const
{
    return m_childQueryUPtr.get();
}

// =============================================================================
// (public)
const NodeDef *ItemQuery::nodeDef(const NodeDef *nDef) const
{
    if (hasChildQuery()) {
        return m_childQueryUPtr->nodeDef(_nodeDef(nDef));
    }
    return _nodeDef(nDef);
}

// =============================================================================
// (public)
bool ItemQuery::canInsertItem(const Item &refItem, int &index) const
{
    if (m_childQueryUPtr) {
        Item item = first(refItem);
        while (!item.isNull()) {
            if (m_childQueryUPtr->canInsertItem(item, index)) {
                return true;
            }
            item = next(refItem, item);
        }
        return false;
    } else {
        // Default ItemQuery can not insert items
        return false;
    }
}

// =============================================================================
// (public)
Item ItemQuery::insertItem(const Item &refItem, int index) const
{
    if (m_childQueryUPtr) {
        Item item = first(refItem);
        while (!item.isNull()) {
            Item newItem = m_childQueryUPtr->insertItem(item, index);
            if (!newItem.isNull()) {
                return newItem;
            }
            item = next(refItem, item);
        }
        return Item();
    } else {
        // Default ItemQuery can not insert items
        return Item();
    }
}

// =============================================================================
// (public)
bool ItemQuery::canCloneItem(const Item &refItem, int &index, const Item &cloneItem) const
{
    if (m_childQueryUPtr) {
        Item item = first(refItem);
        while (!item.isNull()) {
            if (m_childQueryUPtr->canCloneItem(item, index, cloneItem)) {
                return true;
            }
            item = next(refItem, item);
        }
        return false;
    } else {
        // Default ItemQuery can not clone items
        return false;
    }
}

// =============================================================================
// (public)
Item ItemQuery::cloneItem(const Item &refItem, int &index, const Item &cloneItem) const
{
    if (m_childQueryUPtr) {
        Item item = first(refItem);
        while (!item.isNull()) {
            Item newItem = m_childQueryUPtr->cloneItem(item, index, cloneItem);
            if (!newItem.isNull()) {
                return newItem;
            }
            item = next(refItem, item);
        }
        return Item();
    } else {
        // Default ItemQuery can not clone items
        return Item();
    }
}

// =============================================================================
// (public)
bool ItemQuery::canMoveItem(const Item &refItem, int &index, const Item &moveItem) const
{
    if (m_childQueryUPtr) {
        Item item = first(refItem);
        while (!item.isNull()) {
            if (m_childQueryUPtr->canMoveItem(item, index, moveItem)) {
                return true;
            }
            item = next(refItem, item);
        }
        return false;
    } else {
        // Default ItemQuery can not move items
        return false;
    }
}

// =============================================================================
// (public)
Item ItemQuery::moveItem(const Item &refItem, int &index, const Item &moveItem) const
{
    if (m_childQueryUPtr) {
        Item item = first(refItem);
        while (!item.isNull()) {
            Item newItem = m_childQueryUPtr->moveItem(item, index, moveItem);
            if (!newItem.isNull()) {
                return newItem;
            }
            item = next(refItem, item);
        }
        return Item();
    } else {
        // Default ItemQuery can not move items
        return Item();
    }
}

// =============================================================================
// (public)
bool ItemQuery::canRemoveItem(const Item &refItem, int index) const
{
    if (m_childQueryUPtr) {
        Item item = first(refItem);
        while (!item.isNull()) {
            if (m_childQueryUPtr->canRemoveItem(item, index)) {
                return true;
            }
            item = next(refItem, item);
        }
        return false;
    } else {
        // Default ItemQuery can not remove items
        return false;
    }
}

// =============================================================================
// (public)
bool ItemQuery::removeItem(Item &refItem, int index)
{
    if (m_childQueryUPtr) {
        Item item = first(refItem);
        while (!item.isNull()) {
            if (m_childQueryUPtr->removeItem(item, index)) {
                return true;
            }
            item = next(refItem, item);
        }
        return false;
    } else {
        // Default ItemQuery can not remove items
        return false;
    }
}

// =============================================================================
// (protected)
void ItemQuery::addChildQuery(ItemQueryUPtr query)
{
    if (m_childQueryUPtr) {
        m_childQueryUPtr->addChildQuery(std::move(query));
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
// (protected)
const NodeDef *ItemQuery::_nodeDef(const NodeDef *nDef) const
{
    return nDef;
}

// =============================================================================
// (public)
ItemQuery::IteratorUPtr ItemQuery::iterator(const Item &refItem) const
{
    IteratorUPtr it(new Iterator(*this, &refItem));
    return it;
}

// =============================================================================
// Iterator functions
// =============================================================================

// =============================================================================
// (public)
ItemQuery::Iterator::Iterator(const ItemQuery &query, const Item *refItem)
{
    m_query = &query;
    m_refItem = refItem;
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
    if (m_currentItem.isNull()) {
        return first(*m_refItem);
    }

    if (!m_childIterator) { // No child query
        // Move next
        if (m_query) {
            m_currentItem = m_query->next(*m_refItem, m_currentItem);
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
        m_currentItem = m_query->next(*m_refItem, m_currentItem);
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
    if (m_currentItem.isNull()) {
        return last(*m_refItem);
    }

    if (!m_childIterator) { // No child query
        // Move previous
        if (m_query) {
            m_currentItem = m_query->previous(*m_refItem, m_currentItem);
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
        m_currentItem = m_query->previous(*m_refItem, m_currentItem);
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
    m_refItem = &refItem;

    if (m_query) {
        m_currentItem = m_query->first(*m_refItem);
    } else {
        m_currentItem = *m_refItem;
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
    m_refItem = &refItem;

    if (m_query) {
        m_currentItem = m_query->last(*m_refItem);
    } else {
        m_currentItem = *m_refItem;
    }

    if (m_currentItem.isNull()) { return false; }

    if (!m_childIterator) { return true; }

    if (m_childIterator->last(m_currentItem)) { return true; }

    m_currentItem.clear();

    return false;
}

} // namespace Oak::Model

