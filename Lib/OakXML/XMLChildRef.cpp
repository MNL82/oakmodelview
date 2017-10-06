/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#ifdef XML_BACKEND

#include "XMLChildRef.h"

namespace Oak {
namespace XML {

// =============================================================================
// (public)
ChildRef::ChildRef(std::string tagName, int index)
    : Ref(),
      m_tagName(tagName),
      m_index(index)
{
    assert(m_index >= 0);
}

// =============================================================================
// (public)
ChildRef::ChildRef(const ChildRef& copy)
    : Ref()
{
    *this = copy;
}

// =============================================================================
// (public)
ChildRef::ChildRef(ChildRef&& move)
    : Ref()
{
    *this = std::move(move);
}

// =============================================================================
// (public)
ChildRef &ChildRef::operator=(const ChildRef& copy)
{
    m_tagName = copy.m_tagName;
    m_index = copy.m_index;
    return *this;
}

// =============================================================================
// (public)
ChildRef &ChildRef::operator=(ChildRef&& move)
{
    m_tagName = std::move(move.m_tagName);
    m_index = move.m_index;
    return *this;
}

// =============================================================================
// (public)
RefUPtr ChildRef::copy() const
{
    return MakeUPtr(*this);
}

// =============================================================================
// (public)
ChildRefUPtr ChildRef::copyChild() const
{
    return MakeUPtr(*this);
}

// =============================================================================
// (public)
Element ChildRef::getTarget(Element source, bool create) const
{
    if (source.isNull()) { return Element(); }

    int index = 0;
    Element refElement = source.firstChild(m_tagName);

    while (index < m_index && !refElement.isNull()) {
        refElement = refElement.nextSibling(m_tagName);
        index++;
    }

    // Creats the reference element if it is missing
    if (create && refElement.isNull() && !m_tagName.empty()) {
        index--; // Last element was not found
        refElement = source.lastChild(m_tagName);
        while (index < m_index) {
            refElement = source.insertAfter(m_tagName, refElement);
            index++;
        }
    }

    return refElement;
}

// =============================================================================
// (public)
void ChildRef::clearTarget(Element source, bool onlyIfEmpty) const
{
    Element refElement = getTarget(source);

    if (refElement.isNull()) { return; }

    if (!onlyIfEmpty || refElement.empty()) {
        source.removeChild(refElement);
    }
}

// =============================================================================
// (public)
Element ChildRef::getSource(Element target) const
{
    if (target.isNull()) { return Element(); }

    return target.parentElement();
}

} // namespace XML
} // namespace Oak

#endif // XML_BACKEND
