/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "QueryChildren.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
QueryChildren::QueryChildren(const std::string &nodeName)
    : m_nodeName(nodeName)
{

}

// =============================================================================
// (public)
bool QueryChildren::moveCurrentNext()
{
    assert(!m_refItem.isNull());
    if (m_currentItem.isNull()) {
        if (m_nodeName.empty()) {
            m_currentItem = m_refItem.firstChild();
        } else {
            m_currentItem = m_refItem.firstChild(m_nodeName);
        }
    } else {
        if (m_nodeName.empty()) {
            m_currentItem = m_refItem.nextChild(m_currentItem);
        } else {
            m_currentItem = m_refItem.nextChild(m_nodeName, m_currentItem);
        }
    }
    return !m_currentItem.isNull();
}

} // namespace Model
} // namespace Oak
