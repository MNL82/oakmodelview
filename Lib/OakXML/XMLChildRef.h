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

namespace Oak {
namespace XML {

class ChildRef;
typedef std::unique_ptr<ChildRef> ChildRefUPtr;

// =============================================================================
// Class definition
// =============================================================================
class ChildRef : public Ref
{
public:
    ChildRef(std::string tagName, int index = 0);
    ChildRef(const ChildRef& copy);
    ChildRef(ChildRef&& move);

    ChildRef& operator=(const ChildRef& copy);
    ChildRef& operator=(ChildRef&& move);

    virtual RefUPtr copy() const override;
    ChildRefUPtr copyChild() const;

    virtual Element getTarget(Element source, bool create = false) const override;
    virtual void clearTarget(Element source, bool onlyIfEmpty = true) const override;

    virtual Element getSource(Element target) const override;

    virtual bool isNull() const override { return false; }

    int index() const { return m_index; }
    void setIndex(int index) { m_index = index; }

    const std::string& tagName() const { return m_tagName; }
    void setTagName(std::string tagName) { m_tagName = tagName; }

    virtual const std::string& firstTagName() const override { return m_tagName; }
    virtual const std::string& lastTagName() const override { return m_tagName; }

    template<class... _Types> inline
    static ChildRefUPtr MakeUPtr(_Types&&... _Args)
    {
        return (ChildRefUPtr(new ChildRef(_STD forward<_Types>(_Args)...)));
    }

protected:
    std::string m_tagName;
    int m_index;
};

} // namespace XML
} // namespace Oak

#endif // XML_BACKEND
