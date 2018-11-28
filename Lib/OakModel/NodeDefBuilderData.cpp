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

#include "NodeDefBuilder.h"

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
    //errorMessages.push_back("NodeDef validation not implemented");

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
    //errorMessages.push_back("Derived NodeDef validation not implemented");

    if (keyValue) {
        errorMessages.push_back("Derived NodeDef can not have a key value (It is inherited from it's base NodeDef)");
    }

    if (variantValue) {
        errorMessages.push_back("Derived NodeDef can not have a variant value (It is inherited from it's base NodeDef)");
    }

    if (variantId.empty()) {
        errorMessages.push_back("Derived NodeDef must have a variant id");
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
NodeDefSPtr NodeDefBuilderData::create()
{
    bool isRoot = variantValue != nullptr;
    NodeDefBuilderSPtr builder;
    if (isRoot) {
        builder = NodeDefBuilder::createVariantRoot(name, variantId);
    } else {
        builder = NodeDefBuilder::create(name);
    }

    createShared(builder);

    return builder->get();
}

// =============================================================================
// (public)
bool NodeDefBuilderData::createDerived(NodeDefSPtr baseNodeDef) const
{
    NodeDefBuilderSPtr builder = NodeDefBuilder::createVariant(NodeDefBuilder::use(baseNodeDef), variantId);

    createShared(builder);

    return builder.get();
}

// =============================================================================
// (public)
bool NodeDefBuilderData::createContainers(std::vector<NodeDefSPtr> nodeDefList, std::string rootName) const
{
    NodeDef* nodeDef;

    if (rootName.empty()) { rootName = name; }
    for (auto nDef: nodeDefList) {
        if (nDef->name() == rootName) {
            nodeDef = const_cast<NodeDef*>(nDef->validVariant(variantId)); // Const cast is allowed here
        }
    }

    NodeDefBuilderSPtr builder = NodeDefBuilder::use(nodeDef->sPtr());

    bool success = true;
    for (const auto& container: containers) {
        if (!container->set(builder, nodeDefList)) {
            success = false;
        }
    }

    for (const auto& derivedNodeDef: derivedNodeDefs) {
        if (!derivedNodeDef->createContainers(nodeDefList)) {
            success = false;
        }
    }

    return success;
}

// =============================================================================
// (private)
void NodeDefBuilderData::createShared(NodeDefBuilderSPtr builder) const
{
    if (!displayName.empty()) {
        builder->setDisplayName(displayName);
    }

    if (!tooltip.empty()) {
        builder->setTooltip(tooltip);
    }

    if (color.isValid()) {
        builder->setColor(color);
    }

    if (!imagePath.empty()) {
        builder->setImagePath(imagePath);
    }

#ifdef XML_BACKEND
    if (!tagName.empty()) {
        builder->setTagName(tagName);
    }
#endif // XML_BACKEND

    if (keyValue) {
        builder->addValueKey(keyValue->create());
    }

    if (variantValue) {
        builder->addValueVariant(variantValue->create());
    }

    for (const ValueDefBuilderDataUPtr& value: values) {
        builder->addValueDef(value->create());
    }
}

} // namespace Oak::Model
