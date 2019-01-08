/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ContainerDef.h"

#include "../ServiceFunctions/Assert.h"

#include "NodeDef.h"
#include "Node.h"
#include "ValueOptions.h"


namespace Oak::Model {

// =============================================================================
// (public)
ContainerDef::ContainerDef()
    : m_minCount(-1)
    , m_maxCount(-1)
#ifdef XML_BACKEND
    , m_elementListRef("")
#endif // XML_BACKEND
{
}

// =============================================================================
// (public)
ContainerDef::ContainerDef(NodeDefSPtr nodeDef, int min, int max)
    : m_containerDef(nodeDef)
    , m_minCount(min)
    , m_maxCount(max)
#ifdef XML_BACKEND
    , m_elementListRef(nodeDef->tagName())
#endif // XML_BACKEND
{
    ASSERT(m_containerDef);
    ASSERT(m_minCount >= 0);
    ASSERT(m_maxCount > 0);
    ASSERT(m_maxCount >= m_minCount);
}

// =============================================================================
// (public)
ContainerDef::ContainerDef(const ContainerDef &copy)
    : m_elementListRef(copy.m_elementListRef)
{
    *this = copy;
}

// =============================================================================
// (public)
ContainerDef::ContainerDef(ContainerDef &&move)
{
    *this = std::move(move);
}

// =============================================================================
// (public)
ContainerDef& ContainerDef::operator=(const ContainerDef &copy)
{
    m_hostDef = copy.m_hostDef;
    m_containerDef = copy.m_containerDef;
    m_minCount = copy.m_minCount;
    m_maxCount = copy.m_maxCount;

#ifdef XML_BACKEND
    m_elementListRef = copy.m_elementListRef;
#endif // XML_BACKEND
    return *this;
}

// =============================================================================
// (public)
ContainerDef& ContainerDef::operator=(ContainerDef &&move)
{
    m_hostDef = move.m_hostDef;
    m_containerDef = move.m_containerDef;
    m_minCount = move.m_minCount;
    m_maxCount = move.m_maxCount;

#ifdef XML_BACKEND
    m_elementListRef = std::move(move.m_elementListRef);
#endif // XML_BACKEND
    return *this;
}

// =============================================================================
// (public)
ContainerDefUPtr ContainerDef::copy() const
{
    return MakeUPtr(*this);
}

// =============================================================================
// (public)
bool ContainerDef::isNull() const
{
    return m_hostDef.expired();
}

// =============================================================================
// (public)
const NodeDef* ContainerDef::containerDef() const
{
    return m_containerDef.get();
}

// =============================================================================
// (public)
const NodeDef* ContainerDef::containerDef(const UnionRef &variantId) const
{
    if (!m_containerDef) { return nullptr; }

    if (variantId.isNull() || variantId.isEqual(m_containerDef->variantId())) {
        return m_containerDef.get();
    }
    return m_containerDef->validVariant(variantId);
}

// =============================================================================
// (public)
const NodeDef* ContainerDef::containerDef(const NodeData &_nodeData) const
{
    if (!m_containerDef) { return nullptr; }

    if (m_containerDef->validate(_nodeData)) {
        return m_containerDef.get();
    }
    return m_containerDef->validVariant(_nodeData);
}

// =============================================================================
// (public)
bool ContainerDef::validate(const NodeData &_nodeData) const
{
    if (_nodeData.isNull()) { return false; }

    switch (_nodeData.type()) {
#ifdef XML_BACKEND
    case NodeData::Type::XML: {
        if (!m_containerDef) { return false; }
        if (_nodeData.xmlNode().compareTagName(containerDef()->tagName()) != 0) { return false; }
        return m_containerDef->validate(_nodeData, false, true);
    }
#endif // XML_BACKEND
    default:
        // _nodeData.type() returns an unhandled type that needs to be implemented
        ASSERT(false);
    }
    return false;
}

// =============================================================================
// (public)
int ContainerDef::minCount() const
{
    ASSERT(m_minCount >= 0);
    return m_minCount;
}

// =============================================================================
// (public)
int ContainerDef::maxCount() const
{
    ASSERT(m_maxCount > 0);
    return m_maxCount;
}

// =============================================================================
// (public)
int ContainerDef::nodeCount(const NodeData &_nodeData) const
{
    if (_nodeData.isNull()) { return -1; }

    switch (_nodeData.type()) {
#ifdef XML_BACKEND
    case NodeData::Type::XML:
        return m_elementListRef.count(_nodeData.xmlNode());
#endif // XML_BACKEND
    default:
        // _nodeData.type() returns an unhandled type that needs to be implemented
        ASSERT(false);
    }
    return -1;
}

// =============================================================================
// (public)
int ContainerDef::nodeIndex(const NodeData &_nodeData, const NodeData &refNode) const
{
    if (_nodeData.isNull()) { return -1; }

    switch (_nodeData.type()) {
#ifdef XML_BACKEND
    case NodeData::Type::XML:
        return m_elementListRef.indexOf(_nodeData.xmlNode(), refNode.xmlNode());
#endif // XML_BACKEND
    default:
        // _nodeData.type() returns an unhandled type that needs to be implemented
        ASSERT(false);
    }
    return -1;
}

// =============================================================================
// (public)
NodeData ContainerDef::node(const NodeData &_nodeData, int index, const NodeDef** nodeDef) const
{
    if (_nodeData.isNull()) { return NodeData(); }

    NodeData childNode;
    switch (_nodeData.type()) {
#ifdef XML_BACKEND
    case NodeData::Type::XML:
        childNode =  m_elementListRef.at(_nodeData.xmlNode(), index);
        break;
#endif // XML_BACKEND
    default:
        // _nodeData.type() returns an unhandled type that needs to be implemented
        ASSERT(false);
    }

    if (nodeDef) {
        *nodeDef = (childNode.isNull()) ? nullptr : containerDef(childNode);
    }
    return childNode;
}

// =============================================================================
// (public)
NodeData ContainerDef::firstNode(const NodeData &_nodeData, const NodeDef** nodeDef) const
{
    if (_nodeData.isNull()) { return NodeData(); }

    NodeData childNode;
    switch (_nodeData.type()) {
#ifdef XML_BACKEND
    case NodeData::Type::XML:
        childNode = m_elementListRef.first(_nodeData.xmlNode());
        break;
#endif // XML_BACKEND
    default:
        // _nodeData.type() returns an unhandled type that needs to be implemented
        ASSERT(false);
    }

   if (nodeDef) {
        *nodeDef = (childNode.isNull()) ? nullptr : containerDef(childNode);
    }
    return childNode;
}

// =============================================================================
// (public)
NodeData ContainerDef::lastNode(const NodeData &_nodeData, const NodeDef** nodeDef) const
{
    if (_nodeData.isNull()) { return NodeData(); }

    NodeData childNode;
    switch (_nodeData.type()) {
#ifdef XML_BACKEND
    case NodeData::Type::XML:
        childNode = m_elementListRef.last(_nodeData.xmlNode());
        break;
#endif // XML_BACKEND
    default:
        // _nodeData.type() returns an unhandled type that needs to be implemented
        ASSERT(false);
    }

    if (nodeDef) {
        *nodeDef = (childNode.isNull()) ? nullptr : containerDef(childNode);
    }
    return childNode;
}

// =============================================================================
// (public)
NodeData ContainerDef::nextNode(const NodeData &refNodeData, const NodeDef** nodeDef) const
{
    if (refNodeData.isNull()) { return NodeData(); }

    NodeData childNode;
    switch (refNodeData.type()) {
#ifdef XML_BACKEND
    case NodeData::Type::XML:
        childNode =  m_elementListRef.next(refNodeData.xmlNode());
        break;
#endif // XML_BACKEND
    default:
        // _nodeData.type() returns an unhandled type that needs to be implemented
        ASSERT(false);
    }

    if (nodeDef) {
        *nodeDef = (childNode.isNull()) ? nullptr : containerDef(childNode);
    }
    return childNode;
}

// =============================================================================
// (public)
NodeData ContainerDef::previousNode(const NodeData &refNodeData, const NodeDef** nodeDef) const
{
    if (refNodeData.isNull()) { return NodeData(); }

    NodeData childNode;
    switch (refNodeData.type()) {
#ifdef XML_BACKEND
    case NodeData::Type::XML:
        childNode =  m_elementListRef.previous(refNodeData.xmlNode());
        break;
#endif // XML_BACKEND
    default:
        // _nodeData.type() returns an unhandled type that needs to be implemented
        ASSERT(false);
    }

    if (nodeDef) {
        *nodeDef = (childNode.isNull()) ? nullptr : containerDef(childNode);
    }
    return childNode;
}

// =============================================================================
// (public)
const NodeDef* ContainerDef::hostDef() const
{
    return m_hostDef.lock().get();
}

// =============================================================================
// (public)
NodeData ContainerDef::hostNode(const NodeData &refNodeData) const
{
    if (refNodeData.isNull()) { return NodeData(); }

    switch (refNodeData.type()) {
#ifdef XML_BACKEND
    case NodeData::Type::XML: {
        XML::Element hostElement = m_elementListRef.invertedAt(refNodeData.xmlNode());

        if (hostElement.empty()) { return NodeData(); }

        if (hostDef()->tagName().compare(hostElement.tagName()) != 0) { return NodeData(); }
        return hostElement;
    }
#endif // XML_BACKEND
    default:
        // _nodeData.type() returns an unhandled type that needs to be implemented
        ASSERT(false);
    }
    return NodeData();
}

// =============================================================================
// (public)
bool ContainerDef::canInsertNode(const NodeData &_nodeData, int &index) const
{
    if (isNull()) { return false; }

    // The data node and the child definition is validated in the function
    int count = nodeCount(_nodeData);
    if (count < 0) { return false; }

    // Check if there are room for one more child node
    if (count >= maxCount()) { return false; }

    // Check if a unique options only list do not have any options left
    if (!checkUniqueOptionValues(_nodeData)) { return false; }

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
NodeData ContainerDef::insertNode(const NodeData &_nodeData, int &index) const
{
    NodeData newNode;
    if (canInsertNode(_nodeData, index)) {
        switch (_nodeData.type()) {
#ifdef XML_BACKEND
        case NodeData::Type::XML:
            newNode = m_elementListRef.insert(_nodeData.xmlNode(), index);
            break;
#endif // XML_BACKEND
        default:
            // _nodeData.type() returns an unhandled type that needs to be implemented
            ASSERT(false);
            return newNode;
        }
    }
    m_containerDef->onNodeInserted(newNode);
    return newNode;
}

// =============================================================================
// (public)
bool ContainerDef::canCloneNode(const NodeData &_nodeData, int &index, const NodeData &cloneNode) const
{
    if (isNull()) { return false; }

    // Check that clone node is valid
    if (!m_containerDef->validate(cloneNode, false, true)) { return false; }

    // Check if 'cloneNode' is parent(recursive) of 'node'
    // Can not clone it selv into it selv

    if (!m_hostDef.expired() && m_hostDef.lock()->isParent(_nodeData, cloneNode)) { return false; }

    return canInsertNode(_nodeData, index);
}

// =============================================================================
// (public)
NodeData ContainerDef::cloneNode(const NodeData &_nodeData, int &index, const NodeData &cloneNode) const
{
    NodeData newNode;
    if (canCloneNode(_nodeData, index, cloneNode)) {
        switch (_nodeData.type()) {
#ifdef XML_BACKEND
        case NodeData::Type::XML:
            newNode = m_elementListRef.clone(_nodeData.xmlNode(), index, cloneNode.xmlNode());
            break;
#endif // XML_BACKEND
        default:
            // _nodeData.type() returns an unhandled type that needs to be implemented
            ASSERT(false);
            return newNode;
        }
    }

    m_containerDef->onNodeCloned(newNode);
    return newNode;
}

// =============================================================================
// (public)
bool ContainerDef::canMoveNode(const NodeData &_nodeData, int &index, const NodeData &moveNode) const
{
    if (isNull()) { return false; }
    if (index < -1) { return false; }

    // The data node and the child node definition is validated in the function
    int count = nodeCount(_nodeData);
    if (count < 0) { return false; }

    if (index > count) { return false; }

    // Find the definition that match the 'moveNode'
    const NodeDef* moveDef = containerDef(moveNode);
    if (!moveDef) { return false; }

    // Find the parent node of the 'moveNode'
    NodeData moveNodeParent = moveDef->parentNode(moveNode);
    if (moveNodeParent.isNull()) { return false; }

    // Find the node definition of 'moveNode' parent
    const ContainerDef* moveParentContainer = moveDef->parentContainer(moveNodeParent);
    if (moveParentContainer == nullptr) { return false; }

    // Find the index of the 'moveNode'
    int moveIndex = moveParentContainer->nodeIndex(moveNodeParent, moveNode);
    if (moveIndex == -1) { return false; }

    // Handle special case where the 'moveNode' is already a child of 'node'
    if (_nodeData == moveNodeParent) {
        if (index == count) { return false; }

        if (index == -1) {
            if (moveIndex == count-1) { return false; } // Can not move to its own position
            index = count-1;
        } else if (moveIndex == index) {
            return false;  // Can not move to its own position
        }

        return true;
    }

    // Check if the 'moveNode' can be removed
    if (!moveParentContainer->canRemoveNode(moveNodeParent, moveIndex)) { return false; }

    // Check if the 'moveNode' can be cloned
    return canCloneNode(_nodeData, index, moveNode);
}

// =============================================================================
// (public)
NodeData ContainerDef::moveNode(const NodeData &_nodeData, int &index, const NodeData &moveNode) const
{
    NodeData newNode;
    if (canMoveNode(_nodeData, index, moveNode)) {
        switch (_nodeData.type()) {
#ifdef XML_BACKEND
        case NodeData::Type::XML:
            newNode = m_elementListRef.move(_nodeData.xmlNode(), index, moveNode.xmlNode());
            break;
#endif // XML_BACKEND
        default:
            // _nodeData.type() returns an unhandled type that needs to be implemented
            ASSERT(false);
            return newNode;
        }
    }
    m_containerDef->onNodeMoved(newNode);

    return newNode;
}

// =============================================================================
// (public)
bool ContainerDef::canRemoveNode(const NodeData &_nodeData, int index) const
{
    if (isNull()) { return false; }

    // Find the data node that is to be removed
    NodeData dataNodeRemove = node(_nodeData, index);
    if (dataNodeRemove.isNull()) { return false; }

    // Find the number of existing number of child data nodes of the same type
    int count = nodeCount(_nodeData);

    // Test if the removal of the data node will bring the number below the minimum allowed
    if (count-1 < m_minCount) { return false; }

    return true;
}

// =============================================================================
// (public)
bool ContainerDef::removeNode(const NodeData &_nodeData, int index) const
{
    if (canRemoveNode(_nodeData, index)) {
        switch (_nodeData.type()) {
#ifdef XML_BACKEND
        case NodeData::Type::XML: {
            return m_elementListRef.remove(_nodeData.xmlNode(), index);
        }
#endif // XML_BACKEND
        default:
            // _nodeData.type() returns an unhandled type that needs to be implemented
            ASSERT(false);
        }
    }
    return false;
}

// =============================================================================
// (public)
const ContainerDef &ContainerDef::emptyChildNodeDef()
{
    static ContainerDef emptyChild;
    return emptyChild;
}

// =============================================================================
// (public)
bool ContainerDef::checkUniqueOptionValues(const NodeData &_nodeData) const
{
    // Check if a unique options only list do not have any options left
    auto vList = m_containerDef->valueList();
    for (const LeafDef* vDef: vList)
    {
        if (vDef->settings().value(UNIQUE) > 0 &&
            vDef->options().isUsed() &&
            vDef->settings().value(OPTION_ONLY)) {
            NodeData firstSibling = firstNode(_nodeData);
            if (firstSibling.isNull()) {
                break;
            }
            Node node(m_containerDef.get(), firstSibling);
            std::vector<std::string> optionList;
            if (vDef->options().getOptions(optionList, &node)) {
                if (optionList.size() <= 1) {
                    return false;
                }
            }
        }
    }
    return true;
}

} // namespace Oak::Model

