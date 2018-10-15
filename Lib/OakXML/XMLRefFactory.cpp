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

#include "XMLRefFactory.h"

#include "XMLChildRef.h"
#include "XMLParentRef.h"
#include "XMLRefGroup.h"
#include "XMLChildRefGroup.h"

#include "XMLListRef.h"

#include "XMLValueRef.h"
#include "XMLServiceFunctions.h"

#include "../ServiceFunctions/Assert.h"

#include <sstream>
#include <iostream>

namespace Oak {
namespace XML {

// =============================================================================
// (public)
ValueRefUPtr RefFactory::MakeValueRef(const std::string& elementRef, const std::string& attributeName)
{
    return ValueRef::MakeUPtr(attributeName, MakeRef(elementRef));
}

// =============================================================================
// (public)
RefUPtr RefFactory::MakeRef(const std::string &elementRef)
{
    RefGroupUPtr groupRef = MakeGroupRef(elementRef);
    if (groupRef->count() == 0) {
        return Ref::MakeUPtr();
    } else if (groupRef->count() == 1) {
        return groupRef->take(0);
    }
    return std::move(groupRef);
}

// =============================================================================
// (public)
RefGroupUPtr RefFactory::MakeGroupRef(const std::string &elementRef)
{
    std::vector<std::vector<std::string>> sMatrix = doubleSplit(elementRef);

    std::vector<RefUPtr> refList;
    for (const std::vector<std::string> &sList: sMatrix)
    {
        if (sList.empty()) { continue; }

        switch (sList.size()) {
        case 1: {
            refList.push_back(MakeSingle("c", sList.front()));
            break;
        }
        case 2: {
            refList.push_back(MakeSingle(sList[0], sList[1]));
            break;
        }
        case 3: {
            bool ok;
            int index = toInteger(sList[2], &ok);
            ASSERT(ok);
            refList.push_back(MakeSingle(sList[0], sList[1], index));
            break;
        }
        default:
            ASSERT(false);
        }
    }
    RefGroupUPtr refGroup = RefGroup::MakeUPtr();
    for (int i = 0; i < static_cast<int>(refList.size()); i++)
    {
        refGroup->add(std::move(refList[static_cast<vSize>(i)]));
    }
    return refGroup;
}

// =============================================================================
// (public)
ChildRefGroupUPtr RefFactory::MakeChildRef(const std::string &childRef)
{
    RefGroupUPtr groupRef = MakeGroupRef(childRef);
    ChildRefGroupUPtr childGroup = ChildRefGroup::MakeUPtr();
    for (int i = 0; i < groupRef->count(); i++)
    {
        childGroup->add(dynamic_unique_pointer_cast<ChildRef>(groupRef->take(i)));
    }

    return childGroup;
}

// =============================================================================
// (public)
RefUPtr RefFactory::MakeSingle(const std::string &refType, const std::string &refValue, int index)
{
    if (refType.empty() || refType.find("c") == 0) {
        return ChildRef::MakeUPtr(refValue, index);
    } else if (refType.find("p") == 0) {
        // refValue is the parent tagName. If empty the link is to the direct parent
        return ParentRef::MakeUPtr(refValue);
    } else {
        ASSERT(false);
        return Ref::MakeUPtr();
    }
}

} // namespace Model
} // namespace Oak

#endif // XML_BACKEND
