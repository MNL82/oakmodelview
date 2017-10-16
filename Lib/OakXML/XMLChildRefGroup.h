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

#include "XMLChildRef.h"
#include <vector>
#include <stdarg.h>
#include <memory>

namespace Oak {
namespace XML {

class ChildRefGroup;
typedef std::unique_ptr<ChildRefGroup> ChildRefGroupUPtr;

// =============================================================================
// Class definition
// =============================================================================
class ChildRefGroup : public Ref
{
public:
    ChildRefGroup(ChildRefUPtr eRef1 = ChildRefUPtr(), ChildRefUPtr eRef2 = ChildRefUPtr(), ChildRefUPtr eRef3 = ChildRefUPtr(), ChildRefUPtr eRef4 = ChildRefUPtr());
    ChildRefGroup(const ChildRefGroup& copy);
    ChildRefGroup(ChildRefGroup &&move);

    ChildRefGroup& operator=(const ChildRefGroup& copy);
    ChildRefGroup& operator=(ChildRefGroup&& move);

    virtual RefUPtr copy() const override;
    ChildRefGroupUPtr copyChildGroup() const;

    virtual Element getTarget(Element source, bool create = false) const override;
    virtual void clearTarget(Element source, bool onlyIfEmpty = true) const override;

    virtual Element getSource(Element target) const override;

    virtual bool isNull() const { return false; }

    bool empty() const { return m_references.empty(); }
    int count() const { return (int)m_references.size(); }
    ChildRef* at(int index) { return m_references.at(index).get(); }
    ChildRefUPtr take(int index) { return std::move(m_references.at(index)); }

    void add(ChildRefUPtr eRef);
    void clear();

    virtual const std::string& firstTagName() const override { return m_references.front()->firstTagName(); }
    virtual const std::string& lastTagName() const override { return m_references.back()->lastTagName(); }

    template<class... _Types> inline
    static typename ChildRefGroupUPtr MakeUPtr(_Types&&... _Args)
    {
        return (ChildRefGroupUPtr(new ChildRefGroup(_STD forward<_Types>(_Args)...)));
    }

protected:
    std::vector<ChildRefUPtr> m_references;

};

} // namespace XML
} // namespace Oak

#endif // XML_BACKEND
