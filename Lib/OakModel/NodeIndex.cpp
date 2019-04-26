/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "NodeIndex.h"

#include "../ServiceFunctions/Assert.h"
#include <sstream>

namespace Oak::Model {

NodeIndex NodeIndex::s_nodeIndex = NodeIndex();

// =============================================================================
// (public)
NodeIndex::NodeIndex()
{
    m_index = -1;
    m_childIndex = nullptr;
}

// =============================================================================
// (public)
NodeIndex::NodeIndex(int index)
{
    m_index = index;
    m_childIndex = nullptr;
}

// =============================================================================
// (public)
NodeIndex::NodeIndex(const std::string &name, int index)
{
    m_name = name;
    m_index = index;
    m_childIndex = nullptr;
}

// =============================================================================
// (public)
NodeIndex::NodeIndex(const NodeIndex &nodeIndex)
{
    m_name = nodeIndex.m_name;
    m_index = nodeIndex.m_index;
    if (nodeIndex.m_childIndex) {
        m_childIndex = new NodeIndex(*nodeIndex.m_childIndex);
    } else {
        m_childIndex = nullptr;
    }
}

// =============================================================================
// (public)
NodeIndex::~NodeIndex()
{
    if (m_childIndex) {
        delete m_childIndex;
        m_childIndex = nullptr;
    }
}

// =============================================================================
// (public)
bool NodeIndex::isNull() const
{
    return m_index == -1;
}

// =============================================================================
// (public)
bool NodeIndex::isNamed(bool recursive) const
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
bool NodeIndex::isUnnamed(bool recursive) const
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
bool NodeIndex::equal(const NodeIndex &nodeIndex) const
{
    const NodeIndex *ii1 = this;
    const NodeIndex *ii2 = &nodeIndex;
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
int NodeIndex::depthWhereEqual(const NodeIndex &nodeIndex) const
{
    const NodeIndex *ii1 = this;
    const NodeIndex *ii2 = &nodeIndex;
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
const std::string &NodeIndex::name() const
{
    return m_name;
}

// =============================================================================
// (public)
int NodeIndex::index() const
{
    return m_index;
}

// =============================================================================
// (public)
int NodeIndex::depth() const
{
    int d = 1;
    const NodeIndex *nodeIndex = this;
    while(nodeIndex->m_childIndex) {
        nodeIndex = nodeIndex->m_childIndex;
        d++;
    }
    return d;
}

// =============================================================================
// (public)
bool NodeIndex::contains(const NodeIndex &nodeIndex) const
{
    if (isNull() ||depth() < nodeIndex.depth()) { return false; }

    const NodeIndex *ii1 = this;
    const NodeIndex *ii2 = &nodeIndex;
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
Node NodeIndex::node(const Node &rootNode, int depth) const
{
    if (isNull()) { return rootNode; }
    Node node = (m_name.empty()) ? rootNode.childAt(m_index) : rootNode.childAt(m_name, m_index);
    if (!m_childIndex || depth == 1 || node.isNull()) {
        ASSERT(depth == 1 || depth < -1);
        return node;
    }
    return m_childIndex->node(node, --depth);
}

// =============================================================================
// (public)
Node NodeIndex::nodeParent(const Node &rootNode) const
{
    if (isNull()) { return Node(); }
    if (m_childIndex == nullptr) { return rootNode; }
    Node node = (m_name.empty()) ? rootNode.childAt(m_index) : rootNode.childAt(m_name, m_index);
    if (m_childIndex->hasChildNodeIndex()) {
        return m_childIndex->nodeParent(node);
    }
    return node;
}

// =============================================================================
// (public)
bool NodeIndex::hasChildNodeIndex() const
{
    return m_childIndex != nullptr;
}

// =============================================================================
// (public)
const NodeIndex &NodeIndex::childNodeIndex(int depth) const
{
    if (depth == 0) { return *this; }
    if (m_childIndex) {
        return m_childIndex->childNodeIndex(--depth);
    } else {
        return emptyNodeIndex();
    }
}

// =============================================================================
// (public)
NodeIndex &NodeIndex::childNodeIndex(int depth)
{
    if (depth == 0) { return *this; }
    if (m_childIndex) {
        return m_childIndex->childNodeIndex(--depth);
    } else {
        return emptyNodeIndex();
    }
}

// =============================================================================
// (public)
const NodeIndex &NodeIndex::lastNodeIndex() const
{
    if (m_childIndex) {
        return m_childIndex->lastNodeIndex();
    } else {
        return *this;
    }
}

// =============================================================================
// (public)
NodeIndex &NodeIndex::lastNodeIndex()
{
    if (m_childIndex) {
        return m_childIndex->lastNodeIndex();
    } else {
        return *this;
    }
}

// =============================================================================
// (public)
NodeIndex &NodeIndex::emptyNodeIndex()
{
    return s_nodeIndex;
}

// =============================================================================
// (public)
NodeIndexUPtr NodeIndex::create(const Node &_node, bool namedIndex)
{
    if (_node.isNull()) { return NodeIndexUPtr(); }
    Node cNode = _node;
    Node pNode = cNode.parent();

    NodeIndex *cIndex = nullptr;
    NodeIndex *pIndex = nullptr;
    while(!pNode.isNull()) {
        pIndex = new NodeIndex();
        if (namedIndex) {
            pIndex->m_name = cNode.def()->name();
            pIndex->m_index = pNode.childIndex(pIndex->m_name, cNode);
        } else {
            pIndex->m_index = pNode.childIndex(cNode);
        }
        if (pIndex->m_index == -1) {  // If node is invalid (F.eks can be invalid after a move)
            return NodeIndexUPtr();
        };
        pIndex->m_childIndex = cIndex;
        cIndex = pIndex;
        cNode = pNode;
        pNode = cNode.parent();
    }
    return NodeIndexUPtr(cIndex);
}

// =============================================================================
// (public)
void NodeIndex::setChildNodeIndex(NodeIndex *nodeIndex)
{
    if (m_childIndex != nullptr) {
        delete m_childIndex;
    }
    m_childIndex = nodeIndex;
}

// =============================================================================
// (public)
int NodeIndex::convertIndexToUnnamed(const Node &_node) const
{
    if (m_name.empty()) {
        return m_index;
    } else {
        return _node.convertChildIndexToUnnamed(m_name, m_index);
    }
}

// =============================================================================
// (public)
int NodeIndex::convertIndexToNamed(const Node &_node, std::string &name) const
{
    if (m_name.empty()) {
        return _node.convertChildIndexToNamed(name, m_index);
    } else {
        name = m_name;
        return m_index;
    }
}

// =============================================================================
// (public)
std::string NodeIndex::toString() const
{
    std::stringstream ss;
    toString(ss);
    return ss.str();
}

// =============================================================================
// (public)
void NodeIndex::toString(std::stringstream &ss) const
{
    if (m_name.empty()) {
        ss << "[" << std::to_string(m_index) << "]";
    } else {
        ss << m_name << "[" << m_index << "]";
    }
    if (m_childIndex) {
        ss << ";";
        m_childIndex->toString(ss);
    }
}

} // namespace Oak::Model

