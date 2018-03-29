/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "NodeDefBuilder.h"

namespace Oak {
namespace Model {

// =============================================================================
// (protected)
NodeDefBuilder::NodeDefBuilder(const std::string &name)
{
    m_nodeDef = NodeDef::MakeSPtr(name);

#ifdef XML_BACKEND
    if (XML::Element::validateTagName(name)) {
        m_nodeDef->m_tagName = name;
    }
#endif // XML_BACKEND

}

// =============================================================================
// (protected)
NodeDefBuilder::NodeDefBuilder(const std::string &name, const UnionRef &derivedId)
{
    m_nodeDef = NodeDef::MakeSPtr(name, derivedId);

#ifdef XML_BACKEND
    if (XML::Element::validateTagName(name)) {
        m_nodeDef->m_tagName = name;
    }
#endif // XML_BACKEND
}

// =============================================================================
// (protected)
NodeDefBuilder::NodeDefBuilder(NodeDefSPtr derivedBaseNode, const UnionRef &derivedId)
{
    // derivedBase has to be a valid pointer
    assert(derivedBaseNode);

    // derivedBase has to have an valid derivedId
    assert(!derivedBaseNode->derivedId().isNull());

    // The derivedId's have to be unique
    assert(!derivedBaseNode->validateForAny(derivedId));

    // DerivedId's of derived definitions have to be of the same derivedId type
    assert(derivedBaseNode->derivedId().type() == UnionValue::GetType(derivedId));

    m_nodeDef = NodeDef::MakeSPtr(derivedBaseNode->name(), derivedId);

    // Adds the derived definition to the inheritance heiraki
    m_nodeDef->m_derivedBase = derivedBaseNode;
    derivedBaseNode->m_derivedDirectList.push_back(m_nodeDef);

    // The name is the same for all definitions in the inheritance heiraki
    m_nodeDef->m_name = derivedBaseNode->m_name;

    // The keyValue and derivedIdValue are the same for all definitions in the inheritance heiraki
    m_nodeDef->m_keyValueDefIndex = derivedBaseNode->m_keyValueDefIndex;
    m_nodeDef->m_derivedIdValueDefIndex = derivedBaseNode->m_derivedIdValueDefIndex;

    if (derivedBaseNode->hasDerivedId()) {
        VDB::use(&derivedBaseNode->derivedIdValueDef())->addOptionStatic(derivedId);
    }

#ifdef XML_BACKEND
    m_nodeDef->m_tagName = derivedBaseNode->m_tagName;
#endif // XML_BACKEND
}

// =============================================================================
// (protected)
NodeDefBuilder::NodeDefBuilder(NodeDefSPtr nodeDef)
{
    m_nodeDef = nodeDef;
}

// =============================================================================
// (public static)
NodeDefBuilderSPtr NodeDefBuilder::create(const std::string &name)
{
    NodeDefBuilderSPtr sPtr = NodeDefBuilderSPtr(new NodeDefBuilder(name));
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

// =============================================================================
// (public static)
NodeDefBuilderSPtr NodeDefBuilder::createInheritanceRoot(const std::string &name, const UnionRef &derivedId)
{
    NodeDefBuilderSPtr sPtr = NodeDefBuilderSPtr(new NodeDefBuilder(name, derivedId));
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

// =============================================================================
// (public static)
NodeDefBuilderSPtr NodeDefBuilder::createInheritancDerived(NodeDefBuilderSPtr derivedBaseNode, const UnionRef &derivedId)
{
    NodeDefBuilderSPtr sPtr = NodeDefBuilderSPtr(new NodeDefBuilder(derivedBaseNode->get(), derivedId));
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

// =============================================================================
// (public static)
NodeDefBuilderSPtr NodeDefBuilder::use(NodeDefSPtr nodeDef)
{
    NodeDefBuilderSPtr sPtr = NodeDefBuilderSPtr(new NodeDefBuilder(nodeDef));
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

// =============================================================================
// (public)
NodeDefSPtr NodeDefBuilder::get()
{
    assert(m_nodeDef);
    return m_nodeDef;
}

// =============================================================================
// (public)
NodeDefBuilderSPtr NodeDefBuilder::addValueDef(ValueDefBuilderSPtr valueDef)
{
    assert(m_nodeDef);
    assert(valueDef);
    // A NodeDef can only have
    assert(!hasValueI(m_nodeDef, valueDef));

    m_nodeDef->m_valueList.push_back(valueDef->get());

    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
NodeDefBuilderSPtr NodeDefBuilder::addValueKey(ValueDefBuilderSPtr valueDefKey)
{
    assert(m_nodeDef);
    assert(valueDefKey);

    // Derived node definitions inherate its node id value from its base and can not have it's own
    assert(!m_nodeDef->hasDerivedBase());

    valueDefKey->setSetting("Unique", true);
    valueDefKey->setSetting("Required", true);

    addValueDef(valueDefKey);

    int index = m_nodeDef->valueCount()-1;
    setKeyValueThisAndDerived(m_nodeDef, index);
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
NodeDefBuilderSPtr NodeDefBuilder::addValueInheritanceId(ValueDefBuilderSPtr valueDefDerivedId)
{
    assert(m_nodeDef);
    assert(valueDefDerivedId);

    // The node must have an derived id
    assert(!m_nodeDef->derivedId().isNull());

    // The value type of the derivedId and the derivedIdValue must match
    assert(m_nodeDef->derivedId().type() == valueDefDerivedId->valueDef().valueType());

    // Derived node definitions inherate its derived id value from its base and can not have it's own
    assert(!m_nodeDef->hasDerivedBase());

    //
    std::vector<UnionRef> optionList;
    m_nodeDef->derivedIdListAll(optionList);
    valueDefDerivedId->setOptionsStatic(optionList);
    valueDefDerivedId->setSetting("OptionsOnly", true);
    if (!valueDefDerivedId->valueDef().hasDefaultValue()) {
        valueDefDerivedId->setDefaultValue(m_nodeDef->derivedId());
    }

    addValueDef(valueDefDerivedId);
    int index = m_nodeDef->valueCount()-1;
    setDerivedIdValueThisAndDerived(m_nodeDef, index);
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
NodeDefBuilderSPtr NodeDefBuilder::addContainerDef(ContainerDefUPtr cDef)
{
    assert(m_nodeDef);
    assert(cDef);

    // Check if the NodeDef will be referenced twice (Not sure this is needed)
    assert(!hasContainerI(m_nodeDef, cDef));

    cDef->m_hostDef = m_nodeDef;

    m_nodeDef->m_containerList.push_back(std::move(cDef));

    // Clear the list of existing containers
    if (m_nodeDef->m_containerGroup) {
        m_nodeDef->m_containerGroup->updateContainerList();
    }

    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
NodeDefBuilderSPtr NodeDefBuilder::setDisplayName(const std::string& displayName)
{
    assert(m_nodeDef);
    m_nodeDef->m_displayName = displayName;
    return m_thisWPtr.lock();
}

#ifdef XML_BACKEND
// =============================================================================
// (public)
NodeDefBuilderSPtr NodeDefBuilder::setTagName(const std::string &tagName)
{
    assert(m_nodeDef);
    assert(XML::Element::validateTagName(tagName));
    if (m_nodeDef->m_tagName == tagName) { return m_thisWPtr.lock(); }

    NodeDefSPtr derivedRoot = m_nodeDef;
    while (derivedRoot->hasDerivedBase()) {
        derivedRoot = derivedRoot->m_derivedBase.lock();
    }

    setTagNameThisAndDerived(derivedRoot, tagName);

    return m_thisWPtr.lock();
}
#endif // XML_BACKEND



//// =============================================================================
//// (public)
//NodeDefSPtr NodeDefBuilder::Make(const std::string & name)
//{
//    NodeDefSPtr nodeDef = NodeDef::MakeSPtr(name);

//#ifdef XML_BACKEND
//    if (XML::Element::validateTagName(name)) {
//        nodeDef->m_tagName = name;
//    }
//#endif // XML_BACKEND

//    return nodeDef;
//}

//// =============================================================================
//// (public)
//bool NodeDefBuilder::addValueDef(NodeDefSPtr nodeDef, ValueDefBuilderSPtr valueDef)
//{
//    if (!nodeDef) { return false; }
//    if (!valueDef) { return false; }

//    // A NodeDef can only have
//    if (hasValueI(nodeDef, valueDef)) { return false; }

//    nodeDef->m_valueList.push_back(valueDef->get());

//    return true;
//}

//// =============================================================================
//// (public)
//bool NodeDefBuilder::addValueKey(NodeDefSPtr nodeDef, ValueDefBuilderSPtr valueDefKey)
//{
//    if (!nodeDef) { return false; }
//    if (!valueDefKey) { return false; }

//    // Derived node definitions inherate its node id value from its base and can not have it's own
//    if (nodeDef->hasDerivedBase()) { return false; }

//    valueDefKey->setSetting("Unique", true);
//    valueDefKey->setSetting("Required", true);

//    if (addValueDef(nodeDef, valueDefKey)) {
//        int index = nodeDef->valueCount()-1;
//        setKeyValueThisAndDerived(nodeDef, index);
//        return true;
//    }
//    return false;
//}

//// =============================================================================
//// (public)
//bool NodeDefBuilder::addValueInheritanceId(NodeDefSPtr nodeDef, ValueDefBuilderSPtr valueDefDerivedId)
//{
//    if (!nodeDef) { return false; }
//    if (!valueDefDerivedId) { return false; }

//    // The node must have an derived id
//    if (nodeDef->derivedId().isNull()) { return false; }

//    // The value type of the derivedId and the derivedIdValue must match
//    if (nodeDef->derivedId().type() != valueDefDerivedId->valueDef().valueType()) { return false; }

//    // Derived node definitions inherate its derived id value from its base and can not have it's own
//    if (nodeDef->hasDerivedBase()) { return false; }

//    //
//    std::vector<UnionRef> optionList;
//    nodeDef->derivedIdListAll(optionList);
//    valueDefDerivedId->setOptionsStatic(optionList);
//    valueDefDerivedId->setSetting("OptionsOnly", true);
//    if (!valueDefDerivedId->valueDef().hasDefaultValue()) {
//        valueDefDerivedId->setDefaultValue(nodeDef->derivedId());
//    }

//    if (addValueDef(nodeDef, valueDefDerivedId)) {
//        int index = nodeDef->valueCount()-1;
//        setDerivedIdValueThisAndDerived(nodeDef, index);
//        return true;
//    }
//    return false;
//}

// =============================================================================
// (public)
ValueDefUPtr NodeDefBuilder::takeValueDef(NodeDefSPtr nodeDef, const std::string &valueName)
{
    if (!nodeDef || valueName.empty()) { return ValueDefUPtr(); }

    auto it = nodeDef->m_valueList.begin();
    while (it != nodeDef->m_valueList.end()) {
        if ((*it)->name() == valueName) {
            ValueDefUPtr movedValue(std::move(*it));
            nodeDef->m_valueList.erase(it);
            return movedValue;
        }
    }

    return ValueDefUPtr();
}

//// =============================================================================
//// (public)
//bool NodeDefBuilder::addContainerDef(NodeDefSPtr nodeDef, ContainerDefUPtr cDef)
//{
//    if (!nodeDef || !cDef) {
//        assert(false);
//        return false;
//    }

//    // Check if the NodeDef will be referenced twice (Not sure this is needed)
//    if (hasContainerI(nodeDef, cDef)) { return false; }

//    cDef->m_hostDef = nodeDef;

//    nodeDef->m_containerList.push_back(std::move(cDef));

//    // Clear the list of existing containers
//    if (nodeDef->m_containerGroup) {
//        nodeDef->m_containerGroup->updateContainerList();
//    }

//    return true;
//}

// =============================================================================
// (public)
ContainerDefUPtr NodeDefBuilder::takeContainerDef(NodeDefSPtr nodeDef, const std::string &name)
{
    if (!nodeDef || name.empty()) { return ContainerDefUPtr(); }

    auto it = nodeDef->m_containerList.begin();
    while (it != nodeDef->m_containerList.end()) {
        if ((*it)->containerDef()->name() == name) {
            ContainerDefUPtr movedContainer(std::move(*it));
            nodeDef->m_containerList.erase(it);

            // Clear the list of existing containers
            if (nodeDef->m_containerGroup) {
                nodeDef->m_containerGroup->updateContainerList();
            }

            return movedContainer;
        }
    }

    return ContainerDefUPtr();
}

//// =============================================================================
//// (public)
//bool NodeDefBuilder::setDisplayName(NodeDefSPtr nodeDef, const std::string& displayName)
//{
//    if (!nodeDef) { return false; }

//    nodeDef->m_displayName = displayName;

//    return true;
//}

//#ifdef XML_BACKEND
//// =============================================================================
//// (public)
//bool NodeDefBuilder::setTagName(NodeDefSPtr nodeDef, const std::string &tagName)
//{
//    if (!nodeDef) { return false; }
//    if (!XML::Element::validateTagName(tagName)) { return false; }
//    if (nodeDef->m_tagName == tagName) { return true; }

//    NodeDefSPtr derivedRoot = nodeDef;
//    while (derivedRoot->hasDerivedBase()) {
//        derivedRoot = derivedRoot->m_derivedBase.lock();
//    }

//    setTagNameThisAndDerived(derivedRoot, tagName);

//    return true;
//}
//#endif // XML_BACKEND

// =============================================================================
// (protected)
bool NodeDefBuilder::hasValueI(NodeDefSPtr nodeDef, const ValueDefBuilderSPtr& valueDef)
{
    if (!nodeDef) { return false; }

    // Check ValueDefs on it's base NodeDefs
    const NodeDef* ni = nodeDef.get();
    while (ni->hasDerivedBase()) {
        ni = ni->derivedBase();
        for (const auto& vi: nodeDef->m_valueList)
        {
            if (vi->name() == valueDef->valueDef().name()) {
                return true;
            }
        }
    }
    // Check ValueDefs on this NodeDef
    // Check ValueDefs on it's derived NodeDefs
    return hasValueIThisAndDerived(nodeDef, valueDef);
}

// =============================================================================
// (protected)
bool NodeDefBuilder::hasValueIThisAndDerived(NodeDefSPtr nodeDef, const ValueDefBuilderSPtr &valueDef)
{
    for (const auto& vi: nodeDef->m_valueList)
    {
        if (vi->name() == valueDef->valueDef().name()) {
            return true;
        }
    }
    if (nodeDef->hasDerivedDiviations()) {
        for (NodeDefSPtr ni: nodeDef->m_derivedDirectList)
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
void NodeDefBuilder::setKeyValueThisAndDerived(NodeDefSPtr nodeDef, int index)
{
    nodeDef->m_keyValueDefIndex = index;
    if (nodeDef->hasDerivedDiviations()) {
        for (NodeDefSPtr ni: nodeDef->m_derivedDirectList)
        {
            setKeyValueThisAndDerived(ni, index);
        }
    }
}

// =============================================================================
// (protected)
void NodeDefBuilder::setDerivedIdValueThisAndDerived(NodeDefSPtr nodeDef, int index)
{
    nodeDef->m_derivedIdValueDefIndex = index;
    if (nodeDef->hasDerivedDiviations()) {
        for (NodeDefSPtr ni: nodeDef->m_derivedDirectList)
        {
            setDerivedIdValueThisAndDerived(ni, index);
        }
    }
}

// =============================================================================
// (protected)
bool NodeDefBuilder::hasContainerI(NodeDefSPtr nodeDef, const ContainerDefUPtr &cDef)
{
    // Check ValueDefs on it's base NodeDefs
    const NodeDef* ni = nodeDef.get();
    while (ni->hasDerivedBase()) {
        ni = ni->derivedBase();
        for (const auto& ci: nodeDef->m_containerList)
        {
            if (ci->m_containerDef->name() == cDef->m_containerDef->name()) {
                return true;
            }
        }
    }
    // Check ValueDefs on this NodeDef
    // Check ValueDefs on it's derived NodeDefs
    return hasContainerIThisAndDerived(nodeDef, cDef);
}

// =============================================================================
// (protected)
bool NodeDefBuilder::hasContainerIThisAndDerived(NodeDefSPtr nodeDef, const ContainerDefUPtr &cDef)
{
    for (const auto& ci: nodeDef->m_containerList)
    {
        if (ci->m_containerDef->name() == cDef->m_containerDef->name()) {
            return true;
        }
    }
    if (nodeDef->hasDerivedDiviations()) {
        for (NodeDefSPtr ni: nodeDef->m_derivedDirectList)
        {
            if (hasContainerIThisAndDerived(ni, cDef)) {
                return true;
            }
        }
    }
    return false;
}

#ifdef XML_BACKEND
// =============================================================================
// (protected)
void NodeDefBuilder::setTagNameThisAndDerived(NodeDefSPtr nodeDef, const std::string &tagName)
{
    nodeDef->m_tagName = tagName;
    if (nodeDef->hasDerivedDiviations()) {
        for (NodeDefSPtr ni: nodeDef->m_derivedDirectList)
        {
            setTagNameThisAndDerived(ni, tagName);
        }
    }
}

#endif // XML_BACKEND

} // namespace Model
} // namespace Oak
