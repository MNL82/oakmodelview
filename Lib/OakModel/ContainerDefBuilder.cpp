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
#include "NodeDefBuilder.h"

#include "NodeDef.h"

#include "../ServiceFunctions/Assert.h"


namespace Oak::Model {

// =============================================================================
// (protected)
ContainerDefBuilder::ContainerDefBuilder(NodeDefBuilderSPtr nodeDefBuilder, int minCount, int maxCount)
{
    ASSERT(nodeDefBuilder);
    ASSERT(minCount >= 0);
    ASSERT(maxCount > 0);
    ASSERT(maxCount >= minCount);

    m_containerDefUPtr = ContainerDef::MakeUPtr(nodeDefBuilder->get(), minCount, maxCount);
    m_containerDef = m_containerDefUPtr.get();

    nodeDefBuilder->get()->m_parentContainerDefs.push_back(m_containerDef);

#ifdef XML_BACKEND
    m_containerDef->m_elementListRef.setTagName(nodeDefBuilder->get()->tagName());
#endif // XML_BACKEND
}

// =============================================================================
// (protected)
ContainerDefBuilder::ContainerDefBuilder(ContainerDef *containerDef)
{
    ASSERT(containerDef);
    m_containerDef = containerDef;
}

// =============================================================================
// (public static)
ContainerDefBuilderSPtr ContainerDefBuilder::create(NodeDefBuilderSPtr nodeDefBuilder, int minCount, int maxCount)
{
    ContainerDefBuilderSPtr sPtr(new ContainerDefBuilder(nodeDefBuilder, minCount, maxCount));
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

// =============================================================================
// (public static)
ContainerDefBuilderSPtr ContainerDefBuilder::use(ContainerDef *containerDef)
{
    ContainerDefBuilderSPtr sPtr(new ContainerDefBuilder(containerDef));
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

// =============================================================================
// (public)
ContainerDefUPtr ContainerDefBuilder::get()
{
    ASSERT(m_containerDefUPtr);
    return std::move(m_containerDefUPtr);
}

// =============================================================================
// (public)
const ContainerDef &ContainerDefBuilder::containerDef() const
{
    ASSERT(m_containerDef);
    return *m_containerDef;
}

// =============================================================================
// (public)
ContainerDefBuilderSPtr ContainerDefBuilder::setNodeDefElement(NodeDefSPtr nodeDef)
{
    ASSERT(m_containerDef);
    ASSERT(nodeDef);

#ifdef XML_BACKEND
    // todo: Update m_elementListRef so that it refers to a alement with the same name as the new container definition
#endif // XML_BACKEND
    m_containerDef->m_containerDef = nodeDef;
    nodeDef->m_parentContainerDefs.push_back(m_containerDef);
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
ContainerDefBuilderSPtr ContainerDefBuilder::setNodeDefParent(NodeDefSPtr hostDef)
{
    ASSERT(m_containerDef);

    m_containerDef->m_hostDef = hostDef;
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
ContainerDefBuilderSPtr ContainerDefBuilder::setMinMaxCount(int minCount, int maxCount)
{
    ASSERT(m_containerDef);
    ASSERT(minCount >= 0 && maxCount >= minCount);

    m_containerDef->m_minCount = minCount;
    m_containerDef->m_maxCount = maxCount;

    return m_thisWPtr.lock();
}

#ifdef XML_BACKEND
// =============================================================================
// (public)
ContainerDefBuilderSPtr ContainerDefBuilder::setElementListRef(XML::ListRef listRef)
{
    ASSERT(m_containerDef);

    // todo: Validate listRef
    // It needs to reference a list of tags with the same tag name as the container node definition
    m_containerDef->m_elementListRef = listRef;
    return m_thisWPtr.lock();
}
#endif // XML_BACKEND

} // namespace Oak::Model

