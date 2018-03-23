/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "NodeRefGroup.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
NodeRefGroup::~NodeRefGroup()
{

}

// =============================================================================
// (public)
Node NodeRefGroup::getNode(const Node &baseNode, bool *ok) const
{
    bool okRef;
    Node refNode = baseNode;
    for (const NodeRef& nRef: m_references)
    {
        refNode = nRef.getNode(baseNode, &okRef);
        if (!okRef) {
            if (ok) { *ok = false; }
            return Node();
        }
    }
    return refNode;
}

} // namespace Model
} // namespace Oak
