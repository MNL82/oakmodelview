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

// =============================================================================
// Class definition
// =============================================================================
class ItemQueryParent : public ItemQuery
{
public:
    ItemQueryParent();
    ItemQueryParent(const ItemQueryParent& copy);
    ItemQueryParent(ItemQueryParent&& move);

protected:
    virtual Item first(const Item &refItem) const override;
    virtual Item last(const Item &refItem) const override;

    virtual const NodeDef *_nodeDef(const NodeDef *nDef) const override;
};

} // namespace Model
} // namespace Oak
