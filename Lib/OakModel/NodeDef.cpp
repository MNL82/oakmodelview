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
// The derivedId is used to identify the correct NodeDef in a inheritance hierarchy
// The NodeDef can not be part of a inheritance hierarchy if the derivedId is null.
// The NodeDef has a derivedId but no base definition, so it is the root NodeDef of a inheritance hierarchy
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
NodeDef::NodeDef(const std::string &_name, const UnionRef &_derivedId)
{
    ASSERT(!_name.empty());
    ASSERT(!_derivedId.isNull());

    m_name = _name;
    m_derivedId = _derivedId;

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
    m_derivedId = copy.m_derivedId;
    m_displayName = copy.m_displayName;
    m_color = copy.m_color;

    m_derivedBase = copy.m_derivedBase;
    m_derivedDirectList.clear();
    for (auto dDef: copy.m_derivedDirectList)
    {
        m_derivedDirectList.push_back(dDef);
    }

    m_valueList.clear();
    for (const auto& value: copy.m_valueList)
    {
        m_valueList.push_back(value->copy());
    }
    m_keyValueDefIndex = copy.m_keyValueDefIndex;
    m_derivedIdValueDefIndex = copy.m_derivedIdValueDefIndex;

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
    m_derivedId = std::move(move.m_derivedId);

    m_derivedBase = move.m_derivedBase;
    m_derivedDirectList = std::move(move.m_derivedDirectList);

    m_valueList = std::move(move.m_valueList);
    m_keyValueDefIndex = move.m_keyValueDefIndex;
    m_derivedIdValueDefIndex = move.m_derivedIdValueDefIndex;

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

    if (basic || m_derivedId.isNull()) { return m_name; }

    std::string inheritanceStr;
    if (!m_derivedId.get(inheritanceStr)) { return m_name; }

    if (hasDerivedBase()) {
        return derivedRoot()->displayName(true) + "(" + inheritanceStr + ")";
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
    if (hasDerivedBase()) {
        return derivedBase()->hasColor();
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
    if (hasDerivedBase()) {
        return derivedBase()->color();
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
    if (hasDerivedBase()) {
        return derivedBase()->hasImagePath();
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
    if (hasDerivedBase()) {
        return derivedBase()->imagePath();
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
const UnionRef NodeDef::derivedId() const
{
    return m_derivedId;
}

// =============================================================================
// (public)
std::vector<UnionRef> NodeDef::derivedIdList(bool includeBase, bool includeDerived) const
{
    std::vector<UnionRef> idList;
    getDerivedIdList(idList, includeBase, includeDerived);
    return idList;
}

// =============================================================================
// (public)
void NodeDef::getDerivedIdList(std::vector<UnionRef> &idList, bool includeBase, bool includeDerived) const
{
    if (includeBase && hasDerivedBase()) {
        m_derivedBase.lock()->getDerivedIdList(idList, true, false);
    }

    idList.push_back(m_derivedId);

    if (includeDerived && hasDerivedDiviations()) {
        for (const auto &dNodeDef: m_derivedDirectList)
        {
            dNodeDef->getDerivedIdList(idList, false, true);
        }
    }
}

// =============================================================================
// (public)
const NodeDef* NodeDef::derivedRoot() const
{
    if (hasDerivedBase()) {
        return derivedBase()->derivedRoot();
    }
    return this;
}

// =============================================================================
// (public)
const NodeDef *NodeDef::validDerived(const UnionRef &derivedId, bool includeBase, bool includeDerived) const
{
    if (m_derivedId.isEqual(derivedId)) { return this; }

    if (includeBase && hasDerivedBase()) {
        const NodeDef *variant = m_derivedBase.lock()->validDerived(derivedId, true, false);
        if (variant) {
            return variant;
        }
    }

    if (includeDerived && hasDerivedDiviations()) {
        for (const auto &dNodeDef: m_derivedDirectList)
        {
            const NodeDef *variant = dNodeDef->validDerived(derivedId, true, false);
            if (variant) {
                return variant;
            }
        }
    }

    return nullptr;
}

// =============================================================================
// (public)
const NodeDef *NodeDef::validDerived(Node node, bool includeBase, bool includeDerived) const
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

    if (!hasDiviations()) {
        return this;
    }

    // Check if the part id of the derived type matches
    UnionValue derivedId = derivedIdValueDef().value(node);

    return validDerived(derivedId, includeBase, includeDerived);
}

// =============================================================================
// (public)
std::vector<const NodeDef *> NodeDef::derivedList(bool includeBase, bool includeDerived) const
{
    std::vector<const NodeDef *> dList;
    getDerivedList(dList, includeBase, includeDerived);
    return dList;
}

// =============================================================================
// (public)
void NodeDef::getDerivedList(std::vector<const NodeDef *> &dList, bool includeBase, bool includeDerived) const
{
    if (includeBase && hasDerivedBase()) {
        m_derivedBase.lock()->getDerivedList(dList, true, false);
    }

    dList.push_back(this);

    if (includeDerived && hasDerivedDiviations()) {
        for (const auto &dNodeDef: m_derivedDirectList)
        {
            dNodeDef->getDerivedList(dList, false, true);
        }
    }
}

// =============================================================================
// (public)
bool NodeDef::validate(const UnionRef &derivedId, bool includeBase, bool includeDerived) const
{
    if (m_derivedId.isEqual(derivedId)) { return true; }

    if (includeBase && hasDerivedBase()) {
        if (m_derivedBase.lock()->validate(derivedId, true, false)) {
            return true;
        }
    }

    if (includeDerived && hasDerivedDiviations()) {
        for (const auto &dNodeDef: m_derivedDirectList)
        {
            if (dNodeDef->validate(derivedId, false, true)) {
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
// 4. The part id of the derived type match
bool NodeDef::validate(Node _node, bool includeBase, bool includeDerived) const
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

    if (!hasDiviations()) {
        return true;
    }

    // Check if the part id of the derived type matches
    UnionValue derivedId = derivedIdValueDef().value(_node);

    return validate(derivedId, includeBase, includeDerived);
}

// =============================================================================
// (public)
ValidationState NodeDef::validationState(const UnionRef &_derivedId) const
{
    if (validate(_derivedId, false, false)) {
        return VALIDATION_STATE_VALID;
    }

    if (hasDerivedBase() && validate(_derivedId, true, false)) {
        return VALIDATION_STATE_VALID_SIBLING_BASE;
    }

    if (hasDerivedDiviations() && validate(_derivedId, false, true)) {
        return VALIDATION_STATE_VALID_SIBLING_DERIVED;
    }

    if (hasDerivedBase() && derivedRoot()->validate(_derivedId, false, true)) {
        return VALIDATION_STATE_VALID_SIBLING_OTHER;
    }
    return VALIDATION_STATE_INVALID;
}

// =============================================================================
// (public)
ValidationState NodeDef::validationState(Node node) const
{
    if (!derivedRoot()->validate(node, false, true)) {
        return VALIDATION_STATE_INVALID;
    }

    UnionValue _derivedId = derivedIdValueDef().value(node);

    if (!hasDerivedDiviations() || validate(_derivedId, false, false)) {
        return VALIDATION_STATE_VALID;
    }

    if (hasDerivedBase() && validate(_derivedId, true, false)) {
        return VALIDATION_STATE_VALID_SIBLING_BASE;
    }

    if (hasDerivedDiviations() && validate(_derivedId, false, true)) {
        return VALIDATION_STATE_VALID_SIBLING_DERIVED;
    }

    return VALIDATION_STATE_VALID_SIBLING_OTHER;
}

// =============================================================================
// (public)
int NodeDef::valueCount(bool includeBase, bool includeDerived) const
{
    int count = static_cast<int>(m_valueList.size());

    if (includeBase && hasDerivedBase()) {
        count += m_derivedBase.lock()->valueCount(true, false);
    }

    if (includeDerived && hasDerivedDiviations()) {
        for (const auto &dNodeDef: m_derivedDirectList)
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

    if (includeBase && hasDerivedBase()) {
        if (m_derivedBase.lock()->hasValue(valueName, true, false)) {
            return true;
        }
    }

    if (includeDerived && hasDerivedDiviations()) {
        for (const auto &dNodeDef: m_derivedDirectList)
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
            if (includeBase && hasDerivedBase()) {
                i += m_derivedBase.lock()->valueCount(true, false);
            }
            return i;
        }
        i++;
    }

    if (includeBase && hasDerivedBase()) {
        i = m_derivedBase.lock()->valueIndex(valueDef, true, false);
        if (i != -1) {
            return i;
        }
    }

    if (includeDerived && hasDerivedDiviations()) {
        i = valueCount(includeBase, false);
        for (const auto &dNodeDef: m_derivedDirectList)
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
    if (includeBase && hasDerivedBase()) {
        int baseDefCount = m_derivedBase.lock()->valueCount(true, false);
        if (index < baseDefCount) {
            return m_derivedBase.lock()->value(index, true, false);
        }
        index -= baseDefCount;
    }

    int vCount = static_cast<int>(m_valueList.size());
    if (index < vCount) {
        return *m_valueList[static_cast<vSize>(index)].get();
    }
    index -= vCount;

    if (includeDerived && hasDerivedDiviations()) {
        for (const auto &dNodeDef: m_derivedDirectList)
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

    if (includeBase && hasDerivedBase()) {
        const ValueDef & result = m_derivedBase.lock()->value(valueName, true, false);
        if (!result.isNull()) {
            return result;
        }
    }

    if (includeDerived && hasDerivedDiviations()) {
        for (const auto &dNodeDef: m_derivedDirectList)
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
    if (includeBase && hasDerivedBase()) {
        int baseDefCount = m_derivedBase.lock()->valueCount(true, false);
        if (index < baseDefCount) {
            return m_derivedBase.lock()->value(index, true, false);
        }
        index -= baseDefCount;
    }

    int vCount = static_cast<int>(m_valueList.size());
    if (index < vCount) {
        return *m_valueList[static_cast<vSize>(index)].get();
    }
    index -= vCount;

    if (includeDerived && hasDerivedDiviations()) {
        for (const auto &dNodeDef: m_derivedDirectList)
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

    if (includeBase && hasDerivedBase()) {
        ValueDef & result = m_derivedBase.lock()->value(valueName, true, false);
        if (!result.isNull()) {
            return result;
        }
    }

    if (includeDerived && hasDerivedDiviations()) {
        for (const auto &dNodeDef: m_derivedDirectList)
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
    if (includeBase && hasDerivedBase()) {
        m_derivedBase.lock()->getValueList(vList, true, false);
    }

    for (const auto& v: m_valueList)
    {
        vList.push_back(v.get());
    }

    if (includeDerived && hasDerivedDiviations()) {
        for (const auto &dNodeDef: m_derivedDirectList)
        {
            dNodeDef->getValueList(vList, false, true);
        }
    }
}

// =============================================================================
// (public)
bool NodeDef::hasKey() const
{
    return m_keyValueDefIndex >= 0;
}

// =============================================================================
// (public)
const ValueDef& NodeDef::keyValueDef() const
{
    return value(m_keyValueDefIndex);
}

// =============================================================================
// (public)
ValueDef& NodeDef::keyValueDef()
{
    return value(m_keyValueDefIndex);
}

// =============================================================================
// (public)
bool NodeDef::hasDerivedId() const
{
    return m_derivedIdValueDefIndex >= 0;
}

// =============================================================================
// (public)
const ValueDef& NodeDef::derivedIdValueDef() const
{
    return value(m_derivedIdValueDefIndex);
}

// =============================================================================
// (public)
ValueDef& NodeDef::derivedIdValueDef()
{
    return value(m_derivedIdValueDefIndex);
}

// =============================================================================
// (public)
int NodeDef::containerCount(bool includeBase, bool includeDerived) const
{
    int count = static_cast<int>(m_containerList.size());

    if (includeBase && hasDerivedBase()) {
        count += m_derivedBase.lock()->containerCount();
    }
    if (includeDerived && hasDerivedDiviations()) {
        for (const auto &dNodeDef: m_derivedDirectList)
        {
            count += dNodeDef->containerCount(false, true);
        }
    }

    return count;
}

// =============================================================================
// (public)
const ContainerDef &NodeDef::container(int index, bool includeBase, bool includeDerived) const
{
    if (includeBase && hasDerivedBase()) {
        int baseDefCount = m_derivedBase.lock()->containerCount(true, false);
        if (index < baseDefCount) {
            return m_derivedBase.lock()->container(index, true, false);
        }
        index -= baseDefCount;
    }

    int pCount = static_cast<int>(m_parentContainerDefs.size());
    if (index < pCount) {
        return *m_containerList[static_cast<size_t>(index)].get();
    }
    index -= pCount;

    if (includeDerived && hasDerivedDiviations()) {
        for (const auto &dNodeDef: m_derivedDirectList)
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

    if (includeBase && hasDerivedBase()) {
        const ContainerDef & childContainer = m_derivedBase.lock()->container(_name);
        if (!childContainer.isNull()) { return childContainer; }
    }

    if (includeDerived && hasDerivedDiviations()) {
        // Check if a derived NodeDefs has the container
        for (const auto &dNodeDef: m_derivedDirectList)
        {
            const ContainerDef & result = dNodeDef->container(_name, false, true);
            if (!result.isNull()) { return result; }
        }
    }

    return ContainerDef::emptyChildNodeDef();
}

// =============================================================================
// (public)
const ContainerDef &NodeDef::container(Node childNode, bool includeBase, bool includeDerived) const
{
    if (childNode.isNull()) { return ContainerDef::emptyChildNodeDef(); }

    for (const auto& _child: m_containerList)
    {
        if (_child->validate(childNode)) {
            return *_child.get();
        }
    }

    if (includeBase && hasDerivedBase()) {
        return m_derivedBase.lock()->container(childNode);
    }

    if (includeDerived && hasDerivedDiviations()) {
        // Check if a derived NodeDefs has the container
        for (const auto &dNodeDef: m_derivedDirectList)
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
    if (includeBase && hasDerivedBase()) {
        m_derivedBase.lock()->getContainerList(list);
    }
    for (const auto& _child: m_containerList) {
        list.push_back(_child.get());
    }

    if (includeDerived && hasDerivedDiviations()) {
        // get contains on derived NodeDefs
        for (const auto &dNodeDef: m_derivedDirectList)
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
const NodeDef* NodeDef::childDef(Node childNode, bool includeBase, bool includeDerived) const
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
    if (includeBase && hasDerivedBase()) {
        m_derivedBase.lock()->getChildDefList(cList, true);
    }
    for (const auto& _child: m_containerList) {
        cList.push_back(_child.get()->containerDef());
    }

    if (includeDerived && hasDerivedDiviations()) {
        // get child NodeDefs on derived NodeDefs
        for (const auto &dNodeDef: m_derivedDirectList)
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
Node NodeDef::parentNode(Node node, const NodeDef** parentNodeDef, bool includeBase, bool includeDerived) const
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

    if (includeBase && hasDerivedBase()) {
        Node result = m_derivedBase.lock()->parentNode(node, parentNodeDef, true, false);
        if (!result.isNull()) {
            return result;
        }
    }

    if (includeDerived && hasDerivedDiviations()) {
        for (const auto &dNodeDef: m_derivedDirectList)
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

    if (includeBase && hasDerivedBase()) { count += m_derivedBase.lock()->parentContainerCount(true, false); }

    if (includeDerived && hasDerivedDiviations()) {
        for (const auto &dNodeDef: m_derivedDirectList)
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
    if (includeBase && hasDerivedBase()) {
        int baseDefCount = m_derivedBase.lock()->parentContainerCount(true, false);
        if (index < baseDefCount) {
            return m_derivedBase.lock()->parentContainer(index, true, false);
        }
        index -= baseDefCount;
    }

    int pCount = static_cast<int>(m_parentContainerDefs.size());
    if (index < pCount) {
        return m_parentContainerDefs[static_cast<size_t>(index)];
    }
    index -= pCount;

    if (includeDerived && hasDerivedDiviations()) {
        for (const auto &dNodeDef: m_derivedDirectList)
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

    if (includeBase && hasDerivedBase()) {
        return derivedBase()->parentContainer(_name, true, false);
    }

    if (includeDerived && hasDerivedDiviations()) {
        for (const auto &dNodeDef: m_derivedDirectList)
        {
            const ContainerDef * result = dNodeDef->parentContainer(_name, false, true);
            if (result) { return result; }
        }
    }

    return nullptr;
}

// =============================================================================
// (public)
const ContainerDef *NodeDef::parentContainer(Node parentNode, bool includeBase, bool includeDerived) const
{
    if (parentNode.isNull()) { return nullptr; }
    for (const ContainerDef* parentContainer: m_parentContainerDefs)
    {
        if (parentContainer->hostDef()->validate(parentNode, false, true)) {
            return parentContainer;
        }
    }

    if (includeBase && hasDerivedBase()) {
        return derivedBase()->parentContainer(parentNode);
    }

    if (includeDerived && hasDerivedDiviations()) {
        for (const auto &dNodeDef: m_derivedDirectList)
        {
            const ContainerDef * result = dNodeDef->parentContainer(parentNode, false, true);
            if (result) { return result; }
        }
    }

    return nullptr;
}

// =============================================================================
// (public)
bool NodeDef::isParent(Node node, Node refNode, bool recursive) const
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
void NodeDef::onNodeInserted(Node _node) const
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
void NodeDef::onNodeMoved(Node _node) const
{
    UNUSED(_node);
}

// =============================================================================
// (public)
void NodeDef::onNodeCloned(Node _node) const
{
    UNUSED(_node);
}

} // namespace Model
} // namespace Oak
