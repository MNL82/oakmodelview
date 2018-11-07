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

#include <vector>
#include <memory>

#include "XMLChildRefGroup.h"

namespace Oak {
namespace XML {

class ListRef;
typedef std::unique_ptr<ListRef> ListRefUPtr;

// =============================================================================
// Class definition
// =============================================================================
class ListRef
{
public:
    ListRef(const std::string& tagName = std::string());
    ListRef(RefUPtr listBaseRef, const std::string& tagName, ChildRefGroupUPtr subRef = ChildRefGroup::MakeUPtr());
    ListRef(const ListRef &copy);
    ListRef(ListRef &&move);

    virtual ~ListRef();

    ListRef& operator=(const ListRef &copy);
    ListRef& operator=(ListRef&& move);

    ListRefUPtr copy() const;

    int count(Element refBase) const;
    int indexOf(Element refBase, Element refElement) const;
    Element at(Element refBase, int index) const;
    std::vector<Element> list(Element refBase) const;

    Element first(Element refBase) const;
    Element last(Element refBase) const;

    Element next(Element element) const;
    Element previous(Element element) const;

    Element insert(Element refBase, int index) const;
    Element insertBefore(Element refBase, Element refElement) const;
    Element insertAfter(Element refBase, Element refElement) const;

    Element clone(Element refBase, int index, Element cloneElement) const;
    Element cloneBefore(Element refBase, Element refElement, Element cloneElement) const;
    Element cloneAfter(Element refBase, Element refElement, Element cloneElement) const;

    Element move(Element refBase, int index, Element moveElement) const;
    Element moveBefore(Element refBase, Element refElement, Element moveElement) const;
    Element moveAfter(Element refBase, Element refElement, Element moveElement) const;

    bool remove(Element refBase, Element refElement) const;
    bool remove(Element refBase, int index) const;

    virtual Element invertedAt(Element refElement) const;

    const Ref& listBaseRef() const { return *m_listBaseRef.get(); }
    const std::string& tagName() const { return m_tagName; }
    const ChildRefGroup& subRef() const { return *m_subRef.get(); }

    void setListBaseRef(RefUPtr value);
    void setTagName(const std::string& value);
    void setSubRef(ChildRefGroupUPtr value);

    template<class... _Types> inline
    static ListRefUPtr MakeUPtr(_Types&&... _Args)
    {
        return (ListRefUPtr(new ListRef(_STD forward<_Types>(_Args)...)));
    }

protected:
    // The number of elements in a element reference list is the number of elements
    // with element tag name in in the list base ref element.
    // The ref element in each element can be a sub ref of the element
    RefUPtr m_listBaseRef;
    std::string m_tagName;
    ChildRefGroupUPtr m_subRef;
};

} // namespace XML
} // namespace Oak

#endif // XML_BACKEND
