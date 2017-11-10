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
#include <Node.h>

#include "VariantCRef.h"

#ifdef XML_BACKEND
#include "XMLListRef.h"
#endif // XML_BACKEND

namespace Oak {
namespace Model {

class NodeDefinition;

class ContainerDefinition;
typedef std::unique_ptr<ContainerDefinition> ContainerDefinitionUPtr;
typedef std::unique_ptr<ContainerDefinition> ContainerDefUPtr;

class NodeDefinition;
typedef std::shared_ptr<NodeDefinition> NodeDefinitionSPtr;
typedef std::shared_ptr<NodeDefinition> NodeDefSPtr;
typedef std::weak_ptr<NodeDefinition> NodeDefinitionWPtr;
typedef std::weak_ptr<NodeDefinition> NodeDefWPtr;

// =============================================================================
// Class definition
// =============================================================================
class ContainerDefinition {
public:
    ContainerDefinition();
    ContainerDefinition(NodeDefinitionSPtr containerDefinition, int min = 0, int max = std::numeric_limits<int>::max());
    ContainerDefinition(const ContainerDefinition &copy);
    ContainerDefinition(ContainerDefinition &&move);

    virtual ~ContainerDefinition() {}

    ContainerDefinition& operator=(const ContainerDefinition &copy);
    ContainerDefinition& operator=(ContainerDefinition&& move);

    virtual ContainerDefinitionUPtr copy() const;

    bool isNull() const;

    virtual const NodeDefinition* containerDefinition() const;
    virtual const NodeDefinition* containerDefinition(VariantCRef derivedId) const;
    virtual const NodeDefinition* containerDefinition(Node _node) const;

    virtual bool validate(Node _node) const;

    int minCount() const;
    int maxCount() const;

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

    virtual Node nextNode(Node refNode, const NodeDefinition** nodeDefinition = nullptr) const;
    virtual Node previousNode(Node refNode, const NodeDefinition** nodeDefinition = nullptr) const;
// *****************************************************************************


// *****************************************************************************
// Host Access
// *****************************************************************************
public:
    virtual const NodeDefinition* hostDefinition() const;
    virtual Node hostNode(Node refdNode) const;
// *****************************************************************************


// *****************************************************************************
// Data Node Modufication
// *****************************************************************************
public:
    /// Se the function 'insertChildDataNode' bellow
    virtual bool canInsertNode(Node _node, int &index) const;

    /// Parameter 'node' must match this definition and will be the parent node of the new data Node.
    /// Parameter 'index' is the index of new new data node. The new node will be appended after the last sibling if
    ///  'index' is -1. 'index' in updated with the actual index.
    /// The return value is the inserted data node, or an empty Node if the operation failed
    virtual Node insertNode(Node _node, int &index) const;

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
    /// Parameter 'node' must match this definition and will be the parent node of the new data Node
    /// Parameter 'index' is the index of new new data node. The new node will be appended after the last sibling if
    ///  'index' is -1. 'index' in updated with the actual index.
    /// The return value is the inserted data node, or an empty Node if the operation failed
    virtual Node moveNode(Node _node, int &index, Node moveNode) const;

    /// Se the function 'removeChildDataNode' bellow
    virtual bool canRemoveNode(Node _node, int index) const;

    /// Parameter 'node' must match this definition and is the parent of the removed node.
    /// Parameter 'index' is the index of the data node that will be removed.
    /// Returns true if the data node has been removed
    virtual bool removeNode(Node _node, int index) const;

// ****************************************************************************

    template<class... _Types> inline
    static typename ContainerDefinitionUPtr MakeUPtr(_Types&&... _Args)
    {
        return (ContainerDefinitionUPtr(new ContainerDefinition(_STD forward<_Types>(_Args)...)));
    }

    static const ContainerDefinition &emptyChildNodeDefinition();

protected:
    bool checkUniqueOptionValues(Node _node) const;

protected:
    NodeDefinitionWPtr m_hostDefinition;
    NodeDefinitionSPtr m_containerDefinition;
    int m_minCount;
    int m_maxCount;

#ifdef XML_BACKEND
    XML::ListRef m_elementListRef;
#endif // XML_BACKEND

    friend class ContainerDefinitionBuilder;
    friend class NodeDefinitionBuilder;
};

typedef ContainerDefinition ContainerDef;

} // namespace Model
} // namespace Oak
