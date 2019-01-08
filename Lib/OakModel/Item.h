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
class Item
{
public:
    Item();
    Item(const NodeDef* def, const NodeData &nodeData, const OakModel* model = nullptr);
    Item(const Item &copy);
    Item(Item&& move);

    Item& operator=(const Item& copy);
    Item& operator=(Item&& move);

    bool operator==(const Item& _item) const;
    bool operator!=(const Item& _item) const;

    const Leaf& operator()(const std::string &name) const;

    Item operator[](int index) const;

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
    Item::LeafIterator leafBegin();

    const LeafIterator leafEnd() const;
    LeafIterator leafEnd();

    bool hasKey() const;
    const Leaf& keyLeaf() const;

    bool hasVariants() const;
    const Leaf& variantLeaf() const;

    // ************* Child Item access *************
    int childCount() const;
    int childCount(const std::string &name) const;

    int childIndex(const Item &refChild) const;
    int childIndex(const std::string &name, const Item &refChild) const;

    Item childAt(int index) const;
    Item childAt(const std::string &name, int index) const;

    Item firstChild() const;
    Item firstChild(const std::string &name) const;

    Item lastChild() const;
    Item lastChild(const std::string &name) const;

    Item nextChild(const Item& refChild) const;
    Item nextChild(const std::string &name, const Item& refChild) const;

    Item previousChild(const Item& refChild) const;
    Item previousChild(const std::string &name, const Item& refChild) const;

    // ************* Parent Item access *************
    Item parent() const;

    // ************* Child Item edit *************
    bool canInsertChild(const std::string &name, int &index) const;

    Item insertChild(const std::string &name, int &index) const;

    bool canCloneChild(int &index, const Item &cloneItem) const;
    bool canCloneChild(const std::string &name, int &index, const Item &cloneItem) const;

    Item cloneChild(int &index, const Item &cloneItem) const;
    Item cloneChild(const std::string &name, int &index, const Item &cloneItem) const;

    bool canMoveChild(int &index, const Item &moveItem) const;
    bool canMoveChild(const std::string &name, int &index, const Item &moveItem) const;

    Item moveChild(int &index, const Item &moveItem) const;
    Item moveChild(const std::string &name, int &index, const Item &moveItem) const;

    bool canRemoveChild(int index) const;
    bool canRemoveChild(const std::string &name, int index) const;

    bool removeChild(int index) const;
    bool removeChild(const std::string &name, int index) const;


    // ************* Convert child index *************
    int convertChildIndexToUnnamed(const std::string &name, int index) const;
    int convertChildIndexToNamed(std::string &name, int index) const;

protected:
    inline void initLeafList() const;

    static void updateUniqueValues(const Item &item);

protected:
    const NodeDef* m_def;
    NodeData m_nodeData;
    const OakModel* m_model;

    mutable std::vector<Leaf> m_leafList;
};

} // namespace Oak::Model

