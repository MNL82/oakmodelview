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

#include "NodeDefinition.h"

namespace Oak {
namespace Model {

// =============================================================================
// Class definition
// =============================================================================
class NodeDefinitionBuilder
{
    NodeDefinitionBuilder() = delete;

public:
    static NodeDefinitionSPtr Make(VariantCRef primaryKey);
    static NodeDefinitionSPtr MakeRoot(VariantCRef primaryKey, VariantCRef derivedId);
    static NodeDefinitionSPtr MakeDerived(NodeDefinitionSPtr derivedBase, VariantCRef derivedId);

    static bool addValueDef(NodeDefinitionSPtr nodeDef, ValueDefinitionUPtr valueDef);
    static bool addValueDefAsKey(NodeDefinitionSPtr nodeDef, ValueDefinitionUPtr valueDefKey);
    static bool addValueDefAsDerivedId(NodeDefinitionSPtr nodeDef, ValueDefinitionUPtr valueDefDerivedId);
    static ValueDefinitionUPtr takeValueDef(NodeDefinitionSPtr nodeDef, VariantCRef valueId);

    static bool addContainerDef(NodeDefinitionSPtr nodeDef, ContainerDefinitionUPtr cDef);
    static ContainerDefinitionUPtr takeContainerDef(NodeDefinitionSPtr nodeDef, VariantCRef primaryKey);

    static bool setName(NodeDefinitionSPtr nodeDef, const std::string& name);

#ifdef XML_BACKEND
    static bool setTagName(NodeDefinitionSPtr nodeDef, const std::string& tagName);
#endif // XML_BACKEND

protected:
    // ************* SERVICE FUNCTIONS *************
    // Check if the ValueDefinition will conflict with any other ValueDefinition
    static bool hasValueI(NodeDefinitionSPtr nodeDef, const ValueDefinitionUPtr& valueDef);
    static bool hasValueIThisAndDerived(NodeDefinitionSPtr nodeDef, const ValueDefinitionUPtr& valueDef);

    // Set the keyValue or derivedIdValue on the whole existing inheritance heiracky
    static void setKeyValueThisAndDerived(NodeDefinitionSPtr nodeDef, int index);
    static void setDerivedIdValueThisAndDerived(NodeDefinitionSPtr nodeDef, int index);

    // Check if the ValueDefinition will conflict with any other ValueDefinition
    static bool hasContainerI(NodeDefinitionSPtr nodeDef, const ContainerDefinitionUPtr& cDefinition);
    static bool hasContainerIThisAndDerived(NodeDefinitionSPtr nodeDef, const ContainerDefinitionUPtr& cDefinition);

#ifdef XML_BACKEND
    static void setTagNameThisAndDerived(NodeDefinitionSPtr nodeDef, const std::string& tagName);
#endif // XML_BACKEND
};

typedef NodeDefinitionBuilder NDB;

} // namespace Model
} // namespace Oak
