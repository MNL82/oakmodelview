/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "NodeQueryChildren.h"

#include "../ServiceFunctions/Trace.h"


namespace Oak::Model {

// =============================================================================
// (public)
NodeQueryChildren::NodeQueryChildren(const std::string &nodeName)
    : NodeQuery(), m_nodeName(nodeName)
{
    ASSERT(!m_nodeName.empty());
}

// =============================================================================
// (public)
NodeQueryChildren::NodeQueryChildren(const NodeQueryChildren &copy)
    : NodeQuery(copy)
{
    m_nodeName = copy.m_nodeName;
}

// =============================================================================
// (public)
NodeQueryChildren::NodeQueryChildren(NodeQueryChildren &&move)
    : NodeQuery(move)
{
    m_nodeName = std::move(move.m_nodeName);
}

// =============================================================================
// (public)
bool NodeQueryChildren::canInsertNode(const Node &refNode, int &index) const
{
    if (m_childQueryUPtr) {
        Node node = first(refNode);
        while (!node.isNull()) {
            if (m_childQueryUPtr->canInsertNode(node, index)) {
                return true;
            }
            node = next(refNode, node);
        }
    } else {
        if (refNode.canInsertChild(m_nodeName, index)) {
            return true;
        } else {
            index -= refNode.childCount(m_nodeName);
        }
    }
    return false;
}

// =============================================================================
// (public)
Node NodeQueryChildren::insertNode(const Node &refNode, int index) const
{
    if (m_childQueryUPtr) {
        Node node = first(refNode);
        while (!node.isNull()) {
            Node newNode = m_childQueryUPtr->insertNode(node, index);
            if (!newNode.isNull()) {
                return newNode;
            }
            node = next(refNode, node);
        }
    } else {
        Node newNode = refNode.insertChild(m_nodeName, index);
        if (!newNode.isNull()) {
            return newNode;
        } else {
            index -= refNode.childCount(m_nodeName);
        }
    }
    return Node();
}

// =============================================================================
// (public)
bool NodeQueryChildren::canCloneNode(const Node &refNode, int &index, const Node &cloneNode) const
{
    if (m_childQueryUPtr) {
        Node node = first(refNode);
        while (!node.isNull()) {
            if (m_childQueryUPtr->canCloneNode(node, index, cloneNode)) {
                return true;
            }
            node = next(refNode, node);
        }
    } else {
        if (refNode.canCloneChild(m_nodeName, index, cloneNode)) {
            return true;
        } else {
            index -= refNode.childCount(m_nodeName);
        }
    }
    return false;
}

// =============================================================================
// (public)
Node NodeQueryChildren::cloneNode(const Node &refNode, int &index, const Node &cloneNode) const
{
    if (m_childQueryUPtr) {
        Node node = first(refNode);
        while (!node.isNull()) {
            Node newNode = m_childQueryUPtr->cloneNode(node, index, cloneNode);
            if (!newNode.isNull()) {
                return newNode;
            }
            node = next(refNode, node);
        }
    } else {
        Node newNode = refNode.cloneChild(m_nodeName, index, cloneNode);
        if (!newNode.isNull()) {
            return newNode;
        } else {
            index -= refNode.childCount(m_nodeName);
        }
    }
    return Node();
}

// =============================================================================
// (public)
bool NodeQueryChildren::canMoveNode(const Node &refNode, int &index, const Node &moveNode) const
{
    if (m_childQueryUPtr) {
        Node node = first(refNode);
        while (!node.isNull()) {
            if (m_childQueryUPtr->canMoveNode(node, index, moveNode)) {
                return true;
            }
            node = next(refNode, node);
        }
    } else {
        if (refNode.canMoveChild(m_nodeName, index, moveNode)) {
            return true;
        } else {
            index -= refNode.childCount(m_nodeName);
        }
    }
    return false;
}

// =============================================================================
// (public)
Node NodeQueryChildren::moveNode(const Node &refNode, int &index, const Node &moveNode) const
{
    if (m_childQueryUPtr) {
        Node node = first(refNode);
        while (!node.isNull()) {
            Node newNode = m_childQueryUPtr->moveNode(node, index, moveNode);
            if (!newNode.isNull()) {
                return newNode;
            }
            node = next(refNode, node);
        }
    } else {
        Node newNode = refNode.moveChild(m_nodeName, index, moveNode);
        if (!newNode.isNull()) {
            return newNode;
        } else {
            index -= refNode.childCount(m_nodeName);
        }
    }
    return Node();
}

// =============================================================================
// (public)
bool NodeQueryChildren::canRemoveNode(const Node &refNode, int index) const
{
    if (m_childQueryUPtr) {
        Node node = first(refNode);
        while (!node.isNull()) {
            if (m_childQueryUPtr->canRemoveNode(node, index)) {
                return true;
            }
            node = next(refNode, node);
        }
    } else {
        if (refNode.canRemoveChild(m_nodeName, index)) {
            return true;
        } else {
            index -= refNode.childCount(m_nodeName);
        }
    }
    return false;
}

// =============================================================================
// (public)
bool NodeQueryChildren::removeNode(Node &refNode, int index)
{
    if (m_childQueryUPtr) {
        Node node = first(refNode);
        while (!node.isNull()) {
            if (m_childQueryUPtr->removeNode(node, index)) {
                return true;
            }
            node = next(refNode, node);
        }
    } else {
        if (refNode.removeChild(m_nodeName, index)) {
            return true;
        } else {
            index -= refNode.childCount(m_nodeName);
        }
    }
    return false;
}

// =============================================================================
// (protected)
Node NodeQueryChildren::first(const Node &refNode) const
{
    return refNode.firstChild(m_nodeName);
}

// =============================================================================
// (protected)
Node NodeQueryChildren::last(const Node &refNode) const
{
    return refNode.lastChild(m_nodeName);
}

// =============================================================================
// (protected)
Node NodeQueryChildren::next(const Node &refNode, const Node &cNode) const
{
    return refNode.nextChild(m_nodeName, cNode);
}

// =============================================================================
// (protected)
Node NodeQueryChildren::previous(const Node &refNode, const Node &cNode) const
{
    return refNode.previousChild(m_nodeName, cNode);
}

// =============================================================================
// (public)
const NodeDef *NodeQueryChildren::_nodeDef(const NodeDef *nDef) const
{
    if (nDef == nullptr) { return nullptr; }
    return nDef->childDef(m_nodeName, true, true);
}

} // namespace Oak::Model

