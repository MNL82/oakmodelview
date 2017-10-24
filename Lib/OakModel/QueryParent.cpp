/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "QueryParent.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
QueryParent::QueryParent()
    : Query()
{

}

// =============================================================================
// (public)
bool QueryParent::moveCurrentNext()
{
    assert(!m_refItem.isNull());
    if (m_currentItem.isNull()) {
        m_currentItem = m_refItem.parent();
    } else {
        m_currentItem.clear();
    }
    return !m_currentItem.isNull();
}

} // namespace Model
} // namespace Oak
