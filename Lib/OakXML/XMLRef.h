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

#include "XMLElement.h"

#include <memory>

namespace Oak {
namespace XML {

class Ref;
typedef std::unique_ptr<Ref> RefUPtr;

// =============================================================================
// Class definition
// =============================================================================
class Ref
{
public:
// TODO: Make copy and move constructors for all ElementRef's
    Ref() {}
    Ref(const Ref&) {}
    Ref(Ref&&) {}

    virtual ~Ref() {}

    Ref& operator=(const Ref&) { return *this; }
    Ref& operator=(Ref&&) { return *this; }

    virtual RefUPtr copy() const;

    virtual Element getTarget(Element source, bool create = false) const;
    virtual void clearTarget(Element source, bool onlyIfEmpty = true) const;
    bool hasTarget(Element source) const;

    virtual Element getSource(Element target) const;

    virtual bool isNull() const { return true; }

    virtual const std::string& firstTagName() const { return emptyStr; }
    virtual const std::string& lastTagName() const { return emptyStr; }

    template<class... _Types> inline
    static RefUPtr MakeUPtr(_Types&&... _Args)
    {
        return (RefUPtr(new Ref(_STD forward<_Types>(_Args)...)));
    }

protected:
    static std::string emptyStr;
};

} // namespace XML
} // namespace Oak

#endif // XML_BACKEND
