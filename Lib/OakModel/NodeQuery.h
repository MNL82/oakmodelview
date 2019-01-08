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

class NodeQuery;
typedef std::unique_ptr<NodeQuery> NodeQueryUPtr;

// =============================================================================
// Class definition
// =============================================================================
class NodeQuery
{
public:
    NodeQuery();
    NodeQuery(const NodeQuery& copy);
    NodeQuery(NodeQuery&& move);

    virtual ~NodeQuery();

    int count(const Node &refNode);
    std::vector<Node> nodeList(const Node &refNode);

    bool hasChildQuery() const;
    const NodeQuery *childQuery() const;

    const NodeDef * nodeDef(const NodeDef *nDef) const;

    virtual bool canInsertNode(const Node &refNode, int &index) const;
    virtual Node insertNode(const Node &refNode, int index) const;

    virtual bool canCloneNode(const Node &refNode, int &index, const Node &cloneNode) const;
    virtual Node cloneNode(const Node &refNode, int &index, const Node &cloneNode) const;

    virtual bool canMoveNode(const Node &refNode, int &index, const Node &moveNode) const;
    virtual Node moveNode(const Node &refNode, int &index, const Node &moveNode) const;

    virtual bool canRemoveNode(const Node &refNode, int index) const;
    virtual bool removeNode(Node &refNode, int index);

protected:
    void addChildQuery(NodeQueryUPtr query);

    virtual Node first(const Node &refNode) const;
    virtual Node last(const Node &refNode) const;
    virtual Node next(const Node &refNode, const Node &cNode) const;
    virtual Node previous(const Node &refNode, const Node &cNode) const;

    virtual const NodeDef * _nodeDef(const NodeDef *nDef) const;

protected:
    NodeQueryUPtr m_childQueryUPtr = NodeQueryUPtr();


    // Iterator navigation implementation
public:
    class Iterator {

    public:
        Iterator(const NodeQuery & query, const Node *refNode = nullptr);

        virtual ~Iterator();

        bool isValid() const;
        bool next();
        bool previous();

        const Node &node() const;

        bool first(const Node &refNode);
        bool last(const Node &refNode);

    protected:
        const NodeQuery *m_query;
        const Node *m_refNode;
        Node m_currentNode;

        Iterator *m_childIterator;

        friend class NodeQuery;
    };
    typedef std::unique_ptr<Iterator> IteratorUPtr;

    IteratorUPtr iterator(const Node &refNode) const;
//    IteratorUPtr begin(const Node &refNode) const;
//    IteratorUPtr rBegin(const Node &refNode) const;


    friend class QueryBuilder;
};

} // namespace Oak::Model

