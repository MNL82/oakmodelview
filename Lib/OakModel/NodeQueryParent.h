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

// =============================================================================
// Class definition
// =============================================================================
class NodeQueryParent : public NodeQuery
{
public:
    NodeQueryParent();
    NodeQueryParent(const NodeQueryParent& copy);
    NodeQueryParent(NodeQueryParent&& move);

protected:
    virtual Node first(const Node &refNode) const override;
    virtual Node last(const Node &refNode) const override;

    virtual const NodeDef *_nodeDef(const NodeDef *nDef) const override;
};

} // namespace Oak::Model

