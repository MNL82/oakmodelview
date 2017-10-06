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
#include <memory>

namespace Oak {
namespace XML {

class ValueRef;
typedef std::unique_ptr<ValueRef> ValueRefUPtr;

// =============================================================================
// Class definition
// =============================================================================
class ValueRef
{
public:
    ValueRef(std::string attName = std::string(), RefUPtr elRef = Ref::MakeUPtr());
    ValueRef(const ValueRef &copy);
    ValueRef(ValueRef &&move);

    ValueRef& operator=(const ValueRef &copy);
    ValueRef& operator=(ValueRef&& move);

    ValueRefUPtr copy() const;

    bool hasValue(Element baseElement);
    int compareValue(Element baseElement, const std::string& value) const;

    void getValue(Element baseElement, std::string& value) const;
    bool setValue(Element baseElement, const std::string& value) const;

    bool clearValue(Element baseElement) const;

    const std::string& attributeName() const { return m_attributeName; }
    void setAttributeName(std::string attName);

    Ref* elementRef() const { return m_elementRef.get(); }
    template<typename T>
    T* elementRef() const { return dynamic_cast<T*>(m_elementRef.get()); }

    void setElementRef(RefUPtr elRef);

    template<class... _Types> inline
    static typename ValueRefUPtr MakeUPtr(_Types&&... _Args)
    {
        return (ValueRefUPtr(new ValueRef(_STD forward<_Types>(_Args)...)));
    }

protected:

    RefUPtr m_elementRef;
    /*!
     * \brief Name of the attribute that contains the value.
     * If empty the value is stored as element text.
    */
    std::string m_attributeName;
};

} // namespace XML
} // namespace Oak

#endif // XML_BACKEND
