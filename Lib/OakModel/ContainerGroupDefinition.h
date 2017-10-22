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

#include "VariantCRef.h"
#include "Node.h"

namespace Oak {
namespace Model {

class NodeDefinition;

class ContainerGroupDefinition;
typedef std::unique_ptr<ContainerGroupDefinition> ContainerGroupDefinitionUPtr;

class NodeDefinition;
class ContainerDefinition;

// =============================================================================
// Class definition
// =============================================================================
class ContainerGroupDefinition
{
public:
    ContainerGroupDefinition(const NodeDefinition *hostDefinition);
    ContainerGroupDefinition(const ContainerGroupDefinition &copy);
    ContainerGroupDefinition(ContainerGroupDefinition &&move);

    virtual ~ContainerGroupDefinition() {}

    ContainerGroupDefinition& operator=(const ContainerGroupDefinition &copy);
    ContainerGroupDefinition& operator=(ContainerGroupDefinition&& move);

    virtual ContainerGroupDefinitionUPtr copy() const;

    virtual const NodeDefinition* containerDefinition(const std::string &name, VariantCRef derivedId = VariantCRef()) const;
    virtual const NodeDefinition* containerDefinition(Node _node) const;

    virtual bool validate(Node _node) const;

// *****************************************************************************
// Data Node Access
// *****************************************************************************
public:

    // Return -1 is dataNode failed to validate
    virtual int nodeCount(Node _node) const;
    virtual int nodeIndex(Node _node, Node refNode) const;

    virtual Node node(Node _node, int index, const NodeDefinition** nodeDefinition = nullptr) const;

    virtual Node firstNode(Node _node, const NodeDefinition** nodeDefinition = nullptr) const;
    virtual Node lastNode(Node _node, const NodeDefinition** nodeDefinition = nullptr) const;

    virtual Node nextNode(Node _node, Node refNode, const NodeDefinition** nodeDefinition = nullptr) const;
    virtual Node previousNode(Node _node, Node refNode, const NodeDefinition** nodeDefinition = nullptr) const;
// *****************************************************************************


// *****************************************************************************
// Data Node Modufication
// *****************************************************************************
public:
    /// Se the function 'insertChildDataNode' bellow
    virtual bool canInsertNode(Node _node, VariantCRef name, int &index) const;

    /// Parameter 'node' must match this definition and will be the parent node of the new data Node.
    /// Parameter 'index' is the index of new new data node. The new node will be appended after the last sibling if
    ///  'index' is -1. 'index' in updated with the actual index.
    /// The return value is the inserted data node, or an empty Node if the operation failed
    virtual Node insertNode(Node _node, VariantCRef name, int &index) const;

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
    static typename ContainerGroupDefinitionUPtr MakeUPtr(_Types&&... _Args)
    {
        return (ContainerGroupDefinitionUPtr(new ContainerGroupDefinition(_STD forward<_Types>(_Args)...)));
    }

protected:
    void updateContainerList() const;

    const NodeDefinition* m_hostDefinition;
    mutable std::vector<const ContainerDefinition*> m_containerList;

    friend class NodeDefinitionBuilder;
};

} // namespace Model
} // namespace Oak
