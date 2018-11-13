/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#pragma once


#include "ContainerDef.h"

namespace Oak::Model {

class NodeDefBuilder;
typedef std::shared_ptr<NodeDefBuilder> NodeDefBuilderSPtr;

class ContainerDefBuilder;
typedef std::shared_ptr<ContainerDefBuilder> ContainerDefBuilderSPtr;
typedef std::weak_ptr<ContainerDefBuilder> ContainerDefBuilderWPtr;

// =============================================================================
// Class definition
// =============================================================================
class ContainerDefBuilder
{
protected:
    ContainerDefBuilder(NodeDefBuilderSPtr nodeDefBuilder, int minCount, int maxCount);
    ContainerDefBuilder(ContainerDef *containerDef);

public:
    static ContainerDefBuilderSPtr create(NodeDefBuilderSPtr nodeDefBuilder, int minCount = 0, int maxCount = std::numeric_limits<int>::max());
    static ContainerDefBuilderSPtr use(ContainerDef *containerDef);

public:
    ContainerDefUPtr get();
    const ContainerDef & containerDef() const;

    ContainerDefBuilderSPtr setNodeDefElement(NodeDefSPtr nodeDef);
    ContainerDefBuilderSPtr setNodeDefParent(NodeDefSPtr hostDef);

    ContainerDefBuilderSPtr setMinMaxCount(int minCount = 0, int maxCount = std::numeric_limits<int>::max());

#ifdef XML_BACKEND
    ContainerDefBuilderSPtr setElementListRef(XML::ListRef listRef);
#endif // XML_BACKEND

private:
    ContainerDefUPtr m_containerDefUPtr;
    ContainerDef * m_containerDef;

    ContainerDefBuilderWPtr m_thisWPtr;
};

typedef ContainerDefBuilder CDB;

} // namespace Oak::Model

