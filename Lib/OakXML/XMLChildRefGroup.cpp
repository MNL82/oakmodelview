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

#include "XMLChildRefGroup.h"

namespace Oak::XML {

// =============================================================================
// (public)
ChildRefGroup::ChildRefGroup(ChildRefUPtr eRef1, ChildRefUPtr eRef2, ChildRefUPtr eRef3, ChildRefUPtr eRef4)
{
    if (eRef1) { m_references.push_back(std::move(eRef1)); }
    if (eRef2) { m_references.push_back(std::move(eRef2)); }
    if (eRef3) { m_references.push_back(std::move(eRef3)); }
    if (eRef4) { m_references.push_back(std::move(eRef4)); }
}

// =============================================================================
// (public)
ChildRefGroup::ChildRefGroup(const ChildRefGroup &copy)
    : Ref()
{
    *this = copy;
}

// =============================================================================
// (public)
ChildRefGroup::ChildRefGroup(ChildRefGroup&& move)
    : Ref()
{
    *this = std::move(move);
}

// =============================================================================
// (public)
ChildRefGroup& ChildRefGroup::operator=(const ChildRefGroup &copy)
{
    m_references.clear();
    for (const auto& ref: copy.m_references)
    {
        m_references.push_back(ref->copyChild());
    }
    return *this;
}

// =============================================================================
// (public)
ChildRefGroup& ChildRefGroup::operator=(ChildRefGroup &&move)
{
    m_references = std::move(move.m_references);
    return *this;
}

// =============================================================================
// (public)
RefUPtr ChildRefGroup::copy() const
{
    return MakeUPtr(*this);
}

// =============================================================================
// (public)
ChildRefGroupUPtr ChildRefGroup::copyChildGroup() const
{
    return MakeUPtr(*this);
}

// =============================================================================
// (public)
Element ChildRefGroup::getTarget(Element source, bool create) const
{
    Element refElement = source;
    for (const auto& eRef: m_references)
    {
        refElement = eRef->getTarget(refElement, create);
        if (refElement.isNull()) { return Element(); }
    }

    return refElement;
}

// =============================================================================
// (public)
void ChildRefGroup::clearTarget(Element source, bool onlyIfEmpty) const
{
    if (source.isNull()) { return; }

    Element refElement = source;

    if (onlyIfEmpty) {
        // The clearRefElement() function is called for each ElementRef in m_references, with a valid Element.
        // The last one have to be called first so a list of valid elements is first created.
        std::vector<Element> elementList;
        elementList.push_back(source);

        Element element = source;
        auto it = m_references.begin();
        while (it != m_references.end())
        {
            element = it->get()->getTarget(element);
            if (element.isNull()) { break; }
            elementList.push_back(element);
            it++;
        }
        it--;
        // The iterator ends up pointing to the last element reference that gave a valid reference

        // Now calls that clearIfEmpty function on all valid element references, the last one first
        while (elementList.size() > 1) {
            elementList.pop_back();
            it->get()->clearTarget(elementList.back(), onlyIfEmpty);
        }
    } else {
        for (const auto& eRef: m_references)
        {
            eRef->clearTarget(refElement, onlyIfEmpty);
            refElement = eRef->getTarget(refElement);
            if (refElement.isNull()) { return; }
        }
    }

}

// =============================================================================
// (public)
Element ChildRefGroup::getSource(Element target) const
{
    auto it = m_references.end();
    while (it != m_references.begin() && !target.isNull()) {
        it--;
        target = (*it)->getSource(target);
    }
    return target;
}

// =============================================================================
// (public)
void ChildRefGroup::add(ChildRefUPtr eRef)
{
    m_references.push_back(std::move(eRef));
}

// =============================================================================
// (public)
void ChildRefGroup::clear()
{
    m_references.clear();
}

} // namespace Oak::XML

#endif // XML_BACKEND
