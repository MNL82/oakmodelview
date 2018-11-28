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

#include "ItemQuery.h"


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
    ItemQueryUPtr UPtr();
    EntryQuerySPtr EntrySPtr(const std::string &entryName);

    QueryBuilderSPtr children(const std::string &nodeName, bool invertOrder = false);
    QueryBuilderSPtr parent(bool invertOrder = false);
    QueryBuilderSPtr siblings(bool invertOrder = false);

    static QueryBuilderSPtr createChildren(const std::string &nodeName);
    static QueryBuilderSPtr createParent();
    static QueryBuilderSPtr createSiblings();

    static QueryBuilderSPtr createInverse(const ItemQuery &query, const NodeDef *sourceNodeDef);

    static EntryQuerySPtr createEntry(const std::string &entryName);

    static ItemQueryUPtr duplicate(const ItemQueryUPtr &c);

    static ItemQueryUPtr createItemQuery(const std::string& queryString);
    static EntryQuerySPtr createEntryQuery(const std::string& queryString);

protected:
    ItemQueryUPtr m_itemQuery;
    QueryBuilderWPtr m_thisWPtr;
};

typedef QueryBuilder QB;

} // namespace Oak::Model

