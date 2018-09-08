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
Item ItemQueryParent::first(const Item &refItem) const
{
    return refItem.parent();
}

// =============================================================================
// (public)
Item ItemQueryParent::last(const Item &refItem) const
{
    return refItem.parent();
}

} // namespace Model
} // namespace Oak
