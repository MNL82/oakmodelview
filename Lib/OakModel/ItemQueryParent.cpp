/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ItemQueryParent.h"

#include "../ServiceFunctions/Trace.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
ItemQueryParent::ItemQueryParent()
    : ItemQuery()
{

}

// =============================================================================
// (public)
ItemQueryParent::ItemQueryParent(const ItemQueryParent &copy)
    : ItemQuery(copy)
{

}

// =============================================================================
// (public)
ItemQueryParent::ItemQueryParent(ItemQueryParent &&move)
    : ItemQuery(move)
{

}

// =============================================================================
// (protected)
Item ItemQueryParent::first(const Item &refItem) const
{
    return refItem.parent();
}

// =============================================================================
// (protected)
Item ItemQueryParent::last(const Item &refItem) const
{
    return refItem.parent();
}

// =============================================================================
// (protected)
const NodeDef *ItemQueryParent::_nodeDef(const NodeDef *nDef) const
{
    // TODO: Node can have multiple parents! Fix this...
    ASSERT(nDef);
    ASSERT(nDef->parentContainerCount() == 1); // Do not work unleass there are only one parent container
    return nDef->parentContainer(0)->hostDef();
}

} // namespace Model
} // namespace Oak
