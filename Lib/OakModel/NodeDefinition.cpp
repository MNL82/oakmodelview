/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "NodeDefinition.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
// This constructor is used when the NodeDefinition has no base definition
// The derivedId is used to identify the correct NodeDefinition in a inheritance hierarchy
// The NodeDefinition can not be part of a inheritance hierarchy if the derivedId is null.
// The NodeDefinition has a derivedId but no base definition, so it is the root NodeDefinition of a inheritance hierarchy
NodeDefinition::NodeDefinition(VariantCRef _name)
{
    assert(!_name.isNull());

    m_name = _name;
}

// =============================================================================
// (public)
NodeDefinition::NodeDefinition(VariantCRef _name, VariantCRef _derivedId)
{
    assert(!_name.isNull());
    assert(!_derivedId.isNull());

    m_name = _name;
    m_derivedId = _derivedId;
}

// =============================================================================
// (public)
NodeDefinition::~NodeDefinition()
{

}

// =============================================================================
// (public)
NodeDefinition::NodeDefinition(const NodeDefinition &copy)
{
    *this = copy;
}

// =============================================================================
// (public)
NodeDefinition::NodeDefinition(NodeDefinition &&move)
{
    *this = std::move(move);
}

// =============================================================================
// (public)
NodeDefinition &NodeDefinition::operator=(const NodeDefinition &copy)
{
    // Copy of self is not allowed
    assert(this != &copy);

    m_name = copy.m_name;
    m_tagName = copy.m_tagName;
    m_derivedId = copy.m_derivedId;
    m_displayName = copy.m_displayName;

    m_derivedBase = copy.m_derivedBase;
    m_derivedDirectList.clear();
    for (auto dDefinition: copy.m_derivedDirectList)
    {
        m_derivedDirectList.push_back(dDefinition);
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

    m_parentContainerDefinitions.clear();
    for (const ContainerDefinition* container: copy.m_parentContainerDefinitions) {
        m_parentContainerDefinitions.push_back(container);
    }
    return *this;
}

// =============================================================================
// (public)
NodeDefinition &NodeDefinition::operator=(NodeDefinition &&move)
{
    m_name = std::move(move.m_name);
    m_displayName = std::move(move.m_displayName);
    m_tagName = std::move(move.m_tagName);
    m_derivedId = std::move(move.m_derivedId);

    m_derivedBase = move.m_derivedBase;
    m_derivedDirectList = std::move(move.m_derivedDirectList);

    m_valueList = std::move(move.m_valueList);
    m_keyValueDefIndex = move.m_keyValueDefIndex;
    m_derivedIdValueDefIndex = move.m_derivedIdValueDefIndex;

    m_containerList = std::move(move.m_containerList);
    m_containerGroup = std::move(move.m_containerGroup);

    m_parentContainerDefinitions = std::move(move.m_parentContainerDefinitions);
    return *this;
}

// =============================================================================
// (public)
bool NodeDefinition::isNull() const
{
    return m_name.isNull();
}

// =============================================================================
// (public)
VariantCRef NodeDefinition::name() const
{
    return m_name;
}

// =============================================================================
// (public)
std::string NodeDefinition::displayName() const
{
    if (!m_displayName.empty()) { return m_displayName; }

    if (m_name.isNull()) { return std::string(); }

    std::string nodeStr;
    if (!m_name.get(nodeStr)) { return std::string(); }

    if (m_derivedId.isNull()) { return nodeStr; }

    std::string inheritanceStr;
    if (!m_derivedId.get(inheritanceStr)) { return nodeStr; }

    return nodeStr + "(" + inheritanceStr + ")";
}

// =============================================================================
// (public)
const NodeSettings &NodeDefinition::settings() const
{
    return m_settings;
}

// =============================================================================
// (public)
NodeDefinitionSPtr NodeDefinition::sPtr() const
{
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
void NodeDefinition::setWPtr(NodeDefinitionSPtr sPtr)
{
    assert(sPtr.get() == this);
    m_thisWPtr = sPtr;
}

// =============================================================================
// (public)
const std::string& NodeDefinition::tagName() const
{
    return m_tagName;
}

// =============================================================================
// (public)
VariantCRef NodeDefinition::derivedId() const
{
    return m_derivedId;
}

// =============================================================================
// (public)
void NodeDefinition::derivedIdListAll(std::vector<VariantCRef>& idList) const
{
    derivedRoot()->derivedIdListFromThisAndDerived(idList);
}

// =============================================================================
// (public)
void NodeDefinition::derivedIdListFromDerived(std::vector<VariantCRef>& idList) const
{
    for (NodeDefinitionSPtr derivedNode: m_derivedDirectList) {
        derivedNode->derivedIdListFromThisAndDerived(idList);
    }
}

// =============================================================================
// (public)
void NodeDefinition::derivedIdListFromThisAndDerived(std::vector<VariantCRef>& idList) const
{
    idList.push_back(m_derivedId);
    derivedIdListFromDerived(idList);
}

// =============================================================================
// (public)
const NodeDefinition* NodeDefinition::derivedRoot() const
{
    if (hasDerivedBase()) {
        derivedBase()->derivedRoot();
    }
    return this;
}

// =============================================================================
// (public)
const NodeDefinition* NodeDefinition::getDerivedAny(VariantCRef derivedId) const
{
    return derivedRoot()->getDerivedOrThis(derivedId);
}

// =============================================================================
// (public)
const NodeDefinition* NodeDefinition::getDerivedAny(Node node) const
{
    return derivedRoot()->getDerivedOrThis(node);
}

// =============================================================================
// (public)
const NodeDefinition* NodeDefinition::getDerived(VariantCRef derivedId, const NodeDefinition* excluding) const
{
    for (NodeDefinitionSPtr dDefinition: m_derivedDirectList) {
        const NodeDefinition* dDefinition2 = dDefinition->getDerivedOrThis(derivedId, excluding);
        if (dDefinition2) { return dDefinition2; }
    }
    return 0;
}

// =============================================================================
// (public)
const NodeDefinition* NodeDefinition::getDerived(Node node, const NodeDefinition* excluding) const
{
    for (NodeDefinitionSPtr dDefinition: m_derivedDirectList) {
        const NodeDefinition* dDefinition2 = dDefinition->getDerivedOrThis(node, excluding);
        if (dDefinition2) { return dDefinition2; }
    }
    return 0;
}

// =============================================================================
// (public)
const NodeDefinition* NodeDefinition::getDerivedOrThis(VariantCRef derivedId, const NodeDefinition* excluding) const
{
    if (excluding == this) { return 0; }
    if (validateForThis(derivedId)) { return this; }
    return getDerived(derivedId, excluding);
}

// =============================================================================
// (public)
const NodeDefinition* NodeDefinition::getDerivedOrThis(Node node, const NodeDefinition* excluding) const
{
    if (excluding == this) { return 0; }
    if (validateForThis(node)) { return this; }
    return getDerived(node, excluding);
}

// =============================================================================
// (public)
bool NodeDefinition::validateForThis(VariantCRef derivedId) const
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
bool NodeDefinition::validateForThis(Node _node) const
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
        assert(false);
        return false;
    }

    // Check the list of parent node names are empty,
    // otherwise any parent element goes
    if (!m_parentContainerDefinitions.empty()) {
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
bool NodeDefinition::validateForDerived(VariantCRef derivedId, const NodeDefinition* excluding) const
{
    for (NodeDefinitionSPtr derivedDefinition: m_derivedDirectList) {
        if (derivedDefinition->validateForThisOrDerived(derivedId, excluding)) {
            return true;
        }
    }
    return false;
}

// =============================================================================
// (public)
bool NodeDefinition::validateForDerived(Node node, const NodeDefinition* excluding) const
{
    for (NodeDefinitionSPtr derivedDefinition: m_derivedDirectList) {
        if (derivedDefinition->validateForThisOrDerived(node, excluding)) {
            return true;
        }
    }
    return false;
}

// =============================================================================
// (public)
bool NodeDefinition::validateForThisOrDerived(VariantCRef derivedId, const NodeDefinition* excluding) const
{
    if (excluding == this) { return false; }
    return validateForThis(derivedId) || validateForDerived(derivedId, excluding);
}

// =============================================================================
// (public)
bool NodeDefinition::validateForThisOrDerived(Node node, const NodeDefinition* excluding) const
{
    if (excluding == this) { return 0; }
    return validateForThis(node) || validateForDerived(node, excluding);
}

// =============================================================================
// (public)
bool NodeDefinition::validateForAny(VariantCRef derivedId) const
{
    return derivedRoot()->validateForThisOrDerived(derivedId);
}

// =============================================================================
// (public)
bool NodeDefinition::validateForAny(Node node) const
{
    const NodeDefinition* rootInt = derivedRoot();
    return rootInt->validateForThis(node) || rootInt->validateForDerived(node);
}

// =============================================================================
// (public)
ValidationState NodeDefinition::validationState(VariantCRef _derivedId) const
{
    if (m_derivedId == _derivedId) {
        return VALIDATION_STATE_VALID;
    }
    if (validateForDerived(_derivedId)) {
        return VALIDATION_STATE_VALID_SIBLING_DERIVED;
    }
    if (hasDerivedBase()) {
        // Base definitions are checked
        const NodeDefinition* definition;
        const NodeDefinition* derivedBase = this;
        while (derivedBase->hasDerivedBase()) {
            definition = derivedBase;
            derivedBase = derivedBase->m_derivedBase.lock().get();
            if (derivedBase->validateForThis(_derivedId)) {
                return VALIDATION_STATE_VALID_SIBLING_BASE;
            }
            if (derivedBase->validateForDerived(_derivedId, definition)) {
                return VALIDATION_STATE_VALID_SIBLING_OTHER;
            }
        }
    }
    return VALIDATION_STATE_INVALID;
}

// =============================================================================
// (public)
ValidationState NodeDefinition::validationState(Node node) const
{
    if (validateForThis(node)) {
        return VALIDATION_STATE_VALID;
    }

    if (validateForDerived(node)) {
        return VALIDATION_STATE_VALID_SIBLING_DERIVED;
    }

    if (hasDerivedBase()) {
        const NodeDefinition* definition;
        const NodeDefinition* derivedBase = this;
        while (derivedBase->hasDerivedBase()) {
            definition = derivedBase;
            derivedBase = derivedBase->m_derivedBase.lock().get();
            if (derivedBase->validateForThis(node)) {
                return VALIDATION_STATE_VALID_SIBLING_BASE;
            }
            if (derivedBase->validateForDerived(node, definition)) {
                return VALIDATION_STATE_VALID_SIBLING_OTHER;
            }
        }
    }

    return VALIDATION_STATE_INVALID;
}

// =============================================================================
// (public)
int NodeDefinition::valueCount() const
{
    if (hasDerivedBase()) {
        return m_derivedBase.lock()->valueCount() + (int)m_valueList.size();
    }
    return (int)m_valueList.size();
}

// =============================================================================
// (public)
const ValueDefinition &NodeDefinition::value(int index) const
{
    if (hasDerivedBase()) {
        int baseDefinitionCount = m_derivedBase.lock()->valueCount();
        if (index < baseDefinitionCount) {
            return m_derivedBase.lock()->value(index);
        }
        index -= baseDefinitionCount;
    }
    return *m_valueList[index].get();
}

// =============================================================================
// (public)
const ValueDefinition &NodeDefinition::value(VariantCRef valueId) const
{
    for (const auto &value: m_valueList) {
        if (value->valueId() == valueId) {
            return *value.get();
        }
    }

    if (hasDerivedBase()) {
        return m_derivedBase.lock()->value(valueId);
    }

    return ValueDefinition::emptyDefinition();
}

// =============================================================================
// (public)
ValueDefinition &NodeDefinition::value(int index)
{
    if (hasDerivedBase()) {
        int baseDefinitionCount = m_derivedBase.lock()->valueCount();
        if (index < baseDefinitionCount) {
            return m_derivedBase.lock()->value(index);
        }
        index -= baseDefinitionCount;
    }
    return *m_valueList[index].get();
}

// =============================================================================
// (public)
ValueDefinition &NodeDefinition::value(VariantCRef valueId)
{
    for (const auto &value: m_valueList) {
        if (value->valueId() == valueId) {
            return *value.get();
        }
    }

    if (hasDerivedBase()) {
        return m_derivedBase.lock()->value(valueId);
    }

    return ValueDefinition::emptyDefinition();
}

// =============================================================================
// (public)
void NodeDefinition::getValueList(std::vector<const ValueDefinition*>& vList) const
{
    if (hasDerivedBase()) {
        m_derivedBase.lock()->getValueList(vList);
    }
    for (const auto& v: m_valueList)
    {
        vList.push_back(v.get());
    }
}

// =============================================================================
// (public)
bool NodeDefinition::hasKey() const
{
    return m_keyValueDefIndex >= 0;
}

// =============================================================================
// (public)
const ValueDefinition& NodeDefinition::keyValueDef() const
{
    return value(m_keyValueDefIndex);
}

// =============================================================================
// (public)
ValueDefinition& NodeDefinition::keyValueDef()
{
    return value(m_keyValueDefIndex);
}

// =============================================================================
// (public)
bool NodeDefinition::hasDerivedId() const
{
    return m_derivedIdValueDefIndex >= 0;
}

// =============================================================================
// (public)
const ValueDefinition& NodeDefinition::derivedIdValueDef() const
{
    return value(m_derivedIdValueDefIndex);
}

// =============================================================================
// (public)
ValueDefinition& NodeDefinition::derivedIdValueDef()
{
    return value(m_derivedIdValueDefIndex);
}

// =============================================================================
// (public)
int NodeDefinition::containerCount() const
{
    int count = (int)m_containerList.size();
    if (hasDerivedBase()) { count += m_derivedBase.lock()->containerCount(); }
    return count;
}

// =============================================================================
// (public)
const ContainerDefinition &NodeDefinition::container(int index) const
{
    if (hasDerivedBase()) {
        int baseCount = m_derivedBase.lock()->containerCount();
        if (index < baseCount) {
            return m_derivedBase.lock()->container(index);
        }
        index -= baseCount;
    }

    if (index >= 0 || index < (int)m_containerList.size()) {
        return *m_containerList.at(index);
    }

    assert(false);
    return ContainerDefinition::emptyChildNodeDefinition();
}

// =============================================================================
// (public)
const ContainerDefinition &NodeDefinition::container(Node childNode) const
{
    if (childNode.isNull()) { return ContainerDefinition::emptyChildNodeDefinition(); }

    for (const auto& _child: m_containerList)
    {
        if (_child->validate(childNode)) {
            return *_child.get();
        }
    }

    if (hasDerivedBase()) {
        return m_derivedBase.lock()->container(childNode);
    }

    return ContainerDefinition::emptyChildNodeDefinition();
}

// =============================================================================
// (public)
void NodeDefinition::getContainerList(std::vector<const ContainerDefinition *> &list) const
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
const ContainerGroupDefinition& NodeDefinition::containerGroup() const
{
    if (!m_containerGroup) {
        m_containerGroup = std::make_unique<ContainerGroupDefinition>(this);
    }
    return *m_containerGroup.get();
}

// =============================================================================
// (public)
const NodeDefinition* NodeDefinition::childDefinition(int index) const
{
    return container(index).containerDefinition();
}

// =============================================================================
// (public)
const NodeDefinition* NodeDefinition::childDefinition(VariantCRef _name) const
{
    return container(_name).containerDefinition();
}

// =============================================================================
// (public)
const NodeDefinition* NodeDefinition::childDefinition(Node childNode) const
{
    for (const auto& child: m_containerList)
    {
        const NodeDefinition* match = child->containerDefinition(childNode);
        if (match) {
            return match;
        }
    }
    if (hasDerivedBase()) {
        return childDefinition(childNode);
    }
    return NULL;
}

// =============================================================================
// (public)
Node NodeDefinition::parentNode(Node node, const NodeDefinition** parentNodeDefinition) const
{
    if (node.isNull()) { return Node(); }

    for (auto parentContainer: m_parentContainerDefinitions)
    {
        Node parentNode = parentContainer->hostNode(node);
        if (!parentNode.isNull()) {
            if (parentNodeDefinition) {
                *parentNodeDefinition = parentContainer->hostDefinition();
            }
            return parentNode;
        }
    }

    if (hasDerivedBase()) {
        return m_derivedBase.lock()->parentNode(node, parentNodeDefinition);
    }

    // None of the parent child definitions could locate a valid parent
    if (parentNodeDefinition) { *parentNodeDefinition = nullptr; }
    return Node();
}

// =============================================================================
// (public)
int NodeDefinition::parentContainerCount() const
{
    int count = (int)m_parentContainerDefinitions.size();
    if (hasDerivedBase()) { count += derivedBase()->parentContainerCount(); }
    return count;
}

// =============================================================================
// (public)
const ContainerDefinition *NodeDefinition::parentContainer(int index) const
{
    if (hasDerivedBase()) {
        int baseCount = derivedBase()->parentContainerCount();
        if (index < baseCount) {
            return derivedBase()->parentContainer(index);
        }
        index -= baseCount;
    }
    return m_parentContainerDefinitions.at(index);
}

// =============================================================================
// (public)
const ContainerDefinition *NodeDefinition::parentContainer(VariantCRef _name) const
{
    for (const ContainerDefinition* parentContainer: m_parentContainerDefinitions)
    {
        if (parentContainer->hostDefinition()->name() == _name) {
            return parentContainer;
        }
    }

    if (hasDerivedBase()) {
        return derivedBase()->parentContainer(_name);
    }

    return NULL;
}

// =============================================================================
// (public)
const ContainerDefinition *NodeDefinition::parentContainer(Node parentNode) const
{
    if (parentNode.isNull()) { return nullptr; }
    for (const ContainerDefinition* parentContainer: m_parentContainerDefinitions)
    {
        if (parentContainer->hostDefinition()->validateForThisOrDerived(parentNode)) {
            return parentContainer;
        }
    }

    if (hasDerivedBase()) {
        return derivedBase()->parentContainer(parentNode);
    }

    return NULL;
}

#ifdef XML_BACKEND
// =============================================================================
// (public)
const ContainerDefinition *NodeDefinition::parentContainer(const std::string &tagName) const
{
    for (const ContainerDefinition* parent: m_parentContainerDefinitions) {
        if (parent->hostDefinition()->tagName().compare(tagName) == 0) {
            return parent;
        }
    }
    if (hasDerivedBase()) {
        return derivedBase()->parentContainer(tagName);
    }
    return NULL;
}
#endif // XML_BACKEND

// =============================================================================
// (public)
bool NodeDefinition::hasParent(const NodeDefinition* nodeDefinition) const
{
    if (!nodeDefinition) { return false; }
    for (const ContainerDefinition* parentContainer: m_parentContainerDefinitions)
    {
        if (parentContainer->hostDefinition()->derivedRoot() == nodeDefinition->derivedRoot()) {
            return true;
        }
    }

    if (hasDerivedBase()) {
        return hasParent(nodeDefinition);
    }

    return false;
}

// =============================================================================
// (public)
bool NodeDefinition::isParent(Node node, Node refNode, bool recursive) const
{
    // Find the parent data node
    Node _parentNode = parentNode(node);
    if (_parentNode.isNull()) { return false; }

    // Find the definition of the parent data node
    const ContainerDefinition* _parentContainer = parentContainer(_parentNode);
    if (_parentContainer == NULL) { return false; }

    if (_parentNode == refNode) {
        return true;
    }

    // Test the parent of the parent if recursive is true
    if (recursive) {
        _parentContainer->hostDefinition()->isParent(node, refNode, recursive);
    }
    return false;
}

// =============================================================================
// (public)
void NodeDefinition::onNodeCreated(Node _node) const
{
    std::vector<const ContainerDefinition*> cList;
    this->getContainerList(cList);
    for (const ContainerDefinition* cDefinition: cList)
    {
        int count = cDefinition->nodeCount(_node);
        while (count < cDefinition->minCount()) {
            cDefinition->insertNode(_node, count);
            count++;
        }
    }
}

// =============================================================================
// (public)
void NodeDefinition::onNodeInserted(Node _node) const
{
    UNUSED(_node);
}

} // namespace Model
} // namespace Oak
