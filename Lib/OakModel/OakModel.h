/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#pragma once

#include "NodeDef.h"
#include "Node.h"
#include "CallbackFunctions.h"

#ifdef XML_BACKEND
#include "XMLDocument.h"
#endif // XML_BACKEND


namespace Oak::Model {

class ObserverInterface;
typedef std::unique_ptr<ObserverInterface> ObserverInterfaceUPtr;

// =============================================================================
// Class definition
// =============================================================================
class OakModel
{
public:
    OakModel();
    virtual ~OakModel();

    bool isNull() const;
    bool isDefNull() const;
    bool isNodeNull() const;

    bool createNewRootDocument(NodeData::Type backendType, bool setAsCurrent = true);
    void clearRoot();
    const Node& rootNode() const;

    const std::string &rootDefName() const;

    const NodeDef* rootNodeDef() const;
    void setRootNodeDef(NodeDefSPtr def);
    void setRootNodeDef(const NodeDef* def);

    void setRootNode(const NodeData &nodeData);

    void setRootNode(const Node& node);

    const Node& currentNode() const;
    const NodeIndex& currentNodeIndex() const;
    void setCurrentNode(const Node &node, bool forceUpdate = false) const;

#ifdef XML_BACKEND
    const std::string& docFilePathXML() { return m_xmlDocFilePath; }
    void setDocFilePathXML(const std::string xmlDocFilePath) { m_xmlDocFilePath = xmlDocFilePath; }
    void setRootNodeXML(const NodeData &rootNodeData, bool setAsCurrent = true);
    bool loadRootNodeXML(const std::string& filePath, bool setAsCurrent = true);
    bool saveRootNodeXML(const std::string& filePath = "");
#endif // XML_BACKEND

    // Starting with the root NodeDef and look through all NodeDefs recursive
    //  untill a match is found
    const NodeDef *findNodeDef(const NodeData &nodeData) const;
    Node nodeFromDataPtr(void * dPtr) const;

    NodeIndexUPtr convertNodeIndexToNamed(const NodeIndex &nodeIndex) const;
    NodeIndexUPtr convertNodeIndexToUnnamed(const NodeIndex &nodeIndex) const;

protected:
    void onNodeInserteBefore(const NodeIndex& nodeIndex) const;
    void onNodeInserteAfter(const NodeIndex& nodeIndex) const;
    void onNodeMoveBefore(const NodeIndex& sourceNodeIndex, const NodeIndex& targetNodeIndex) const;
    void onNodeMoveAfter(const NodeIndex& sourceNodeIndex, const NodeIndex& targetNodeIndex) const;
    void onNodeCloneBefore(const NodeIndex& sourceNodeIndex, const NodeIndex& targetNodeIndex) const;
    void onNodeCloneAfter(const NodeIndex& sourceNodeIndex, const NodeIndex& targetNodeIndex) const;

    void onNodeRemoveAfter(const NodeIndex& nodeIndex) const;
    void onNodeRemoveBefore(const NodeIndex& nodeIndex) const;

    void onLeafChangeBefore(const NodeIndex& nodeIndex, const std::string &valueName) const;
    void onLeafChangeAfter(const NodeIndex& nodeIndex, const std::string &valueName) const;
    void onVariantLeafChangeAfter(const NodeIndex& nodeIndex) const;
    void onKeyLeafChangeAfter(const NodeIndex& nodeIndex) const;

    void createObservers();
    void clearObservers();

public:
    Callback notifier_currentNodeChanged;
    Callback notifier_rootNodeDataChanged;
    Callback notifier_rootNodeDefChanged;

    Callback notifier_destroyed;

    Callback_NodeIndex notifier_nodeInserteBefore;
    Callback_NodeIndex notifier_nodeInserteAfter;
    Callback_NodeIndexNodeIndex notifier_nodeMoveBefore;
    Callback_NodeIndexNodeIndex notifier_nodeMoveAfter;
    Callback_NodeIndexNodeIndex notifier_nodeCloneBefore;
    Callback_NodeIndexNodeIndex notifier_nodeCloneAfter;

    Callback_NodeIndex notifier_nodeRemoveBefore;
    Callback_NodeIndex notifier_nodeRemoveAfter;

    Callback_NodeIndexString notifier_leafChangeBefore;
    Callback_NodeIndexString notifier_leafChangeAfter;
    Callback_NodeIndex notifier_variantLeafChangeAfter;
    Callback_NodeIndex notifier_keyLeafChangeAfter;

protected:
    Node m_rootNode;
    mutable Node m_currentNode;
    mutable NodeIndexUPtr m_currentNodeIndex;

    std::vector<ObserverInterfaceUPtr> m_observerList;

    // Used only to keep the definition alive (Smart Pointer)
    NodeDefSPtr m_def;

#ifdef XML_BACKEND
    NodeData m_rootNodeXML;
    std::string m_xmlDocFilePath;
    XML::Document m_xmlDoc;
#endif // XML_BACKEND

    friend class Node;
    friend class Leaf;
};

} // namespace Oak::Model

