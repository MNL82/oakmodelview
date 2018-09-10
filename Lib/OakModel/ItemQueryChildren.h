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
class ItemQueryChildren : public ItemQuery
{
public:
    ItemQueryChildren(const std::string &nodeName);

    virtual Item insertItem(const Item &refItem, int index = 0) const override;

protected:
    virtual Item first(const Item &refItem) const override;
    virtual Item last(const Item &refItem) const override;
    virtual Item next(const Item &refItem, const Item &cItem) const override;
    virtual Item previous(const Item &refItem, const Item &cItem) const override;

protected:
    std::string m_nodeName;
};

} // namespace Model
} // namespace Oak
