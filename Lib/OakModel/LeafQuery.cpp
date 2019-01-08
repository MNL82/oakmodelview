/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "LeafQuery.h"

#include "NodeQueryChildren.h"
#include "NodeQueryParent.h"

#include "../ServiceFunctions/Assert.h"


namespace Oak::Model {

// =============================================================================
// (protected)
LeafQuery::LeafQuery(const std::string &leafName)
{
    m_leafName = leafName;
}

// =============================================================================
// (public)
LeafQuery::LeafQuery(NodeQueryUPtr nodeQueryUPtr, const std::string &leafName)
{
    m_nodeQueryPtr = std::move(nodeQueryUPtr);
    m_leafName = leafName;
}

// =============================================================================
// (public)
LeafQuery::~LeafQuery()
{
}

// =============================================================================
// (public)
const std::string &LeafQuery::valueName() const
{
    return m_leafName;
}

// =============================================================================
// (public)
LeafQuerySPtr LeafQuery::setValueName(const std::string &leafName)
{
    m_leafName = leafName;
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
int LeafQuery::count(const Node &node)
{
    if (!m_nodeQueryPtr) { return 1; }
    return m_nodeQueryPtr->count(node);
}

// =============================================================================
// (public)
const Leaf &LeafQuery::leaf(const Node &node, int index) const
{
    ASSERT(!m_leafName.empty());

    int i = 0;
    auto it = iterator(node);
    while (it->next()) {
        if (i == index) { return it->leaf(); }
        i++;
    }
    return Leaf::emptyLeaf();
}

// =============================================================================
// (public)
void LeafQuery::getValueList(const Node &node, std::vector<UnionValue> &valueList) const
{
    ASSERT(!m_leafName.empty());

    if (m_nodeQueryPtr) {
        auto it = m_nodeQueryPtr->iterator(node);
        while (it->next()) {
            auto tempNode = it->node();
            if (tempNode.hasLeaf(m_leafName)) {
                valueList.push_back(tempNode.leaf(m_leafName).value());
            }
        }
    } else {
        if (node.hasLeaf(m_leafName)) {
            valueList.push_back(node.leaf(m_leafName).value());
        }
    }
}

// =============================================================================
// (public)
std::vector<UnionValue> LeafQuery::valueList(const Node &node) const
{
    std::vector<UnionValue> valueList;
    getValueList(node, valueList);
    return valueList;
}

// =============================================================================
// (public)
void LeafQuery::getValue(const Node &node, int index, UnionValue value) const
{
    ASSERT(!m_leafName.empty());

    if (m_nodeQueryPtr) {
        int i = 0;
        auto it = m_nodeQueryPtr->iterator(node);
        while (it->next()) {
            if (i == index) {
                auto tempNode = it->node();
                if (tempNode.hasLeaf(m_leafName)) {
                    node.leaf(m_leafName).getValue(value);
                }
                return;
            }
            i++;
        }
        ASSERT(false);
    } else {
        ASSERT(index == 0);
        if (node.hasLeaf(m_leafName)) {
            node.leaf(m_leafName).getValue(value);
        }
    }
}

// =============================================================================
// (public)
std::vector<Node> LeafQuery::toNodeList(const Node &node)
{
    std::vector<Node> nodeList;
    if (!m_nodeQueryPtr) { return nodeList; }

    auto it = m_nodeQueryPtr->iterator(node);
    while (it->next()) {
        nodeList.push_back(it->node());
    }

    return nodeList;
}

// =============================================================================
// (public)
const NodeQuery &LeafQuery::nodeQuery() const
{
    return *m_nodeQueryPtr.get();
}

// =============================================================================
// (public static)
LeafQuerySPtr LeafQuery::create(const std::string &valueName)
{
    LeafQuerySPtr sPtr = LeafQuerySPtr(new LeafQuery(valueName));
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

// =============================================================================
// (public static)
LeafQuerySPtr LeafQuery::create(NodeQueryUPtr nodeQueryUPtr, const std::string &leafName)
{
    LeafQuerySPtr sPtr = LeafQuerySPtr(new LeafQuery(std::move(nodeQueryUPtr), leafName));
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

// =============================================================================
// (public)
LeafQuery::IteratorUPtr LeafQuery::iterator(const Node &refNode) const
{
    IteratorUPtr it(new Iterator(*this, &refNode));
    return it;
}

//// =============================================================================
//// (public)
//LeafQuery::IteratorUPtr LeafQuery::begin(const Node &refNode) const
//{
//    IteratorUPtr it(new Iterator(*this));
//    it->first(refNode);
//    return it;
//}

//// =============================================================================
//// (public)
//LeafQuery::IteratorUPtr LeafQuery::rBegin(const Node &refNode) const
//{
//    IteratorUPtr it(new Iterator(*this));
//    it->last(refNode);
//    return it;
//}

// =============================================================================
// Iterator functions
// =============================================================================

// =============================================================================
// (public)
LeafQuery::Iterator::Iterator(const LeafQuery &leafQuery, const Node *refNode)
    : NodeQuery::Iterator(leafQuery.nodeQuery())
{
    m_leafQuery = &leafQuery;
    m_refNode = refNode;
}

// =============================================================================
// (public)
LeafQuery::Iterator::~Iterator()
{
    m_leafQuery = nullptr;
    NodeQuery::Iterator::~Iterator();
}

// =============================================================================
// (public)
void LeafQuery::Iterator::getValue(UnionValue value) const
{
    leaf().getValue(value);
}

// =============================================================================
// (public)
const Leaf &LeafQuery::Iterator::leaf() const
{
    ASSERT(!m_leafQuery->m_leafName.empty());
    return this->node().leaf(m_leafQuery->m_leafName);
}

} // namespace Oak::Model

