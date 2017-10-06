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

#include "Node.h"

namespace Oak {
namespace Model {

// =============================================================================
// Class definition
// =============================================================================
class NodeRef
{
public:
    virtual Node getNode(const Node& baseNode, bool* ok = 0) const = 0;
};

} // namespace Model
} // namespace Oak
