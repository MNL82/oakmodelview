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
    if (!m_itemQueryPtr) { return 1; }
    return m_itemQueryPtr->count(item);
}

// =============================================================================
// (public)
const Entry &EntryQuery::entry(const Item &item, int index) const
{
    assert(!m_entryName.empty());

    int i = 0;
    auto it = iterator(item);
    while (it->next()) {
        if (i == index) { return it->entry(); }
        i++;
    }
    return Entry::emptyEntry();
}

// =============================================================================
// (public)
void EntryQuery::getValueList(const Item &item, std::vector<UnionValue> &valueList) const
{
    assert(!m_entryName.empty());

    if (m_itemQueryPtr) {
        auto it = m_itemQueryPtr->iterator(item);
        while (it->next()) {
            auto tempItem = it->item();
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
std::vector<UnionValue> EntryQuery::valueList(const Item &item) const
{
    std::vector<UnionValue> valueList;
    getValueList(item, valueList);
    return valueList;
}

// =============================================================================
// (public)
void EntryQuery::getValue(const Item &item, int index, UnionValue value) const
{
    assert(!m_entryName.empty());

    if (m_itemQueryPtr) {
        int i = 0;
        auto it = m_itemQueryPtr->iterator(item);
        while (it->next()) {
            if (i == index) {
                auto tempItem = it->item();
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
std::vector<Item> EntryQuery::toItemList(const Item &item)
{
    std::vector<Item> itemList;
    if (!m_itemQueryPtr) { return itemList; }

    auto it = m_itemQueryPtr->iterator(item);
    while (it->next()) {
        itemList.push_back(it->item());
    }

    return itemList;
}

// =============================================================================
// (public)
const ItemQuery &EntryQuery::itemQuery() const
{
    return *m_itemQueryPtr.get();
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

// =============================================================================
// (public)
EntryQuery::IteratorUPtr EntryQuery::iterator(const Item &refItem) const
{
    IteratorUPtr it(new Iterator(*this, &refItem));
    return it;
}

//// =============================================================================
//// (public)
//EntryQuery::IteratorUPtr EntryQuery::begin(const Item &refItem) const
//{
//    IteratorUPtr it(new Iterator(*this));
//    it->first(refItem);
//    return it;
//}

//// =============================================================================
//// (public)
//EntryQuery::IteratorUPtr EntryQuery::rBegin(const Item &refItem) const
//{
//    IteratorUPtr it(new Iterator(*this));
//    it->last(refItem);
//    return it;
//}

// =============================================================================
// Iterator functions
// =============================================================================

// =============================================================================
// (public)
EntryQuery::Iterator::Iterator(const EntryQuery &entryQuery, const Item *refItem)
    : ItemQuery::Iterator(entryQuery.itemQuery())
{
    m_entryQuery = &entryQuery;
    m_refItem = refItem;
}

// =============================================================================
// (public)
EntryQuery::Iterator::~Iterator()
{
    m_entryQuery = nullptr;
    ItemQuery::Iterator::~Iterator();
}

// =============================================================================
// (public)
void EntryQuery::Iterator::getValue(UnionValue value) const
{
    entry().getValue(value);
}

// =============================================================================
// (public)
const Entry &EntryQuery::Iterator::entry() const
{
    assert(!m_entryQuery->m_entryName.empty());
    return this->item().entry(m_entryQuery->m_entryName);
}

} // namespace Model
} // namespace Oak
