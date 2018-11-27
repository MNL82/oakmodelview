/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ContainerDefBuilderData.h"

#include "NodeDefBuilderData.h"

#include "../ServiceFunctions/Trace.h"

namespace Oak::Model {

// =============================================================================
// (public)
ContainerDefBuilderData::ContainerDefBuilderData()
{
    TRACE("ContainerDefBuilderData");
}

// =============================================================================
// (public)
void ContainerDefBuilderData::validate(std::vector<std::string>& errorMessages) const
{
    errorMessages.push_back("ContainerDef validation not implemented");

    if (nodeDefName.empty()) {
        errorMessages.push_back("ContainerDef's have to have a NodeDef reference");
    }

    if (minCount < 0) {
        errorMessages.push_back("ContainerDef's minimum count have to be greater than or equal to 0");
    }

    if (maxCount == 0) {
        errorMessages.push_back("ContainerDef's maximum count have to be greater than 0");
    } else if (maxCount < minCount) {
        errorMessages.push_back("ContainerDef's maximum count have to be greater than or equal to minimum count");
    }
}

// =============================================================================
// (public)
bool ContainerDefBuilderData::create(NodeDefSPtr nodeDef) const
{
    UNUSED(nodeDef)
    return false;
}

} // namespace Oak::Model
