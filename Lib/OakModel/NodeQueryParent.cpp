/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "NodeQueryParent.h"

#include "../ServiceFunctions/Trace.h"


namespace Oak::Model {

// =============================================================================
// (public)
NodeQueryParent::NodeQueryParent()
    : NodeQuery()
{

}

// =============================================================================
// (public)
NodeQueryParent::NodeQueryParent(const NodeQueryParent &copy)
    : NodeQuery(copy)
{

}

// =============================================================================
// (public)
NodeQueryParent::NodeQueryParent(NodeQueryParent &&move)
    : NodeQuery(move)
{

}

// =============================================================================
// (protected)
Node NodeQueryParent::first(const Node &refNode) const
{
    return refNode.parent();
}

// =============================================================================
// (protected)
Node NodeQueryParent::last(const Node &refNode) const
{
    return refNode.parent();
}

// =============================================================================
// (protected)
const NodeDef *NodeQueryParent::_nodeDef(const NodeDef *nDef) const
{
    // TODO: Node can have multiple parents! Fix this...
    ASSERT(nDef);
    ASSERT(nDef->parentContainerCount() == 1); // Do not work unleass there are only one parent container
    return nDef->parentContainer(0)->hostDef();
}

} // namespace Oak::Model

