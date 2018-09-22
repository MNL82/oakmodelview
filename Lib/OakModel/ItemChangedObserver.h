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

class ItemChangedObserver;
typedef std::unique_ptr<ItemChangedObserver> ItemChangedObserverUPtr;

// =============================================================================
// Class definition
// =============================================================================
class ItemChangedObserver
{
public:
    ItemChangedObserver();
};

} // namespace Model
} // namespace Oak
