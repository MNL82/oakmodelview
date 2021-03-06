/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "NodeServiceFunctions.h"

#include "../ServiceFunctions/Trace.h"


namespace Oak::Model {

// =============================================================================
// (public)
void findOptionQueries(const NodeDef *def, std::vector<NodeLeafDefPair> &queryList, std::vector<NodeLeafDefPair> &queryExcludedList, bool recursive)
{
    std::vector<const LeafDef *> leafDefList = def->valueList(true, true);

    for (const LeafDef *vDef: leafDefList) {
        //TRACE(L"Searching %S:%S", def->displayName().c_str(), vDef->displayName().c_str());
        if (vDef->options().hasQuery()) {
            queryList.push_back({ def, vDef });
        }
        if (vDef->options().hasQueryExcluded()) {
            queryExcludedList.push_back({ def, vDef });
        }
    }

    if (recursive) {
        std::vector<const NodeDef *> childNodeDefList = def->childDefList(true, true);
        def->removeParentDefs(childNodeDefList, true);

        for (const NodeDef *cDef: childNodeDefList)
        {
            findOptionQueries(cDef, queryList, queryExcludedList, recursive);
        }
    }
}

} // namespace Oak::Model

