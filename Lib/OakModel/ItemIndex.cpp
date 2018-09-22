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


namespace Oak {
namespace Model {

// =============================================================================
// (public)
ItemIndex::ItemIndex()
{
    m_childIndex = nullptr;
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
bool ItemIndex::contains(const ItemIndex &itemIndex)
{
    if (depth() < itemIndex.depth()) { return false; }

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
    Item item = rootItem.childAt(m_name, m_index);
    if (!m_childIndex || depth == 1 || item.isNull()) {
        assert(depth == 1 || depth < -1);
        return item;
    }
    return m_childIndex->item(item, --depth);
}

// =============================================================================
// (public)
const ItemIndex &ItemIndex::childItemIndex(int depth) const
{
    assert(m_childIndex);
    if (depth == 1) { return *m_childIndex; }
    return m_childIndex->childItemIndex(--depth);
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
        assert(pIndex->m_index != -1);
        pIndex->m_childIndex = cIndex;
        cIndex = pIndex;
        cItem = pItem;
        pItem = cItem.parent();
    }
    return ItemIndexUPtr(cIndex);
}

} // namespace Model
} // namespace Oak
