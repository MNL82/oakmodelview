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
#include "EntryQuery.h"

namespace Oak {
namespace Model {

typedef std::pair<const NodeDef *, const ValueDef *> NodeValueDefPair;

// =============================================================================
// Service Functions
// =============================================================================
void findOptionQueries(const NodeDef *def, std::vector<NodeValueDefPair> &queryList, std::vector<NodeValueDefPair> &queryExcludedList, bool recursive = false);

void getValueDefList(std::vector<const ValueDef*>& vList, const NodeDef *def);
void getChildNodeDefList(std::vector<const NodeDef*>& cList, const NodeDef *def);

} // namespace Model
} // namespace Oak
