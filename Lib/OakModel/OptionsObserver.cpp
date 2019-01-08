/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "OptionsObserver.h"

#include "OakModel.h"
#include "NodeDef.h"
#include "LeafQuery.h"
#include "NodeIndex.h"
#include "QueryBuilder.h"

#include "../ServiceFunctions/Trace.h"


namespace Oak::Model {

// =============================================================================
// (public)
OptionsObserver::OptionsObserver(OakModel *model, const NodeDef *optionsNodeDef, const LeafDef *optionsLeafDef)
    : ObserverInterface(model),
      m_optionsNodeDef { optionsNodeDef},
      m_optionsLeafDef { optionsLeafDef}
{
    ASSERT(m_optionsNodeDef);
    ASSERT(m_optionsLeafDef);
    ASSERT(m_optionsLeafDef->options().hasQuery());

    const LeafQuery *query = m_optionsLeafDef->options().query();
    m_sourceNodeDef = query->nodeQuery().nodeDef(m_optionsNodeDef);
    m_sourceValueName = query->valueName();

    // Create an inverse query that points from the option values to the leaf where there can be chosen
    m_inverseQuery = QueryBuilder::createInverse(query->nodeQuery(), m_optionsNodeDef)->leafSPtr(m_optionsLeafDef->name());


}

// =============================================================================
// (public)
void OptionsObserver::connect()
{
    m_model->notifier_nodeInserteAfter.add(this, &OptionsObserver::onNodeInserteAfter);
    m_model->notifier_nodeMoveBefore.add(this, &OptionsObserver::onNodeMoveBefore);
    m_model->notifier_nodeMoveAfter.add(this, &OptionsObserver::onNodeMoveAfter);
    m_model->notifier_nodeCloneAfter.add(this, &OptionsObserver::onNodeCloneAfter);

    m_model->notifier_nodeRemoveBefore.add(this, &OptionsObserver::onNodeRemoveBefore);

    m_model->notifier_leafChangeBefore.add(this, &OptionsObserver::onLeafChangeBefore);
    m_model->notifier_leafChangeAfter.add(this, &OptionsObserver::onLeafChangeAfter);
}

// =============================================================================
// (public)
void OptionsObserver::disconnect()
{
    m_model->notifier_nodeInserteAfter.remove(this);
    m_model->notifier_nodeMoveBefore.remove(this);
    m_model->notifier_nodeMoveAfter.remove(this);
    m_model->notifier_nodeCloneAfter.remove(this);

    m_model->notifier_nodeRemoveBefore.remove(this);

    m_model->notifier_leafChangeBefore.remove(this);
    m_model->notifier_leafChangeAfter.remove(this);
}

// =============================================================================
// (protected)
void OptionsObserver::onNodeInserteAfter(const NodeIndex &nodeIndex)
{

}

// =============================================================================
// (protected)
void OptionsObserver::onNodeMoveAfter(const NodeIndex &sourceNodeIndex, const NodeIndex &targetNodeIndex)
{

}

// =============================================================================
// (protected)
void OptionsObserver::onNodeMoveBefore(const NodeIndex &sourceNodeIndex, const NodeIndex &targetNodeIndex)
{

}

// =============================================================================
// (protected)
void OptionsObserver::onNodeCloneAfter(const NodeIndex &sourceNodeIndex, const NodeIndex &targetNodeIndex)
{

}

// =============================================================================
// (protected)
void OptionsObserver::onNodeRemoveBefore(const NodeIndex &nodeIndex)
{

}

// =============================================================================
// (protected)
void OptionsObserver::onLeafChangeBefore(const NodeIndex &nodeIndex, const std::string &valueName)
{
    // If not valid return as fast as possible
    if (m_sourceValueName != valueName) { return; }
    if (m_sourceNodeDef->name() != nodeIndex.lastNodeIndex().name()) { return; }

    Node sourceNode = nodeIndex.node(m_model->rootNode());

    m_valueBeforeChange = sourceNode.leaf(valueName).value();
}

// =============================================================================
// (protected)
void OptionsObserver::onLeafChangeAfter(const NodeIndex &nodeIndex, const std::string &valueName)
{
    // If not valid return as fast as possible
    if (m_valueBeforeChange.isNull()) { return; }
    if (m_sourceValueName != valueName) { return; }
    if (m_sourceNodeDef->name() != nodeIndex.lastNodeIndex().name()) { return; }

    Node sourceNode = nodeIndex.node(m_model->rootNode());
    UnionValue newValue = sourceNode.leaf(valueName).value();

    if (m_valueBeforeChange == newValue) { return; }

    // Loop though all the entries where the option have been used
    auto it = m_inverseQuery->iterator(sourceNode);
    while (it->next()) {
        UnionValue value = it->leaf().value();
        if (value == m_valueBeforeChange) {
            // Update the value if it is the one changed
            it->leaf().setValue(newValue);
        } else if (it->node() == m_model->currentNode()) {
            // Update the current node because the option list have changed
            m_model->setCurrentNode(m_model->currentNode(), true);
        }
    }

    m_valueBeforeChange = UnionValue();
}

} // namespace Oak::Model

