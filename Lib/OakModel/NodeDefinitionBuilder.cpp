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
NodeDefinitionSPtr NodeDefinitionBuilder::Make(const std::string & name)
{
    NodeDefinitionSPtr nodeDef = NodeDefinition::MakeSPtr(name);

#ifdef XML_BACKEND
    if (XML::Element::validateTagName(name)) {
        nodeDef->m_tagName = name;
    }
#endif // XML_BACKEND

    return nodeDef;
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

    ValueSettings &settings = VDB::settings(valueDefKey);
    if (!settings.isUniqueSet()) {
        settings.setUnique(true);
    }
    if (!settings.isRequiredSet()) {
        settings.setRequired(true);
    }

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
    if (nodeDef->derivedId().type() != valueDefDerivedId->valueTemplate().type()) { return false; }

    // Derived node definitions inherate its derived id value from its base and can not have it's own
    if (nodeDef->hasDerivedBase()) { return false; }

    //
    std::vector<UnionRef> optionList;
    nodeDef->derivedIdListAll(optionList);
    VDB::setStaticOptions(valueDefDerivedId, optionList);
    VDB::settings(valueDefDerivedId).setOptionsOnly(true);
    if (!valueDefDerivedId->hasDefaultValue()) {
        VDB::setDefaultValue(valueDefDerivedId, nodeDef->derivedId());
    }

    if (addValueDef(nodeDef, std::move(valueDefDerivedId))) {
        int index = nodeDef->valueCount()-1;
        setDerivedIdValueThisAndDerived(nodeDef, index);
        return true;
    }
    return false;
}

// =============================================================================
// (public)
ValueDefinitionUPtr NodeDefinitionBuilder::takeValueDef(NodeDefinitionSPtr nodeDef, const std::string &valueName)
{
    if (!nodeDef || valueName.empty()) { return ValueDefinitionUPtr(); }

    auto it = nodeDef->m_valueList.begin();
    while (it != nodeDef->m_valueList.end()) {
        if ((*it)->name() == valueName) {
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
    if (!nodeDef || !cDef) {
        assert(false);
        return false;
    }

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
ContainerDefinitionUPtr NodeDefinitionBuilder::takeContainerDef(NodeDefinitionSPtr nodeDef, const std::string &name)
{
    if (!nodeDef || name.empty()) { return ContainerDefinitionUPtr(); }

    auto it = nodeDef->m_containerList.begin();
    while (it != nodeDef->m_containerList.end()) {
        if ((*it)->containerDefinition()->name() == name) {
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
bool NodeDefinitionBuilder::setDisplayName(NodeDefinitionSPtr nodeDef, const std::string& displayName)
{
    if (!nodeDef) { return false; }

    nodeDef->m_displayName = displayName;

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
            if (vi->name() == valueDef->name()) {
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
        if (vi->name() == valueDef->name()) {
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
            if (ci->m_containerDefinition->name() == cDefinition->m_containerDefinition->name()) {
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
        if (ci->m_containerDefinition->name() == cDefinition->m_containerDefinition->name()) {
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
