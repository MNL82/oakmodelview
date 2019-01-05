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
    //TRACE("NodeDefBuilderData");
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

    for (const auto & derivedBuilder: derivedNodeDefs)
    {
        derivedBuilder->createDerived(builder);
    }

    return builder->get();
}

// =============================================================================
// (public)
bool NodeDefBuilderData::createDerived(NodeDefBuilderSPtr baseNodeDefBuilder) const
{
    NodeDefBuilderSPtr builder = NodeDefBuilder::createVariant(baseNodeDefBuilder, variantId);

    createShared(builder);

    for (const auto & derivedBuilder: derivedNodeDefs)
    {
        derivedBuilder->createDerived(builder);
    }

    return builder.get();
}

// =============================================================================
// (public)
bool NodeDefBuilderData::createContainers(NodeDefSPtr nodeDef, std::vector<NodeDefSPtr> nodeDefList) const
{
    NodeDefBuilderSPtr builder = NodeDefBuilder::use(nodeDef);

    bool success = true;
    for (const auto& container: containers) {
        if (!container->set(builder, nodeDefList)) {
            success = false;
        }
    }

    for (const auto& variantNodeDef: derivedNodeDefs) {
        NodeDefSPtr vnd = builder->getDerivedNodeDef(variantNodeDef->variantId);
        if (!vnd || vnd->isNull()) {
            success = false;
        } else if (!variantNodeDef->createContainers(vnd, nodeDefList)) {
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
