/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ContainerDefBuilder.h"

#include "NodeDef.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
ContainerDefUPtr ContainerDefBuilder::Make(NodeDefSPtr containerDef, int minCount, int maxCount)
{
    assert(containerDef);
    assert(minCount >= 0);
    assert(maxCount > 0);
    assert(maxCount >= minCount);

    auto container = ContainerDef::MakeUPtr();

    container->m_containerDef = containerDef;
    container->m_minCount = minCount;
    container->m_maxCount = maxCount;

    containerDef->m_parentContainerDefs.push_back(container.get());

#ifdef XML_BACKEND
    container->m_elementListRef.setTagName(containerDef->tagName());
#endif // XML_BACKEND

    return std::move(container);
}

// =============================================================================
// (public)
bool ContainerDefBuilder::setNodeDefElement(const ContainerDefUPtr &nContainer, NodeDefSPtr containerDef)
{
    if (!nContainer) { return false; }
    if (!containerDef) { return false; }

#ifdef XML_BACKEND
    // todo: Update m_elementListRef so that it refers to a alement with the same name as the new container definition
#endif // XML_BACKEND
    nContainer->m_containerDef = containerDef;
    containerDef->m_parentContainerDefs.push_back(nContainer.get());
    return true;
}

// =============================================================================
// (public)
bool ContainerDefBuilder::setNodeDefParent(const ContainerDefUPtr &nContainer, NodeDefSPtr hostDef)
{
    if (!nContainer) { return false; }

    nContainer->m_hostDef = hostDef;
    return true;
}

// =============================================================================
// (public)
bool ContainerDefBuilder::setMinMaxCount(const ContainerDefUPtr &nContainer, int minCount, int maxCount)
{
    if (!nContainer) { return false; }
    if (minCount < 0 || maxCount < minCount) { return false; }

    nContainer->m_minCount = minCount;
    nContainer->m_maxCount = maxCount;

    return true;
}

#ifdef XML_BACKEND
// =============================================================================
// (public)
bool ContainerDefBuilder::setElementListRef(const ContainerDefUPtr &nContainer, XML::ListRef listRef)
{
    if (!nContainer) { return false; }

    // todo: Validate listRef
    // It needs to reference a list of tags with the same tag name as the container node definition
    nContainer->m_elementListRef = listRef;
    return true;
}
#endif // XML_BACKEND

} // namespace Model
} // namespace Oak
