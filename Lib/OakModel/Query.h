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

#include "Item.h"

namespace Oak {
namespace Model {

// =============================================================================
// Class definition
// =============================================================================
class Query
{
public:
    Query();
    virtual ~Query();

    void reset(Item refItem);
    bool moveNext();
    const Item &current() const;

    void add(Query *query);

protected:
    virtual bool moveCurrentNext() = 0;

protected:
    Item m_refItem;
    Item m_currentItem;
    Query *m_childQuery = nullptr;
};

} // namespace Model
} // namespace Oak
