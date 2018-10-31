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
NodeDefSPtr NodeDef::sPtr() const
{
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
void NodeDef::setWPtr(NodeDefSPtr sPtr)
{
    ASSERT(sPtr.get() == this);
    m_thisWPtr = sPtr;
}

// =============================================================================
// (public)
const std::string& NodeDef::tagName() const
{
    return m_tagName;
}

// =============================================================================
// (public)
const UnionRef NodeDef::derivedId() const
{
    return m_derivedId;
}

// =============================================================================
// (public)
void NodeDef::derivedIdListAll(std::vector<UnionRef> &idList) const
{
    derivedRoot()->derivedIdListFromThisAndDerived(idList);
}

// =============================================================================
// (public)
void NodeDef::derivedIdListFromDerived(std::vector<UnionRef> &idList) const
{
    for (const NodeDefSPtr &derivedNode: m_derivedDirectList) {
        derivedNode->derivedIdListFromThisAndDerived(idList);
    }
}

// =============================================================================
// (public)
void NodeDef::derivedIdListFromThisAndDerived(std::vector<UnionRef> &idList) const
{
    idList.push_back(m_derivedId);
    derivedIdListFromDerived(idList);
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
const NodeDef* NodeDef::getDerivedAny(const UnionRef &derivedId) const
{
    return derivedRoot()->getDerivedOrThis(derivedId);
}

// =============================================================================
// (public)
const NodeDef* NodeDef::getDerivedAny(Node node) const
{
    return derivedRoot()->getDerivedOrThis(node);
}

// =============================================================================
// (public)
const NodeDef* NodeDef::getDerived(const UnionRef &derivedId, const NodeDef* excluding) const
{
    for (const NodeDefSPtr &dDef: m_derivedDirectList) {
        const NodeDef* dDef2 = dDef->getDerivedOrThis(derivedId, excluding);
        if (dDef2) { return dDef2; }
    }
    return nullptr;
}

// =============================================================================
// (public)
const NodeDef* NodeDef::getDerived(Node node, const NodeDef* excluding) const
{
    for (const NodeDefSPtr &dDef: m_derivedDirectList) {
        const NodeDef* dDef2 = dDef->getDerivedOrThis(node, excluding);
        if (dDef2) { return dDef2; }
    }
    return nullptr;
}

// =============================================================================
// (public)
const NodeDef* NodeDef::getDerivedOrThis(const UnionRef &derivedId, const NodeDef* excluding) const
{
    if (excluding == this) { return nullptr; }
    if (validateForThis(derivedId)) { return this; }
    return getDerived(derivedId, excluding);
}

// =============================================================================
// (public)
const NodeDef* NodeDef::getDerivedOrThis(Node node, const NodeDef* excluding) const
{
    if (excluding == this) { return nullptr; }
    if (validateForThis(node)) { return this; }
    return getDerived(node, excluding);
}

// =============================================================================
// (public)
void NodeDef::getDerivedList(std::vector<const NodeDef *> &dList, bool recursive) const
{
    for (const NodeDefSPtr &dDef: m_derivedDirectList) {
        dList.push_back(dDef.get());
        if (recursive) {
            dDef->getDerivedList(dList, true);
        }
    }
}

// =============================================================================
// (public)
bool NodeDef::validateForThis(const UnionRef &derivedId) const
{
    return m_derivedId.isEqual(derivedId);
}

// =============================================================================
// (public)
// The validation of a data node return true if:
// 1. The element is valid
// 2. The tag name match
// 3. One of the parent tag names match
// 4. The part id of the derived type match
bool NodeDef::validateForThis(Node _node) const
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
        if (parentNode(_node).isNull()) {
            return false;
        }
    }

    if (hasDiviations()) {
        // Check if the part id of the derived type matches
        return derivedIdValueDef().compareValue(_node, derivedId()) == 0;
    }
    return true;
}

// =============================================================================
// (public)
bool NodeDef::validateForDerived(const UnionRef &derivedId, const NodeDef* excluding) const
{
    for (const NodeDefSPtr &derivedDef: m_derivedDirectList) {
        if (derivedDef->validateForThisOrDerived(derivedId, excluding)) {
            return true;
        }
    }
    return false;
}

// =============================================================================
// (public)
bool NodeDef::validateForDerived(Node node, const NodeDef* excluding) const
{
    for (const NodeDefSPtr &derivedDef: m_derivedDirectList) {
        if (derivedDef->validateForThisOrDerived(node, excluding)) {
            return true;
        }
    }
    return false;
}

// =============================================================================
// (public)
bool NodeDef::validateForThisOrDerived(const UnionRef &derivedId, const NodeDef* excluding) const
{
    if (excluding == this) { return false; }
    return validateForThis(derivedId) || validateForDerived(derivedId, excluding);
}

// =============================================================================
// (public)
bool NodeDef::validateForThisOrDerived(Node node, const NodeDef* excluding) const
{
    if (excluding == this) { return 0; }
    return validateForThis(node) || validateForDerived(node, excluding);
}

// =============================================================================
// (public)
bool NodeDef::validateForAny(const UnionRef &derivedId) const
{
    return derivedRoot()->validateForThisOrDerived(derivedId);
}

// =============================================================================
// (public)
bool NodeDef::validateForAny(Node node) const
{
    const NodeDef* rootInt = derivedRoot();
    return rootInt->validateForThis(node) || rootInt->validateForDerived(node);
}

// =============================================================================
// (public)
ValidationState NodeDef::validationState(const UnionRef &_derivedId) const
{
    if (m_derivedId == _derivedId) {
        return VALIDATION_STATE_VALID;
    }
    if (validateForDerived(_derivedId)) {
        return VALIDATION_STATE_VALID_SIBLING_DERIVED;
    }
    if (hasDerivedBase()) {
        // Base definitions are checked
        const NodeDef* def;
        const NodeDef* derivedBase = this;
        while (derivedBase->hasDerivedBase()) {
            def = derivedBase;
            derivedBase = derivedBase->m_derivedBase.lock().get();
            if (derivedBase->validateForThis(_derivedId)) {
                return VALIDATION_STATE_VALID_SIBLING_BASE;
            }
            if (derivedBase->validateForDerived(_derivedId, def)) {
                return VALIDATION_STATE_VALID_SIBLING_OTHER;
            }
        }
    }
    return VALIDATION_STATE_INVALID;
}

// =============================================================================
// (public)
ValidationState NodeDef::validationState(Node node) const
{
    if (validateForThis(node)) {
        return VALIDATION_STATE_VALID;
    }

    if (validateForDerived(node)) {
        return VALIDATION_STATE_VALID_SIBLING_DERIVED;
    }

    if (hasDerivedBase()) {
        const NodeDef* def;
        const NodeDef* derivedBase = this;
        while (derivedBase->hasDerivedBase()) {
            def = derivedBase;
            derivedBase = derivedBase->m_derivedBase.lock().get();
            if (derivedBase->validateForThis(node)) {
                return VALIDATION_STATE_VALID_SIBLING_BASE;
            }
            if (derivedBase->validateForDerived(node, def)) {
                return VALIDATION_STATE_VALID_SIBLING_OTHER;
            }
        }
    }

    return VALIDATION_STATE_INVALID;
}

// =============================================================================
// (public)
int NodeDef::valueCount() const
{
    if (hasDerivedBase()) {
        return m_derivedBase.lock()->valueCount() + static_cast<int>(m_valueList.size());
    }
    return static_cast<int>(m_valueList.size());
}

// =============================================================================
// (public)
bool NodeDef::hasValue(const std::string &valueName) const
{
    for (const auto &value: m_valueList) {
        if (value->name() == valueName) {
            return true;
        }
    }

    if (hasDerivedBase()) {
        return m_derivedBase.lock()->hasValue(valueName);
    }

    return false;
}

// =============================================================================
// (public)
int NodeDef::valueIndex(const ValueDef *valueDef) const
{
    int i = 0;
    for (const ValueDefUPtr &value: m_valueList)
    {
        if (value.get() == valueDef) { break; }
        i++;
    }

    if (i != static_cast<int>(m_valueList.size()))  {   // ValueDef found
        if (hasDerivedBase()) { // Derived base ValueDef's comes first
            i += derivedBase()->valueCount();
        }
        return i;
    } else {                                            // ValueDef NOT found
        if (hasDerivedBase()) {
            return derivedBase()->valueIndex(valueDef);
        } else {
            return -1;
        }
    }
}

// =============================================================================
// (public)
const ValueDef &NodeDef::value(int index) const
{
    if (hasDerivedBase()) {
        int baseDefCount = m_derivedBase.lock()->valueCount();
        if (index < baseDefCount) {
            return m_derivedBase.lock()->value(index);
        }
        index -= baseDefCount;
    }
    return *m_valueList[static_cast<vSize>(index)].get();
}

// =============================================================================
// (public)
const ValueDef &NodeDef::value(const std::string &valueName) const
{
    for (const auto &value: m_valueList) {
        if (value->name() == valueName) {
            return *value.get();
        }
    }

    if (hasDerivedBase()) {
        return m_derivedBase.lock()->value(valueName);
    }

    return ValueDef::emptyDef();
}

// =============================================================================
// (public)
ValueDef &NodeDef::value(int index)
{
    if (hasDerivedBase()) {
        int baseDefCount = m_derivedBase.lock()->valueCount();
        if (index < baseDefCount) {
            return m_derivedBase.lock()->value(index);
        }
        index -= baseDefCount;
    }
    return *m_valueList[static_cast<vSize>(index)].get();
}

// =============================================================================
// (public)
ValueDef &NodeDef::value(const std::string &valueName)
{
    for (const auto &value: m_valueList) {
        if (value->name() == valueName) {
            return *value.get();
        }
    }

    if (hasDerivedBase()) {
        return m_derivedBase.lock()->value(valueName);
    }

    return ValueDef::emptyDef();
}

// =============================================================================
// (public)
std::vector<const ValueDef *> NodeDef::valueList() const
{
    std::vector<const ValueDef *> vList;
    getValueList(vList);
    return vList;
}

// =============================================================================
// (public)
void NodeDef::getValueList(std::vector<const ValueDef*>& vList, bool includeDerived) const
{
    if (includeDerived && hasDerivedBase()) {
        m_derivedBase.lock()->getValueList(vList);
    }
    for (const auto& v: m_valueList)
    {
        vList.push_back(v.get());
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
int NodeDef::containerCount() const
{
    int count = static_cast<int>(m_containerList.size());
    if (hasDerivedBase()) { count += m_derivedBase.lock()->containerCount(); }
    return count;
}

// =============================================================================
// (public)
const ContainerDef &NodeDef::container(int index) const
{
    if (hasDerivedBase()) {
        int baseCount = m_derivedBase.lock()->containerCount();
        if (index < baseCount) {
            return m_derivedBase.lock()->container(index);
        }
        index -= baseCount;
    }

    if (index >= 0 || index < static_cast<int>(m_containerList.size())) {
        return *m_containerList[static_cast<vSize>(index)];
    }

    ASSERT(false);
    return ContainerDef::emptyChildNodeDef();
}

// =============================================================================
// (public)
const ContainerDef &NodeDef::container(const std::string& _name, bool includeDerived) const
{
    for (const auto& _child: m_containerList)
    {
        if (_child->containerDef()->name() == _name) {
            return *_child.get();
        }
    }

    if (hasDerivedBase()) {
        const ContainerDef & childContainer = m_derivedBase.lock()->container(_name, false);
        if (!childContainer.isNull()) { return childContainer; }
    }

    if (includeDerived) {
        // Check if a child container with _name exists on a derived NodeDef
        std::vector<const NodeDef*> dList;
        getDerivedList(dList, true);
        for (const NodeDef *dDef: dList)
        {
            for (const auto& _child: dDef->m_containerList)
            {
                if (_child->containerDef()->name() == _name) {
                    return *_child.get();
                }
            }
        }
    }

    ASSERT(false); // Not sure assert is right here? (it just didn't find any)
    return ContainerDef::emptyChildNodeDef();
}

// =============================================================================
// (public)
const ContainerDef &NodeDef::container(Node childNode) const
{
    if (childNode.isNull()) { return ContainerDef::emptyChildNodeDef(); }

    for (const auto& _child: m_containerList)
    {
        if (_child->validate(childNode)) {
            return *_child.get();
        }
    }

    if (hasDerivedBase()) {
        return m_derivedBase.lock()->container(childNode);
    }

    return ContainerDef::emptyChildNodeDef();
}

// =============================================================================
// (public)
std::vector<const ContainerDef *> NodeDef::containerList() const
{
    std::vector<const ContainerDef *> cList;
    getContainerList(cList);
    return cList;
}

// =============================================================================
// (public)
void NodeDef::getContainerList(std::vector<const ContainerDef *> &list) const
{
    if (hasDerivedBase()) {
        m_derivedBase.lock()->getContainerList(list);
    }
    for (const auto& _child: m_containerList) {
        list.push_back(_child.get());
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
const NodeDef* NodeDef::childDef(int index) const
{
    return container(index).containerDef();
}

// =============================================================================
// (public)
const NodeDef* NodeDef::childDef(const std::string &_name, bool includeDerived) const
{
    return container(_name, includeDerived).containerDef();
}

// =============================================================================
// (public)
const NodeDef* NodeDef::childDef(Node childNode) const
{
    for (const auto& child: m_containerList)
    {
        const NodeDef* match = child->containerDef(childNode);
        if (match) {
            return match;
        }
    }
    if (hasDerivedBase()) {
        return childDef(childNode);
    }
    return nullptr;
}

// =============================================================================
// (public)
void NodeDef::getChildDefList(std::vector<const NodeDef *> &cList, bool includeDerived) const
{
    if (includeDerived && hasDerivedBase()) {
        m_derivedBase.lock()->getChildDefList(cList, includeDerived);
    }
    for (const auto& _child: m_containerList) {
        cList.push_back(_child.get()->containerDef());
    }
}

// =============================================================================
// (public)
Node NodeDef::parentNode(Node node, const NodeDef** parentNodeDef) const
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

    if (hasDerivedBase()) {
        return m_derivedBase.lock()->parentNode(node, parentNodeDef);
    }

    // None of the parent child definitions could locate a valid parent
    if (parentNodeDef) { *parentNodeDef = nullptr; }
    return Node();
}

// =============================================================================
// (public)
int NodeDef::parentContainerCount() const
{
    int count = static_cast<int>(m_parentContainerDefs.size());
    if (hasDerivedBase()) { count += derivedBase()->parentContainerCount(); }
    return count;
}

// =============================================================================
// (public)
const ContainerDef *NodeDef::parentContainer(int index) const
{
    if (hasDerivedBase()) {
        int baseCount = derivedBase()->parentContainerCount();
        if (index < baseCount) {
            return derivedBase()->parentContainer(index);
        }
        index -= baseCount;
    }
    return m_parentContainerDefs[static_cast<vSize>(index)];
}

// =============================================================================
// (public)
const ContainerDef *NodeDef::parentContainer(const std::string &_name) const
{
    for (const ContainerDef* parentContainer: m_parentContainerDefs)
    {
        if (parentContainer->hostDef()->name() == _name) {
            return parentContainer;
        }
    }

    if (hasDerivedBase()) {
        return derivedBase()->parentContainer(_name);
    }

    return nullptr;
}

// =============================================================================
// (public)
const ContainerDef *NodeDef::parentContainer(Node parentNode) const
{
    if (parentNode.isNull()) { return nullptr; }
    for (const ContainerDef* parentContainer: m_parentContainerDefs)
    {
        if (parentContainer->hostDef()->validateForThisOrDerived(parentNode)) {
            return parentContainer;
        }
    }

    if (hasDerivedBase()) {
        return derivedBase()->parentContainer(parentNode);
    }

    return nullptr;
}

#ifdef XML_BACKEND
// =============================================================================
// (public)
const ContainerDef *NodeDef::parentContainerTagName(const std::string &tagName) const
{
    for (const ContainerDef* parent: m_parentContainerDefs) {
        if (parent->hostDef()->tagName().compare(tagName) == 0) {
            return parent;
        }
    }
    if (hasDerivedBase()) {
        return derivedBase()->parentContainer(tagName);
    }
    return nullptr;
}
#endif // XML_BACKEND

// =============================================================================
// (public)
bool NodeDef::hasParent(const NodeDef* nodeDef) const
{
    if (!nodeDef) { return false; }
    for (const ContainerDef* parentContainer: m_parentContainerDefs)
    {
        if (parentContainer->hostDef()->derivedRoot() == nodeDef->derivedRoot()) {
            return true;
        }
    }

    if (hasDerivedBase()) {
        return hasParent(nodeDef);
    }

    return false;
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
