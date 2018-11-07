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
#include "ValueDefBuilder.h"
#include "EntryQuery.h"
#include "ServiceFunctions.h"
#include "QueryBuilder.h"

#include "../ServiceFunctions/Assert.h"

namespace Oak {
namespace Model {

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
    m_indexOfKeyValueDef = copy.m_indexOfKeyValueDef;
    m_indexOfVariantValueDef = copy.m_indexOfVariantValueDef;

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
    m_indexOfKeyValueDef = move.m_indexOfKeyValueDef;
    m_indexOfVariantValueDef = move.m_indexOfVariantValueDef;

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
    if (m_variantId.isEqual(variantId)) { return this; }

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
const NodeDef *NodeDef::validVariant(const Node &node, bool includeBase, bool includeDerived) const
{
    if (node.isNull()) { return nullptr; }

    switch (node.type()) {
#ifdef XML_BACKEND
    case Node::Type::XML:
        if (node.xmlNode().compareTagName(tagName()) != 0) { return nullptr; }
        break;
#endif // XML_BACKEND
    default:
        // _node.type() returns an unhandled type that needs to be implemented
        ASSERT(false);
        return nullptr;
    }

    // Check the list of parent node names are empty,
    // otherwise any parent element goes
    if (!m_parentContainerDefs.empty()) {
        if (parentNode(node, nullptr, includeBase, includeDerived).isNull()) {
            return nullptr;
        }
    }

    if (!hasVariants()) {
        return this;
    }

    // Check if the part id of the variant type matches
    UnionValue variantId = variantValueDef().value(node);

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
bool NodeDef::validate(const Node &_node, bool includeBase, bool includeDerived) const
{
    if (_node.isNull()) { return false; }

    switch (_node.type()) {
#ifdef XML_BACKEND
    case Node::Type::XML:
        if (_node.xmlNode().compareTagName(tagName()) != 0) { return false; }
        break;
#endif // XML_BACKEND
    default:
        // _node.type() returns an unhandled type that needs to be implemented
        ASSERT(false);
        return false;
    }

    // Check the list of parent node names are empty,
    // otherwise any parent element goes
    if (!m_parentContainerDefs.empty()) {
        if (parentNode(_node, nullptr, includeBase, includeDerived).isNull()) {
            return false;
        }
    }

    if (!hasVariants()) {
        return true;
    }

    // Check if the part id of the variant type matches
    UnionValue variantId = variantValueDef().value(_node);

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
ValidationState NodeDef::validationState(const Node &node) const
{
    if (!baseRoot()->validate(node, false, true)) {
        return VALIDATION_STATE_INVALID;
    }

    UnionValue _variantId = variantValueDef().value(node);

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
int NodeDef::valueIndex(const ValueDef *valueDef, bool includeBase, bool includeDerived) const
{
    int i = 0;
    for (const ValueDefUPtr &value: m_valueList)
    {
        if (value.get() == valueDef) {
            if (includeBase && hasBase()) {
                i += m_base.lock()->valueCount(true, false);
            }
            return i;
        }
        i++;
    }

    if (includeBase && hasBase()) {
        i = m_base.lock()->valueIndex(valueDef, true, false);
        if (i != -1) {
            return i;
        }
    }

    if (includeDerived && hasDerived()) {
        i = valueCount(includeBase, false);
        for (const auto &dNodeDef: m_derivedList)
        {
            int i2 = dNodeDef->valueIndex(valueDef, false, true);
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
const ValueDef &NodeDef::value(int index, bool includeBase, bool includeDerived) const
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

    return ValueDef::emptyDef();
}

// =============================================================================
// (public)
const ValueDef &NodeDef::value(const std::string &valueName, bool includeBase, bool includeDerived) const
{
    for (const auto &value: m_valueList) {
        if (value->name() == valueName) {
            return *value.get();
        }
    }

    if (includeBase && hasBase()) {
        const ValueDef & result = m_base.lock()->value(valueName, true, false);
        if (!result.isNull()) {
            return result;
        }
    }

    if (includeDerived && hasDerived()) {
        for (const auto &dNodeDef: m_derivedList)
        {
            const ValueDef & result = dNodeDef->value(valueName, false, true);
            if (!result.isNull()) {
                return result;
            }
        }
    }

    return ValueDef::emptyDef();
}

// =============================================================================
// (public)
ValueDef &NodeDef::value(int index, bool includeBase, bool includeDerived)
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

    return ValueDef::emptyDef();
}

// =============================================================================
// (public)
ValueDef &NodeDef::value(const std::string &valueName, bool includeBase, bool includeDerived)
{
    for (const auto &value: m_valueList) {
        if (value->name() == valueName) {
            return *value.get();
        }
    }

    if (includeBase && hasBase()) {
        ValueDef & result = m_base.lock()->value(valueName, true, false);
        if (!result.isNull()) {
            return result;
        }
    }

    if (includeDerived && hasDerived()) {
        for (const auto &dNodeDef: m_derivedList)
        {
            ValueDef & result = dNodeDef->value(valueName, false, true);
            if (!result.isNull()) {
                return result;
            }
        }
    }

    return ValueDef::emptyDef();
}

// =============================================================================
// (public)
std::vector<const ValueDef *> NodeDef::valueList(bool includeBase, bool includeDerived) const
{
    std::vector<const ValueDef *> vList;
    getValueList(vList, includeBase, includeDerived);
    return vList;
}

// =============================================================================
// (public)
void NodeDef::getValueList(std::vector<const ValueDef*>& vList, bool includeBase, bool includeDerived) const
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
    return m_indexOfKeyValueDef >= 0;
}

// =============================================================================
// (public)
const ValueDef& NodeDef::keyValueDef() const
{
    return value(m_indexOfKeyValueDef);
}

// =============================================================================
// (public)
ValueDef& NodeDef::keyValueDef()
{
    return value(m_indexOfKeyValueDef);
}

// =============================================================================
// (public)
const ValueDef& NodeDef::variantValueDef() const
{
    if (m_indexOfVariantValueDef < 0) { return ValueDef::emptyDef(); }
    else { return value(m_indexOfVariantValueDef); }
}

// =============================================================================
// (public)
ValueDef& NodeDef::variantValueDef()
{
    if (m_indexOfVariantValueDef < 0) { return ValueDef::emptyDef(); }
    else { return value(m_indexOfVariantValueDef); }
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
const ContainerDef &NodeDef::container(const Node &childNode, bool includeBase, bool includeDerived) const
{
    if (childNode.isNull()) { return ContainerDef::emptyChildNodeDef(); }

    for (const auto& _child: m_containerList)
    {
        if (_child->validate(childNode)) {
            return *_child.get();
        }
    }

    if (includeBase && hasBase()) {
        return m_base.lock()->container(childNode);
    }

    if (includeDerived && hasDerived()) {
        // Check if a derived NodeDefs has the container
        for (const auto &dNodeDef: m_derivedList)
        {
            const ContainerDef & result = dNodeDef->container(childNode, false, true);
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
const NodeDef* NodeDef::childDef(const Node &childNode, bool includeBase, bool includeDerived) const
{
    return container(childNode, includeBase, includeDerived).containerDef(childNode);
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
Node NodeDef::parentNode(const Node &node, const NodeDef** parentNodeDef, bool includeBase, bool includeDerived) const
{
    if (node.isNull()) { return Node(); }

    for (auto parentContainer: m_parentContainerDefs)
    {
        Node parentNode = parentContainer->hostNode(node);
        if (!parentNode.isNull()) {
            if (parentNodeDef) {
                *parentNodeDef = parentContainer->hostDef();
            }
            return parentNode;
        }
    }

    if (includeBase && hasBase()) {
        Node result = m_base.lock()->parentNode(node, parentNodeDef, true, false);
        if (!result.isNull()) {
            return result;
        }
    }

    if (includeDerived && hasDerived()) {
        for (const auto &dNodeDef: m_derivedList)
        {
            Node result = dNodeDef->parentNode(node, parentNodeDef, false, true);
            if (!result.isNull()) {
                return result;
            }
        }
    }

    // None of the parent child definitions could locate a valid parent
    if (parentNodeDef) { *parentNodeDef = nullptr; }
    return Node();
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
const ContainerDef *NodeDef::parentContainer(const Node &parentNode, bool includeBase, bool includeDerived) const
{
    if (parentNode.isNull()) { return nullptr; }
    for (const ContainerDef* parentContainer: m_parentContainerDefs)
    {
        if (parentContainer->hostDef()->validate(parentNode, false, true)) {
            return parentContainer;
        }
    }

    if (includeBase && hasBase()) {
        return base()->parentContainer(parentNode);
    }

    if (includeDerived && hasDerived()) {
        for (const auto &dNodeDef: m_derivedList)
        {
            const ContainerDef * result = dNodeDef->parentContainer(parentNode, false, true);
            if (result) { return result; }
        }
    }

    return nullptr;
}

// =============================================================================
// (public)
bool NodeDef::isParent(const Node &node, const Node &refNode, bool recursive) const
{
    // Find the parent data node
    Node _parentNode = parentNode(node);
    if (_parentNode.isNull()) { return false; }

    // Find the definition of the parent data node
    const ContainerDef* _parentContainer = parentContainer(_parentNode);
    if (_parentContainer == nullptr) { return false; }

    if (_parentNode == refNode) {
        return true;
    }

    // Test the parent of the parent if recursive is true
    if (recursive) {
        _parentContainer->hostDef()->isParent(node, refNode, recursive);
    }
    return false;
}

// =============================================================================
// (public)
void NodeDef::onNodeInserted(const Node &_node) const
{
    // Get all containers also from derived nodes
    auto cList = containerList();
    for (const ContainerDef* cDef: cList)
    {
        int count = cDef->nodeCount(_node);
        while (count < cDef->minCount()) {
            cDef->insertNode(_node, count);
            count++;
        }
    }

    Item item(this, _node);
    auto vList = valueList();
    for (const ValueDef* vDef: vList)
    {
        if (vDef->settings().value(REQUIRED) > 0 &&
            vDef->settings().value(UNIQUE) > 0) {

            std::vector<std::string> valueList = QB::createSiblings()->EntryUPtr(vDef->name())->toValueList<std::string>(item);
            if (vDef->options().isUsed() && vDef->settings().value(OPTION_ONLY)) {
                std::vector<std::string> optionList;
                if (vDef->options().getOptions(optionList, &item)) {
                    for (const std::string &option: optionList) {
                        if (std::find(valueList.begin(), valueList.end(), option) == valueList.end()) {
                            vDef->setValue(_node, option);
                        }
                    }

                }
            } else if (vDef->hasDefaultValue()) {
                std::string defaultValue = vDef->defaultValue().value<std::string>();
                if (std::find(valueList.begin(), valueList.end(), defaultValue) == valueList.end()) {
                    vDef->setValue(_node, vDef->defaultValue());
                    return;
                }

                std::string value;
                int count = 1;
                do {
                    value = defaultValue + "_" + std::to_string(count++);
                } while (contains(valueList, value));

                vDef->setValue(_node, value);
            }
        }
    }
}

// =============================================================================
// (public)
void NodeDef::onNodeMoved(const Node &_node) const
{
    UNUSED(_node);
}

// =============================================================================
// (public)
void NodeDef::onNodeCloned(const Node &_node) const
{
    UNUSED(_node);
}

} // namespace Model
} // namespace Oak
