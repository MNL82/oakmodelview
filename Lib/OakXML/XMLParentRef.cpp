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

#include "XMLParentRef.h"

namespace Oak {
namespace XML {

// =============================================================================
// (public)
ParentRef::ParentRef(std::string parentTagName)
    : Ref(),
      m_parentTagName(parentTagName)
{
}

// =============================================================================
// (public)
ParentRef::ParentRef(const ParentRef& copy)
    : Ref()
{
    *this = copy;
}

// =============================================================================
// (public)
ParentRef::ParentRef(ParentRef&& move)
    : Ref()
{
    *this = std::move(move);
}

// =============================================================================
// (public)
ParentRef& ParentRef::operator=(const ParentRef &copy)
{
    m_parentTagName = copy.m_parentTagName;
    return *this;
}

// =============================================================================
// (public)
ParentRef& ParentRef::operator=(ParentRef &&move)
{
    m_parentTagName = std::move(move.m_parentTagName);
    return *this;
}

// =============================================================================
// (public)
RefUPtr ParentRef::copy() const
{
    return MakeUPtr(*this);
}

// =============================================================================
// (public)
ParentRefUPtr ParentRef::copyParent() const
{
    return MakeUPtr(*this);
}

// =============================================================================
// (public)
Element ParentRef::getTarget(Element source, bool create) const
{
    UNUSED(create);
    if (source.isNull()) { return Element(); }

    Element refElement = source.parentElement();

    // Direct parent is returned if the parent tag name is empty
    if (m_parentTagName.empty()) { return refElement; }

    while (!refElement.isNull() && refElement.compareTagName(m_parentTagName) != 0) {
        refElement = refElement.parentElement();
    }

    return refElement;
}

// =============================================================================
// (public)
Element ParentRef::getSource(Element target) const
{
    UNUSED(target);
    // It can be any of the child elements so none is returned
    return Element();
}

} // namespace XML
} // namespace Oak

#endif // XML_BACKEND
