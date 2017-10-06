/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "NodeDefinitionBuilder.h"

#include "ValueDefinitionBuilder.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
NodeDefinitionSPtr NodeDefinitionBuilder::Make(VariantCRef primaryKey)
{
    // primaryKey must be valid
    assert(!primaryKey.isNull());

    NodeDefinitionSPtr nodeDef = NodeDefinition::MakeSPtr(primaryKey);

#ifdef XML_BACKEND
    primaryKey.get(nodeDef->m_tagName);
    if (!XML::Element::validateTagName(nodeDef->m_tagName)) {
        // Clear the tag name if it is not valid
        nodeDef->m_tagName.clear();
    }
#endif // XML_BACKEND

    return nodeDef;
}

// =============================================================================
// (public)
NodeDefinitionSPtr NodeDefinitionBuilder::MakeRoot(VariantCRef primaryKey, VariantCRef derivedId)
{
    // primaryKey must be valid
    assert(!primaryKey.isNull());
    // derivedId must be valid
    assert(!derivedId.isNull());

    NodeDefinitionSPtr nodeDef = NodeDefinition::MakeSPtr(primaryKey, derivedId);

#ifdef XML_BACKEND
    primaryKey.get(nodeDef->m_tagName);
    if (!XML::Element::validateTagName(nodeDef->m_tagName)) {
        // Clear the tag name if it is not valid
        nodeDef->m_tagName.clear();
    }
#endif // XML_BACKEND

    return nodeDef;
}

// =============================================================================
// (public)
NodeDefinitionSPtr NodeDefinitionBuilder::MakeDerived(NodeDefinitionSPtr derivedBase, VariantCRef derivedId)
{
    // derivedBase has to be a valid pointer
    assert(derivedBase);

    // derivedId must be valid
    assert(!derivedId.isNull());

    // derivedBase has to have an valid derivedId
    assert(!derivedBase->derivedId().isNull());

    // The derivedId's have to be unique
    assert(!derivedBase->validateForAny(derivedId));

    // DerivedId's of derived definitions have to be of the same derivedId type
    assert(derivedBase->derivedId().isBaseTypeEqual(derivedId));

    NodeDefinitionSPtr derivedDefinition = NodeDefinition::MakeSPtr(derivedBase->primaryKey(), derivedId);

    // Adds the derived definition to the inheritance heiraki
    derivedDefinition->m_derivedBase = derivedBase;
    derivedBase->m_derivedDirectList.push_back(derivedDefinition);

    // The primaryKey is the same for all definitions in the inheritance heiraki
    derivedDefinition->m_primaryKey = derivedBase->m_primaryKey;

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

// =============================================================================
// (public)
bool NodeDefinitionBuilder::addValueDef(NodeDefinitionSPtr nodeDef, ValueDefinitionUPtr valueDef)
{
    if (!nodeDef) { return false; }
    if (!valueDef) { return false; }

    // A NodeDefinition can only have
    if (hasValueI(nodeDef, valueDef)) { return false; }

    nodeDef->m_valueList.push_back(std::move(valueDef));

    return true;
}

// =============================================================================
// (public)
bool NodeDefinitionBuilder::addValueDefAsKey(NodeDefinitionSPtr nodeDef, ValueDefinitionUPtr valueDefKey)
{
    if (!nodeDef) { return false; }
    if (!valueDefKey) { return false; }

    // Derived node definitions inherate its node id value from its base and can not have it's own
    if (nodeDef->hasDerivedBase()) { return false; }

    if (addValueDef(nodeDef, std::move(valueDefKey))) {
        int index = nodeDef->valueCount()-1;
        setKeyValueThisAndDerived(nodeDef, index);
        return true;
    }
    return false;
}

// =============================================================================
// (public)
bool NodeDefinitionBuilder::addValueDefAsDerivedId(NodeDefinitionSPtr nodeDef, ValueDefinitionUPtr valueDefDerivedId)
{
    if (!nodeDef) { return false; }
    if (!valueDefDerivedId) { return false; }

    // The node must have an derived id
    if (nodeDef->derivedId().isNull()) { return false; }

    // The value type of the derivedId and the derivedIdValue must match
    if (!nodeDef->derivedId().isBaseTypeEqual(valueDefDerivedId->valueTemplate())) { return false; }

    // Derived node definitions inherate its derived id value from its base and can not have it's own
    if (nodeDef->hasDerivedBase()) { return false; }

    //
    std::vector<VariantCRef> optionList;
    nodeDef->derivedIdListAll(optionList);
    VDB::setOptions(valueDefDerivedId, optionList);
    VDB::settings(valueDefDerivedId).setOptionsOnly(true);

    if (addValueDef(nodeDef, std::move(valueDefDerivedId))) {
        int index = nodeDef->valueCount()-1;
        setDerivedIdValueThisAndDerived(nodeDef, index);
        return true;
    }
    return false;
}

// =============================================================================
// (public)
ValueDefinitionUPtr NodeDefinitionBuilder::takeValueDef(NodeDefinitionSPtr nodeDef, VariantCRef valueId)
{
    if (!nodeDef || valueId.isNull()) { return ValueDefinitionUPtr(); }

    auto it = nodeDef->m_valueList.begin();
    while (it != nodeDef->m_valueList.end()) {
        if ((*it)->valueId() == valueId) {
            ValueDefinitionUPtr movedValue(std::move(*it));
            nodeDef->m_valueList.erase(it);
            return std::move(movedValue);
        }
    }

    return ValueDefinitionUPtr();
}

// =============================================================================
// (public)
bool NodeDefinitionBuilder::addContainerDef(NodeDefinitionSPtr nodeDef, ContainerDefinitionUPtr cDef)
{
    if (!nodeDef || !cDef) { return false; }

    // Check if the NodeDefinition will be referenced twice (Not sure this is needed)
    if (hasContainerI(nodeDef, cDef)) { return false; }

    cDef->m_hostDefinition = nodeDef;

    nodeDef->m_containerList.push_back(std::move(cDef));

    // Clear the list of existing containers
    if (nodeDef->m_containerGroup) {
        nodeDef->m_containerGroup->updateContainerList();
    }

    return true;
}

// =============================================================================
// (public)
ContainerDefinitionUPtr NodeDefinitionBuilder::takeContainerDef(NodeDefinitionSPtr nodeDef, VariantCRef primaryKey)
{
    if (!nodeDef || primaryKey.isNull()) { return ContainerDefinitionUPtr(); }

    auto it = nodeDef->m_containerList.begin();
    while (it != nodeDef->m_containerList.end()) {
        if ((*it)->containerDefinition()->primaryKey().isEqual(primaryKey)) {
            ContainerDefinitionUPtr movedContainer(std::move(*it));
            nodeDef->m_containerList.erase(it);

            // Clear the list of existing containers
            if (nodeDef->m_containerGroup) {
                nodeDef->m_containerGroup->updateContainerList();
            }

            return std::move(movedContainer);
        }
    }

    return ContainerDefinitionUPtr();
}

// =============================================================================
// (public)
bool NodeDefinitionBuilder::setName(NodeDefinitionSPtr nodeDef, const std::string& name)
{
    if (!nodeDef) { return false; }

    nodeDef->m_name = name;

    return true;
}

#ifdef XML_BACKEND
// =============================================================================
// (public)
bool NodeDefinitionBuilder::setTagName(NodeDefinitionSPtr nodeDef, const std::string &tagName)
{
    if (!nodeDef) { return false; }
    if (!XML::Element::validateTagName(tagName)) { return false; }
    if (nodeDef->m_tagName == tagName) { return true; }

    NodeDefinitionSPtr derivedRoot = nodeDef;
    while (derivedRoot->hasDerivedBase()) {
        derivedRoot = derivedRoot->m_derivedBase.lock();
    }

    setTagNameThisAndDerived(derivedRoot, tagName);

    return true;
}
#endif // XML_BACKEND

// =============================================================================
// (protected)
bool NodeDefinitionBuilder::hasValueI(NodeDefinitionSPtr nodeDef, const ValueDefinitionUPtr& valueDef)
{
    if (!nodeDef) { return false; }

    // Check ValueDefinitions on it's base NodeDefinitions
    const NodeDefinition* ni = nodeDef.get();
    while (ni->hasDerivedBase()) {
        ni = ni->derivedBase();
        for (const auto& vi: nodeDef->m_valueList)
        {
            if (vi->valueId() == valueDef->valueId()) {
                return true;
            }
        }
    }
    // Check ValueDefinitions on this NodeDefinition
    // Check ValueDefinitions on it's derived NodeDefinitions
    return hasValueIThisAndDerived(nodeDef, valueDef);
}

// =============================================================================
// (protected)
bool NodeDefinitionBuilder::hasValueIThisAndDerived(NodeDefinitionSPtr nodeDef, const ValueDefinitionUPtr &valueDef)
{
    for (const auto& vi: nodeDef->m_valueList)
    {
        if (vi->valueId() == valueDef->valueId()) {
            return true;
        }
    }
    if (nodeDef->hasDerivedDiviations()) {
        for (NodeDefinitionSPtr ni: nodeDef->m_derivedDirectList)
        {
            if (hasValueIThisAndDerived(ni, valueDef)) {
                return true;
            }
        }
    }
    return false;
}

// =============================================================================
// (protected)
void NodeDefinitionBuilder::setKeyValueThisAndDerived(NodeDefinitionSPtr nodeDef, int index)
{
    nodeDef->m_keyValueDefIndex = index;
    if (nodeDef->hasDerivedDiviations()) {
        for (NodeDefinitionSPtr ni: nodeDef->m_derivedDirectList)
        {
            setKeyValueThisAndDerived(ni, index);
        }
    }
}

// =============================================================================
// (protected)
void NodeDefinitionBuilder::setDerivedIdValueThisAndDerived(NodeDefinitionSPtr nodeDef, int index)
{
    nodeDef->m_derivedIdValueDefIndex = index;
    if (nodeDef->hasDerivedDiviations()) {
        for (NodeDefinitionSPtr ni: nodeDef->m_derivedDirectList)
        {
            setDerivedIdValueThisAndDerived(ni, index);
        }
    }
}

// =============================================================================
// (protected)
bool NodeDefinitionBuilder::hasContainerI(NodeDefinitionSPtr nodeDef, const ContainerDefinitionUPtr &cDefinition)
{
    // Check ValueDefinitions on it's base NodeDefinitions
    const NodeDefinition* ni = nodeDef.get();
    while (ni->hasDerivedBase()) {
        ni = ni->derivedBase();
        for (const auto& ci: nodeDef->m_containerList)
        {
            if (ci->m_containerDefinition->primaryKey() == cDefinition->m_containerDefinition->primaryKey()) {
                return true;
            }
        }
    }
    // Check ValueDefinitions on this NodeDefinition
    // Check ValueDefinitions on it's derived NodeDefinitions
    return hasContainerIThisAndDerived(nodeDef, cDefinition);
}

// =============================================================================
// (protected)
bool NodeDefinitionBuilder::hasContainerIThisAndDerived(NodeDefinitionSPtr nodeDef, const ContainerDefinitionUPtr &cDefinition)
{
    for (const auto& ci: nodeDef->m_containerList)
    {
        if (ci->m_containerDefinition->primaryKey() == cDefinition->m_containerDefinition->primaryKey()) {
            return true;
        }
    }
    if (nodeDef->hasDerivedDiviations()) {
        for (NodeDefinitionSPtr ni: nodeDef->m_derivedDirectList)
        {
            if (hasContainerIThisAndDerived(ni, cDefinition)) {
                return true;
            }
        }
    }
    return false;
}

#ifdef XML_BACKEND
// =============================================================================
// (protected)
void NodeDefinitionBuilder::setTagNameThisAndDerived(NodeDefinitionSPtr nodeDef, const std::string &tagName)
{
    nodeDef->m_tagName = tagName;
    if (nodeDef->hasDerivedDiviations()) {
        for (NodeDefinitionSPtr ni: nodeDef->m_derivedDirectList)
        {
            setTagNameThisAndDerived(ni, tagName);
        }
    }
}

#endif // XML_BACKEND

} // namespace Model
} // namespace Oak
