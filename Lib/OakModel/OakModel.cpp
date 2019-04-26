/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "OakModel.h"

#include <list>
#include <QDebug>

#include "../ServiceFunctions/Trace.h"
#include "NodeServiceFunctions.h"
#include "OptionsObserver.h"


namespace Oak::Model {

// =============================================================================
// (public)
OakModel::OakModel()
{
    //qDebug() << "OakModel()";
}

// =============================================================================
// (public)
OakModel::~OakModel()
{
    //qDebug() << "~OakModel()";
    notifier_destroyed.trigger();
}

// =============================================================================
// (public)
bool OakModel::isNull() const
{
    return isDefNull() || isNodeNull();
}

// =============================================================================
// (public)
bool OakModel::isDefNull() const
{
    return m_rootNode.isDefNull();
}

// =============================================================================
// (public)
bool OakModel::isNodeNull() const
{
    return m_rootNode.isNodeNull();
}

// =============================================================================
// (public)
bool OakModel::createNewRootDocument(NodeData::Type backendType, bool setAsCurrent)
{
    if (m_rootNode.isDefNull()) {
        return false;
    }

    if (backendType == NodeData::Type::XML) {
        m_xmlDoc.clear();
        NodeData documentNode(m_xmlDoc.appendChild(m_rootNode.def()->tagName()));
        m_rootNode.def()->onNodeInserted(documentNode);
        setRootNodeXML(documentNode, setAsCurrent);
        return true;
    }

    ASSERT(false);
    return false;
}

// =============================================================================
// (public)
void OakModel::clearRoot()
{
    setRootNode(NodeData());
}

// =============================================================================
// (public)
const Node &OakModel::rootNode() const
{
    return m_rootNode;
}

// =============================================================================
// (public)
const std::string &OakModel::rootDefName() const
{
    return m_rootNode.def()->name();
}

// =============================================================================
// (public)
const NodeDef* OakModel::rootNodeDef() const
{
    return m_rootNode.def();
}

// =============================================================================
// (public)
void OakModel::setRootNodeDef(NodeDefSPtr def)
{
    m_def = def; // Saved only to keep the definition alive (Smart Pointer)
    setRootNodeDef(def.get());
}

// =============================================================================
// (public)
void OakModel::setRootNodeDef(const NodeDef *def)
{
    if (m_rootNode.def() != def) {
        NodeData currentNode = m_currentNode.nodeData();
        // Clear the current node before it becomes invalid
        setCurrentNode(Node());

        clearObservers();

        // Update the root definition and trigger event
        m_rootNode = Node(def, m_rootNode.nodeData(), this);

        createObservers();

        notifier_rootNodeDefChanged.trigger();

        // Set the current node to the same node as before
        // Otherwise sent the root node
        const NodeDef * currentNodeDef = findNodeDef(currentNode);
        if (currentNodeDef) {
            setCurrentNode(Node(currentNodeDef, currentNode, this));
        } else {
            setCurrentNode(rootNode());
        }
    }
}

// =============================================================================
// (public)
void OakModel::setRootNode(const NodeData &nodeData)
{
    if (m_rootNode.nodeData() != nodeData) {
        setCurrentNode(Node());
        m_rootNode = Node(m_rootNode.def(), nodeData, this);

        notifier_rootNodeDataChanged.trigger();
        setCurrentNode(rootNode());
    }
}

// =============================================================================
// (public)
void OakModel::setRootNode(const Node& node)
{
    setRootNodeDef(node.def());
    setRootNode(node.nodeData());
}

// =============================================================================
// (public)
const Node& OakModel::currentNode() const
{
    return m_currentNode;
}

// =============================================================================
// (public)
const NodeIndex &OakModel::currentNodeIndex() const
{
    if (m_currentNodeIndex) {
        return *m_currentNodeIndex.get();
    } else {
        return NodeIndex::emptyNodeIndex();
    }
}

// =============================================================================
// (public)
void OakModel::setCurrentNode(const Node &node, bool forceUpdate) const
{
    if (m_currentNode != node || forceUpdate) {
        m_currentNode = node;
        m_currentNodeIndex = NodeIndex::create(m_currentNode);
        notifier_currentNodeChanged.trigger();
    }
}

#ifdef XML_BACKEND
// =============================================================================
// (public)
void OakModel::setRootNodeXML(const NodeData &rootNodeData, bool setAsCurrent)
{
    m_rootNodeXML = rootNodeData;

    if (setAsCurrent) { setRootNode(m_rootNodeXML); }
}

// =============================================================================
// (public)
bool OakModel::loadRootNodeXML(const std::string& filePath, bool setAsCurrent)
{
    if (!m_xmlDoc.load(filePath)) { return false; }

    m_xmlDocFilePath = filePath;

    setRootNodeXML(m_xmlDoc.documentElement(), setAsCurrent);

    return true;
}

// =============================================================================
// (public)
bool OakModel::saveRootNodeXML(const std::string& filePath)
{
    if (m_xmlDoc.isNull()) {
        // Implement this
        return false;
    }
    if (filePath.empty()) {
        if (m_xmlDocFilePath.empty()) { return false; }
        return m_xmlDoc.save(m_xmlDocFilePath);
    } else {
        if (m_xmlDoc.save(filePath)) {
            m_xmlDocFilePath = filePath;
            return true;
        }
        return false;
    }
}

#endif // XML_BACKEND

// =============================================================================
// (public)
const NodeDef* OakModel::findNodeDef(const NodeData &nodeData) const
{
    if (m_rootNode.isDefNull() || nodeData.isNull()) { return nullptr; }

    // Check if the root definition match
    const NodeDef* def = m_rootNode.def()->baseRoot()->validVariant(nodeData, false, true);
    if (def) { return def; }

    std::list<const NodeDef*> defList;
    std::vector<const NodeDef*> ignoreList;
    defList.push_back(m_rootNode.def());

    // Recursive check if any child definition match the node
    const NodeDef * currentDef;
    while (!defList.empty()) {
        currentDef = defList.front();
        defList.remove(currentDef);

        // Check if on of the child definitions match
        def = currentDef->containerGroup().containerDef(nodeData);
        if (def) { return def; }
        ignoreList.push_back(currentDef);

        // Add child definitions to the list
        auto cList = currentDef->containerList();
        for(const ContainerDef* c: cList)
        {
            // Only add the definition if it is not in the ignore list
            //  to avoid checking an definition twice and enter an infinete loop
            if (std::find(ignoreList.begin(), ignoreList.end(), c->containerDef()) == ignoreList.end()) {
                defList.push_back(c->containerDef());
            }
        }
    }
    return nullptr;
}

// =============================================================================
// (public)
Node OakModel::nodeFromDataPtr(void *dPtr) const
{
    if (dPtr == nullptr) { return Node(); }

    Oak::Model::NodeData node(dPtr, m_rootNode.nodeData().type());
    const Oak::Model::NodeDef *nDef = findNodeDef(node);
    return Oak::Model::Node(nDef, node, this);
}

// =============================================================================
// (public)
NodeIndexUPtr OakModel::convertNodeIndexToNamed(const NodeIndex &nodeIndex) const
{
    ASSERT(!m_rootNode.isNull());
    ASSERT(!nodeIndex.isNull());

    NodeIndex * newRootNodeIndex = nullptr;
    const NodeIndex * sNodeIndex = &nodeIndex;
    NodeIndex *tNodeIndex;
    Node node = m_rootNode;

    while (true) {
        if (sNodeIndex->isNamed(true)) { // The all of the remaining node index is already named
            tNodeIndex = new NodeIndex(*sNodeIndex); // Copy the node index
            break;
        }
        if (sNodeIndex->isNamed()) {
            node = sNodeIndex->node(node);
            tNodeIndex = new NodeIndex(sNodeIndex->name(), sNodeIndex->index());
        } else {
            // Node index needs to be converted from unnamed to named
            int index = sNodeIndex->index();
            Node childNode = sNodeIndex->node(node, 1);
            std::string name = childNode.def()->name();
            if (node.def()->containerCount() > 1) {
                // Index needs to be updated if more than one container
                index = node.childIndex(name, childNode);
            }
            node = childNode;
            tNodeIndex = new NodeIndex(name, index);
        }

        if (newRootNodeIndex == nullptr) {
            newRootNodeIndex = tNodeIndex;
        } else {
            newRootNodeIndex->lastNodeIndex().setChildNodeIndex(tNodeIndex);
        }

        if (!sNodeIndex->hasChildNodeIndex()) {
            // Conversion is complete
            break;
        }
        sNodeIndex = &sNodeIndex->childNodeIndex();
    }

    return NodeIndexUPtr(newRootNodeIndex);
}

// =============================================================================
// (public)
NodeIndexUPtr OakModel::convertNodeIndexToUnnamed(const NodeIndex &nodeIndex) const
{
    ASSERT(!m_rootNode.isNull());
    ASSERT(!nodeIndex.isNull());

    NodeIndex * newRootNodeIndex = nullptr;
    const NodeIndex * sNodeIndex = &nodeIndex;
    NodeIndex *tNodeIndex;
    Node node = m_rootNode;

    while (true) {
        if (sNodeIndex->isUnnamed(true)) { // The all of the remaining node index is already unnamed
            tNodeIndex = new NodeIndex(*sNodeIndex); // Copy the node index
            break;
        }
        if (sNodeIndex->isUnnamed()) {
            node = sNodeIndex->node(node);
            tNodeIndex = new NodeIndex(sNodeIndex->index());
        } else {
            // Node index needs to be converted from named to unnamed
            int index = sNodeIndex->index();
            Node childNode = sNodeIndex->node(node, 1);
            if (node.def()->containerCount() > 1) {
                // Index needs to be updated if more than one container
                index = node.childIndex(childNode);
            }
            node = childNode;
            tNodeIndex = new NodeIndex(index);
        }

        if (newRootNodeIndex == nullptr) {
            newRootNodeIndex = tNodeIndex;
        } else {
            newRootNodeIndex->lastNodeIndex().setChildNodeIndex(tNodeIndex);
        }

        if (!sNodeIndex->hasChildNodeIndex()) {
            // Conversion is complete
            break;
        }
        sNodeIndex = &sNodeIndex->childNodeIndex();
    }

    return NodeIndexUPtr(newRootNodeIndex);
}

// =============================================================================
// (protected)
void OakModel::onNodeRemoveBefore(const NodeIndex &nodeIndex) const
{
    notifier_nodeRemoveBefore.trigger(nodeIndex);
}

// =============================================================================
// (protected)
void OakModel::onNodeInserteBefore(const NodeIndex &nodeIndex) const
{
    notifier_nodeInserteBefore.trigger(nodeIndex);
}

// =============================================================================
// (protected)
void OakModel::onNodeInserteAfter(const NodeIndex &nodeIndex) const
{
    notifier_nodeInserteAfter.trigger(nodeIndex);
}

// =============================================================================
// (protected)
void OakModel::onNodeMoveBefore(const NodeIndex &sourceNodeIndex, const NodeIndex &targetNodeIndex) const
{
    notifier_nodeMoveBefore.trigger(sourceNodeIndex, targetNodeIndex);
}

// =============================================================================
// (protected)
void OakModel::onNodeMoveAfter(const NodeIndex &sourceNodeIndex, const NodeIndex &targetNodeIndex) const
{
    notifier_nodeMoveAfter.trigger(sourceNodeIndex, targetNodeIndex);

    // Check if the current node have moved and update it if so
    NodeIndexUPtr currentNodeIndex = NodeIndex::create(m_currentNode);
    if (!currentNodeIndex) {
        setCurrentNode(targetNodeIndex.node(m_rootNode));
    } else if (!m_currentNodeIndex->equal(*currentNodeIndex.get())) {
        setCurrentNode(m_currentNode, true);
    }
}

// =============================================================================
// (protected)
void OakModel::onNodeCloneBefore(const NodeIndex &sourceNodeIndex, const NodeIndex &targetNodeIndex) const
{
    notifier_nodeCloneBefore.trigger(sourceNodeIndex, targetNodeIndex);
}

// =============================================================================
// (protected)
void OakModel::onNodeCloneAfter(const NodeIndex &sourceNodeIndex, const NodeIndex &targetNodeIndex) const
{
    notifier_nodeCloneAfter.trigger(sourceNodeIndex, targetNodeIndex);

    // Change the current node to the clone if it was the one cloned
    if (m_currentNodeIndex->equal(sourceNodeIndex)) {
        setCurrentNode(targetNodeIndex.node(m_rootNode));
    }
}

// =============================================================================
// (protected)
void OakModel::onNodeRemoveAfter(const NodeIndex &nodeIndex) const
{
    // Notify the view
    notifier_nodeRemoveAfter.trigger(nodeIndex);

    // Check if the current node is removed and update it if so
    if (m_currentNode.isNull()) { return; }

    if (m_currentNodeIndex->contains(nodeIndex)) {
        // The current node is no longer valid
        if (m_currentNodeIndex->depth() == nodeIndex.depth()) {
            // The deleted node is the current node
            Node parentNode = nodeIndex.nodeParent(m_rootNode);
            const NodeIndex &lastNodeIndex = nodeIndex.lastNodeIndex();
            int count = parentNode.childCount(lastNodeIndex.name());
            if (count > lastNodeIndex.index()) {
                // Set the next sibling as the current node
                setCurrentNode(parentNode.childAt(lastNodeIndex.name(), lastNodeIndex.index()));
            } else if (count > 0) {
                // Set the previous sibling as the current node
                setCurrentNode(parentNode.childAt(lastNodeIndex.name(), count-1));
            } else {
                // Set the parent node as the current node
                setCurrentNode(parentNode);
            }
        } else {
            // The parent node is removed so clear the current node
            setCurrentNode(Node());
        }

        return;
    }
}

// =============================================================================
// (protected)
void OakModel::onLeafChangeBefore(const NodeIndex &nodeIndex, const std::string &valueName) const
{
    notifier_leafChangeBefore.trigger(nodeIndex, valueName);
}

// =============================================================================
// (protected)
void OakModel::onLeafChangeAfter(const NodeIndex &nodeIndex, const std::string &valueName) const
{
    notifier_leafChangeAfter.trigger(nodeIndex, valueName);
}

// =============================================================================
// (protected)
void OakModel::onVariantLeafChangeAfter(const NodeIndex &nodeIndex) const
{
    Node node = nodeIndex.node(m_rootNode);
    const NodeDef* def = findNodeDef(node.nodeData());
    // ASSERTION can be caused by the variantValue being changed to a value that do not corespond to a node definition variant
    // Make sure the variantLeaf optionsOnly setting is set to true
    ASSERT(def);
    Node newNode(def, node.nodeData(), this);

    notifier_variantLeafChangeAfter.trigger(nodeIndex);
    if (node == m_currentNode) {
        setCurrentNode(newNode);
    }
}

// =============================================================================
// (protected)
void OakModel::onKeyLeafChangeAfter(const NodeIndex &nodeIndex) const
{
    notifier_keyLeafChangeAfter.trigger(nodeIndex);
}

// =============================================================================
// (public)
void OakModel::createObservers()
{
    // Insert code to create new observers here (Do not connect)

    /******************* Create Option Observers ******************/
    // Find all option queries in the node definition tree
    std::vector<NodeLeafDefPair> queryList;
    std::vector<NodeLeafDefPair> queryExcludedList;
    findOptionQueries(m_rootNode.def(), queryList, queryExcludedList, true);
    for (const auto & query: queryList)
    {
        m_observerList.push_back(std::make_unique<OptionsObserver>(this, query.first, query.second));
    }
    //

    for (ObserverInterfaceUPtr &observer: m_observerList)
    {
        observer->connect();
    }
}

// =============================================================================
// (public)
void OakModel::clearObservers()
{
    for (ObserverInterfaceUPtr &observer: m_observerList)
    {
        observer->disconnect();
    }
    m_observerList.clear();
}

} // namespace Oak::Model

