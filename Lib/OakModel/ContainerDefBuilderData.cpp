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
#include "NodeDefBuilder.h"
#include "ContainerDefBuilder.h"

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
    //errorMessages.push_back("ContainerDef validation not implemented");

    if (name.empty()) {
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
bool ContainerDefBuilderData::set(NodeDefBuilderSPtr builder, std::vector<NodeDefSPtr> nodeDefList) const
{
    NodeDef* containerNodeDef = nullptr;
    for (auto nDef: nodeDefList) {
        if (nDef->name() == name) {
            containerNodeDef = const_cast<NodeDef*>(nDef->validVariant(variantId)); // Const cast is allowed here
        }
    }
    if (containerNodeDef == nullptr) { return false; }

    auto containerNodeBuilder = NodeDefBuilder::use(containerNodeDef->sPtr());
    auto containerBuilder = ContainerDefBuilder::create(containerNodeBuilder, minCount, maxCount);
    builder->addContainerDef(containerBuilder);
    return true;
}

} // namespace Oak::Model
