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

#include "Node.h"


namespace Oak::Model {

class NodeIndex;
typedef std::unique_ptr<NodeIndex> NodeIndexUPtr;

// =============================================================================
// Class definition
// =============================================================================
class NodeIndex
{
public:
    NodeIndex();
    NodeIndex(int index);
    NodeIndex(const std::string &name, int index);
    NodeIndex(const NodeIndex &nodeIndex);
    ~NodeIndex();

    bool isNull() const;
    bool isNamed(bool recursive = false) const;
    bool isUnnamed(bool recursive = false) const;

    bool equal(const NodeIndex &nodeIndex) const;
    int depthWhereEqual(const NodeIndex &nodeIndex) const;

    const std::string &name() const;
    int index() const;

    // Returns how many levels down the NodeIndex points
    int depth() const;

    bool contains(const NodeIndex &nodeIndex) const;

    Node node(const Node &rootNode, int depth = -1) const;
    Node nodeParent(const Node &rootNode) const;

    bool hasChildNodeIndex() const;

    const NodeIndex &childNodeIndex(int depth = 1) const;
    NodeIndex &childNodeIndex(int depth = 1);
    const NodeIndex &lastNodeIndex() const;
    NodeIndex &lastNodeIndex();

    static NodeIndex &emptyNodeIndex();

    static NodeIndexUPtr create(const Node &_node, bool namedIndex = true);

    // Manual creation of NodeIndex
    void setChildNodeIndex(NodeIndex *nodeIndex);

    // ************* Convert index to grouped of seperate format *************
    int convertIndexToUnnamed(const Node &_node) const;
    int convertIndexToNamed(const Node &_node, std::string &name) const;

protected:
    std::string m_name;
    int m_index;

    NodeIndex *m_childIndex;

    static NodeIndex s_nodeIndex;
};

} // namespace Oak::Model

