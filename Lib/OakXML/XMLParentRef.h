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

#include "XMLRef.h"

namespace Oak::XML {

class ParentRef;
typedef std::unique_ptr<ParentRef> ParentRefUPtr;

//! \brief This class is a relative reference to a parent Element.
/*!
 * It is one of several ElementRef derivities that together makes it possible to refer any other Element in the XML::Document.
 * \sa ElementRefChild, ElementRefGroup
*/

// =============================================================================
// Class definition
// =============================================================================
class ParentRef : public Ref
{
public:
    ParentRef(std::string parentTagName = std::string());
    ParentRef(const ParentRef& copy);
    ParentRef(ParentRef &&move);

    ParentRef& operator=(const ParentRef& copy);
    ParentRef& operator=(ParentRef&& move);

    virtual RefUPtr copy() const override;
    ParentRefUPtr copyParent() const;

    /*!
     * The function takes a Element as argument and returns the referenced Element if possible.<br>
     * An empty Element is returned if the operation fails.<br>
     * On success a parent Element that matches m_parentTagName is returned. Parent elements are checked recursively.<br>
     * If m_parentTagName is empty the first parent Element is returned.
    */
    virtual Element getTarget(Element source, bool create = false) const override;

    virtual Element getSource(Element target) const override;

    virtual bool isNull() const override { return false; }

    virtual const std::string& firstTagName() const override { return emptyStr; }
    virtual const std::string& lastTagName() const override { return emptyStr; }

    template<class... _Types> inline
    static ParentRefUPtr MakeUPtr(_Types&&... _Args)
    {
        return (ParentRefUPtr(new ParentRef(_STD forward<_Types>(_Args)...)));
    }

protected:
    /*! \brief Name of the referenced tag */
    std::string m_parentTagName;
};

} // namespace Oak::XML

#endif // XML_BACKEND
