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

#include "Query.h"

namespace Oak {
namespace Model {

class QueryBase;
typedef std::shared_ptr<QueryBase> QueryBaseSPtr;
typedef std::weak_ptr<QueryBase> QueryBaseWPtr;

// =============================================================================
// Class definition
// =============================================================================
class QueryBase
{
protected:
    QueryBase(Item item);

public:
    ~QueryBase();

    QueryBaseSPtr children(const std::string nodeName);

    int count();

    static QueryBaseSPtr MakeSPtr(Item item);

protected:
    void add(Query * query);

protected:
    Item m_item;
    Query *m_queryPtr = nullptr;

    QueryBaseWPtr m_thisWPtr;
};

} // namespace Model
} // namespace Oak
