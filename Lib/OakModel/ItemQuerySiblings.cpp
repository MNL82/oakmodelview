#include "ItemQuerySiblings.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
ItemQuerySiblings::ItemQuerySiblings(bool matchName)
{
    m_matchName = matchName;
}

// =============================================================================
// (public)
ItemQuerySiblings::ItemQuerySiblings(const ItemQuerySiblings &copy)
    : ItemQuery(copy)
{
    m_matchName = copy.m_matchName;
}

// =============================================================================
// (public)
ItemQuerySiblings::ItemQuerySiblings(ItemQuerySiblings &&move)
    : ItemQuery(move)
{
    m_matchName = move.m_matchName;
}

// =============================================================================
// (public)
Item ItemQuerySiblings::first(const Item &refItem) const
{
    m_parent = refItem.parent();
    Item sibling;
    if (m_matchName) {
        sibling = m_parent.firstChild(refItem.def()->name());
    } else {
        sibling = m_parent.firstChild();
    }

    if (refItem == sibling) {
        // Skip self
        return next(refItem, sibling);
    } else {
        return sibling;
    }
}

// =============================================================================
// (public)
Item ItemQuerySiblings::last(const Item &refItem) const
{
    m_parent = refItem.parent();
    Item sibling;
    if (m_matchName) {
        sibling = m_parent.lastChild(refItem.def()->name());
    } else {
        sibling = m_parent.lastChild();
    }

    if (refItem == sibling) {
        // Skip self
        return previous(refItem, sibling);
    } else {
        return sibling;
    }
}

// =============================================================================
// (public)
Item ItemQuerySiblings::next(const Item &refItem, const Item &cItem) const
{
    Item sibling;
    if (m_matchName) {
        sibling = m_parent.nextChild(refItem.def()->name(), cItem);
    } else {
        sibling = m_parent.nextChild(cItem);
    }

    if (refItem == sibling) {
        // Skip self
        return next(refItem, sibling);
    } else {
        return sibling;
    }
}

// =============================================================================
// (public)
Item ItemQuerySiblings::previous(const Item &refItem, const Item &cItem) const
{
    Item sibling;
    if (m_matchName) {
        sibling = m_parent.previousChild(refItem.def()->name(), cItem);
    } else {
        sibling = m_parent.previousChild(cItem);
    }

    if (refItem == sibling) {
        // Skip self
        return previous(refItem, sibling);
    } else {
        return sibling;
    }
}

} // namespace Model
} // namespace Oak
