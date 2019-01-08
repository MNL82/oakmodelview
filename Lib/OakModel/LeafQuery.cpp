/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "LeafQuery.h"

#include "ItemQueryChildren.h"
#include "ItemQueryParent.h"

#include "../ServiceFunctions/Assert.h"


namespace Oak::Model {

// =============================================================================
// (protected)
LeafQuery::LeafQuery(const std::string &leafName)
{
    m_leafName = leafName;
}

// =============================================================================
// (public)
LeafQuery::LeafQuery(ItemQueryUPtr itemQueryUPtr, const std::string &leafName)
{
    m_itemQueryPtr = std::move(itemQueryUPtr);
    m_leafName = leafName;
}

// =============================================================================
// (public)
LeafQuery::~LeafQuery()
{
}

// =============================================================================
// (public)
const std::string &LeafQuery::valueName() const
{
    return m_leafName;
}

// =============================================================================
// (public)
LeafQuerySPtr LeafQuery::setValueName(const std::string &leafName)
{
    m_leafName = leafName;
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
int LeafQuery::count(const Item &item)
{
    if (!m_itemQueryPtr) { return 1; }
    return m_itemQueryPtr->count(item);
}

// =============================================================================
// (public)
const Leaf &LeafQuery::leaf(const Item &item, int index) const
{
    ASSERT(!m_leafName.empty());

    int i = 0;
    auto it = iterator(item);
    while (it->next()) {
        if (i == index) { return it->leaf(); }
        i++;
    }
    return Leaf::emptyLeaf();
}

// =============================================================================
// (public)
void LeafQuery::getValueList(const Item &item, std::vector<UnionValue> &valueList) const
{
    ASSERT(!m_leafName.empty());

    if (m_itemQueryPtr) {
        auto it = m_itemQueryPtr->iterator(item);
        while (it->next()) {
            auto tempItem = it->item();
            if (tempItem.hasLeaf(m_leafName)) {
                valueList.push_back(tempItem.leaf(m_leafName).value());
            }
        }
    } else {
        if (item.hasLeaf(m_leafName)) {
            valueList.push_back(item.leaf(m_leafName).value());
        }
    }
}

// =============================================================================
// (public)
std::vector<UnionValue> LeafQuery::valueList(const Item &item) const
{
    std::vector<UnionValue> valueList;
    getValueList(item, valueList);
    return valueList;
}

// =============================================================================
// (public)
void LeafQuery::getValue(const Item &item, int index, UnionValue value) const
{
    ASSERT(!m_leafName.empty());

    if (m_itemQueryPtr) {
        int i = 0;
        auto it = m_itemQueryPtr->iterator(item);
        while (it->next()) {
            if (i == index) {
                auto tempItem = it->item();
                if (tempItem.hasLeaf(m_leafName)) {
                    item.leaf(m_leafName).getValue(value);
                }
                return;
            }
            i++;
        }
        ASSERT(false);
    } else {
        ASSERT(index == 0);
        if (item.hasLeaf(m_leafName)) {
            item.leaf(m_leafName).getValue(value);
        }
    }
}

// =============================================================================
// (public)
std::vector<Item> LeafQuery::toItemList(const Item &item)
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
const ItemQuery &LeafQuery::itemQuery() const
{
    return *m_itemQueryPtr.get();
}

// =============================================================================
// (public static)
LeafQuerySPtr LeafQuery::create(const std::string &valueName)
{
    LeafQuerySPtr sPtr = LeafQuerySPtr(new LeafQuery(valueName));
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

// =============================================================================
// (public static)
LeafQuerySPtr LeafQuery::create(ItemQueryUPtr itemQueryUPtr, const std::string &leafName)
{
    LeafQuerySPtr sPtr = LeafQuerySPtr(new LeafQuery(std::move(itemQueryUPtr), leafName));
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

// =============================================================================
// (public)
LeafQuery::IteratorUPtr LeafQuery::iterator(const Item &refItem) const
{
    IteratorUPtr it(new Iterator(*this, &refItem));
    return it;
}

//// =============================================================================
//// (public)
//LeafQuery::IteratorUPtr LeafQuery::begin(const Item &refItem) const
//{
//    IteratorUPtr it(new Iterator(*this));
//    it->first(refItem);
//    return it;
//}

//// =============================================================================
//// (public)
//LeafQuery::IteratorUPtr LeafQuery::rBegin(const Item &refItem) const
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
LeafQuery::Iterator::Iterator(const LeafQuery &leafQuery, const Item *refItem)
    : ItemQuery::Iterator(leafQuery.itemQuery())
{
    m_leafQuery = &leafQuery;
    m_refItem = refItem;
}

// =============================================================================
// (public)
LeafQuery::Iterator::~Iterator()
{
    m_leafQuery = nullptr;
    ItemQuery::Iterator::~Iterator();
}

// =============================================================================
// (public)
void LeafQuery::Iterator::getValue(UnionValue value) const
{
    leaf().getValue(value);
}

// =============================================================================
// (public)
const Leaf &LeafQuery::Iterator::leaf() const
{
    ASSERT(!m_leafQuery->m_leafName.empty());
    return this->item().leaf(m_leafQuery->m_leafName);
}

} // namespace Oak::Model

