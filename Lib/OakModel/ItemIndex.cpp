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
ItemIndexUPtr ItemIndex::Create(const Item &item, bool namedIndex)
{
    Item cItem = item;
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
    }
    return ItemIndexUPtr(cIndex);
}

} // namespace Model
} // namespace Oak
