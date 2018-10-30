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

namespace Oak {
namespace Model {

class QueryBuilder;
typedef std::shared_ptr<QueryBuilder> QueryBuilderSPtr;
typedef std::weak_ptr<QueryBuilder> QueryBuilderWPtr;

// =============================================================================
// Class definition
// =============================================================================
class QueryBuilder
{
protected:
    QueryBuilder();

public:
    ItemQueryUPtr UPtr();
    EntryQuerySPtr ValueUPtr(const std::string &entryName);

    QueryBuilderSPtr children(const std::string &nodeName);
    QueryBuilderSPtr parent();
    QueryBuilderSPtr siblings();

    static QueryBuilderSPtr createChildren(const std::string &nodeName);
    static QueryBuilderSPtr createParent();
    static QueryBuilderSPtr createSiblings();

    static EntryQuerySPtr createEntry(const std::string &entryName);

    static ItemQueryUPtr duplicate(const ItemQueryUPtr &c);

protected:
    ItemQueryUPtr m_itemQuery;
    QueryBuilderWPtr m_thisWPtr;
};

typedef QueryBuilder QB;

} // namespace Model
} // namespace Oak
