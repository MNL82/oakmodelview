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

#include <limits>
#include <algorithm>
#include <NodeData.h>

#include "UnionRef.h"

#ifdef XML_BACKEND
#include "XMLListRef.h"
#endif // XML_BACKEND


namespace Oak::Model {

class NodeDef;

class ContainerDef;
typedef std::unique_ptr<ContainerDef> ContainerDefUPtr;
typedef std::unique_ptr<ContainerDef> ContainerDefUPtr;

class NodeDef;
typedef std::shared_ptr<NodeDef> NodeDefSPtr;
typedef std::weak_ptr<NodeDef> NodeDefWPtr;

// =============================================================================
// Class definition
// =============================================================================
class ContainerDef {
public:
    ContainerDef();
    ContainerDef(NodeDefSPtr nodeDef, int min = 0, int max = std::numeric_limits<int>::max());
    ContainerDef(const ContainerDef &copy);
    ContainerDef(ContainerDef &&move);

    virtual ~ContainerDef() {}

    ContainerDef& operator=(const ContainerDef &copy);
    ContainerDef& operator=(ContainerDef&& move);

    virtual ContainerDefUPtr copy() const;

    bool isNull() const;

    virtual const NodeDef* containerDef() const;
    virtual const NodeDef* containerDef(const UnionRef& variantId) const;
    virtual const NodeDef* containerDef(const NodeData &_nodeData) const;

    virtual bool validate(const NodeData &_nodeData) const;

    int minCount() const;
    int maxCount() const;

// *****************************************************************************
// Data Node Access
// *****************************************************************************
public:
    // Return -1 is dataNode failed to validate
    virtual int nodeCount(const NodeData &_nodeData) const;
    virtual int nodeIndex(const NodeData &_nodeData, const NodeData &refNode) const;

    virtual NodeData node(const NodeData &_nodeData, int index, const NodeDef** nodeDef = nullptr) const;

    virtual NodeData firstNode(const NodeData &_nodeData, const NodeDef** nodeDef = nullptr) const;
    virtual NodeData lastNode(const NodeData &_nodeData, const NodeDef** nodeDef = nullptr) const;

    virtual NodeData nextNode(const NodeData &refNodeData, const NodeDef** nodeDef = nullptr) const;
    virtual NodeData previousNode(const NodeData &refNodeData, const NodeDef** nodeDef = nullptr) const;
// *****************************************************************************


// *****************************************************************************
// Host Access
// *****************************************************************************
public:
    virtual const NodeDef* hostDef() const;
    virtual NodeData hostNode(const NodeData &refNodeData) const;
// *****************************************************************************


// *****************************************************************************
// Data Node Modufication
// *****************************************************************************
public:
    /// Se the function 'insertChildDataNode' bellow
    virtual bool canInsertNode(const NodeData &_nodeData, int &index) const;

    /// Parameter 'node' must match this definition and will be the parent node of the new data Node.
    /// Parameter 'index' is the index of new new data node. The new node will be appended after the last sibling if
    ///  'index' is -1. 'index' in updated with the actual index.
    /// The return value is the inserted data node, or an empty Node if the operation failed
    virtual NodeData insertNode(const NodeData &_nodeData, int &index) const;

    /// Se the function 'cloneChildDataNode' bellow
    virtual bool canCloneNode(const NodeData &_nodeData, int &index, const NodeData &cloneNode) const;

    /// Parameter 'cloneNode' must match one of the child node definitions. A deep copy will be performed.
    /// Parameter 'node' must match this definition and will be the parent node of the cloned data node.
    /// Parameter 'index' is the index of new new data node. The new node will be appended after the last sibling if
    ///  'index' is -1. 'index' in updated with the actual index.
    /// The return value is the inserted data node, or an empty Node if the operation failed
    virtual NodeData cloneNode(const NodeData &_nodeData, int &index, const NodeData &cloneNode) const;

    /// Se the function 'moveChildDataNode' bellow
    virtual bool canMoveNode(const NodeData &_nodeData, int &index, const NodeData &moveNode) const;

    /// Parameter 'moveDataNode' must match one of the child node definitions.
    /// Parameter 'node' must match this definition and will be the parent node of the new data Node
    /// Parameter 'index' is the index of new new data node. The new node will be appended after the last sibling if
    ///  'index' is -1. 'index' in updated with the actual index.
    /// The return value is the inserted data node, or an empty Node if the operation failed
    virtual NodeData moveNode(const NodeData &_nodeData, int &index, const NodeData &moveNode) const;

    /// Se the function 'removeChildDataNode' bellow
    virtual bool canRemoveNode(const NodeData &_nodeData, int index) const;

    /// Parameter 'node' must match this definition and is the parent of the removed node.
    /// Parameter 'index' is the index of the data node that will be removed.
    /// Returns true if the data node has been removed
    virtual bool removeNode(const NodeData &_nodeData, int index) const;

// ****************************************************************************

    template<class... _Types> inline
    static ContainerDefUPtr MakeUPtr(_Types&&... _Args)
    {
        return (ContainerDefUPtr(new ContainerDef(_STD forward<_Types>(_Args)...)));
    }

    static const ContainerDef &emptyChildNodeDef();

protected:
    bool checkUniqueOptionValues(const NodeData &_nodeData) const;

protected:
    NodeDefWPtr m_hostDef;
    NodeDefSPtr m_containerDef;
    int m_minCount;
    int m_maxCount;

#ifdef XML_BACKEND
    XML::ListRef m_elementListRef;
#endif // XML_BACKEND

    friend class ContainerDefBuilder;
    friend class NodeDefBuilder;
};

typedef ContainerDef ContainerDef;

} // namespace Oak::Model

