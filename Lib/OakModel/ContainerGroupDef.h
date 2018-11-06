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
#include "Node.h"

namespace Oak {
namespace Model {

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
    virtual const NodeDef* containerDef(Node _node) const;

    virtual bool validate(Node _node) const;

// *****************************************************************************
// Data Node Access
// *****************************************************************************
public:

    // Return -1 is dataNode failed to validate
    virtual int nodeCount(Node _node) const;
    virtual int nodeIndex(Node _node, Node refNode) const;

    virtual Node node(Node _node, int index, const NodeDef** nodeDef = nullptr) const;

    virtual Node firstNode(Node _node, const NodeDef** nodeDef = nullptr) const;
    virtual Node lastNode(Node _node, const NodeDef** nodeDef = nullptr) const;

    virtual Node nextNode(Node _node, Node refNode, const NodeDef** nodeDef = nullptr) const;
    virtual Node previousNode(Node _node, Node refNode, const NodeDef** nodeDef = nullptr) const;
// *****************************************************************************


// *****************************************************************************
// Data Node Modufication
// *****************************************************************************
public:
    /// Se the function 'insertChildDataNode' bellow
    virtual bool canInsertNode(Node _node, const UnionRef& name, int &index) const;

    /// Parameter 'node' must match this definition and will be the parent node of the new data Node.
    /// Parameter 'index' is the index of new new data node. The new node will be appended after the last sibling if
    ///  'index' is -1. 'index' in updated with the actual index.
    /// The return value is the inserted data node, or an empty Node if the operation failed
    virtual Node insertNode(Node _node, const UnionRef& name, int &index) const;

    /// Se the function 'cloneChildDataNode' bellow
    virtual bool canCloneNode(Node _node, int &index, Node cloneNode) const;

    /// Parameter 'cloneNode' must match one of the child node definitions. A deep copy will be performed.
    /// Parameter 'node' must match this definition and will be the parent node of the cloned data node.
    /// Parameter 'index' is the index of new new data node. The new node will be appended after the last sibling if
    ///  'index' is -1. 'index' in updated with the actual index.
    /// The return value is the inserted data node, or an empty Node if the operation failed
    virtual Node cloneNode(Node _node, int &index, Node cloneNode) const;

    /// Se the function 'moveChildDataNode' bellow
    virtual bool canMoveNode(Node _node, int &index, Node moveNode) const;

    /// Parameter 'moveDataNode' must match one of the child node definitions.
    /// Parameter 'dataNode' must match this definition and will be the parent node of the new data Node
    /// Parameter 'index' is the index of new new data node. The new node will be appended after the last sibling if
    ///  'index' is -1. 'index' in updated with the actual index.
    /// The return value is the inserted data node, or an empty Node if the operation failed
    virtual Node moveNode(Node _node, int &index, Node moveNode) const;

    /// Se the function 'removeChildDataNode' bellow
    virtual bool canRemoveNode(Node _node, int index) const;

    /// Parameter 'dataNode' must match this definition and is the parent of the removed node.
    /// Parameter 'index' is the index of the data node that will be removed.
    /// Returns true if the data node has been removed
    virtual bool removeNode(Node _node, int index) const;

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

} // namespace Model
} // namespace Oak
