/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ContainerDefinition.h"

#include <assert.h>

#include "NodeDefinition.h"
#include "Item.h"
#include "ValueOptions.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
ContainerDefinition::ContainerDefinition()
    : m_minCount(-1)
    , m_maxCount(-1)
#ifdef XML_BACKEND
    , m_elementListRef("")
#endif // XML_BACKEND
{
}

// =============================================================================
// (public)
ContainerDefinition::ContainerDefinition(NodeDefinitionSPtr containerDefinition, int min, int max)
    : m_containerDefinition(containerDefinition)
    , m_minCount(min)
    , m_maxCount(max)
#ifdef XML_BACKEND
    , m_elementListRef(containerDefinition->tagName())
#endif // XML_BACKEND
{
    assert(m_containerDefinition);
    assert(m_minCount >= 0);
    assert(m_maxCount > 0);
    assert(m_maxCount >= m_minCount);
}

// =============================================================================
// (public)
ContainerDefinition::ContainerDefinition(const ContainerDefinition &copy)
    : m_elementListRef(copy.m_elementListRef)
{
    *this = copy;
}

// =============================================================================
// (public)
ContainerDefinition::ContainerDefinition(ContainerDefinition &&move)
{
    *this = std::move(move);
}

// =============================================================================
// (public)
ContainerDefinition& ContainerDefinition::operator=(const ContainerDefinition &copy)
{
    m_hostDefinition = copy.m_hostDefinition;
    m_containerDefinition = copy.m_containerDefinition;
    m_minCount = copy.m_minCount;
    m_maxCount = copy.m_maxCount;

#ifdef XML_BACKEND
    m_elementListRef = copy.m_elementListRef;
#endif // XML_BACKEND
    return *this;
}

// =============================================================================
// (public)
ContainerDefinition& ContainerDefinition::operator=(ContainerDefinition &&move)
{
    m_hostDefinition = move.m_hostDefinition;
    m_containerDefinition = move.m_containerDefinition;
    m_minCount = move.m_minCount;
    m_maxCount = move.m_maxCount;

#ifdef XML_BACKEND
    m_elementListRef = std::move(move.m_elementListRef);
#endif // XML_BACKEND
    return *this;
}

// =============================================================================
// (public)
ContainerDefinitionUPtr ContainerDefinition::copy() const
{
    return MakeUPtr(*this);
}

// =============================================================================
// (public)
bool ContainerDefinition::isNull() const
{
    return m_hostDefinition.expired();
}

// =============================================================================
// (public)
const NodeDefinition* ContainerDefinition::containerDefinition() const
{
    return m_containerDefinition.get();
}

// =============================================================================
// (public)
const NodeDefinition* ContainerDefinition::containerDefinition(const UnionRef &derivedId) const
{
    if (derivedId.isNull() || derivedId.isEqual(m_containerDefinition->derivedId())) {
        return m_containerDefinition.get();
    }
    return m_containerDefinition->getDerived(derivedId);
}

// =============================================================================
// (public)
const NodeDefinition* ContainerDefinition::containerDefinition(Node _node) const
{
    if (m_containerDefinition->validateForThis(_node)) {
        return m_containerDefinition.get();
    }
    return m_containerDefinition->getDerived(_node);
}

// =============================================================================
// (public)
bool ContainerDefinition::validate(Node _node) const
{
    if (_node.isNull()) { return false; }

    switch (_node.type()) {
#ifdef XML_BACKEND
    case Node::Type::XML: {
        if (!m_containerDefinition) { return false; }
        if (_node.xmlNode().compareTagName(containerDefinition()->tagName()) != 0) { return false; }
        return m_containerDefinition->validateForThisOrDerived(_node);
    }
#endif // XML_BACKEND
    default:
        // _node.type() returns an unhandled type that needs to be implemented
        assert(false);
    }
    return false;
}

// =============================================================================
// (public)
int ContainerDefinition::minCount() const
{
    assert(m_minCount >= 0);
    return m_minCount;
}

// =============================================================================
// (public)
int ContainerDefinition::maxCount() const
{
    assert(m_maxCount > 0);
    return m_maxCount;
}

// =============================================================================
// (public)
int ContainerDefinition::nodeCount(Node _node) const
{
    if (_node.isNull()) { return -1; }

    switch (_node.type()) {
#ifdef XML_BACKEND
    case Node::Type::XML:
        return m_elementListRef.count(_node.xmlNode());
#endif // XML_BACKEND
    default:
        // _node.type() returns an unhandled type that needs to be implemented
        assert(false);
    }
    return -1;
}

// =============================================================================
// (public)
int ContainerDefinition::nodeIndex(Node _node, Node refNode) const
{
    if (_node.isNull()) { return -1; }

    switch (_node.type()) {
#ifdef XML_BACKEND
    case Node::Type::XML:
        return m_elementListRef.indexOf(_node.xmlNode(), refNode.xmlNode());
#endif // XML_BACKEND
    default:
        // _node.type() returns an unhandled type that needs to be implemented
        assert(false);
    }
    return -1;
}

// =============================================================================
// (public)
Node ContainerDefinition::node(Node _node, int index, const NodeDefinition** nodeDefinition) const
{
    if (_node.isNull()) { return Node(); }

    Node childNode;
    switch (_node.type()) {
#ifdef XML_BACKEND
    case Node::Type::XML:
        childNode =  m_elementListRef.at(_node.xmlNode(), index);
        break;
#endif // XML_BACKEND
    default:
        // _node.type() returns an unhandled type that needs to be implemented
        assert(false);
    }

    if (nodeDefinition) {
        *nodeDefinition = (childNode.isNull()) ? 0 : containerDefinition(childNode);
    }
    return childNode;
}

// =============================================================================
// (public)
Node ContainerDefinition::firstNode(Node _node, const NodeDefinition** nodeDefinition) const
{
    if (_node.isNull()) { return Node(); }

    Node childNode;
    switch (_node.type()) {
#ifdef XML_BACKEND
    case Node::Type::XML:
        childNode = m_elementListRef.first(_node.xmlNode());
        break;
#endif // XML_BACKEND
    default:
        // _node.type() returns an unhandled type that needs to be implemented
        assert(false);
    }

   if (nodeDefinition) {
        *nodeDefinition = (childNode.isNull()) ? 0 : containerDefinition(childNode);
    }
    return childNode;
}

// =============================================================================
// (public)
Node ContainerDefinition::lastNode(Node _node, const NodeDefinition** nodeDefinition) const
{
    if (_node.isNull()) { return Node(); }

    Node childNode;
    switch (_node.type()) {
#ifdef XML_BACKEND
    case Node::Type::XML:
        childNode = m_elementListRef.last(_node.xmlNode());
        break;
#endif // XML_BACKEND
    default:
        // _node.type() returns an unhandled type that needs to be implemented
        assert(false);
    }

    if (nodeDefinition) {
        *nodeDefinition = (childNode.isNull()) ? 0 : containerDefinition(childNode);
    }
    return childNode;
}

// =============================================================================
// (public)
Node ContainerDefinition::nextNode(Node refNode, const NodeDefinition** nodeDefinition) const
{
    if (refNode.isNull()) { return Node(); }

    Node childNode;
    switch (refNode.type()) {
#ifdef XML_BACKEND
    case Node::Type::XML:
        childNode =  m_elementListRef.next(refNode.xmlNode());
        break;
#endif // XML_BACKEND
    default:
        // _node.type() returns an unhandled type that needs to be implemented
        assert(false);
    }

    if (nodeDefinition) {
        *nodeDefinition = (childNode.isNull()) ? 0 : containerDefinition(childNode);
    }
    return childNode;
}

// =============================================================================
// (public)
Node ContainerDefinition::previousNode(Node refNode, const NodeDefinition** nodeDefinition) const
{
    if (refNode.isNull()) { return Node(); }

    Node childNode;
    switch (refNode.type()) {
#ifdef XML_BACKEND
    case Node::Type::XML:
        childNode =  m_elementListRef.previous(refNode.xmlNode());
        break;
#endif // XML_BACKEND
    default:
        // _node.type() returns an unhandled type that needs to be implemented
        assert(false);
    }

    if (nodeDefinition) {
        *nodeDefinition = (childNode.isNull()) ? 0 : containerDefinition(childNode);
    }
    return childNode;
}

// =============================================================================
// (public)
const NodeDefinition* ContainerDefinition::hostDefinition() const
{
    return m_hostDefinition.lock().get();
}

// =============================================================================
// (public)
Node ContainerDefinition::hostNode(Node refdNode) const
{
    if (refdNode.isNull()) { return Node(); }

    switch (refdNode.type()) {
#ifdef XML_BACKEND
    case Node::Type::XML: {
        XML::Element hostElement = m_elementListRef.invertedAt(refdNode.xmlNode());

        if (hostElement.empty()) { return Node(); }

        if (hostDefinition()->tagName().compare(hostElement.tagName()) != 0) { return Node(); }
        return hostElement;
    }
#endif // XML_BACKEND
    default:
        // _node.type() returns an unhandled type that needs to be implemented
        assert(false);
    }
    return Node();
}

// =============================================================================
// (public)
bool ContainerDefinition::canInsertNode(Node _node, int &index) const
{
    if (isNull()) { return false; }

    // The data node and the child definition is validated in the function
    int count = nodeCount(_node);
    if (count < 0) { return false; }

    // Check if there are room for one more child node
    if (count >= maxCount()) { return false; }

    // Check if a unique options only list do not have any options left
    if (!checkUniqueOptionValues(_node)) { return false; }

    // if index is -1 then the child node is added to the end
    if (index == -1) {
        index = count;
        return true;
    }

    // Check if the index is out of bounds
    return index <= count;
}

// =============================================================================
// (public)
Node ContainerDefinition::insertNode(Node _node, int &index) const
{
    Node newNode;
    if (canInsertNode(_node, index)) {
        switch (_node.type()) {
#ifdef XML_BACKEND
        case Node::Type::XML:
            newNode = m_elementListRef.insert(_node.xmlNode(), index);
            break;
#endif // XML_BACKEND
        default:
            // _node.type() returns an unhandled type that needs to be implemented
            assert(false);
            return newNode;
        }
    }
    m_containerDefinition->onNodeInserted(newNode);
    return newNode;
}

// =============================================================================
// (public)
bool ContainerDefinition::canCloneNode(Node _node, int &index, Node cloneNode) const
{
    if (isNull()) { return false; }

    // Check that clone node is valid
    if (!m_containerDefinition->validateForAny(cloneNode)) { return false; }

    // Check if 'cloneNode' is parent(recursive) of 'node'
    // Can not clone it selv into it selv

    if (!m_hostDefinition.expired() && m_hostDefinition.lock()->isParent(_node, cloneNode)) { return false; }

    return canInsertNode(_node, index);
}

// =============================================================================
// (public)
Node ContainerDefinition::cloneNode(Node _node, int &index, Node cloneNode) const
{
    Node newNode;
    if (canCloneNode(_node, index, cloneNode)) {
        switch (_node.type()) {
#ifdef XML_BACKEND
        case Node::Type::XML:
            newNode = m_elementListRef.clone(_node.xmlNode(), index, cloneNode.xmlNode());
            break;
#endif // XML_BACKEND
        default:
            // _node.type() returns an unhandled type that needs to be implemented
            assert(false);
            return newNode;
        }
    }

    m_containerDefinition->onNodeCloned(newNode);
    return newNode;
}

// =============================================================================
// (public)
bool ContainerDefinition::canMoveNode(Node _node, int &index, Node moveNode) const
{
    if (isNull()) { return false; }
    if (index < -1) { return false; }

    // The data node and the child node definition is validated in the function
    int count = nodeCount(_node);
    if (count < 0) { return false; }

    if (index > count) { return false; }

    // Find the definition that match the 'moveNode'
    const NodeDefinition* moveDefinition = containerDefinition(moveNode);
    if (!moveDefinition) { return false; }

    // Find the parent node of the 'moveNode'
    Node moveNodeParent = moveDefinition->parentNode(moveNode);
    if (moveNodeParent.isNull()) { return false; }

    // Handle special case where the 'moveNode' is already a child of 'node'
    if (_node == moveNodeParent) {
        if (index == count) { return false; }

        if (index == -1) { index = count-1; }

        return true;
    }

    // Find the node definition of 'moveNode' parent
    const ContainerDefinition* moveParentContainer = moveDefinition->parentContainer(moveNodeParent);
    if (moveParentContainer == NULL) { return false; }

    // Find the index of the 'moveNode'
    int moveIndex = moveParentContainer->nodeIndex(moveNodeParent, moveNode);
    if (moveIndex == -1) { return false; }

    // Check if the 'moveNode' can be removed
    if (!moveParentContainer->canRemoveNode(moveNodeParent, moveIndex)) { return false; }

    // Check if the 'moveNode' can be cloned
    return canCloneNode(_node, index, moveNode);
}

// =============================================================================
// (public)
Node ContainerDefinition::moveNode(Node _node, int &index, Node moveNode) const
{
    Node newNode;
    if (canMoveNode(_node, index, moveNode)) {
        switch (_node.type()) {
#ifdef XML_BACKEND
        case Node::Type::XML:
            newNode = m_elementListRef.move(_node.xmlNode(), index, moveNode.xmlNode());
            break;
#endif // XML_BACKEND
        default:
            // _node.type() returns an unhandled type that needs to be implemented
            assert(false);
            return newNode;
        }
    }
    m_containerDefinition->onNodeMoved(newNode);

    return newNode;
}

// =============================================================================
// (public)
bool ContainerDefinition::canRemoveNode(Node _node, int index) const
{
    if (isNull()) { return false; }

    // Find the data node that is to be removed
    Node dataNodeRemove = node(_node, index);
    if (dataNodeRemove.isNull()) { return false; }

    // Find the number of existing number of child data nodes of the same type
    int count = nodeCount(_node);

    // Test if the removal of the data node will bring the number below the minimum allowed
    if (count-1 < m_minCount) { return false; }

    return true;
}

// =============================================================================
// (public)
bool ContainerDefinition::removeNode(Node _node, int index) const
{
    if (canRemoveNode(_node, index)) {
        switch (_node.type()) {
#ifdef XML_BACKEND
        case Node::Type::XML: {
            return m_elementListRef.remove(_node.xmlNode(), index);
        }
#endif // XML_BACKEND
        default:
            // _node.type() returns an unhandled type that needs to be implemented
            assert(false);
        }
    }
    return false;
}

// =============================================================================
// (public)
const ContainerDefinition &ContainerDefinition::emptyChildNodeDefinition()
{
    static ContainerDefinition emptyChild;
    return emptyChild;
}

// =============================================================================
// (public)
bool ContainerDefinition::checkUniqueOptionValues(Node _node) const
{
    // Check if a unique options only list do not have any options left
    auto vList = m_containerDefinition->valueList();
    for (const ValueDefinition* vDef: vList)
    {
        if (vDef->settings().unique() &&
            vDef->options().isUsed() &&
            vDef->settings().optionsOnly()) {
            Node firstSibling = firstNode(_node);
            if (firstSibling.isNull()) {
                break;
            }
            Item item(m_containerDefinition.get(), firstSibling);
            std::vector<std::string> optionList;
            if (vDef->options().getOptions(optionList, &item)) {
                if (optionList.size() <= 1) {
                    return false;
                }
            }
        }
    }
    return true;
}

} // namespace Model
} // namespace Oak
