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

#include "../ServiceFunctions/Trace.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
ItemQueryChildren::ItemQueryChildren(const std::string &nodeName)
    : ItemQuery(), m_nodeName(nodeName)
{
    ASSERT(!m_nodeName.empty());
}

// =============================================================================
// (public)
ItemQueryChildren::ItemQueryChildren(const ItemQueryChildren &copy)
    : ItemQuery(copy)
{
    m_nodeName = copy.m_nodeName;
}

// =============================================================================
// (public)
ItemQueryChildren::ItemQueryChildren(ItemQueryChildren &&move)
    : ItemQuery(move)
{
    m_nodeName = std::move(move.m_nodeName);
}

// =============================================================================
// (public)
bool ItemQueryChildren::canInsertItem(const Item &refItem, int &index) const
{
    if (m_childQueryUPtr) {
        Item item = first(refItem);
        while (!item.isNull()) {
            if (m_childQueryUPtr->canInsertItem(item, index)) {
                return true;
            }
            item = next(refItem, item);
        }
    } else {
        if (refItem.canInsertChild(m_nodeName, index)) {
            return true;
        } else {
            index -= refItem.childCount(m_nodeName);
        }
    }
    return false;
}

// =============================================================================
// (public)
Item ItemQueryChildren::insertItem(const Item &refItem, int index) const
{
    if (m_childQueryUPtr) {
        Item item = first(refItem);
        while (!item.isNull()) {
            Item newItem = m_childQueryUPtr->insertItem(item, index);
            if (!newItem.isNull()) {
                return newItem;
            }
            item = next(refItem, item);
        }
    } else {
        Item newItem = refItem.insertChild(m_nodeName, index);
        if (!newItem.isNull()) {
            return newItem;
        } else {
            index -= refItem.childCount(m_nodeName);
        }
    }
    return Item();
}

// =============================================================================
// (public)
bool ItemQueryChildren::canCloneItem(const Item &refItem, int &index, const Item &cloneItem) const
{
    if (m_childQueryUPtr) {
        Item item = first(refItem);
        while (!item.isNull()) {
            if (m_childQueryUPtr->canCloneItem(item, index, cloneItem)) {
                return true;
            }
            item = next(refItem, item);
        }
    } else {
        if (refItem.canCloneChild(m_nodeName, index, cloneItem)) {
            return true;
        } else {
            index -= refItem.childCount(m_nodeName);
        }
    }
    return false;
}

// =============================================================================
// (public)
Item ItemQueryChildren::cloneItem(const Item &refItem, int &index, const Item &cloneItem) const
{
    if (m_childQueryUPtr) {
        Item item = first(refItem);
        while (!item.isNull()) {
            Item newItem = m_childQueryUPtr->cloneItem(item, index, cloneItem);
            if (!newItem.isNull()) {
                return newItem;
            }
            item = next(refItem, item);
        }
    } else {
        Item newItem = refItem.cloneChild(m_nodeName, index, cloneItem);
        if (!newItem.isNull()) {
            return newItem;
        } else {
            index -= refItem.childCount(m_nodeName);
        }
    }
    return Item();
}

// =============================================================================
// (public)
bool ItemQueryChildren::canMoveItem(const Item &refItem, int &index, const Item &moveItem) const
{
    if (m_childQueryUPtr) {
        Item item = first(refItem);
        while (!item.isNull()) {
            if (m_childQueryUPtr->canMoveItem(item, index, moveItem)) {
                return true;
            }
            item = next(refItem, item);
        }
    } else {
        if (refItem.canMoveChild(m_nodeName, index, moveItem)) {
            return true;
        } else {
            index -= refItem.childCount(m_nodeName);
        }
    }
    return false;
}

// =============================================================================
// (public)
Item ItemQueryChildren::moveItem(const Item &refItem, int &index, const Item &moveItem) const
{
    if (m_childQueryUPtr) {
        Item item = first(refItem);
        while (!item.isNull()) {
            Item newItem = m_childQueryUPtr->moveItem(item, index, moveItem);
            if (!newItem.isNull()) {
                return newItem;
            }
            item = next(refItem, item);
        }
    } else {
        Item newItem = refItem.moveChild(m_nodeName, index, moveItem);
        if (!newItem.isNull()) {
            return newItem;
        } else {
            index -= refItem.childCount(m_nodeName);
        }
    }
    return Item();
}

// =============================================================================
// (public)
bool ItemQueryChildren::canRemoveItem(const Item &refItem, int index) const
{
    if (m_childQueryUPtr) {
        Item item = first(refItem);
        while (!item.isNull()) {
            if (m_childQueryUPtr->canRemoveItem(item, index)) {
                return true;
            }
            item = next(refItem, item);
        }
    } else {
        if (refItem.canRemoveChild(m_nodeName, index)) {
            return true;
        } else {
            index -= refItem.childCount(m_nodeName);
        }
    }
    return false;
}

// =============================================================================
// (public)
bool ItemQueryChildren::removeItem(const Item &refItem, int index) const
{
    if (m_childQueryUPtr) {
        Item item = first(refItem);
        while (!item.isNull()) {
            if (m_childQueryUPtr->removeItem(item, index)) {
                return true;
            }
            item = next(refItem, item);
        }
    } else {
        if (refItem.removeChild(m_nodeName, index)) {
            return true;
        } else {
            index -= refItem.childCount(m_nodeName);
        }
    }
    return false;
}

// =============================================================================
// (protected)
Item ItemQueryChildren::first(const Item &refItem) const
{
    return refItem.firstChild(m_nodeName);
}

// =============================================================================
// (protected)
Item ItemQueryChildren::last(const Item &refItem) const
{
    return refItem.lastChild(m_nodeName);
}

// =============================================================================
// (protected)
Item ItemQueryChildren::next(const Item &refItem, const Item &cItem) const
{
    return refItem.nextChild(m_nodeName, cItem);
}

// =============================================================================
// (protected)
Item ItemQueryChildren::previous(const Item &refItem, const Item &cItem) const
{
    return refItem.previousChild(m_nodeName, cItem);
}

// =============================================================================
// (public)
const NodeDef *ItemQueryChildren::_nodeDef(const NodeDef *nDef) const
{
    if (nDef == nullptr) { return nullptr; }
    return nDef->childDef(m_nodeName, true);
}

} // namespace Model
} // namespace Oak
