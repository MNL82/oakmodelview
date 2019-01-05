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

#include "NodeData.h"


namespace Oak::Model {

// =============================================================================
// Class definition
// =============================================================================
class NodeRef
{
public:
    virtual ~NodeRef() {}
    virtual NodeData getNode(const NodeData& baseNode, bool* ok = nullptr) const = 0;
};

} // namespace Oak::Model

