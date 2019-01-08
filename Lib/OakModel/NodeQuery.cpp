/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "NodeQuery.h"
#include "QueryBuilder.h"

#include <utility>
#include <iterator>

namespace Oak::Model {

// =============================================================================
// (public)
NodeQuery::NodeQuery()
{

}

// =============================================================================
// (public)
NodeQuery::NodeQuery(const NodeQuery& copy)
{
    m_childQueryUPtr = QueryBuilder::duplicate(copy.m_childQueryUPtr);
}

// =============================================================================
// (public)
NodeQuery::NodeQuery(NodeQuery&& move)
{
    m_childQueryUPtr = std::move(move.m_childQueryUPtr);
}

// =============================================================================
// (public)
NodeQuery::~NodeQuery()
{
    std::vector<std::string>::iterator it;
    std::vector<std::string> list;
}

// =============================================================================
// (public)
int NodeQuery::count(const Node &refNode)
{
    auto it = iterator(refNode);
    int count = 0;
    while (it->next()) {
        count++;
    }
    return count;
}

// =============================================================================
// (public)
std::vector<Node> NodeQuery::nodeList(const Node &refNode)
{
    std::vector<Node> nodeList;
    if (!m_childQueryUPtr) { return nodeList; }

    auto it = iterator(refNode);
    while (it->next()) {
        nodeList.push_back(it->node());
    }

    return nodeList;
}

// =============================================================================
// (public)
Node NodeQuery::previous(const Node &refNode, const Node &cNode) const
{
    UNUSED(refNode);
    UNUSED(cNode);
    return Node();
}

// =============================================================================
// (public)
const NodeQuery *NodeQuery::childQuery() const
{
    return m_childQueryUPtr.get();
}

// =============================================================================
// (public)
const NodeDef *NodeQuery::nodeDef(const NodeDef *nDef) const
{
    if (hasChildQuery()) {
        return m_childQueryUPtr->nodeDef(_nodeDef(nDef));
    }
    return _nodeDef(nDef);
}

// =============================================================================
// (public)
bool NodeQuery::canInsertNode(const Node &refNode, int &index) const
{
    if (m_childQueryUPtr) {
        Node node = first(refNode);
        while (!node.isNull()) {
            if (m_childQueryUPtr->canInsertNode(node, index)) {
                return true;
            }
            node = next(refNode, node);
        }
        return false;
    } else {
        // Default NodeQuery can not insert nodes
        return false;
    }
}

// =============================================================================
// (public)
Node NodeQuery::insertNode(const Node &refNode, int index) const
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
        return Node();
    } else {
        // Default NodeQuery can not insert nodes
        return Node();
    }
}

// =============================================================================
// (public)
bool NodeQuery::canCloneNode(const Node &refNode, int &index, const Node &cloneNode) const
{
    if (m_childQueryUPtr) {
        Node node = first(refNode);
        while (!node.isNull()) {
            if (m_childQueryUPtr->canCloneNode(node, index, cloneNode)) {
                return true;
            }
            node = next(refNode, node);
        }
        return false;
    } else {
        // Default NodeQuery can not clone nodes
        return false;
    }
}

// =============================================================================
// (public)
Node NodeQuery::cloneNode(const Node &refNode, int &index, const Node &cloneNode) const
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
        return Node();
    } else {
        // Default NodeQuery can not clone nodes
        return Node();
    }
}

// =============================================================================
// (public)
bool NodeQuery::canMoveNode(const Node &refNode, int &index, const Node &moveNode) const
{
    if (m_childQueryUPtr) {
        Node node = first(refNode);
        while (!node.isNull()) {
            if (m_childQueryUPtr->canMoveNode(node, index, moveNode)) {
                return true;
            }
            node = next(refNode, node);
        }
        return false;
    } else {
        // Default NodeQuery can not move nodes
        return false;
    }
}

// =============================================================================
// (public)
Node NodeQuery::moveNode(const Node &refNode, int &index, const Node &moveNode) const
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
        return Node();
    } else {
        // Default NodeQuery can not move nodes
        return Node();
    }
}

// =============================================================================
// (public)
bool NodeQuery::canRemoveNode(const Node &refNode, int index) const
{
    if (m_childQueryUPtr) {
        Node node = first(refNode);
        while (!node.isNull()) {
            if (m_childQueryUPtr->canRemoveNode(node, index)) {
                return true;
            }
            node = next(refNode, node);
        }
        return false;
    } else {
        // Default NodeQuery can not remove nodes
        return false;
    }
}

// =============================================================================
// (public)
bool NodeQuery::removeNode(Node &refNode, int index)
{
    if (m_childQueryUPtr) {
        Node node = first(refNode);
        while (!node.isNull()) {
            if (m_childQueryUPtr->removeNode(node, index)) {
                return true;
            }
            node = next(refNode, node);
        }
        return false;
    } else {
        // Default NodeQuery can not remove nodes
        return false;
    }
}

// =============================================================================
// (protected)
void NodeQuery::addChildQuery(NodeQueryUPtr query)
{
    if (m_childQueryUPtr) {
        m_childQueryUPtr->addChildQuery(std::move(query));
    } else {
        m_childQueryUPtr = std::move(query);
    }
}

// =============================================================================
// (protected)
bool NodeQuery::hasChildQuery() const
{
    return static_cast<bool>(m_childQueryUPtr);
}

// =============================================================================
// (protected)
Node NodeQuery::first(const Node &refNode) const
{
    return refNode;
}

// =============================================================================
// (protected)
Node NodeQuery::last(const Node &refNode) const
{
    return refNode;
}

// =============================================================================
// (protected)
Node NodeQuery::next(const Node &refNode, const Node &cNode) const
{
    UNUSED(refNode);
    UNUSED(cNode);
    return Node();
}

// =============================================================================
// (protected)
const NodeDef *NodeQuery::_nodeDef(const NodeDef *nDef) const
{
    return nDef;
}

// =============================================================================
// (public)
NodeQuery::IteratorUPtr NodeQuery::iterator(const Node &refNode) const
{
    IteratorUPtr it(new Iterator(*this, &refNode));
    return it;
}

// =============================================================================
// Iterator functions
// =============================================================================

// =============================================================================
// (public)
NodeQuery::Iterator::Iterator(const NodeQuery &query, const Node *refNode)
{
    m_query = &query;
    m_refNode = refNode;
    if (m_query && m_query->hasChildQuery()) {
        m_childIterator = new Iterator(*m_query->m_childQueryUPtr.get());
    } else {
        m_childIterator = nullptr;
    }
}

// =============================================================================
// (public)
NodeQuery::Iterator::~Iterator()
{
    if (m_childIterator) {
        delete m_childIterator;
        m_childIterator = nullptr;
    }
}

// =============================================================================
// (public)
bool NodeQuery::Iterator::isValid() const
{
    return !m_currentNode.isNull();
}

// =============================================================================
// (public)
bool NodeQuery::Iterator::next()
{
    if (m_currentNode.isNull()) {
        return first(*m_refNode);
    }

    if (!m_childIterator) { // No child query
        // Move next
        if (m_query) {
            m_currentNode = m_query->next(*m_refNode, m_currentNode);
        } else {
            m_currentNode.clear();
        }
        return isValid();
    }

    // Child query move next
    m_childIterator->next();

    while (!m_childIterator->isValid()) { // Child query not valid
        // Move next
        if (!m_query) { // Query is empty
            m_currentNode.clear();
            return isValid();
        }
        m_currentNode = m_query->next(*m_refNode, m_currentNode);
        if (m_currentNode.isNull()) { // End of Query
            return isValid();
        } else {
            // Initialize child query
            m_childIterator->first(m_currentNode);
        }
    }
    return isValid();
}

// =============================================================================
// (public)
bool NodeQuery::Iterator::previous()
{
    if (m_currentNode.isNull()) {
        return last(*m_refNode);
    }

    if (!m_childIterator) { // No child query
        // Move previous
        if (m_query) {
            m_currentNode = m_query->previous(*m_refNode, m_currentNode);
        } else {
            m_currentNode.clear();
        }
        return isValid();
    }

    // Child query move previous
    m_childIterator->previous();

    while (!m_childIterator->isValid()) { // Child query not valid
        // Move previous
        if (m_query) { // Query is empty
            m_currentNode.clear();
            return isValid();
        }
        m_currentNode = m_query->previous(*m_refNode, m_currentNode);
        if (m_currentNode.isNull()) { // End of Query
            return isValid();
        } else {
            // Initialize child query
            m_childIterator->last(m_currentNode);
        }
    }
    return isValid();
}

// =============================================================================
// (public)
const Node &NodeQuery::Iterator::node() const
{
    if (m_childIterator) {
        return m_childIterator->node();
    }
    return m_currentNode;
}

// =============================================================================
// (public)
bool NodeQuery::Iterator::first(const Node &refNode)
{
    m_refNode = &refNode;

    if (m_query) {
        m_currentNode = m_query->first(*m_refNode);
    } else {
        m_currentNode = *m_refNode;
    }

    if (m_currentNode.isNull()) { return false; }

    if (!m_childIterator) { return true; }

    if (m_childIterator->first(m_currentNode)) { return true; }

    m_currentNode.clear();

    return false;
}

// =============================================================================
// (public)
bool NodeQuery::Iterator::last(const Node &refNode)
{
    m_refNode = &refNode;

    if (m_query) {
        m_currentNode = m_query->last(*m_refNode);
    } else {
        m_currentNode = *m_refNode;
    }

    if (m_currentNode.isNull()) { return false; }

    if (!m_childIterator) { return true; }

    if (m_childIterator->last(m_currentNode)) { return true; }

    m_currentNode.clear();

    return false;
}

} // namespace Oak::Model

