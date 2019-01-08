/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include <algorithm>
#include <functional>

#include "NodeDef.h"
#include "LeafDefBuilder.h"
#include "LeafQuery.h"
#include "OakModelServiceFunctions.h"
#include "QueryBuilder.h"

#include "../ServiceFunctions/Assert.h"


namespace Oak::Model {

// =============================================================================
// (public)
// This constructor is used when the NodeDef has no base definition
// The variantId is used to identify the correct NodeDef in a inheritance hierarchy
// The NodeDef can not be part of a inheritance hierarchy if the variantId is null.
// The NodeDef has a variantId but no base definition, so it is the root NodeDef of a inheritance hierarchy
NodeDef::NodeDef(const std::string &_name)
{
    ASSERT(!_name.empty());

    m_name = _name;

#ifdef XML_BACKEND
    if (XML::Element::validateTagName(_name)) {
        m_tagName = _name;
    }
#endif // XML_BACKEND
}

// =============================================================================
// (public)
NodeDef::NodeDef(const std::string &_name, const UnionRef &_variantId)
{
    ASSERT(!_name.empty());
    ASSERT(!_variantId.isNull());

    m_name = _name;
    m_variantId = _variantId;

#ifdef XML_BACKEND
    if (XML::Element::validateTagName(_name)) {
        m_tagName = _name;
    }
#endif // XML_BACKEND
}

// =============================================================================
// (public)
NodeDef::~NodeDef()
{

}

// =============================================================================
// (public)
NodeDef::NodeDef(const NodeDef &copy)
{
    *this = copy;
}

// =============================================================================
// (public)
NodeDef::NodeDef(NodeDef &&move)
{
    *this = std::move(move);
}

// =============================================================================
// (public)
NodeDef &NodeDef::operator=(const NodeDef &copy)
{
    // Copy of self is not allowed
    ASSERT(this != &copy);

    m_name = copy.m_name;
    m_tagName = copy.m_tagName;
    m_variantId = copy.m_variantId;
    m_displayName = copy.m_displayName;
    m_color = copy.m_color;

    m_base = copy.m_base;
    m_derivedList.clear();
    for (auto dDef: copy.m_derivedList)
    {
        m_derivedList.push_back(dDef);
    }

    m_valueList.clear();
    for (const auto& value: copy.m_valueList)
    {
        m_valueList.push_back(value->copy());
    }
    m_indexOfKeyLeafDef = copy.m_indexOfKeyLeafDef;
    m_indexOfVariantLeafDef = copy.m_indexOfVariantLeafDef;

    m_containerList.clear();
    for (const auto& container: copy.m_containerList) {
        m_containerList.push_back(container->copy());
    }
    m_containerGroup = copy.m_containerGroup->copy();

    m_parentContainerDefs.clear();
    for (const ContainerDef* container: copy.m_parentContainerDefs) {
        m_parentContainerDefs.push_back(container);
    }
    return *this;
}

// =============================================================================
// (public)
NodeDef &NodeDef::operator=(NodeDef &&move)
{
    m_name = std::move(move.m_name);
    m_displayName = std::move(move.m_displayName);
    m_color = std::move(move.m_color);
    m_tagName = std::move(move.m_tagName);
    m_variantId = std::move(move.m_variantId);

    m_base = move.m_base;
    m_derivedList = std::move(move.m_derivedList);

    m_valueList = std::move(move.m_valueList);
    m_indexOfKeyLeafDef = move.m_indexOfKeyLeafDef;
    m_indexOfVariantLeafDef = move.m_indexOfVariantLeafDef;

    m_containerList = std::move(move.m_containerList);
    m_containerGroup = std::move(move.m_containerGroup);

    m_parentContainerDefs = std::move(move.m_parentContainerDefs);
    return *this;
}

// =============================================================================
// (public)
bool NodeDef::isNull() const
{
    return m_name.empty();
}

// =============================================================================
// (public)
const std::string& NodeDef::name() const
{
    return m_name;
}

// =============================================================================
// (public)
std::string NodeDef::displayName(bool basic) const
{
    if (!m_displayName.empty()) { return m_displayName; }

    if (m_name.empty()) { return std::string(); }

    if (basic || m_variantId.isNull()) { return m_name; }

    std::string inheritanceStr;
    if (!m_variantId.get(inheritanceStr)) { return m_name; }

    if (hasBase()) {
        return baseRoot()->displayName(true) + "(" + inheritanceStr + ")";
    } else {
        return m_name + "(" + inheritanceStr + ")";
    }
}
// =============================================================================
// (public)
const std::string&NodeDef::tooltip() const
{
    return m_tooltip;
}

#ifdef XML_BACKEND
// =============================================================================
// (public)
const std::string& NodeDef::tagName() const
{
    return m_tagName;
}
#endif // XML_BACKEND

// =============================================================================
// (public)
bool NodeDef::hasColor() const
{
    if (m_color.isValid()) {
        return true;
    }
    if (hasBase()) {
        return base()->hasColor();
    }
    return false;
}

// =============================================================================
// (public)
const Color &NodeDef::color() const
{
    if (m_color.isValid()) {
        return m_color;
    }
    if (hasBase()) {
        return base()->color();
    }
    ASSERT(false);
    return m_color;
}

// =============================================================================
// (public)
bool NodeDef::hasImagePath() const
{
    if (!m_imagePath.empty()) {
        return true;
    }
    if (hasBase()) {
        return base()->hasImagePath();
    }
    return false;
}

// =============================================================================
// (public)
const std::string &NodeDef::imagePath() const
{
    if (!m_imagePath.empty()) {
        return m_imagePath;
    }
    if (hasBase()) {
        return base()->imagePath();
    }
    ASSERT(false);
    return m_imagePath;
}

// =============================================================================
// (public)
const NodeSettings &NodeDef::settings() const
{
    return m_settings;
}

// =============================================================================
// (public)
const UnionRef NodeDef::variantId() const
{
    return m_variantId;
}

// =============================================================================
// (public)
std::vector<UnionRef> NodeDef::variantIdList(bool includeBase, bool includeDerived) const
{
    std::vector<UnionRef> idList;
    getVariantIdList(idList, includeBase, includeDerived);
    return idList;
}

// =============================================================================
// (public)
void NodeDef::getVariantIdList(std::vector<UnionRef> &idList, bool includeBase, bool includeDerived) const
{
    if (includeBase && hasBase()) {
        m_base.lock()->getVariantIdList(idList, true, false);
    }

    idList.push_back(m_variantId);

    if (includeDerived && hasDerived()) {
        for (const auto &dNodeDef: m_derivedList)
        {
            dNodeDef->getVariantIdList(idList, false, true);
        }
    }
}

// =============================================================================
// (public)
const NodeDef* NodeDef::baseRoot() const
{
    if (hasBase()) {
        return base()->baseRoot();
    }
    return this;
}

// =============================================================================
// (public)
const NodeDef *NodeDef::validVariant(const UnionRef &variantId, bool includeBase, bool includeDerived) const
{
    if (!hasVariants() || m_variantId.isEqual(variantId)) { return this; }

    if (includeBase && hasBase()) {
        const NodeDef *variant = m_base.lock()->validVariant(variantId, true, false);
        if (variant) {
            return variant;
        }
    }

    if (includeDerived && hasDerived()) {
        for (const auto &dNodeDef: m_derivedList)
        {
            const NodeDef *variant = dNodeDef->validVariant(variantId, true, false);
            if (variant) {
                return variant;
            }
        }
    }

    return nullptr;
}

// =============================================================================
// (public)
const NodeDef *NodeDef::validVariant(const NodeData &nodeData, bool includeBase, bool includeDerived) const
{
    if (nodeData.isNull()) { return nullptr; }

    switch (nodeData.type()) {
#ifdef XML_BACKEND
    case NodeData::Type::XML:
        if (nodeData.xmlNode().compareTagName(tagName()) != 0) { return nullptr; }
        break;
#endif // XML_BACKEND
    default:
        // _nodeData.type() returns an unhandled type that needs to be implemented
        ASSERT(false);
        return nullptr;
    }

    // Check the list of parent node names are empty,
    // otherwise any parent element goes
    if (!m_parentContainerDefs.empty()) {
        if (parentNode(nodeData, nullptr, includeBase, includeDerived).isNull()) {
            return nullptr;
        }
    }

    if (!hasVariants()) {
        return this;
    }

    // Check if the part id of the variant type matches
    UnionValue variantId = variantLeafDef().value(nodeData);

    return validVariant(variantId, includeBase, includeDerived);
}

// =============================================================================
// (public)
std::vector<const NodeDef *> NodeDef::variantList(bool includeBase, bool includeDerived) const
{
    std::vector<const NodeDef *> dList;
    getVariantList(dList, includeBase, includeDerived);
    return dList;
}

// =============================================================================
// (public)
void NodeDef::getVariantList(std::vector<const NodeDef *> &vList, bool includeBase, bool includeDerived) const
{
    if (includeBase && hasBase()) {
        m_base.lock()->getVariantList(vList, true, false);
    }

    vList.push_back(this);

    if (includeDerived && hasDerived()) {
        for (const auto &dNodeDef: m_derivedList)
        {
            dNodeDef->getVariantList(vList, false, true);
        }
    }
}

// =============================================================================
// (public)
bool NodeDef::validate(const UnionRef &variantId, bool includeBase, bool includeDerived) const
{
    if (m_variantId.isEqual(variantId)) { return true; }

    if (includeBase && hasBase()) {
        if (m_base.lock()->validate(variantId, true, false)) {
            return true;
        }
    }

    if (includeDerived && hasDerived()) {
        for (const auto &dNodeDef: m_derivedList)
        {
            if (dNodeDef->validate(variantId, false, true)) {
                return true;
            }
        }
    }

    return false;
}

// =============================================================================
// (public)
// The validation of a data node return true if:
// 1. The element is valid
// 2. The tag name match
// 3. One of the parent tag names match
// 4. The part id of the variant type match
bool NodeDef::validate(const NodeData &_nodeData, bool includeBase, bool includeDerived) const
{
    if (_nodeData.isNull()) { return false; }

    switch (_nodeData.type()) {
#ifdef XML_BACKEND
    case NodeData::Type::XML:
        if (_nodeData.xmlNode().compareTagName(tagName()) != 0) { return false; }
        break;
#endif // XML_BACKEND
    default:
        // _nodeData.type() returns an unhandled type that needs to be implemented
        ASSERT(false);
        return false;
    }

    // Check the list of parent node names are empty,
    // otherwise any parent element goes
    if (!m_parentContainerDefs.empty()) {
        if (parentNode(_nodeData, nullptr, includeBase, includeDerived).isNull()) {
            return false;
        }
    }

    if (!hasVariants()) {
        return true;
    }

    // Check if the part id of the variant type matches
    UnionValue variantId = variantLeafDef().value(_nodeData);

    return validate(variantId, includeBase, includeDerived);
}

// =============================================================================
// (public)
ValidationState NodeDef::validationState(const UnionRef &_variantId) const
{
    if (validate(_variantId, false, false)) {
        return VALIDATION_STATE_VALID;
    }

    if (hasBase() && validate(_variantId, true, false)) {
        return VALIDATION_STATE_VALID_BASE;
    }

    if (hasDerived() && validate(_variantId, false, true)) {
        return VALIDATION_STATE_VALID_DERIVED;
    }

    if (hasBase() && baseRoot()->validate(_variantId, false, true)) {
        return VALIDATION_STATE_VALID_OTHER;
    }
    return VALIDATION_STATE_INVALID;
}

// =============================================================================
// (public)
ValidationState NodeDef::validationState(const NodeData &nodeData) const
{
    if (!baseRoot()->validate(nodeData, false, true)) {
        return VALIDATION_STATE_INVALID;
    }

    UnionValue _variantId = variantLeafDef().value(nodeData);

    if (!hasDerived() || validate(_variantId, false, false)) {
        return VALIDATION_STATE_VALID;
    }

    if (hasBase() && validate(_variantId, true, false)) {
        return VALIDATION_STATE_VALID_BASE;
    }

    if (hasDerived() && validate(_variantId, false, true)) {
        return VALIDATION_STATE_VALID_DERIVED;
    }

    return VALIDATION_STATE_VALID_OTHER;
}

// =============================================================================
// (public)
int NodeDef::valueCount(bool includeBase, bool includeDerived) const
{
    int count = static_cast<int>(m_valueList.size());

    if (includeBase && hasBase()) {
        count += m_base.lock()->valueCount(true, false);
    }

    if (includeDerived && hasDerived()) {
        for (const auto &dNodeDef: m_derivedList)
        {
            count += dNodeDef->valueCount(false, true);
        }
    }

    return count;
}

// =============================================================================
// (public)
bool NodeDef::hasValue(const std::string &valueName, bool includeBase, bool includeDerived) const
{
    for (const auto &value: m_valueList) {
        if (value->name() == valueName) {
            return true;
        }
    }

    if (includeBase && hasBase()) {
        if (m_base.lock()->hasValue(valueName, true, false)) {
            return true;
        }
    }

    if (includeDerived && hasDerived()) {
        for (const auto &dNodeDef: m_derivedList)
        {
            if (dNodeDef->hasValue(valueName, false, true)) {
                return true;
            }
        }
    }

    return false;
}

// =============================================================================
// (public)
int NodeDef::valueIndex(const LeafDef *leafDef, bool includeBase, bool includeDerived) const
{
    int i = 0;
    for (const LeafDefUPtr &value: m_valueList)
    {
        if (value.get() == leafDef) {
            if (includeBase && hasBase()) {
                i += m_base.lock()->valueCount(true, false);
            }
            return i;
        }
        i++;
    }

    if (includeBase && hasBase()) {
        i = m_base.lock()->valueIndex(leafDef, true, false);
        if (i != -1) {
            return i;
        }
    }

    if (includeDerived && hasDerived()) {
        i = valueCount(includeBase, false);
        for (const auto &dNodeDef: m_derivedList)
        {
            int i2 = dNodeDef->valueIndex(leafDef, false, true);
            if (i2 != -1) {
                return i + i2;
            } else {
                i += dNodeDef->valueCount(false, true);
            }
        }
    }

    return -1;
}

// =============================================================================
// (public)
const LeafDef &NodeDef::value(int index, bool includeBase, bool includeDerived) const
{
    if (includeBase && hasBase()) {
        int baseDefCount = m_base.lock()->valueCount(true, false);
        if (index < baseDefCount) {
            return m_base.lock()->value(index, true, false);
        }
        index -= baseDefCount;
    }

    int vCount = static_cast<int>(m_valueList.size());
    if (index < vCount) {
        return *m_valueList[static_cast<vSize>(index)].get();
    }
    index -= vCount;

    if (includeDerived && hasDerived()) {
        for (const auto &dNodeDef: m_derivedList)
        {
            vCount = dNodeDef->valueCount(false, true);
            if (index < vCount) {
                return dNodeDef->value(index, false, true);
            }
            index -= vCount;
        }
    }

    return LeafDef::emptyDef();
}

// =============================================================================
// (public)
const LeafDef &NodeDef::value(const std::string &valueName, bool includeBase, bool includeDerived) const
{
    for (const auto &value: m_valueList) {
        if (value->name() == valueName) {
            return *value.get();
        }
    }

    if (includeBase && hasBase()) {
        const LeafDef & result = m_base.lock()->value(valueName, true, false);
        if (!result.isNull()) {
            return result;
        }
    }

    if (includeDerived && hasDerived()) {
        for (const auto &dNodeDef: m_derivedList)
        {
            const LeafDef & result = dNodeDef->value(valueName, false, true);
            if (!result.isNull()) {
                return result;
            }
        }
    }

    return LeafDef::emptyDef();
}

// =============================================================================
// (public)
LeafDef &NodeDef::value(int index, bool includeBase, bool includeDerived)
{
    if (includeBase && hasBase()) {
        int baseDefCount = m_base.lock()->valueCount(true, false);
        if (index < baseDefCount) {
            return m_base.lock()->value(index, true, false);
        }
        index -= baseDefCount;
    }

    int vCount = static_cast<int>(m_valueList.size());
    if (index < vCount) {
        return *m_valueList[static_cast<vSize>(index)].get();
    }
    index -= vCount;

    if (includeDerived && hasDerived()) {
        for (const auto &dNodeDef: m_derivedList)
        {
            vCount = dNodeDef->valueCount(false, true);
            if (index < vCount) {
                return dNodeDef->value(index, false, true);
            }
            index -= vCount;
        }
    }

    return LeafDef::emptyDef();
}

// =============================================================================
// (public)
LeafDef &NodeDef::value(const std::string &valueName, bool includeBase, bool includeDerived)
{
    for (const auto &value: m_valueList) {
        if (value->name() == valueName) {
            return *value.get();
        }
    }

    if (includeBase && hasBase()) {
        LeafDef & result = m_base.lock()->value(valueName, true, false);
        if (!result.isNull()) {
            return result;
        }
    }

    if (includeDerived && hasDerived()) {
        for (const auto &dNodeDef: m_derivedList)
        {
            LeafDef & result = dNodeDef->value(valueName, false, true);
            if (!result.isNull()) {
                return result;
            }
        }
    }

    return LeafDef::emptyDef();
}

// =============================================================================
// (public)
std::vector<const LeafDef *> NodeDef::valueList(bool includeBase, bool includeDerived) const
{
    std::vector<const LeafDef *> vList;
    getValueList(vList, includeBase, includeDerived);
    return vList;
}

// =============================================================================
// (public)
void NodeDef::getValueList(std::vector<const LeafDef*>& vList, bool includeBase, bool includeDerived) const
{
    if (includeBase && hasBase()) {
        m_base.lock()->getValueList(vList, true, false);
    }

    for (const auto& v: m_valueList)
    {
        vList.push_back(v.get());
    }

    if (includeDerived && hasDerived()) {
        for (const auto &dNodeDef: m_derivedList)
        {
            dNodeDef->getValueList(vList, false, true);
        }
    }
}

// =============================================================================
// (public)
bool NodeDef::hasKey() const
{
    return m_indexOfKeyLeafDef >= 0;
}

// =============================================================================
// (public)
const LeafDef& NodeDef::keyLeafDef() const
{
    return value(m_indexOfKeyLeafDef);
}

// =============================================================================
// (public)
LeafDef& NodeDef::keyLeafDef()
{
    return value(m_indexOfKeyLeafDef);
}

// =============================================================================
// (public)
const LeafDef& NodeDef::variantLeafDef() const
{
    if (m_indexOfVariantLeafDef < 0) { return LeafDef::emptyDef(); }
    else { return value(m_indexOfVariantLeafDef); }
}

// =============================================================================
// (public)
LeafDef& NodeDef::variantLeafDef()
{
    if (m_indexOfVariantLeafDef < 0) { return LeafDef::emptyDef(); }
    else { return value(m_indexOfVariantLeafDef); }
}

// =============================================================================
// (public)
int NodeDef::containerCount(bool includeBase, bool includeDerived) const
{
    int count = static_cast<int>(m_containerList.size());

    if (includeBase && hasBase()) {
        count += m_base.lock()->containerCount();
    }
    if (includeDerived && hasDerived()) {
        for (const auto &dNodeDef: m_derivedList)
        {
            count += dNodeDef->containerCount(false, true);
        }
    }

    return count;
}

// =============================================================================
// (public)
bool NodeDef::hasContainer(const std::string &_name, bool includeBase, bool includeDerived) const
{
    for (const auto &container: m_containerList) {
        if (container->containerDef()->name() == _name) {
            return true;
        }
    }

    if (includeBase && hasBase()) {
        if (m_base.lock()->hasContainer(_name, true, false)) {
            return true;
        }
    }

    if (includeDerived && hasDerived()) {
        for (const auto &dNodeDef: m_derivedList)
        {
            if (dNodeDef->hasContainer(_name, false, true)) {
                return true;
            }
        }
    }

    return false;
}

// =============================================================================
// (public)
const ContainerDef &NodeDef::container(int index, bool includeBase, bool includeDerived) const
{
    if (includeBase && hasBase()) {
        int baseDefCount = m_base.lock()->containerCount(true, false);
        if (index < baseDefCount) {
            return m_base.lock()->container(index, true, false);
        }
        index -= baseDefCount;
    }

    int pCount = static_cast<int>(m_parentContainerDefs.size());
    if (index < pCount) {
        return *m_containerList[static_cast<size_t>(index)].get();
    }
    index -= pCount;

    if (includeDerived && hasDerived()) {
        for (const auto &dNodeDef: m_derivedList)
        {
            pCount = dNodeDef->containerCount(false, true);
            if (index < pCount) {
                return dNodeDef->container(index, false, true);
            }
            index -= pCount;
        }
    }

    return ContainerDef::emptyChildNodeDef();
}

// =============================================================================
// (public)
const ContainerDef &NodeDef::container(const std::string& _name, bool includeBase, bool includeDerived) const
{
    for (const auto& _child: m_containerList)
    {
        if (_child->containerDef()->name() == _name) {
            return *_child.get();
        }
    }

    if (includeBase && hasBase()) {
        const ContainerDef & childContainer = m_base.lock()->container(_name);
        if (!childContainer.isNull()) { return childContainer; }
    }

    if (includeDerived && hasDerived()) {
        // Check if a derived NodeDefs has the container
        for (const auto &dNodeDef: m_derivedList)
        {
            const ContainerDef & result = dNodeDef->container(_name, false, true);
            if (!result.isNull()) { return result; }
        }
    }

    return ContainerDef::emptyChildNodeDef();
}

// =============================================================================
// (public)
const ContainerDef &NodeDef::container(const NodeData &childNodeData, bool includeBase, bool includeDerived) const
{
    if (childNodeData.isNull()) { return ContainerDef::emptyChildNodeDef(); }

    for (const auto& _child: m_containerList)
    {
        if (_child->validate(childNodeData)) {
            return *_child.get();
        }
    }

    if (includeBase && hasBase()) {
        return m_base.lock()->container(childNodeData);
    }

    if (includeDerived && hasDerived()) {
        // Check if a derived NodeDefs has the container
        for (const auto &dNodeDef: m_derivedList)
        {
            const ContainerDef & result = dNodeDef->container(childNodeData, false, true);
            if (!result.isNull()) { return result; }
        }
    }

    return ContainerDef::emptyChildNodeDef();
}

// =============================================================================
// (public)
std::vector<const ContainerDef *> NodeDef::containerList(bool includeBase, bool includeDerived) const
{
    std::vector<const ContainerDef *> cList;
    getContainerList(cList, includeBase, includeDerived);
    return cList;
}

// =============================================================================
// (public)
void NodeDef::getContainerList(std::vector<const ContainerDef *> &list, bool includeBase, bool includeDerived) const
{
    if (includeBase && hasBase()) {
        m_base.lock()->getContainerList(list);
    }
    for (const auto& _child: m_containerList) {
        list.push_back(_child.get());
    }

    if (includeDerived && hasDerived()) {
        // get contains on derived NodeDefs
        for (const auto &dNodeDef: m_derivedList)
        {
            dNodeDef->getContainerList(list, false, true);
        }
    }
}

// =============================================================================
// (public)
const NodeDef* NodeDef::childDef(int index, bool includeBase) const
{
    return container(index, includeBase).containerDef();
}

// =============================================================================
// (public)
const NodeDef* NodeDef::childDef(const std::string &_name, bool includeBase, bool includeDerived) const
{
    return container(_name, includeBase, includeDerived).containerDef();
}

// =============================================================================
// (public)
const NodeDef* NodeDef::childDef(const NodeData &childNodeData, bool includeBase, bool includeDerived) const
{
    return container(childNodeData, includeBase, includeDerived).containerDef(childNodeData);
}

// =============================================================================
// (public)
std::vector<const NodeDef *> NodeDef::childDefList(bool includeBase, bool includeDerived) const
{
    std::vector<const NodeDef *> cList;
    getChildDefList(cList, includeBase, includeDerived);
    return cList;
}

// =============================================================================
// (public)
void NodeDef::getChildDefList(std::vector<const NodeDef *> &cList, bool includeBase, bool includeDerived) const
{
    if (includeBase && hasBase()) {
        m_base.lock()->getChildDefList(cList, true);
    }
    for (const auto& _child: m_containerList) {
        cList.push_back(_child.get()->containerDef());
    }

    if (includeDerived && hasDerived()) {
        // get child NodeDefs on derived NodeDefs
        for (const auto &dNodeDef: m_derivedList)
        {
            dNodeDef->getChildDefList(cList, false, true);
        }
    }
}

// =============================================================================
// (public)
const ContainerGroupDef& NodeDef::containerGroup() const
{
    if (!m_containerGroup) {
        m_containerGroup = std::make_unique<ContainerGroupDef>(this);
    }
    return *m_containerGroup.get();
}

// =============================================================================
// (public)
NodeData NodeDef::parentNode(const NodeData &nodeData, const NodeDef** parentNodeDef, bool includeBase, bool includeDerived) const
{
    if (nodeData.isNull()) { return NodeData(); }

    for (auto parentContainer: m_parentContainerDefs)
    {
        NodeData parentNode = parentContainer->hostNode(nodeData);
        if (!parentNode.isNull()) {
            if (parentNodeDef) {
                *parentNodeDef = parentContainer->hostDef();
            }
            return parentNode;
        }
    }

    if (includeBase && hasBase()) {
        NodeData result = m_base.lock()->parentNode(nodeData, parentNodeDef, true, false);
        if (!result.isNull()) {
            return result;
        }
    }

    if (includeDerived && hasDerived()) {
        for (const auto &dNodeDef: m_derivedList)
        {
            NodeData result = dNodeDef->parentNode(nodeData, parentNodeDef, false, true);
            if (!result.isNull()) {
                return result;
            }
        }
    }

    // None of the parent child definitions could locate a valid parent
    if (parentNodeDef) { *parentNodeDef = nullptr; }
    return NodeData();
}

// =============================================================================
// (public)
int NodeDef::parentContainerCount(bool includeBase, bool includeDerived) const
{
    int count = static_cast<int>(m_parentContainerDefs.size());

    if (includeBase && hasBase()) { count += m_base.lock()->parentContainerCount(true, false); }

    if (includeDerived && hasDerived()) {
        for (const auto &dNodeDef: m_derivedList)
        {
            count += dNodeDef->parentContainerCount(false, true);
        }
    }

    return count;
}

// =============================================================================
// (public)
const ContainerDef *NodeDef::parentContainer(int index, bool includeBase, bool includeDerived) const
{
    if (includeBase && hasBase()) {
        int baseDefCount = m_base.lock()->parentContainerCount(true, false);
        if (index < baseDefCount) {
            return m_base.lock()->parentContainer(index, true, false);
        }
        index -= baseDefCount;
    }

    int pCount = static_cast<int>(m_parentContainerDefs.size());
    if (index < pCount) {
        return m_parentContainerDefs[static_cast<size_t>(index)];
    }
    index -= pCount;

    if (includeDerived && hasDerived()) {
        for (const auto &dNodeDef: m_derivedList)
        {
            pCount = dNodeDef->parentContainerCount(false, true);
            if (index < pCount) {
                return dNodeDef->parentContainer(index, false, true);
            }
            index -= pCount;
        }
    }

    return nullptr;
}

// =============================================================================
// (public)
const ContainerDef *NodeDef::parentContainer(const std::string &_name, bool includeBase, bool includeDerived) const
{
    for (const ContainerDef* parentContainer: m_parentContainerDefs)
    {
        if (parentContainer->hostDef()->name() == _name) {
            return parentContainer;
        }
    }

    if (includeBase && hasBase()) {
        return base()->parentContainer(_name, true, false);
    }

    if (includeDerived && hasDerived()) {
        for (const auto &dNodeDef: m_derivedList)
        {
            const ContainerDef * result = dNodeDef->parentContainer(_name, false, true);
            if (result) { return result; }
        }
    }

    return nullptr;
}

// =============================================================================
// (public)
const ContainerDef *NodeDef::parentContainer(const NodeData &parentNodeData, bool includeBase, bool includeDerived) const
{
    if (parentNodeData.isNull()) { return nullptr; }
    for (const ContainerDef* parentContainer: m_parentContainerDefs)
    {
        if (parentContainer->hostDef()->validate(parentNodeData, false, true)) {
            return parentContainer;
        }
    }

    if (includeBase && hasBase()) {
        return base()->parentContainer(parentNodeData);
    }

    if (includeDerived && hasDerived()) {
        for (const auto &dNodeDef: m_derivedList)
        {
            const ContainerDef * result = dNodeDef->parentContainer(parentNodeData, false, true);
            if (result) { return result; }
        }
    }

    return nullptr;
}

// =============================================================================
// (public)
bool NodeDef::isParent(const NodeData &nodeData, const NodeData &refNodeData, bool recursive) const
{
    // Find the parent data node
    NodeData _parentNode = parentNode(nodeData);
    if (_parentNode.isNull()) { return false; }

    // Find the definition of the parent data node
    const ContainerDef* _parentContainer = parentContainer(_parentNode);
    if (_parentContainer == nullptr) { return false; }

    if (_parentNode == refNodeData) {
        return true;
    }

    // Test the parent of the parent if recursive is true
    if (recursive) {
        _parentContainer->hostDef()->isParent(nodeData, refNodeData, recursive);
    }
    return false;
}

// =============================================================================
// (public)
void NodeDef::onNodeInserted(const NodeData &_nodeData) const
{
    // Get all containers also from derived nodes
    auto cList = containerList();
    for (const ContainerDef* cDef: cList)
    {
        int count = cDef->nodeCount(_nodeData);
        while (count < cDef->minCount()) {
            cDef->insertNode(_nodeData, count);
            count++;
        }
    }

    Node node(this, _nodeData);
    auto vList = valueList();
    for (const LeafDef* vDef: vList)
    {
        if (vDef->settings().value(REQUIRED) > 0 &&
            vDef->settings().value(UNIQUE) > 0) {

            std::vector<std::string> valueList = QB::createSiblings()->leafSPtr(vDef->name())->toValueList<std::string>(node);
            if (vDef->options().isUsed() && vDef->settings().value(OPTION_ONLY)) {
                std::vector<std::string> optionList;
                if (vDef->options().getOptions(optionList, &node)) {
                    for (const std::string &option: optionList) {
                        if (std::find(valueList.begin(), valueList.end(), option) == valueList.end()) {
                            vDef->setValue(_nodeData, option);
                        }
                    }

                }
            } else if (vDef->hasDefaultValue()) {
                std::string defaultValue = vDef->defaultValue().value<std::string>();
                if (std::find(valueList.begin(), valueList.end(), defaultValue) == valueList.end()) {
                    vDef->setValue(_nodeData, vDef->defaultValue());
                    return;
                }

                std::string value;
                int count = 1;
                do {
                    value = defaultValue + "_" + std::to_string(count++);
                } while (contains(valueList, value));

                vDef->setValue(_nodeData, value);
            }
        }
    }
}

// =============================================================================
// (public)
void NodeDef::onNodeMoved(const NodeData &_nodeData) const
{
    UNUSED(_nodeData);
}

// =============================================================================
// (public)
void NodeDef::onNodeCloned(const NodeData &_nodeData) const
{
    UNUSED(_nodeData);
}

} // namespace Oak::Model

