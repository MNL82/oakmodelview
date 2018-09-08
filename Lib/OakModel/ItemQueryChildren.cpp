/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ItemQueryChildren.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
ItemQueryChildren::ItemQueryChildren(const std::string &nodeName)
    : ItemQuery(), m_nodeName(nodeName)
{

}

// =============================================================================
// (public)
Item ItemQueryChildren::addItem(const Item &refItem) const
{
    Item item = last(refItem);


    if (item.isNull()) {
        if (m_nodeName.empty()) { // Can not add child node when name is not specified
            return Item();
        }
        // Add missing child node is possible
        int index = -1;
        item = refItem.insertChild(m_nodeName, index);
        if (!m_childQueryUPtr) { // No child query means task completed
            return item;
        }
    }

    if (m_childQueryUPtr) {
        return std::move(m_childQueryUPtr->addItem(item));
    } else {
        // Add missing child node is possible
        int index = -1;
        return std::move(refItem.insertChild(m_nodeName, index));
    }
}

// =============================================================================
// (public)
Item ItemQueryChildren::first(const Item &refItem) const
{
    if (m_nodeName.empty()) {
        return refItem.firstChild();
    } else {
        return refItem.firstChild(m_nodeName);
    }
}

// =============================================================================
// (public)
Item ItemQueryChildren::last(const Item &refItem) const
{
    if (m_nodeName.empty()) {
        return refItem.lastChild();
    } else {
        return refItem.lastChild(m_nodeName);
    }
}

// =============================================================================
// (public)
Item ItemQueryChildren::next(const Item &refItem, const Item &cItem) const
{
    if (m_nodeName.empty()) {
        return refItem.nextChild(cItem);
    } else {
        return refItem.nextChild(m_nodeName, cItem);
    }
}

// =============================================================================
// (public)
Item ItemQueryChildren::previous(const Item &refItem, const Item &cItem) const
{
    if (m_nodeName.empty()) {
        return refItem.previousChild(cItem);
    } else {
        return refItem.previousChild(m_nodeName, cItem);
    }
}

} // namespace Model
} // namespace Oak
