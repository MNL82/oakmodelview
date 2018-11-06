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

#include "ContainerDefBuilder.h"

#include "../ServiceFunctions/Assert.h"

namespace Oak {
namespace Model {

// =============================================================================
// (protected)
NodeDefBuilder::NodeDefBuilder(const std::string &name)
{
    m_nodeDef = NodeDef::MakeSPtr(name);
}

// =============================================================================
// (protected)
NodeDefBuilder::NodeDefBuilder(const std::string &name, const UnionRef &variantId)
{
    m_nodeDef = NodeDef::MakeSPtr(name, variantId);
}

// =============================================================================
// (protected)
NodeDefBuilder::NodeDefBuilder(NodeDefSPtr variantRoot, const UnionRef &variantId)
{
    // base has to be a valid pointer
    ASSERT(variantRoot);

    // base has to have an valid variantId
    ASSERT(!variantRoot->variantId().isNull());

    // The variantId's have to be unique
    ASSERT(!variantRoot->baseRoot()->validate(variantId, false, true));

    // VariantId's of variant definitions have to be of the same variantId type
    ASSERT(variantRoot->variantId().type() == UnionValue::GetType(variantId));

    m_nodeDef = NodeDef::MakeSPtr(variantRoot->name(), variantId);

    // Adds the variant definition to the inheritance heiraki
    m_nodeDef->m_base = variantRoot;
    variantRoot->m_derivedList.push_back(m_nodeDef);

    // The name is the same for all definitions in the inheritance heiraki
    m_nodeDef->m_name = variantRoot->m_name;

    // The keyValueDef and variantValueDef are the same for all definitions in the inheritance heiraki
    m_nodeDef->m_indexOfKeyValueDef = variantRoot->m_indexOfKeyValueDef;
    m_nodeDef->m_indexOfVariantValueDef = variantRoot->m_indexOfVariantValueDef;

    if (!variantRoot->variantValueDef().isNull()) {
        VDB::use(&variantRoot->variantValueDef())->addOptionStatic(variantId);
    }

#ifdef XML_BACKEND
    m_nodeDef->m_tagName = variantRoot->m_tagName;
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
NodeDefBuilderSPtr NodeDefBuilder::createVariantRoot(const std::string &name, const UnionRef &variantId)
{
    NodeDefBuilderSPtr sPtr = NodeDefBuilderSPtr(new NodeDefBuilder(name, variantId));
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

// =============================================================================
// (public static)
NodeDefBuilderSPtr NodeDefBuilder::createVariant(NodeDefBuilderSPtr variantRoot, const UnionRef &variantId)
{
    NodeDefBuilderSPtr sPtr = NodeDefBuilderSPtr(new NodeDefBuilder(variantRoot->get(), variantId));
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
    ASSERT(m_nodeDef);
    return m_nodeDef;
}

// =============================================================================
// (public)
NodeDefBuilderSPtr NodeDefBuilder::addValueDef(ValueDefBuilderSPtr valueDef)
{
    ASSERT(m_nodeDef);
    ASSERT(valueDef);
    // A NodeDef can only have
    ASSERT(!m_nodeDef->hasValue(valueDef->valueDef().name()));

    m_nodeDef->m_valueList.push_back(valueDef->get());

    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
NodeDefBuilderSPtr NodeDefBuilder::addValueKey(ValueDefBuilderSPtr valueDefKey)
{
    ASSERT(m_nodeDef);
    ASSERT(valueDefKey);

    // Variant node definitions inherate its node id value from its base and can not have it's own
    ASSERT(!m_nodeDef->hasBase());

    if (valueDefKey->valueDef().settings().value(UNIQUE) != 0) {
        valueDefKey->setSetting(UNIQUE, 1);
    }
    if (valueDefKey->valueDef().settings().value(REQUIRED) != 0) {
        valueDefKey->setSetting(REQUIRED, 1);
    }

    addValueDef(valueDefKey);

    int index = m_nodeDef->valueCount()-1;
    setKeyValueDefForAllVariants(m_nodeDef, index);
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
NodeDefBuilderSPtr NodeDefBuilder::addValueInheritanceId(ValueDefBuilderSPtr variantValueDef)
{
    ASSERT(m_nodeDef);
    ASSERT(variantValueDef);

    // The node must have an variant id
    ASSERT(!m_nodeDef->variantId().isNull());

    // The value type of the variantId and the variantValueDef must match
    ASSERT(m_nodeDef->variantId().type() == variantValueDef->valueDef().valueType());

    // Derived node definitions inherate its variantValueDef value from its base and can not have it's own
    ASSERT(!m_nodeDef->hasBase());

    //
    std::vector<UnionRef> optionList = m_nodeDef->baseRoot()->variantIdList(false, true);
    variantValueDef->setOptionsStatic(optionList);
    variantValueDef->setSetting("OptionsOnly", true);
    if (!variantValueDef->valueDef().hasDefaultValue()) {
        variantValueDef->setDefaultValue(m_nodeDef->variantId());
    }

    addValueDef(variantValueDef);
    int index = m_nodeDef->valueCount()-1;
    setVariantValueDefForAllVariants(m_nodeDef, index);
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
NodeDefBuilderSPtr NodeDefBuilder::addContainerDef(ContainerDefBuilderSPtr cDef)
{
    ASSERT(m_nodeDef);
    ASSERT(cDef);

    // Check if the NodeDef will be referenced twice (Not sure this is needed)
    ASSERT(!m_nodeDef->hasContainer(cDef->containerDef().containerDef()->name()));

    auto containerDef = cDef->get();

    containerDef->m_hostDef = m_nodeDef;

    m_nodeDef->m_containerList.push_back(std::move(containerDef));

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
    ASSERT(m_nodeDef);
    m_nodeDef->m_displayName = displayName;
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
NodeDefBuilderSPtr NodeDefBuilder::setColor(int red, int green, int blue, int alpha)
{
    ASSERT(m_nodeDef);
    m_nodeDef->m_color.set(red, green, blue, alpha);
    return m_thisWPtr.lock();
}
// =============================================================================
// (public)
NodeDefBuilderSPtr NodeDefBuilder::setImagePath(const std::string &imagePath)
{
    ASSERT(m_nodeDef);
    m_nodeDef->m_imagePath = imagePath;
    return m_thisWPtr.lock();
}

#ifdef XML_BACKEND
// =============================================================================
// (public)
NodeDefBuilderSPtr NodeDefBuilder::setTagName(const std::string &tagName)
{
    ASSERT(m_nodeDef);
    ASSERT(XML::Element::validateTagName(tagName));
    if (m_nodeDef->m_tagName == tagName) { return m_thisWPtr.lock(); }

    NodeDefSPtr baseRoot = m_nodeDef;
    while (baseRoot->hasBase()) {
        baseRoot = baseRoot->m_base.lock();
    }

    setTagNameForAllVariants(baseRoot, tagName);

    return m_thisWPtr.lock();
}
#endif // XML_BACKEND

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

// =============================================================================
// (protected)
void NodeDefBuilder::setKeyValueDefForAllVariants(NodeDefSPtr nodeDef, int index)
{
    nodeDef->m_indexOfKeyValueDef = index;
    if (nodeDef->hasDerived()) {
        for (NodeDefSPtr ni: nodeDef->m_derivedList)
        {
            setKeyValueDefForAllVariants(ni, index);
        }
    }
}

// =============================================================================
// (protected)
void NodeDefBuilder::setVariantValueDefForAllVariants(NodeDefSPtr nodeDef, int index)
{
    nodeDef->m_indexOfVariantValueDef = index;
    if (nodeDef->hasDerived()) {
        for (NodeDefSPtr ni: nodeDef->m_derivedList)
        {
            setVariantValueDefForAllVariants(ni, index);
        }
    }
}

#ifdef XML_BACKEND
// =============================================================================
// (protected)
void NodeDefBuilder::setTagNameForAllVariants(NodeDefSPtr nodeDef, const std::string &tagName)
{
    nodeDef->m_tagName = tagName;
    if (nodeDef->hasDerived()) {
        for (NodeDefSPtr ni: nodeDef->m_derivedList)
        {
            setTagNameForAllVariants(ni, tagName);
        }
    }
}

#endif // XML_BACKEND

} // namespace Model
} // namespace Oak
