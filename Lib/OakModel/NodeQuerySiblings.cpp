#include "NodeQuerySiblings.h"

#include "../ServiceFunctions/Trace.h"


namespace Oak::Model {

// =============================================================================
// (public)
NodeQuerySiblings::NodeQuerySiblings()
{
}

// =============================================================================
// (public)
NodeQuerySiblings::NodeQuerySiblings(const NodeQuerySiblings &copy)
    : NodeQuery(copy)
{
}

// =============================================================================
// (public)
NodeQuerySiblings::NodeQuerySiblings(NodeQuerySiblings &&move)
    : NodeQuery(move)
{
}

// =============================================================================
// (protected)
Node NodeQuerySiblings::first(const Node &refNode) const
{
    m_parent = refNode.parent();
    Node sibling = m_parent.firstChild(refNode.def()->name());

    if (refNode == sibling) {
        // Skip self
        return next(refNode, sibling);
    } else {
        return sibling;
    }
}

// =============================================================================
// (protected)
Node NodeQuerySiblings::last(const Node &refNode) const
{
    m_parent = refNode.parent();
    Node sibling = m_parent.lastChild(refNode.def()->name());

    if (refNode == sibling) {
        // Skip self
        return previous(refNode, sibling);
    } else {
        return sibling;
    }
}

// =============================================================================
// (protected)
Node NodeQuerySiblings::next(const Node &refNode, const Node &cNode) const
{
    Node sibling = m_parent.nextChild(refNode.def()->name(), cNode);

    if (refNode == sibling) {
        // Skip self
        return next(refNode, sibling);
    } else {
        return sibling;
    }
}

// =============================================================================
// (protected)
Node NodeQuerySiblings::previous(const Node &refNode, const Node &cNode) const
{
    Node sibling = m_parent.previousChild(refNode.def()->name(), cNode);

    if (refNode == sibling) {
        // Skip self
        return previous(refNode, sibling);
    } else {
        return sibling;
    }
}

// =============================================================================
// (protected)
const NodeDef *NodeQuerySiblings::_nodeDef(const NodeDef *nDef) const
{
    // TODO: Node can have multiple parents! Fix this...
    ASSERT(nDef);
    ASSERT(nDef->parentContainerCount() == 1); // Do not work unleass there are only one parent container

    // Return the container variant base class
    return nDef->parentContainer(0)->containerDef();
}

} // namespace Oak::Model

