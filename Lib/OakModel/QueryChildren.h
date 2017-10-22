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

// =============================================================================
// Class definition
// =============================================================================
class QueryChildren : public Query
{
public:
    QueryChildren(const std::string &nodeName);

protected:
    virtual bool moveCurrentNext() override;

protected:
    std::string m_nodeName;
};

} // namespace Model
} // namespace Oak
