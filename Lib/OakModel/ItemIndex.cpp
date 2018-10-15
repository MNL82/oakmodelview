/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ItemIndex.h"

#include "../ServiceFunctions/Assert.h"

namespace Oak {
namespace Model {

ItemIndex ItemIndex::s_itemIndex = ItemIndex();

// =============================================================================
// (public)
ItemIndex::ItemIndex()
{
    m_index = -1;
    m_childIndex = nullptr;
}

// =============================================================================
// (public)
ItemIndex::ItemIndex(int index)
{
    m_index = index;
    m_childIndex = nullptr;
}

// =============================================================================
// (public)
ItemIndex::ItemIndex(const std::string &name, int index)
{
    m_name = name;
    m_index = index;
    m_childIndex = nullptr;
}

// =============================================================================
// (public)
ItemIndex::ItemIndex(const ItemIndex &itemIndex)
{
    m_name = itemIndex.m_name;
    m_index = itemIndex.m_index;
    if (itemIndex.m_childIndex) {
        m_childIndex = new ItemIndex(*itemIndex.m_childIndex);
    } else {
        m_childIndex = nullptr;
    }
}

// =============================================================================
// (public)
ItemIndex::~ItemIndex()
{
    if (m_childIndex) {
        delete m_childIndex;
        m_childIndex = nullptr;
    }
}

// =============================================================================
// (public)
bool ItemIndex::isNull() const
{
    return m_index == -1;
}

// =============================================================================
// (public)
bool ItemIndex::isNamed(bool recursive) const
{
    if (m_name.empty()) {
        return false;
    } else if (recursive && m_childIndex != nullptr) {
        return m_childIndex->isNamed(recursive);
    }
    return true;
}

// =============================================================================
// (public)
bool ItemIndex::isUnnamed(bool recursive) const
{
    if (!m_name.empty()) {
        return false;
    } else if (recursive && m_childIndex != nullptr) {
        return m_childIndex->isUnnamed(recursive);
    }
    return true;
}

// =============================================================================
// (public)
bool ItemIndex::equal(const ItemIndex &itemIndex) const
{
    const ItemIndex *ii1 = this;
    const ItemIndex *ii2 = &itemIndex;
    while (ii1 != nullptr &&
           ii2 != nullptr &&
           ii1->m_name == ii2->m_name &&
           ii1->m_index == ii2->m_index) {
        ii1 = ii1->m_childIndex;
        ii2 = ii2->m_childIndex;
    }
    return ii1 == nullptr && ii2 == nullptr;
}

// =============================================================================
// (public)
int ItemIndex::depthWhereEqual(const ItemIndex &itemIndex) const
{
    const ItemIndex *ii1 = this;
    const ItemIndex *ii2 = &itemIndex;
    int d = 0;
    while (ii1 != nullptr &&
           ii2 != nullptr &&
           ii1->m_name == ii2->m_name &&
           ii1->m_index == ii2->m_index) {
        d++;
        ii1 = ii1->m_childIndex;
        ii2 = ii2->m_childIndex;
    }
    return d;
}

// =============================================================================
// (public)
const std::string &ItemIndex::name() const
{
    return m_name;
}

// =============================================================================
// (public)
int ItemIndex::index() const
{
    return m_index;
}

// =============================================================================
// (public)
int ItemIndex::depth() const
{
    int d = 1;
    const ItemIndex *itemIndex = this;
    while(itemIndex->m_childIndex) {
        itemIndex = itemIndex->m_childIndex;
        d++;
    }
    return d;
}

// =============================================================================
// (public)
bool ItemIndex::contains(const ItemIndex &itemIndex) const
{
    if (isNull() ||depth() < itemIndex.depth()) { return false; }

    const ItemIndex *ii1 = this;
    const ItemIndex *ii2 = &itemIndex;
    while (ii2) {
        if (ii1->m_name != ii2->m_name) {
            return false;
        }
        if (ii1->m_index != -1 && ii1->m_index != ii2->m_index) {
            return false;
        }
        ii1 = ii1->m_childIndex;
        ii2 = ii2->m_childIndex;
    }
    return true;
}

// =============================================================================
// (public)
Item ItemIndex::item(const Item &rootItem, int depth) const
{
    if (isNull()) { return rootItem; }
    Item item = (m_name.empty()) ? rootItem.childAt(m_index) : rootItem.childAt(m_name, m_index);
    if (!m_childIndex || depth == 1 || item.isNull()) {
        ASSERT(depth == 1 || depth < -1);
        return item;
    }
    return m_childIndex->item(item, --depth);
}

// =============================================================================
// (public)
Item ItemIndex::itemParent(const Item &rootItem) const
{
    if (isNull()) { return Item(); }
    if (m_childIndex == nullptr) { return rootItem; }
    Item item = (m_name.empty()) ? rootItem.childAt(m_index) : rootItem.childAt(m_name, m_index);
    if (m_childIndex->hasChildItemIndex()) {
        return m_childIndex->itemParent(item);
    }
    return item;
}

// =============================================================================
// (public)
bool ItemIndex::hasChildItemIndex() const
{
    return m_childIndex != nullptr;
}

// =============================================================================
// (public)
const ItemIndex &ItemIndex::childItemIndex(int depth) const
{
    if (depth == 0) { return *this; }
    if (m_childIndex) {
        return m_childIndex->childItemIndex(--depth);
    } else {
        return emptyItemIndex();
    }
}

// =============================================================================
// (public)
ItemIndex &ItemIndex::childItemIndex(int depth)
{
    if (depth == 0) { return *this; }
    if (m_childIndex) {
        return m_childIndex->childItemIndex(--depth);
    } else {
        return emptyItemIndex();
    }
}

// =============================================================================
// (public)
const ItemIndex &ItemIndex::lastItemIndex() const
{
    if (m_childIndex) {
        return m_childIndex->lastItemIndex();
    } else {
        return *this;
    }
}

// =============================================================================
// (public)
ItemIndex &ItemIndex::lastItemIndex()
{
    if (m_childIndex) {
        return m_childIndex->lastItemIndex();
    } else {
        return *this;
    }
}

// =============================================================================
// (public)
ItemIndex &ItemIndex::emptyItemIndex()
{
    return s_itemIndex;
}

// =============================================================================
// (public)
ItemIndexUPtr ItemIndex::create(const Item &_item, bool namedIndex)
{
    if (_item.isNull()) { return ItemIndexUPtr(); }
    Item cItem = _item;
    Item pItem = cItem.parent();

    ItemIndex *cIndex = nullptr;
    ItemIndex *pIndex = nullptr;
    while(!pItem.isNull()) {
        pIndex = new ItemIndex();
        if (namedIndex) {
            pIndex->m_name = cItem.def()->name();
            pIndex->m_index = pItem.childIndex(pIndex->m_name, cItem);
        } else {
            pIndex->m_index = pItem.childIndex(cItem);
        }
        if (pIndex->m_index == -1) {  // If item is invalid (F.eks can be invalid after a move)
            return ItemIndexUPtr();
        };
        pIndex->m_childIndex = cIndex;
        cIndex = pIndex;
        cItem = pItem;
        pItem = cItem.parent();
    }
    return ItemIndexUPtr(cIndex);
}

// =============================================================================
// (public)
void ItemIndex::setChildItemIndex(ItemIndex *itemIndex)
{
    if (m_childIndex != nullptr) {
        delete m_childIndex;
    }
    m_childIndex = itemIndex;
}

// =============================================================================
// (public)
int ItemIndex::convertIndexToUnnamed(const Item &_item) const
{
    if (m_name.empty()) {
        return m_index;
    } else {
        return _item.convertChildIndexToUnnamed(m_name, m_index);
    }
}

// =============================================================================
// (public)
int ItemIndex::convertIndexToNamed(const Item &_item, std::string &name) const
{
    if (m_name.empty()) {
        return _item.convertChildIndexToNamed(name, m_index);
    } else {
        name = m_name;
        return m_index;
    }
}

} // namespace Model
} // namespace Oak
