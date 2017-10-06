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

#include "XMLRef.h"

namespace Oak {
namespace XML {

// =============================================================================
// (public)
RefUPtr Ref::copy() const
{
    return MakeUPtr();
}

// =============================================================================
// (public)
Element Ref::getTarget(Element source, bool create) const
{
    UNUSED(create);
    return source;
}

// =============================================================================
// (public)
void Ref::clearTarget(Element source, bool onlyIfEmpty) const
{
    UNUSED(source);
    UNUSED(onlyIfEmpty);
}

// =============================================================================
// (public)
bool Ref::hasTarget(Element source) const
{
    return !getTarget(source).isNull();
}

// =============================================================================
// (public)
Element Ref::getSource(Element target) const
{
    return target;
}

} // namespace XML
} // namespace Oak

#endif // XML_BACKEND
