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
    ItemQueryChildren(const ItemQueryChildren& copy);
    ItemQueryChildren(ItemQueryChildren&& move);

    virtual bool canInsertItem(const Item &refItem, int &index) const override;
    virtual Item insertItem(const Item &refItem, int index) const override;

    virtual bool canCloneItem(const Item &refItem, int &index, const Item &cloneItem) const override;
    virtual Item cloneItem(const Item &refItem, int &index, const Item &cloneItem) const override;

    virtual bool canMoveItem(const Item &refItem, int &index, const Item &moveItem) const override;
    virtual Item moveItem(const Item &refItem, int &index, const Item &moveItem) const override;

    virtual bool canRemoveItem(const Item &refItem, int index) const override;
    virtual bool removeItem(const Item &refItem, int index) const override;
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
