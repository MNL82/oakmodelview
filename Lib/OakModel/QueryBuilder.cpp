/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "QueryBuilder.h"

#include "NodeQueryChildren.h"
#include "NodeQueryParent.h"
#include "NodeQuerySiblings.h"
#include "LeafQuery.h"

#include "../ServiceFunctions/Trace.h"
#include "OakModelServiceFunctions.h"

namespace Oak::Model {

// =============================================================================
// (public)
QueryBuilder::QueryBuilder()
{

}

// =============================================================================
// (public)
NodeQueryUPtr QueryBuilder::UPtr()
{
    return std::move(m_nodeQuery);
}

// =============================================================================
// (public)
LeafQuerySPtr QueryBuilder::leafSPtr(const std::string &leafName)
{
    return LeafQuery::create(std::move(m_nodeQuery), leafName);
}

// =============================================================================
// (public)
QueryBuilderSPtr QueryBuilder::children(const std::string &nodeName, bool invertOrder)
{
    if (m_nodeQuery) {
        if (invertOrder) {
            NodeQueryUPtr tempQuery = std::make_unique<NodeQueryChildren>(nodeName);
            tempQuery->addChildQuery(std::move(m_nodeQuery));
            m_nodeQuery = std::move(tempQuery);
        } else {
            m_nodeQuery->addChildQuery(std::make_unique<NodeQueryChildren>(nodeName));
        }
    } else {
        m_nodeQuery = std::make_unique<NodeQueryChildren>(nodeName);
    }
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
QueryBuilderSPtr QueryBuilder::parent(bool invertOrder)
{
    if (m_nodeQuery) {
        if (invertOrder) {
            NodeQueryUPtr tempQuery = std::make_unique<NodeQueryParent>();
            tempQuery->addChildQuery(std::move(m_nodeQuery));
            m_nodeQuery = std::move(tempQuery);
        } else {
            m_nodeQuery->addChildQuery(std::make_unique<NodeQueryParent>());
        }
    } else {
        m_nodeQuery = std::make_unique<NodeQueryParent>();
    }
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
QueryBuilderSPtr QueryBuilder::siblings(bool invertOrder)
{
    if (m_nodeQuery) {
        if (invertOrder) {
            NodeQueryUPtr tempQuery = std::make_unique<NodeQuerySiblings>();
            tempQuery->addChildQuery(std::move(m_nodeQuery));
            m_nodeQuery = std::move(tempQuery);
        } else {
            m_nodeQuery->addChildQuery(std::make_unique<NodeQuerySiblings>());
        }
    } else {
        m_nodeQuery = std::make_unique<NodeQuerySiblings>();
    }
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
QueryBuilderSPtr QueryBuilder::createChildren(const std::string &nodeName)
{
    QueryBuilderSPtr sPtr = std::make_shared<QueryBuilder>();
    sPtr->children(nodeName);
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

// =============================================================================
// (public)
QueryBuilderSPtr QueryBuilder::createParent()
{
    QueryBuilderSPtr sPtr = std::make_shared<QueryBuilder>();
    sPtr->parent();
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

// =============================================================================
// (public)
QueryBuilderSPtr QueryBuilder::createSiblings()
{
    QueryBuilderSPtr sPtr = std::make_shared<QueryBuilder>();
    sPtr->siblings();
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

// =============================================================================
// (public)
QueryBuilderSPtr QueryBuilder::createInverse(const NodeQuery &query, const NodeDef *sourceNodeDef)
{
    ASSERT(sourceNodeDef);
    QueryBuilderSPtr sPtr = std::make_shared<QueryBuilder>();
    sPtr->m_thisWPtr = sPtr;

    const NodeQuery * q = &query;
    const NodeDef *nodeDef = sourceNodeDef;

    while (q != nullptr) {
        const NodeQueryChildren * nodeQueryChildren = dynamic_cast<const NodeQueryChildren *>(q);
        if (nodeQueryChildren) {
            sPtr->parent(true);
        }
        const NodeQueryParent * nodeQueryParent = dynamic_cast<const NodeQueryParent *>(q);
        if (nodeQueryParent) {
            sPtr->children(nodeDef->name(), true);
        }
        const NodeQuerySiblings * nodeQuerySiblings = dynamic_cast<const NodeQuerySiblings *>(q);
        if (nodeQuerySiblings) {
            sPtr->siblings(true);
        }

        nodeDef = q->_nodeDef(nodeDef);
        q = q->childQuery();
    }

    return sPtr;
}

// =============================================================================
// (public)
LeafQuerySPtr QueryBuilder::createLeaf(const std::string &leafName)
{
    return LeafQuery::create(leafName);
}

// =============================================================================
// (public)
NodeQueryUPtr QueryBuilder::duplicate(const NodeQueryUPtr &c)
{
    {
        const NodeQueryChildren * nodeQuery = dynamic_cast<const NodeQueryChildren * >(c.get());
        if (nodeQuery) {
            return NodeQueryUPtr(new NodeQueryChildren(*nodeQuery));
        }
    }

    {
        const NodeQueryParent * nodeQuery = dynamic_cast<const NodeQueryParent * >(c.get());
        if (nodeQuery) {
            return NodeQueryUPtr(new NodeQueryParent(*nodeQuery));
        }
    }

    {
        const NodeQuerySiblings * nodeQuery = dynamic_cast<const NodeQuerySiblings * >(c.get());
        if (nodeQuery) {
            return NodeQueryUPtr(new NodeQuerySiblings(*nodeQuery));
        }
    }

    return NodeQueryUPtr(new NodeQuery(*c.get()));
}

// =============================================================================
// (public)
NodeQueryUPtr QueryBuilder::createNodeQuery(const std::string& queryString)
{
    std::vector<std::string> strList = split(queryString, ';', true);

    QueryBuilderSPtr builder = std::make_shared<QueryBuilder>();

    for (const std::string& str: strList) {

        auto it1 = std::find(str.begin(), str.end(), '{');
        auto it2 = std::find(str.begin(), str.end(), '}');

        std::string type(str.begin(), it1);

        if (type == "C" || type == "Children") {
            ASSERT(it1 != it2);
            std::string name(it1+1, it2);
            builder->children(name);
        } else if (type == "P" || type == "Parent") {
            builder->parent();
        } else if (type == "S" || type == "Siblings") {
            builder->siblings();
        }
    }
    return builder->UPtr();
}

// =============================================================================
// (public)
LeafQuerySPtr QueryBuilder::createLeafQuery(const std::string& queryString)
{
    if (queryString.empty()) { return LeafQuerySPtr(); }

    auto it = std::find(queryString.rbegin(), queryString.rend(), ';');

    if (it == queryString.rend()) {
        // No NodeQuery part
        return createLeaf(queryString);
    }

    size_t index = static_cast<size_t>(queryString.rend() - it);
    std::string nodeQueryString = queryString.substr(0, index-1);
    std::string leafName = queryString.substr(index);


    return LeafQuery::create(createNodeQuery(nodeQueryString), leafName);
}

} // namespace Oak::Model

