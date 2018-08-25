/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ItemQueryIgnoreSelf.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
ItemQueryIgnoreSelf::ItemQueryIgnoreSelf()
    : ItemQuery()
{

}
// =============================================================================
// (public)
bool ItemQueryIgnoreSelf::moveNext()
{
    assert(m_childQueryUPtr);
    if (!ItemQuery::moveNext()) { return false; }
    ItemQuery* cq = m_childQueryUPtr.get();
    while (cq) {
        if (cq->current(false) == m_currentItem) {
            return moveNext();
        }
        cq = cq->childQuery();
    }
    return true;
}

// =============================================================================
// (public)
bool ItemQueryIgnoreSelf::moveCurrentNext()
{
    assert(!m_refItem.isNull());
    if (m_currentItem.isNull()) {
        m_currentItem = m_refItem;
    } else {
        m_currentItem.clear();
    }
    return !m_currentItem.isNull();
}

} // namespace Model
} // namespace Oak
