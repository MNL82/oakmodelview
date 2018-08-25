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

#include "ValueQuery.h"

namespace Oak {
namespace Model {

class TableQuery;
typedef std::shared_ptr<TableQuery> TableQuerySPtr;
typedef std::weak_ptr<TableQuery> TableQueryWPtr;

// =============================================================================
// Class definition
// =============================================================================
class TableQuery
{
protected:
    TableQuery(ItemQueryUPtr itemQuery);


};

} // namespace Model
} // namespace Oak

