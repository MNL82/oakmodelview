/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ItemServiceFunctions.h"

#include "../ServiceFunctions/Trace.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
void findOptionQueries(const NodeDef *def, std::vector<NodeEntryQuery> &queryList, std::vector<NodeEntryQuery> &queryExcludedList, bool recursive)
{
    std::vector<const ValueDef *> valueDefList;
    getValueDefList(valueDefList, def);

    for (const ValueDef *vDef: valueDefList) {
        //TRACE(L"Searching %S:%S", def->displayName().c_str(), vDef->displayName().c_str());
        if (vDef->options().hasQuery()) {
            queryList.push_back({ def, vDef->options().query() });
        }
        if (vDef->options().hasQueryExcluded()) {
            queryExcludedList.push_back({ def, vDef->options().queryExcluded() });
        }
    }

    if (recursive) {
        std::vector<const NodeDef *> childNodeDefList;
        getChildNodeDefList(childNodeDefList, def);
        for (const NodeDef *cDef: childNodeDefList)
        {
            findOptionQueries(cDef, queryList, queryExcludedList, recursive);
        }
    }
}

// =============================================================================
// (public)
void getValueDefList(std::vector<const ValueDef *> &vList, const NodeDef *def)
{
    //TRACE(L"Values from: %S", def->displayName().c_str());
    def->getValueList(vList, true);

    std::vector<const NodeDef*> dList;
    def->getDerivedList(dList, true);
    for (const NodeDef *dDef: dList)
    {
        //TRACE(L"Values from derived: %S", dDef->displayName().c_str());
        dDef->getValueList(vList, false);
    }
}

// =============================================================================
// (public)
void getChildNodeDefList(std::vector<const NodeDef *> &cList, const NodeDef *def)
{
    //TRACE(L"Children from: %S", def->displayName().c_str());
    def->getChildDefList(cList, true);

    std::vector<const NodeDef*> dList;
    def->getDerivedList(dList, true);
    for (const NodeDef *dDef: dList)
    {
        //TRACE(L"Children from derived: %S", dDef->displayName().c_str());
        dDef->getChildDefList(cList, false);
    }
}

} // namespace Model
} // namespace Oak
