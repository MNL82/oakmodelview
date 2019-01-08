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

#include "NodeData.h"
#include "NodeDef.h"
#include "Leaf.h"


namespace Oak::Model {

class OakModel;

// =============================================================================
// Class definition
// =============================================================================
class Node
{
public:
    Node();
    Node(const NodeDef* def, const NodeData &nodeData, const OakModel* model = nullptr);
    Node(const Node &copy);
    Node(Node&& move);

    Node& operator=(const Node& copy);
    Node& operator=(Node&& move);

    bool operator==(const Node& _node) const;
    bool operator!=(const Node& _node) const;

    const Leaf& operator()(const std::string &name) const;

    Node operator[](int index) const;

    bool isNull() const;
    bool isDefNull() const;
    bool isNodeNull() const;

    void clear();

    const NodeData &nodeData() const { return m_nodeData; }
    NodeData nodeData() { return m_nodeData; }

    const NodeDef* def() const { return m_def; }
    const OakModel* model() const;

    void setCurrent();

    std::vector<std::string> valueNameList() const;
    std::vector<std::string> childNameList() const;

    // ************* Leaf Access *************
    int leafCount() const;
    bool hasLeaf(const std::string &leafName) const;
    int leafIndex(const Leaf& leaf) const;

    const Leaf& leafAt(int index) const;
    Leaf& leafAt(int index);

    const Leaf& leaf(const std::string &leafName) const;
    Leaf& leaf(const std::string &leafName);

    typedef std::vector<Leaf>::const_iterator LeafIterator;

    const LeafIterator leafBegin() const;
    Node::LeafIterator leafBegin();

    const LeafIterator leafEnd() const;
    LeafIterator leafEnd();

    bool hasKey() const;
    const Leaf& keyLeaf() const;

    bool hasVariants() const;
    const Leaf& variantLeaf() const;

    // ************* Child Node access *************
    int childCount() const;
    int childCount(const std::string &name) const;

    int childIndex(const Node &refChild) const;
    int childIndex(const std::string &name, const Node &refChild) const;

    Node childAt(int index) const;
    Node childAt(const std::string &name, int index) const;

    Node firstChild() const;
    Node firstChild(const std::string &name) const;

    Node lastChild() const;
    Node lastChild(const std::string &name) const;

    Node nextChild(const Node& refChild) const;
    Node nextChild(const std::string &name, const Node& refChild) const;

    Node previousChild(const Node& refChild) const;
    Node previousChild(const std::string &name, const Node& refChild) const;

    // ************* Parent Node access *************
    Node parent() const;

    // ************* Child Node edit *************
    bool canInsertChild(const std::string &name, int &index) const;

    Node insertChild(const std::string &name, int &index) const;

    bool canCloneChild(int &index, const Node &cloneNode) const;
    bool canCloneChild(const std::string &name, int &index, const Node &cloneNode) const;

    Node cloneChild(int &index, const Node &cloneNode) const;
    Node cloneChild(const std::string &name, int &index, const Node &cloneNode) const;

    bool canMoveChild(int &index, const Node &moveNode) const;
    bool canMoveChild(const std::string &name, int &index, const Node &moveNode) const;

    Node moveChild(int &index, const Node &moveNode) const;
    Node moveChild(const std::string &name, int &index, const Node &moveNode) const;

    bool canRemoveChild(int index) const;
    bool canRemoveChild(const std::string &name, int index) const;

    bool removeChild(int index) const;
    bool removeChild(const std::string &name, int index) const;


    // ************* Convert child index *************
    int convertChildIndexToUnnamed(const std::string &name, int index) const;
    int convertChildIndexToNamed(std::string &name, int index) const;

protected:
    inline void initLeafList() const;

    static void updateUniqueValues(const Node &node);

protected:
    const NodeDef* m_def;
    NodeData m_nodeData;
    const OakModel* m_model;

    mutable std::vector<Leaf> m_leafList;
};

} // namespace Oak::Model

