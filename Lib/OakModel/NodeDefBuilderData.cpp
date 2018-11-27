/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "NodeDefBuilderData.h"

#include "../ServiceFunctions/Trace.h"

namespace Oak::Model {

// =============================================================================
// (public)
NodeDefBuilderData::NodeDefBuilderData()
{
    TRACE("NodeDefBuilderData");
}

// =============================================================================
// (public)
NodeDefBuilderData::~NodeDefBuilderData()
{

}

// =============================================================================
// (public)
void NodeDefBuilderData::validate(std::vector<std::string>& errorMessages) const
{
    errorMessages.push_back("NodeDef validation not implemented");

    if (keyValue) {
        keyValue->validate(errorMessages);
    }

    if (variantValue) {
        variantValue->validate(errorMessages);
    }

    for (const auto& value: values) {
        value->validate(errorMessages);
    }

    for (const auto& container: containers) {
        container->validate(errorMessages);
    }

    for (const auto& nodeDef: derivedNodeDefs) {
        nodeDef->validateDerived(errorMessages);
    }
}

// =============================================================================
// (public)
void NodeDefBuilderData::validateDerived(std::vector<std::string>& errorMessages) const
{
    if (keyValue) {
        errorMessages.push_back("Derived NodeDef can not have a key value (It is inherited from it's base NodeDef)");
    }

    if (variantValue) {
        errorMessages.push_back("Derived NodeDef can not have a variant value (It is inherited from it's base NodeDef)");
    }

    for (const auto& value: values) {
        value->validate(errorMessages);
    }

    for (const auto& container: containers) {
        container->validate(errorMessages);
    }

    for (const auto& nodeDef: derivedNodeDefs) {
        nodeDef->validateDerived(errorMessages);
    }

    errorMessages.push_back("Derived NodeDef validation not implemented");
}

// =============================================================================
// (public)
NodeDefSPtr NodeDefBuilderData::create()
{
    return NodeDefSPtr();
}

// =============================================================================
// (public)
bool NodeDefBuilderData::createDerived(NodeDefSPtr baseNodeDef) const
{
    UNUSED(baseNodeDef)
    return false;
}

} // namespace Oak::Model
