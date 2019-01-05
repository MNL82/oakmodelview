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

#include <memory>

#include "UnionRef.h"
#include "NodeData.h"


namespace Oak::Model {

class NodeDef;

class ContainerGroupDef;
typedef std::unique_ptr<ContainerGroupDef> ContainerGroupDefUPtr;

class NodeDef;
class ContainerDef;

// =============================================================================
// Class definition
// =============================================================================
class ContainerGroupDef
{
public:
    ContainerGroupDef(const NodeDef *hostDef);
    ContainerGroupDef(const ContainerGroupDef &copy);
    ContainerGroupDef(ContainerGroupDef &&move);

    virtual ~ContainerGroupDef() {}

    ContainerGroupDef& operator=(const ContainerGroupDef &copy);
    ContainerGroupDef& operator=(ContainerGroupDef&& move);

    virtual ContainerGroupDefUPtr copy() const;

    virtual const NodeDef* containerDef(const std::string &name, const UnionRef& variantId = UnionRef()) const;
    virtual const NodeDef* containerDef(const NodeData &_nodeData) const;

    virtual bool validate(const NodeData &_nodeData) const;

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

    virtual NodeData nextNode(const NodeData &_nodeData, const NodeData &refNode, const NodeDef** nodeDef = nullptr) const;
    virtual NodeData previousNode(const NodeData &_nodeData, const NodeData &refNode, const NodeDef** nodeDef = nullptr) const;
// *****************************************************************************


// *****************************************************************************
// Data Node Modufication
// *****************************************************************************
public:
    /// Se the function 'insertChildDataNode' bellow
    virtual bool canInsertNode(const NodeData &_nodeData, const UnionRef& name, int &index) const;

    /// Parameter 'node' must match this definition and will be the parent node of the new data Node.
    /// Parameter 'index' is the index of new new data node. The new node will be appended after the last sibling if
    ///  'index' is -1. 'index' in updated with the actual index.
    /// The return value is the inserted data node, or an empty Node if the operation failed
    virtual NodeData insertNode(const NodeData &_nodeData, const UnionRef& name, int &index) const;

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
    /// Parameter 'dataNode' must match this definition and will be the parent node of the new data Node
    /// Parameter 'index' is the index of new new data node. The new node will be appended after the last sibling if
    ///  'index' is -1. 'index' in updated with the actual index.
    /// The return value is the inserted data node, or an empty Node if the operation failed
    virtual NodeData moveNode(const NodeData &_nodeData, int &index, const NodeData &moveNode) const;

    /// Se the function 'removeChildDataNode' bellow
    virtual bool canRemoveNode(const NodeData &_nodeData, int index) const;

    /// Parameter 'dataNode' must match this definition and is the parent of the removed node.
    /// Parameter 'index' is the index of the data node that will be removed.
    /// Returns true if the data node has been removed
    virtual bool removeNode(const NodeData &_nodeData, int index) const;

// ****************************************************************************

    template<class... _Types> inline
    static ContainerGroupDefUPtr MakeUPtr(_Types&&... _Args)
    {
        return (ContainerGroupDefUPtr(new ContainerGroupDef(_STD forward<_Types>(_Args)...)));
    }

protected:
    void updateContainerList() const;

    const NodeDef* m_hostDef;
    mutable std::vector<const ContainerDef*> m_containerList;

    friend class NodeDefBuilder;
};

} // namespace Oak::Model

