/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#pragma once

#include "NodeQuery.h"


namespace Oak::Model {

class QueryBuilder;
typedef std::shared_ptr<QueryBuilder> QueryBuilderSPtr;
typedef std::weak_ptr<QueryBuilder> QueryBuilderWPtr;

// =============================================================================
// Class definition
// =============================================================================
class QueryBuilder
{
public:
    QueryBuilder();
    NodeQueryUPtr UPtr();
    LeafQuerySPtr leafSPtr(const std::string &leafName);

    QueryBuilderSPtr children(const std::string &nodeName, bool invertOrder = false);
    QueryBuilderSPtr parent(bool invertOrder = false);
    QueryBuilderSPtr siblings(bool invertOrder = false);

    static QueryBuilderSPtr createChildren(const std::string &nodeName);
    static QueryBuilderSPtr createParent();
    static QueryBuilderSPtr createSiblings();

    static QueryBuilderSPtr createInverse(const NodeQuery &query, const NodeDef *sourceNodeDef);

    static LeafQuerySPtr createLeaf(const std::string &leafName);

    static NodeQueryUPtr duplicate(const NodeQueryUPtr &c);

    static NodeQueryUPtr createNodeQuery(const std::string& queryString);
    static LeafQuerySPtr createLeafQuery(const std::string& queryString);

protected:
    NodeQueryUPtr m_nodeQuery;
    QueryBuilderWPtr m_thisWPtr;
};

typedef QueryBuilder QB;

} // namespace Oak::Model

