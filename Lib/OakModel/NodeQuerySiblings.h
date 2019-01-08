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
class NodeQuerySiblings : public NodeQuery
{
public:
    NodeQuerySiblings();
    NodeQuerySiblings(const NodeQuerySiblings& copy);
    NodeQuerySiblings(NodeQuerySiblings&& move);

protected:
    virtual Node first(const Node &refNode) const override;
    virtual Node last(const Node &refNode) const override;
    virtual Node next(const Node &refNode, const Node &cNode) const override;
    virtual Node previous(const Node &refNode, const Node &cNode) const override;

    virtual const NodeDef * _nodeDef(const NodeDef *nDef) const override;

protected:
    mutable Node m_parent;
};

} // namespace Oak::Model

