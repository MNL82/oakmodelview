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

#include "NodeQuery.h"


namespace Oak::Model {

// =============================================================================
// Class definition
// =============================================================================
class NodeQueryChildren : public NodeQuery
{
public:
    NodeQueryChildren(const std::string &nodeName);
    NodeQueryChildren(const NodeQueryChildren& copy);
    NodeQueryChildren(NodeQueryChildren&& move);

    virtual bool canInsertNode(const Node &refNode, int &index) const override;
    virtual Node insertNode(const Node &refNode, int index) const override;

    virtual bool canCloneNode(const Node &refNode, int &index, const Node &cloneNode) const override;
    virtual Node cloneNode(const Node &refNode, int &index, const Node &cloneNode) const override;

    virtual bool canMoveNode(const Node &refNode, int &index, const Node &moveNode) const override;
    virtual Node moveNode(const Node &refNode, int &index, const Node &moveNode) const override;

    virtual bool canRemoveNode(const Node &refNode, int index) const override;
    virtual bool removeNode(Node &refNode, int index) override;

protected:
    virtual Node first(const Node &refNode) const override;
    virtual Node last(const Node &refNode) const override;
    virtual Node next(const Node &refNode, const Node &cNode) const override;
    virtual Node previous(const Node &refNode, const Node &cNode) const override;

    virtual const NodeDef *_nodeDef(const NodeDef *nDef) const override;

protected:
    std::string m_nodeName;
};

} // namespace Oak::Model

