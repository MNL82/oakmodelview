/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "TableQuery.h"

#include "../ServiceFunctions/Assert.h"


namespace Oak::Model {

// =============================================================================
// (public)
TableQuery::TableQuery()
{

}

// =============================================================================
// (public)
TableQuery::TableQuery(NodeQueryUPtr nodeQuery)
{
    if (nodeQuery) {
        m_nodeQuery = std::move(nodeQuery);
    }
}

// =============================================================================
// (public)
void TableQuery::setNodeQuery(NodeQueryUPtr nodeQuery)
{
    m_nodeQuery = std::move(nodeQuery);
}

// =============================================================================
// (public)
int TableQuery::columnCount() const
{
    return static_cast<int>(m_leafList.size());
}

// =============================================================================
// (public)
void TableQuery::addValueQuery(LeafQuerySPtr valueQuery)
{
    m_leafList.push_back(valueQuery);
}

// =============================================================================
// (public)
int TableQuery::count(const Node &node) const
{
    ASSERT(m_nodeQuery);
    return m_nodeQuery->count(node);
}

// =============================================================================
// (public)
NodeQuery &TableQuery::nodeQuery()
{
    return *m_nodeQuery.get();
}

// =============================================================================
// (public)
const NodeQuery &TableQuery::nodeQuery() const
{
    return *m_nodeQuery.get();
}

// =============================================================================
// (public)
TableQuery::IteratorUPtr TableQuery::iterator(const Node &refNode) const
{
    IteratorUPtr it(new Iterator(*this, &refNode));
    return it;
}

//// =============================================================================
//// (public)
//TableQuery::IteratorUPtr TableQuery::begin(const Node &refNode) const
//{
//    IteratorUPtr it(new Iterator(*this));
//    it->first(refNode);
//    return it;
//}

//// =============================================================================
//// (public)
//TableQuery::IteratorUPtr TableQuery::rBegin(const Node &refNode) const
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
TableQuery::Iterator::Iterator(const TableQuery &tableQuery, const Node *refNode)
    : NodeQuery::Iterator(tableQuery.nodeQuery())
{
    m_tableQuery = &tableQuery;
    m_refNode = refNode;

    size_t count = static_cast<size_t>(m_tableQuery->columnCount());
    for (size_t i = 0; i < count; i++)
    {
        m_leafIteratorList.push_back(new LeafQuery::Iterator(*m_tableQuery->m_leafList[i].get()));
    }
}

// =============================================================================
// (public)
TableQuery::Iterator::~Iterator()
{
    for (auto eIt: m_leafIteratorList)
    {
        delete eIt;
    }
    m_leafIteratorList.clear();
    m_tableQuery = nullptr;
    NodeQuery::Iterator::~Iterator();
}

// =============================================================================
// (public)
const Leaf &TableQuery::Iterator::leaf(int index) const
{
    if (!isValid() ||
        index >= static_cast<int>(m_leafIteratorList.size())) {
        return Leaf::emptyLeaf();
    }

    auto eIt = m_leafIteratorList[static_cast<size_t>(index)];
    Node i = node();
    if (eIt->first(i)) {
        return eIt->leaf();
    }
    return Leaf::emptyLeaf();
}

// =============================================================================
// (public)
void TableQuery::Iterator::getValue(int index, UnionValue value) const
{
    const Leaf &e = leaf(index);
    if (e.isNull()) { return; }
    e.getValue(value);
}

} // namespace Oak::Model

