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

#ifdef XML_BACKEND

#include <memory>
#include <string>

namespace Oak {
namespace XML {

class Ref;
typedef std::unique_ptr<Ref> RefUPtr;

class ValueRef;
typedef std::unique_ptr<ValueRef> ValueRefUPtr;

class RefGroup;
typedef std::unique_ptr<RefGroup> RefGroupUPtr;

class ChildRefGroup;
typedef std::unique_ptr<ChildRefGroup> ChildRefGroupUPtr;

// =============================================================================
// Class definition
// =============================================================================
class RefFactory
{
    RefFactory() = delete;
public:
    static ValueRefUPtr MakeValueRef(const std::string& elementRef = "", const std::string& attributeName = "");

    static RefUPtr MakeRef(const std::string& elementRef);

    static RefGroupUPtr MakeGroupRef(const std::string& elementRef);

    static ChildRefGroupUPtr MakeChildRef(const std::string& childRef);

    static RefUPtr MakeSingle(const std::string& refType = "c", const std::string& refValue = "", int index = 0);


};

} // namespace XML
} // namespace Oak

#endif // XML_BACKEND
