/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "Node.h"

#include <algorithm>

#include "OakModel.h"
#include "LeafQuery.h"
#include "OakModelServiceFunctions.h"
#include "QueryBuilder.h"

#include "../ServiceFunctions/Trace.h"

namespace Oak::Model {

// =============================================================================
// (public)
Node::Node()
    : m_def(nullptr),
      m_model(nullptr)
{

}

// =============================================================================
// (public)
Node::Node(const NodeDef* nodeDef, const NodeData &nodeData, const OakModel* model)
    : m_nodeData(nodeData),
      m_model(model)
{
    if (nodeDef == nullptr || nodeData.isNull()) {
        m_def = nodeDef;
    } else {
        m_def = nodeDef->validVariant(nodeData);
    }
}

// =============================================================================
// (public)
Node::Node(const Node& copy)
    : m_def(copy.m_def),
      m_nodeData(copy.m_nodeData),
      m_model(copy.m_model),
      m_leafList(copy.m_leafList)
{

}

// =============================================================================
// (public)
Node::Node(Node &&move)
    : m_def(std::move(move.m_def)),
      m_nodeData(std::move(move.m_nodeData)),
      m_model(move.m_model),
      m_leafList(std::move(move.m_leafList))
{

}

// =============================================================================
// (public)
Node& Node::operator=(const Node& copy)
{
    m_nodeData = copy.m_nodeData;
    m_model = copy.m_model;

    if (m_def == copy.m_def) {  // NodeDef is the same: Leaf list is still valid
        if (!m_leafList.empty()) { // Leaf list is initialized
            // Update node for leaf lists
            for (auto &leaf: m_leafList) {
                leaf.m_nodeData = m_nodeData;
            }
        }
    } else {
        m_leafList.assign(copy.m_leafList.begin(), copy.m_leafList.end());
    }

    m_def = copy.m_def;

    return *this;
}

// =============================================================================
// (public)
Node& Node::operator=(Node&& move)
{
    m_leafList = std::move(move.m_leafList);
    m_def = std::move(move.m_def);
    m_nodeData = std::move(move.m_nodeData);
    m_model = move.m_model;
    return *this;
}

// =============================================================================
// (public)
bool Node::operator==(const Node& _node) const
{
    return m_def == _node.m_def && m_nodeData == _node.m_nodeData;
}

// =============================================================================
// (public)
bool Node::operator!=(const Node& _node) const
{
    return !(operator==(_node));
}

// =============================================================================
// (public)
const Leaf& Node::operator()(const std::string &valueName) const
{
    return leaf(valueName);
}

// =============================================================================
// (public)
Node Node::operator[](int index) const
{
    return childAt(index);
}

// =============================================================================
// (public)
bool Node::isNull() const
{
    return isNodeNull() || isDefNull();
}

// =============================================================================
// (public)
bool Node::isDefNull() const
{
    return m_def == nullptr || m_def->isNull();
}

// =============================================================================
// (public)
bool Node::isNodeNull() const
{
    return m_nodeData.isNull();
}

// =============================================================================
// (public)
void Node::clear()
{
    m_def = nullptr;
    m_nodeData.clear();
    m_model = nullptr;
    m_leafList.clear();
}

// =============================================================================
// (public)
const OakModel* Node::model() const
{
    return m_model;
}

// =============================================================================
// (public)
void Node::setCurrent()
{
    ASSERT(m_def);
    if (m_model) {
        m_model->setCurrentNode(*this);
    }
}

// =============================================================================
// (public)
std::vector<std::string> Node::valueNameList() const
{
    initLeafList();
    std::vector<std::string> nameList;
    for (const Leaf& iv: m_leafList)
    {
        nameList.push_back(iv.name());
    }
    return nameList;
}

// =============================================================================
// (public)
std::vector<std::string> Node::childNameList() const
{
    ASSERT(m_def);
    std::vector<const ContainerDef*> containerList;
    m_def->getContainerList(containerList);
    std::vector<std::string> nameList;
    for (const ContainerDef* container: containerList)
    {
        nameList.push_back(container->containerDef()->name());
    }
    return nameList;
}

// =============================================================================
// (public)
int Node::leafCount() const
{
    initLeafList();
    return static_cast<int>(m_leafList.size());
}

// =============================================================================
// (public)
bool Node::hasLeaf(const std::string &leafName) const
{
    return m_def->hasValue(leafName);
}

// =============================================================================
// (public)
int Node::leafIndex(const Leaf &leaf) const
{
    initLeafList();
    auto it = std::find(m_leafList.begin(), m_leafList.end(), leaf);
    if (it == m_leafList.end()) {
        return -1;
    } else {
        return static_cast<int>(std::distance(m_leafList.begin(), it));
    }
}

// =============================================================================
// (public)
const Leaf& Node::leafAt(int index) const
{
    initLeafList();
    if (index < 0 || index >= static_cast<int>(m_leafList.size())) { return Leaf::emptyLeaf(); }
    return m_leafList.at(static_cast<vSize>(index));
}

// =============================================================================
// (public)
Leaf &Node::leafAt(int index)
{
    initLeafList();
    if (index < 0 || index >= static_cast<int>(m_leafList.size())) { return Leaf::emptyLeaf(); }
    return m_leafList.at(static_cast<vSize>(index));
}

// =============================================================================
// (public)
const Leaf& Node::leaf(const std::string &leafName) const
{
    initLeafList();
    for (const Leaf& iv: m_leafList)
    {
        if (iv.name() == leafName) {
            return iv;
        }
    }
    return Leaf::emptyLeaf();
}

// =============================================================================
// (public)
Leaf &Node::leaf(const std::string &leafName)
{
    initLeafList();
    for (Leaf& iv: m_leafList)
    {
        if (iv.name() == leafName) {
            return iv;
        }
    }
    return Leaf::emptyLeaf();
}

// =============================================================================
// (public)
const Node::LeafIterator Node::leafBegin() const
{
    initLeafList();
    return m_leafList.begin();
}

// =============================================================================
// (public)
Node::LeafIterator Node::leafBegin()
{
    initLeafList();
    return m_leafList.begin();
}

// =============================================================================
// (public)
const Node::LeafIterator Node::leafEnd() const
{
    initLeafList();
    return m_leafList.end();
}

// =============================================================================
// (public)
Node::LeafIterator Node::leafEnd()
{
    initLeafList();
    return m_leafList.end();
}

// =============================================================================
// (public)
bool Node::hasKey() const
{
    ASSERT(m_def);
    return m_def->hasKey();
}

// =============================================================================
// (public)
const Leaf& Node::keyLeaf() const
{
    initLeafList();
    if (hasKey()) { return m_leafList[static_cast<vSize>(m_def->indexOfKeyLeafDef())]; }
    return Leaf::emptyLeaf();
}

// =============================================================================
// (public)
bool Node::hasVariants() const
{
    ASSERT(m_def);
    return m_def->hasVariants();
}

// =============================================================================
// (public)
const Leaf& Node::variantLeaf() const
{
    initLeafList();
    if (hasVariants()) { return m_leafList[static_cast<vSize>(m_def->indexOfVariantLeafDef())]; }
    return Leaf::emptyLeaf();
}

// =============================================================================
// (public)
int Node::childCount() const
{
    ASSERT(m_def);
    return m_def->containerGroup().nodeCount(m_nodeData);
}

// =============================================================================
// (public)
int Node::childCount(const std::string &name) const
{
    ASSERT(m_def);
    return m_def->container(name).nodeCount(m_nodeData);
}

// =============================================================================
// (public)
int Node::childIndex(const Node& refChild) const
{
    ASSERT(m_def);
    return m_def->containerGroup().nodeIndex(m_nodeData, refChild.nodeData());
}

// =============================================================================
// (public)
int Node::childIndex(const std::string &name, const Node &refChild) const
{
    ASSERT(m_def);
    return m_def->container(name).nodeIndex(m_nodeData, refChild.nodeData());
}

// =============================================================================
// (public)
Node Node::childAt(int index) const
{
    ASSERT(m_def);
    const NodeDef* childeNodeDef;
    NodeData childNodeData = m_def->containerGroup().node(m_nodeData, index, &childeNodeDef);
    return Node(childeNodeDef, childNodeData, m_model);
}

// =============================================================================
// (public)
Node Node::childAt(const std::string &name, int index) const
{
    ASSERT(m_def);
    const NodeDef* childeNodeDef;
    NodeData childNodeData = m_def->container(name).node(m_nodeData, index, &childeNodeDef);
    return Node(childeNodeDef, childNodeData, m_model);
}

// =============================================================================
// (public)
Node Node::firstChild() const
{
    ASSERT(m_def);
    const NodeDef* childNodeDef;
    NodeData childNodeData = m_def->containerGroup().firstNode(m_nodeData, &childNodeDef);
    return Node(childNodeDef, childNodeData, m_model);
}

// =============================================================================
// (public)
Node Node::firstChild(const std::string &name) const
{
    ASSERT(m_def);
    const NodeDef* childNodeDef;
    NodeData childNodeData = m_def->container(name).firstNode(m_nodeData, &childNodeDef);
    return Node(childNodeDef, childNodeData, m_model);
}

// =============================================================================
// (public)
Node Node::lastChild() const
{
    ASSERT(m_def);
    const NodeDef* childNodeDef;
    NodeData childNodeData = m_def->containerGroup().lastNode(m_nodeData, &childNodeDef);
    return Node(childNodeDef, childNodeData, m_model);
}

// =============================================================================
// (public)
Node Node::lastChild(const std::string &name) const
{
    ASSERT(m_def);
    const NodeDef* childNodeDef;
    NodeData childNodeData = m_def->container(name).lastNode(m_nodeData, &childNodeDef);
    return Node(childNodeDef, childNodeData, m_model);
}

// =============================================================================
// (public)
Node Node::nextChild(const Node& refChild) const
{
    ASSERT(m_def);
    const NodeDef* childNodeDef;
    NodeData childNodeData = m_def->containerGroup().nextNode(m_nodeData, refChild.m_nodeData, &childNodeDef);
    return Node(childNodeDef, childNodeData, m_model);
}

// =============================================================================
// (public)
Node Node::nextChild(const std::string &name, const Node& refChild) const
{
    ASSERT(m_def);
    const NodeDef* childNodeDef;
    NodeData childNodeData = m_def->container(name).nextNode(refChild.m_nodeData, &childNodeDef);
    return Node(childNodeDef, childNodeData, m_model);
}

// =============================================================================
// (public)
Node Node::previousChild(const Node& refChild) const
{
    ASSERT(m_def);
    const NodeDef* childNodeDef;
    NodeData childNodeData = m_def->containerGroup().previousNode(m_nodeData, refChild.m_nodeData, &childNodeDef);
    return Node(childNodeDef, childNodeData, m_model);
}

// =============================================================================
// (public)
Node Node::previousChild(const std::string &name, const Node& refChild) const
{
    ASSERT(m_def);
    const NodeDef* childNodeDef;
    NodeData childNodeData = m_def->container(name).previousNode(refChild.m_nodeData, &childNodeDef);
    return Node(childNodeDef, childNodeData, m_model);
}

// =============================================================================
// (public)
Node Node::parent() const
{
    ASSERT(m_def);
    const NodeDef* parentNodeDef;
    NodeData parentNodeData = m_def->parentNode(m_nodeData, &parentNodeDef);
    return Node(parentNodeDef, parentNodeData, m_model);
}

// =============================================================================
// (public)
bool Node::canInsertChild(const std::string &name, int &index) const
{
    ASSERT(m_def);
    return m_def->container(name).canInsertNode(m_nodeData, index);
}

// =============================================================================
// (public)
Node Node::insertChild(const std::string &name, int &index) const
{
    ASSERT(m_def);
    const auto& container = m_def->container(name);
    if (m_model) {
        if (!container.canInsertNode(m_nodeData, index)) { return Node(); }
        NodeIndexUPtr iIndex = NodeIndex::create(*this);
        iIndex->lastNodeIndex().setChildNodeIndex(new NodeIndex(container.containerDef()->name(), index));
        m_model->onNodeInserteBefore(*iIndex.get());
    }

    NodeData nodeData = container.insertNode(m_nodeData, index);
    //TRACE("Child node inserted\n");

    Node childNode(container.containerDef(), nodeData, m_model);
    if (m_model && !childNode.isNull()) {
        NodeIndexUPtr iIndex = NodeIndex::create(childNode);
        m_model->onNodeInserteAfter(*iIndex.get());
    }
    return childNode;
}

// =============================================================================
// (public)
bool Node::canCloneChild(int& index, const Node &cloneNode) const
{
    ASSERT(m_def);
    return m_def->containerGroup().canCloneNode(m_nodeData, index, cloneNode.m_nodeData);
}

// =============================================================================
// (public)
bool Node::canCloneChild(const std::string &name, int &index, const Node &cloneNode) const
{
    ASSERT(m_def);
    return m_def->container(name).canCloneNode(m_nodeData, index, cloneNode.m_nodeData);
}

// =============================================================================
// (public)
Node Node::cloneChild(int& index, const Node &cloneNode) const
{
    ASSERT(m_def);
    if (m_model) {
        // Cash data needed to notify change
        NodeIndexUPtr sourceNodeIndex = NodeIndex::create(cloneNode);

        {
            if (!m_def->containerGroup().canCloneNode(m_nodeData, index, cloneNode.nodeData())) { return Node(); }
            NodeIndexUPtr targetNodeIndex = NodeIndex::create(*this);
            targetNodeIndex->lastNodeIndex().setChildNodeIndex(new NodeIndex(index));
            m_model->onNodeCloneBefore(*sourceNodeIndex.get(), *targetNodeIndex.get());
        }

        // Perform the cloneing
        Node node = Node(cloneNode.m_def, m_def->containerGroup().cloneNode(m_nodeData, index, cloneNode.m_nodeData), m_model);

        // Notify everyone if cloning did not fail
        if (!node.isNull()) {
            NodeIndexUPtr targetNodeIndex = NodeIndex::create(node);
            m_model->onNodeCloneAfter(*sourceNodeIndex.get(), *targetNodeIndex.get());
        }
        updateUniqueValues(node);

        return node;
    } else {
        return Node(cloneNode.m_def, m_def->containerGroup().cloneNode(m_nodeData, index, cloneNode.m_nodeData), m_model);
    }
}

// =============================================================================
// (public)
Node Node::cloneChild(const std::string &name, int &index, const Node &cloneNode) const
{
    ASSERT(m_def);
    if (m_model) {
        // Cash data needed to notify change
        NodeIndexUPtr sourceNodeIndex = NodeIndex::create(cloneNode);
        const ContainerDef &container = m_def->container(name);
        {
            if (!container.canCloneNode(m_nodeData, index, cloneNode.nodeData())) { return Node(); }
            NodeIndexUPtr targetNodeIndex = NodeIndex::create(*this);
            targetNodeIndex->lastNodeIndex().setChildNodeIndex(new NodeIndex(container.containerDef()->name(), index));
            m_model->onNodeCloneBefore(*sourceNodeIndex.get(), *targetNodeIndex.get());
        }

        // Perform the cloneing
        Node node = Node(cloneNode.m_def, container.cloneNode(m_nodeData, index, cloneNode.nodeData()), m_model);

        // Notify everyone if cloning did not fail
        if (!node.isNull()) {
            NodeIndexUPtr targetNodeIndex = NodeIndex::create(node);
            m_model->onNodeCloneAfter(*sourceNodeIndex.get(), *targetNodeIndex.get());
        }
        updateUniqueValues(node);

        return node;
    } else {
        return Node(cloneNode.m_def, m_def->container(name).cloneNode(m_nodeData, index, cloneNode.m_nodeData), m_model);
    }
}

// =============================================================================
// (public)
bool Node::canMoveChild(int& index, const Node &moveNode) const
{
    ASSERT(m_def);
    return m_def->containerGroup().canMoveNode(m_nodeData, index, moveNode.m_nodeData);
}

// =============================================================================
// (public)
bool Node::canMoveChild(const std::string &name, int &index, const Node &moveNode) const
{
    ASSERT(m_def);
    return m_def->container(name).canMoveNode(m_nodeData, index, moveNode.m_nodeData);
}

// =============================================================================
// (public)
Node Node::moveChild(int& index, const Node &moveNode) const
{
    ASSERT(m_def);
    if (m_model) {
        // Check if node can be moved
        if (!m_def->containerGroup().canMoveNode(m_nodeData, index, moveNode.m_nodeData)) { return Node(); }

        // Cash data needed to notify change
        NodeIndexUPtr sourceNodeIndex = NodeIndex::create(moveNode);
        NodeIndexUPtr targetNodeIndex = NodeIndex::create(*this);
        targetNodeIndex->setChildNodeIndex(new NodeIndex(index));

        // Notify before move
        m_model->onNodeMoveBefore(*sourceNodeIndex.get(), *targetNodeIndex.get());

        // Perform the move
        Node node = Node(moveNode.m_def, m_def->containerGroup().moveNode(m_nodeData, index, moveNode.m_nodeData), m_model);

        ASSERT(!node.isNull());

        targetNodeIndex = NodeIndex::create(node);

        // Notify after move
        m_model->onNodeMoveAfter(*sourceNodeIndex.get(), *targetNodeIndex.get());

        updateUniqueValues(node);

        return node;
    } else {
        return Node(moveNode.m_def, m_def->containerGroup().moveNode(m_nodeData, index, moveNode.m_nodeData), m_model);
    }
}

// =============================================================================
// (public)
Node Node::moveChild(const std::string &name, int &index, const Node &moveNode) const
{
    ASSERT(m_def);
    if (m_model) {
        // Check if node can be moved
        if (!m_def->container(name).canMoveNode(m_nodeData, index, moveNode.m_nodeData)) { return Node(); }

        // Cash data needed to notify change
        NodeIndexUPtr sourceNodeIndex = NodeIndex::create(moveNode);
        NodeIndexUPtr targetNodeIndex = NodeIndex::create(*this);
        targetNodeIndex->setChildNodeIndex(new NodeIndex(name, index));

        // Notify before move
        m_model->onNodeMoveBefore(*sourceNodeIndex.get(), *targetNodeIndex.get());

        // Perform the move
        Node node = Node(moveNode.m_def, m_def->container(name).moveNode(m_nodeData, index, moveNode.m_nodeData), m_model);

        ASSERT(!node.isNull());

        targetNodeIndex = NodeIndex::create(node);

        // Notify after move
        m_model->onNodeMoveAfter(*sourceNodeIndex.get(), *targetNodeIndex.get());

        updateUniqueValues(node);

        return node;
    } else {
        return Node(moveNode.m_def, m_def->container(name).moveNode(m_nodeData, index, moveNode.m_nodeData), m_model);
    }
}

// =============================================================================
// (public)
bool Node::canRemoveChild(int index) const
{
    ASSERT(m_def);
    return m_def->containerGroup().canRemoveNode(m_nodeData, index);
}

// =============================================================================
// (public)
bool Node::canRemoveChild(const std::string &name, int index) const
{
    ASSERT(m_def);
    return m_def->container(name).canRemoveNode(m_nodeData, index);
}

// =============================================================================
// (public)
bool Node::removeChild(int index) const
{
    ASSERT(m_def);
    if (m_def->containerGroup().canRemoveNode(m_nodeData, index)) {

        NodeIndexUPtr iIndex = NodeIndex::create(childAt(index));
        if (m_model) {
            m_model->onNodeRemoveBefore(*iIndex.get());
        }

        m_def->containerGroup().removeNode(m_nodeData, index);
        if (m_model) {
            m_model->onNodeRemoveAfter(*iIndex.get());
        }
        return true;
    }
    return false;
}

// =============================================================================
// (public)
bool Node::removeChild(const std::string &name, int index) const
{
    ASSERT(m_def);
    if (m_def->container(name).canRemoveNode(m_nodeData, index)) {

        NodeIndexUPtr iIndex = NodeIndex::create(childAt(name, index));
        if (m_model) {
            m_model->onNodeRemoveBefore(*iIndex.get());
        }

        m_def->container(name).removeNode(m_nodeData, index);
        if (m_model) {
            m_model->onNodeRemoveAfter(*iIndex.get());
        }
        return true;
    }
    return false;
}

// =============================================================================
// (public)
int Node::convertChildIndexToUnnamed(const std::string &name, int index) const
{
    if (m_def->containerCount() == 1) { return index; }
    Node childNode = childAt(name, index);
    return childIndex(childNode);
}

// =============================================================================
// (public)
int Node::convertChildIndexToNamed(std::string &name, int index) const
{
    if (m_def->containerCount() == 1) {
        name = m_def->container(0).containerDef()->name();
        return index;
    }
    Node childNode = childAt(index);
    name = childNode.def()->name();
    return childIndex(name, childNode);
}

// =============================================================================
// (protected)
void Node::initLeafList() const
{
    ASSERT(m_def);
    if (m_leafList.empty() && m_def && !m_nodeData.isNull()) {
        auto vList = m_def->valueList();
        for (const LeafDef* vi: vList) {
            m_leafList.push_back(Leaf(vi, m_nodeData, this));
        }
    }
}

// =============================================================================
// (protected)
void Node::updateUniqueValues(const Node &node)
{
    Model::Node::LeafIterator vIt = node.leafBegin();
    Model::Node::LeafIterator vItEnd = node.leafEnd();
    while (vIt != vItEnd) {
        if (vIt->settings().value(REQUIRED) > 0 &&
            vIt->settings().value(UNIQUE) > 0 &&
            vIt->hasDefaultValue()) {

            std::vector<std::string> valueList = QB::createParent()->children(node.def()->name())->leafSPtr(vIt->name())->toValueList<std::string>(node);
            std::string value = vIt->value<std::string>();
            if (count(valueList, value) == 1) {
                return;
            }

            std::string defaultValue = vIt->defaultValue<std::string>();
            value = defaultValue;
            int i = 1;
            while (contains(valueList, value)) {
                value = defaultValue + "_" + std::to_string(i++);
            }
            vIt->setValue(value);
        }

        vIt++;
    }
}

} // namespace Oak::Model

