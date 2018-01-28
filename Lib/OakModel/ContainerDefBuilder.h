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

namespace Oak {
namespace Model {

// =============================================================================
// Class definition
// =============================================================================
class ContainerDefBuilder
{
    ContainerDefBuilder() = delete;
public:
    static ContainerDefUPtr Make(NodeDefSPtr containerDef, int minCount = 0, int maxCount = std::numeric_limits<int>::max());

    static bool setNodeDefElement(const ContainerDefUPtr& nContainer, NodeDefSPtr containerDef);
    static bool setNodeDefParent(const ContainerDefUPtr& nContainer, NodeDefSPtr hostDef);

    static bool setMinMaxCount(const ContainerDefUPtr& nContainer, int minCount = 0, int maxCount = std::numeric_limits<int>::max());

#ifdef XML_BACKEND
    static bool setElementListRef(const ContainerDefUPtr& nContainer, XML::ListRef listRef);
#endif // XML_BACKEND
};

typedef ContainerDefBuilder CDB;

} // namespace Model
} // namespace Oak
