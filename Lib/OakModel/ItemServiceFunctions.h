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
#include "LeafQuery.h"


namespace Oak::Model {

typedef std::pair<const NodeDef *, const LeafDef *> NodeLeafDefPair;

// =============================================================================
// Service Functions
// =============================================================================
void findOptionQueries(const NodeDef *def, std::vector<NodeLeafDefPair> &queryList, std::vector<NodeLeafDefPair> &queryExcludedList, bool recursive = false);

} // namespace Oak::Model

