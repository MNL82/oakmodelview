/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "Leaf.h"
#include "Node.h"
#include "OakModel.h"
#include "NodeIndex.h"

#include "../ServiceFunctions/Assert.h"


namespace Oak::Model {

// =============================================================================
// (public)
Leaf::Leaf()
    : m_def(nullptr),
      m_node(nullptr)
{

}

// =============================================================================
// (public)
Leaf::Leaf(const LeafDef* leafDef, const NodeData &nodeData, const Node *model)
    : m_def(leafDef),
      m_nodeData(nodeData),
      m_node(model)
{

}

// =============================================================================
// (public)
Leaf::Leaf(const Leaf &copy)
    : m_def(copy.m_def),
      m_nodeData(copy.m_nodeData),
      m_node(copy.m_node)
{

}

// =============================================================================
// (public)
Leaf::Leaf(Leaf&& move)
    : m_def(move.m_def),
      m_nodeData(std::move(move.m_nodeData)),
      m_node(move.m_node)
{

}

// =============================================================================
// (public)
Leaf& Leaf::operator=(const Leaf& copy)
{
    m_def = copy.m_def;
    m_nodeData = copy.m_nodeData;
    m_node = copy.m_node;
    return *this;
}

// =============================================================================
// (public)
Leaf& Leaf::operator=(Leaf&& move)
{
    m_def = move.m_def;
    m_nodeData = std::move(move.m_nodeData);
    m_node = move.m_node;
    return *this;
}

// =============================================================================
// (public)
bool Leaf::operator==(const Leaf &_leaf) const
{
    return m_def == _leaf.m_def && m_nodeData == _leaf.m_nodeData;
}

// =============================================================================
// (public)
bool Leaf::operator!=(const Leaf &_leaf) const
{
    return m_def != _leaf.m_def || m_nodeData != _leaf.m_nodeData;
}

// =============================================================================
// (public)
bool Leaf::isNull() const
{
    return isDefNull() || isNodeNull();
}

// =============================================================================
// (public)
bool Leaf::isDefNull() const
{
    return !m_def || m_def->isNull();
}

// =============================================================================
// (public)
bool Leaf::isNodeNull() const
{
    return m_nodeData.isNull();
}

// =============================================================================
// (public)
const std::string &Leaf::name() const
{
    ASSERT(m_def != nullptr);
    return m_def->name();
}

// =============================================================================
// (public)
const std::string & Leaf::displayName() const
{
    ASSERT(m_def != nullptr);
    return m_def->displayName();
}

// =============================================================================
// (public)
const std::string &Leaf::tooltip() const
{
    ASSERT(m_def != nullptr);
    return m_def->tooltip();
}

// =============================================================================
// (public)
const NodeData& Leaf::nodeData() const
{
    ASSERT(m_def != nullptr);
    return m_nodeData;
}

// =============================================================================
// (public)
const LeafDef* Leaf::def() const
{
    ASSERT(m_def != nullptr);
    return m_def;
}

// =============================================================================
// (public)
const Node* Leaf::node() const
{
    return m_node;
}

// =============================================================================
// (public)
UnionValue Leaf::value(bool useDefault) const
{
    ASSERT(m_def != nullptr);
    return m_def->value(m_nodeData, useDefault);
}

// =============================================================================
// (public)
std::string Leaf::toString(bool useDefault) const
{
    ASSERT(m_def != nullptr);
    return m_def->toString(m_nodeData, useDefault);
}

// =============================================================================
// (public)
bool Leaf::hasDefaultValue() const
{
    ASSERT(m_def != nullptr);
    return m_def->hasDefaultValue();
}

// =============================================================================
// (public)
const LeafSettings &Leaf::settings() const
{
    ASSERT(m_def != nullptr);
    return m_def->settings();
}

// =============================================================================
// (public)
Leaf& Leaf::emptyLeaf()
{
    static Leaf empty;
    return empty;
}

// =============================================================================
// (protected)
void Leaf::onLeafChangeBefore() const
{
    if (m_node->model() == nullptr) { return; }
    NodeIndexUPtr iIndex = NodeIndex::create(*m_node);
    m_node->model()->onLeafChangeBefore(*iIndex.get(), m_def->name());
}

// =============================================================================
// (protected)
void Leaf::onLeafChangeAfter() const
{
    if (m_node->model() == nullptr) { return; }

    int index = m_node->leafIndex(*this);
    NodeIndexUPtr iIndex = NodeIndex::create(*m_node);

    if (m_node->def()->indexOfVariantLeafDef() == index) {
        m_node->model()->onVariantLeafChangeAfter(*iIndex.get());
    } else if (m_node->def()->indexOfKeyLeafDef() == index) {
        m_node->model()->onKeyLeafChangeAfter(*iIndex.get());
    }

    m_node->model()->onLeafChangeAfter(*iIndex.get(), m_def->name());
}

} // namespace Oak::Model

