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

#include "XMLListRef.h"

namespace Oak {
namespace XML {

// =============================================================================
// (public)
ListRef::ListRef(const std::string &elementTagName)
    : m_listBaseRef(Ref::MakeUPtr()),
      m_tagName(elementTagName),
      m_subRef(ChildRefGroup::MakeUPtr())
{
}

// =============================================================================
// (public)
ListRef::ListRef(RefUPtr listBaseRef, const std::string &elementTagName, ChildRefGroupUPtr elementSubRef)
    : m_listBaseRef(std::move(listBaseRef)),
      m_tagName(elementTagName),
      m_subRef(std::move(elementSubRef))
{
    assert(!m_tagName.empty());
}

// =============================================================================
// (public)
ListRef::ListRef(const ListRef &copy)
{
    *this = copy;
}

// =============================================================================
// (public)
ListRef::ListRef(ListRef &&move)
{
    *this = std::move(move);
}

// =============================================================================
// (public)
ListRef::~ListRef()
{

}

// =============================================================================
// (public)
ListRef& ListRef::operator=(const ListRef &copy)
{
    m_listBaseRef = copy.m_listBaseRef->copy();
    m_tagName = copy.m_tagName;
    m_subRef = copy.m_subRef->copyChildGroup();
    return *this;
}

// =============================================================================
// (public)
ListRef& ListRef::operator=(ListRef &&move)
{
    m_listBaseRef = std::move(move.m_listBaseRef);
    m_tagName = std::move(move.m_tagName);
    m_subRef = std::move(move.m_subRef);
    return *this;
}

// =============================================================================
// (public)
ListRefUPtr ListRef::copy() const
{
    return MakeUPtr(*this);
}

// =============================================================================
// (public)
int ListRef::count(Element refBase) const
{
    if (refBase.isNull()) { return -1; }

    Element listBase = m_listBaseRef->getTarget(refBase);

    if (listBase.isNull()) { return 0; }

    int nb = 0;
    Element element = listBase.firstChild(m_tagName);
    while (!element.isNull()) {
        nb++;
        element = element.nextSibling(m_tagName);
    }
    return nb;
}

// =============================================================================
// (public)
int ListRef::indexOf(Element refBase, Element refElement) const
{
    if (refBase.isNull()) { return -1; }

    Element listBase = m_listBaseRef->getTarget(refBase);

    if (listBase.isNull()) { return -1; }

    int index = 0;
    Element element = listBase.firstChild(m_tagName);
    while (!element.isNull()) {
        if (m_subRef->getTarget(element) == refElement) {
            return index;
        }
        index++;
        element = element.nextSibling(m_tagName);
    }
    // The child node was not found
    return -1;
}

// =============================================================================
// (public)
Element ListRef::at(Element refBase, int index) const
{
    if (refBase.isNull()) { return Element(); }

    Element listBase = m_listBaseRef->getTarget(refBase);

    if (listBase.isNull()) { return Element(); }

    int nb = 0;
    Element element = listBase.firstChild(m_tagName);
    while (!element.isNull() && nb < index) {
        nb++;
        element = element.nextSibling(m_tagName);
    }
    return m_subRef->getTarget(element, true);
}

// =============================================================================
// (public)
std::vector<Element> ListRef::list(Element refBase) const
{
    if (refBase.isNull()) { return std::vector<Element>(); }

    Element listBase = m_listBaseRef->getTarget(refBase);

    if (listBase.isNull()) { return std::vector<Element>(); }

    std::vector<Element> eList;
    Element element = listBase.firstChild(m_tagName);
    while (!element.isNull()) {
        eList.push_back(m_subRef->getTarget(element, true));
        element = element.nextSibling(m_tagName);
    }
    return eList;
}

// =============================================================================
// (public)
Element ListRef::first(Element refBase) const
{
    if (refBase.isNull()) { return Element(); }

    Element listBase = m_listBaseRef->getTarget(refBase);

    if (listBase.isNull()) { return Element(); }

    return m_subRef->getTarget(listBase.firstChild(m_tagName), true);
}

// =============================================================================
// (public)
Element ListRef::last(Element refBase) const
{
    if (refBase.isNull()) { return Element(); }

    Element listBase = m_listBaseRef->getTarget(refBase);

    if (listBase.isNull()) { return Element(); }

    return m_subRef->getTarget(listBase.lastChild(m_tagName), true);
}

// =============================================================================
// (public)
Element ListRef::next(Element element) const
{
    element = m_subRef->getSource(element);

    if (element.isNull()) { return Element(); }

    return m_subRef->getTarget(element.nextSibling(m_tagName));
}

// =============================================================================
// (public)
Element ListRef::previous(Element element) const
{
    element = m_subRef->getSource(element);

    if (element.isNull()) { return Element(); }

    return m_subRef->getTarget(element.previousSibling(m_tagName));
}

// =============================================================================
// (public)
Element ListRef::insert(Element refBase, int index) const
{
    if (refBase.isNull()) { return Element(); }

    Element listBase = m_listBaseRef->getTarget(refBase, true);

    if (listBase.isNull()) { return Element(); }

    int nb = 1;
    Element refElement = listBase.firstChild(m_tagName);
    while (!refElement.isNull() && nb < index) {
        nb++;
        refElement = refElement.nextSibling(m_tagName);
    }

    if (refElement.isNull()) {
        if (index > 0) {
            // The index of the new element is out of range and therefore not created
            return Element();
        }
        // No elements already exists so the new element is appended
        refElement = listBase.appendChild(m_tagName);
    } else {
        if (index == 0) {
            // The new element has to be inserted before existing elements
            // so insert before has to be used
            refElement = listBase.insertBefore(m_tagName, refElement);
        } else {
            // The new element is inserted after the
            refElement = listBase.insertAfter(m_tagName, refElement);
        }
    }

    return m_subRef->getTarget(refElement, true);
}

// =============================================================================
// (public)
Element ListRef::insertBefore(Element refBase, Element refElement) const
{
    if (refBase.isNull()) { return Element(); }

    Element listBase = m_listBaseRef->getTarget(refBase);

    if (listBase.isNull()) { return Element(); }

    Element element;
    if (listBase == refElement.parentElement()) {
        element = listBase.insertBefore(m_tagName, refElement);
    } else {
        refElement = m_subRef->getSource(refElement);

        if (refElement.isNull()) { return Element(); }

        if (listBase == refElement.parentElement()) {
            element = listBase.insertBefore(m_tagName, refElement);
        } else { return Element(); }
    }

    return m_subRef->getTarget(element, true);
}

// =============================================================================
// (public)
Element ListRef::insertAfter(Element refBase, Element refElement) const
{
    if (refBase.isNull()) { return Element(); }

    Element listBase = m_listBaseRef->getTarget(refBase);

    if (listBase.isNull()) { return Element(); }

    Element element;
    if (listBase == refElement.parentElement()) {
        element = listBase.insertAfter(m_tagName, refElement);
    } else {
        refElement = m_subRef->getSource(refElement);

        if (refElement.isNull()) { return Element(); }

        if (listBase == refElement.parentElement()) {
            element = listBase.insertAfter(m_tagName, refElement);
        } else { return Element(); }
    }

    return m_subRef->getTarget(element, true);
}

// =============================================================================
// (public)
Element ListRef::clone(Element refBase, int index, Element cloneElement) const
{
    Element newElement = insert(refBase, index);
    if (!newElement.isNull()) {
        newElement.cloneElement(cloneElement);
    }
    return newElement;
}

// =============================================================================
// (public)
Element ListRef::cloneBefore(Element refBase, Element refElement, Element cloneElement) const
{
    Element newElement = insertBefore(refBase, refElement);
    if (!newElement.isNull()) {
        newElement.cloneElement(cloneElement);
    }
    return newElement;
}

// =============================================================================
// (public)
Element ListRef::cloneAfter(Element refBase, Element refElement, Element cloneElement) const
{
    Element newElement = insertAfter(refBase, refElement);
    if (!newElement.isNull()) {
        newElement.cloneElement(cloneElement);
    }
    return newElement;
}

// =============================================================================
// (public)
Element ListRef::move(Element refBase, int index, Element moveElement) const
{
    // Adjust the index to take into account that the moveElement is moved away
    int moveIndex = indexOf(refBase, moveElement);
    if (moveIndex >= 0 && moveIndex < index) {
        index++;
    }

    Element tempElement = insert(refBase, index);
    if (!tempElement.isNull()) {
        Element parent = tempElement.parentElement();
        Element newElement = parent.moveAfter(moveElement, tempElement);
        parent.removeChild(tempElement);
        return newElement;
    }
    return Element();
}

// =============================================================================
// (public)
Element ListRef::moveBefore(Element refBase, Element refElement, Element moveElement) const
{
    Element tempElement = insertBefore(refBase, refElement);
    if (!tempElement.isNull()) {
        Element parent = tempElement.parentElement();
        Element newElement = parent.moveAfter(moveElement, tempElement);
        parent.removeChild(tempElement);
        return newElement;
    }
    return Element();
}

// =============================================================================
// (public)
Element ListRef::moveAfter(Element refBase, Element refElement, Element moveElement) const
{
    Element tempElement = insertAfter(refBase, refElement);
    if (!tempElement.isNull()) {
        Element parent = tempElement.parentElement();
        Element newElement = parent.moveAfter(moveElement, tempElement);
        parent.removeChild(tempElement);
        return newElement;
    }
    return Element();
}

// =============================================================================
// (public)
bool ListRef::remove(Element refBase, Element refElement) const
{
    if (refBase.isNull()) { return false; }

    Element listBase = m_listBaseRef->getTarget(refBase);

    if (listBase.isNull()) { return false; }

    if (listBase == refElement.parentElement()) {
        return listBase.removeChild(refElement);
    } else {
        refElement = m_subRef->getSource(refElement);

        if (refElement.isNull()) { return false; }

        if (listBase == refElement.parentElement()) {
            return listBase.removeChild(refElement);
        }
    }

    return false;
}

// =============================================================================
// (public)
bool ListRef::remove(Element &refBase, int index) const
{
    if (refBase.isNull()) { return false; }

    Element listBase = m_listBaseRef->getTarget(refBase);

    if (listBase.isNull()) { return false; }

    int nb = 0;
    Element refElement = listBase.firstChild(m_tagName);
    while (!refElement.isNull() && nb < index) {
        nb++;
        refElement = refElement.nextSibling(m_tagName);
    }

    if (!refElement.isNull()) {
        return listBase.removeChild(refElement);
    }

    return false;
}

// =============================================================================
// (public)
Element ListRef::invertedAt(Element refElement) const
{
    refElement = m_subRef->getSource(refElement);

    if (refElement.isNull()) { return Element(); }
    if (refElement.compareTagName(m_tagName) != 0) { return Element(); }

    return m_listBaseRef->getSource(refElement.parentElement());
}

// =============================================================================
// (public)
void ListRef::setListBaseRef(RefUPtr value)
{
    m_listBaseRef = std::move(value);
}

// =============================================================================
// (public)
void ListRef::setTagName(const std::string &value)
{
    assert(!value.empty());
    m_tagName = value;
}

// =============================================================================
// (public)
void ListRef::setSubRef(ChildRefGroupUPtr value)
{
    m_subRef = std::move(value);
}

} // namespace XML
} // namespace Oak

#endif // XML_BACKEND
