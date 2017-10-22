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
    static NodeDefinitionSPtr Make(const std::string &name);

    template<typename T>
    static NodeDefinitionSPtr MakeRoot(const std::string &name, T derivedId);

    template<typename T>
    static NodeDefinitionSPtr MakeDerived(NodeDefinitionSPtr derivedBase, T derivedId);


    static bool addValueDef(NodeDefinitionSPtr nodeDef, ValueDefinitionUPtr valueDef);
    static bool addValueDefAsKey(NodeDefinitionSPtr nodeDef, ValueDefinitionUPtr valueDefKey);
    static bool addValueDefAsDerivedId(NodeDefinitionSPtr nodeDef, ValueDefinitionUPtr valueDefDerivedId);
    static ValueDefinitionUPtr takeValueDef(NodeDefinitionSPtr nodeDef, const std::string &valueName);

    static bool addContainerDef(NodeDefinitionSPtr nodeDef, ContainerDefinitionUPtr cDef);
    static ContainerDefinitionUPtr takeContainerDef(NodeDefinitionSPtr nodeDef, const std::string &name);

    static bool setDisplayName(NodeDefinitionSPtr nodeDef, const std::string& displayName);

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

// =============================================================================
// (public)
template<typename T>
NodeDefinitionSPtr NodeDefinitionBuilder::MakeRoot(const std::string &name, T derivedId)
{
    NodeDefinitionSPtr nodeDef = NodeDefinition::MakeSPtr(name, derivedId);

#ifdef XML_BACKEND
    if (XML::Element::validateTagName(name)) {
        nodeDef->m_tagName = name;
    }
#endif // XML_BACKEND

    return nodeDef;
}

// =============================================================================
// (public)
template<typename T>
NodeDefinitionSPtr NodeDefinitionBuilder::MakeDerived(NodeDefinitionSPtr derivedBase, T derivedId)
{
    // derivedBase has to be a valid pointer
    assert(derivedBase);

    // derivedBase has to have an valid derivedId
    assert(!derivedBase->derivedId().isNull());

    // The derivedId's have to be unique
    assert(!derivedBase->validateForAny(derivedId));

    // DerivedId's of derived definitions have to be of the same derivedId type
    assert(derivedBase->derivedId().isBaseTypeEqual(derivedId));

    NodeDefinitionSPtr derivedDefinition = NodeDefinition::MakeSPtr(derivedBase->name(), derivedId);

    // Adds the derived definition to the inheritance heiraki
    derivedDefinition->m_derivedBase = derivedBase;
    derivedBase->m_derivedDirectList.push_back(derivedDefinition);

    // The name is the same for all definitions in the inheritance heiraki
    derivedDefinition->m_name = derivedBase->m_name;

    // The keyValue and derivedIdValue are the same for all definitions in the inheritance heiraki
    derivedDefinition->m_keyValueDefIndex = derivedBase->m_keyValueDefIndex;
    derivedDefinition->m_derivedIdValueDefIndex = derivedBase->m_derivedIdValueDefIndex;

    if (derivedBase->hasDerivedId()) {
        VDB::addOption(derivedBase->derivedIdValueDef(), derivedId);
    }

#ifdef XML_BACKEND
    derivedDefinition->m_tagName = derivedBase->m_tagName;
#endif // XML_BACKEND

    return derivedDefinition;
}

typedef NodeDefinitionBuilder NDB;

} // namespace Model
} // namespace Oak
