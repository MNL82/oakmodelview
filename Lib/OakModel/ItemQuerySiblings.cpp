#include "ItemQuerySiblings.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
ItemQuerySiblings::ItemQuerySiblings()
{
}

// =============================================================================
// (public)
ItemQuerySiblings::ItemQuerySiblings(const ItemQuerySiblings &copy)
    : ItemQuery(copy)
{
}

// =============================================================================
// (public)
ItemQuerySiblings::ItemQuerySiblings(ItemQuerySiblings &&move)
    : ItemQuery(move)
{
}

// =============================================================================
// (protected)
Item ItemQuerySiblings::first(const Item &refItem) const
{
    m_parent = refItem.parent();
    Item sibling = m_parent.firstChild(refItem.def()->name());

    if (refItem == sibling) {
        // Skip self
        return next(refItem, sibling);
    } else {
        return sibling;
    }
}

// =============================================================================
// (protected)
Item ItemQuerySiblings::last(const Item &refItem) const
{
    m_parent = refItem.parent();
    Item sibling = m_parent.lastChild(refItem.def()->name());

    if (refItem == sibling) {
        // Skip self
        return previous(refItem, sibling);
    } else {
        return sibling;
    }
}

// =============================================================================
// (protected)
Item ItemQuerySiblings::next(const Item &refItem, const Item &cItem) const
{
    Item sibling = m_parent.nextChild(refItem.def()->name(), cItem);

    if (refItem == sibling) {
        // Skip self
        return next(refItem, sibling);
    } else {
        return sibling;
    }
}

// =============================================================================
// (protected)
Item ItemQuerySiblings::previous(const Item &refItem, const Item &cItem) const
{
    Item sibling = m_parent.previousChild(refItem.def()->name(), cItem);

    if (refItem == sibling) {
        // Skip self
        return previous(refItem, sibling);
    } else {
        return sibling;
    }
}

} // namespace Model
} // namespace Oak
