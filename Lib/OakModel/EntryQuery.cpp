/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "EntryQuery.h"

#include "ItemQueryChildren.h"
#include "ItemQueryParent.h"
#include "ItemQueryIgnoreSelf.h"

namespace Oak {
namespace Model {

// =============================================================================
// (protected)
EntryQuery::EntryQuery(const std::string &valueName)
{
    m_entryName = valueName;
}

// =============================================================================
// (public)
EntryQuery::EntryQuery(ItemQueryUPtr itemQueryUPtr, const std::string &entryName)
{
    m_itemQueryPtr = std::move(itemQueryUPtr);
    m_entryName = entryName;
}

// =============================================================================
// (public)
EntryQuery::~EntryQuery()
{
}

// =============================================================================
// (public)
EntryQuerySPtr EntryQuery::setValueName(const std::string &entryName)
{
    m_entryName = entryName;
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
int EntryQuery::count(const Item &item)
{
    if (!m_itemQueryPtr) { return -1; }

    m_itemQueryPtr->reset(item);
    int count = 0;
    while(m_itemQueryPtr->moveNext()) {
        count++;
    }
    return count;
}

// =============================================================================
// (public)
void EntryQuery::addValueList(const Item &item, std::vector<UnionValue> &valueList) const
{
    assert(!m_entryName.empty());

    if (m_itemQueryPtr) {
        m_itemQueryPtr->reset(item);
        while(m_itemQueryPtr->moveNext()) {
            Item tempItem = m_itemQueryPtr->current();
            if (tempItem.hasEntry(m_entryName)) {
                valueList.push_back(tempItem.entry(m_entryName).value());
            }
        }
    } else {
        if (item.hasEntry(m_entryName)) {
            valueList.push_back(item.entry(m_entryName).value());
        }
    }
}

// =============================================================================
// (public)
std::vector<UnionValue> EntryQuery::getValueList(const Item &item) const
{
    std::vector<UnionValue> valueList;
    addValueList(item, valueList);
    return valueList;
}

// =============================================================================
// (public)
void EntryQuery::getValue(const Item &item, int index, UnionValue value) const
{
    assert(!m_entryName.empty());

    if (m_itemQueryPtr) {
        m_itemQueryPtr->reset(item);
        int i = 0;
        while(m_itemQueryPtr->moveNext()) {
            if (i == index) {
                Item tempItem = m_itemQueryPtr->current();
                if (tempItem.hasEntry(m_entryName)) {
                    item.entry(m_entryName).getValue(value);
                }
                return;
            }
            i++;
        }
        assert(false);
    } else {
        assert(index == 0);
        if (item.hasEntry(m_entryName)) {
            item.entry(m_entryName).getValue(value);
        }
    }
}

// =============================================================================
// (public)
const Entry &EntryQuery::entry(const Item &item, int index) const
{
    assert(!m_entryName.empty());

    if (m_itemQueryPtr) {
        m_itemQueryPtr->reset(item);
        int i = 0;
        while(m_itemQueryPtr->moveNext()) {
            if (i == index) {
                Item tempItem = m_itemQueryPtr->current();
                if (tempItem.hasEntry(m_entryName)) {
                    return item.entry(m_entryName);
                }
                return Entry::emptyEntry();
            }
            i++;
        }
        assert(false);
    } else {
        assert(index == 0);
        if (item.hasEntry(m_entryName)) {
            return item.entry(m_entryName);
        }
    }
    return Entry::emptyEntry();
}

// =============================================================================
// (public)
std::vector<Item> EntryQuery::toItemList(const Item &item)
{
    std::vector<Item> itemList;
    if (!m_itemQueryPtr) { return itemList; }

    m_itemQueryPtr->reset(item);
    while(m_itemQueryPtr->moveNext()) {
        itemList.push_back(m_itemQueryPtr->current());
    }

    return itemList;
}

// =============================================================================
// (public static)
EntryQuerySPtr EntryQuery::create(const std::string &valueName)
{
    EntryQuerySPtr sPtr = EntryQuerySPtr(new EntryQuery(valueName));
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

// =============================================================================
// (public static)
EntryQuerySPtr EntryQuery::create(ItemQueryUPtr itemQueryUPtr, const std::string &valueName)
{
    EntryQuerySPtr sPtr = EntryQuerySPtr(new EntryQuery(std::move(itemQueryUPtr), valueName));
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

} // namespace Model
} // namespace Oak
