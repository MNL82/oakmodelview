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

#include "ValueDefinition.h"

namespace Oak {
namespace Model {

class ValueListDefinition;
typedef std::unique_ptr<ValueListDefinition> ValueListDefinitionUPtr;

// =============================================================================
// Class definition
// =============================================================================
class ValueListDefinition : public ValueDefinition
{
public:
    ValueListDefinition(const UnionRef& valueTemplate);

    ValueListDefinition(const ValueListDefinition &copy);
    ValueListDefinition(ValueListDefinition &&move);

    virtual ValueDefinitionUPtr copy() const override;
    virtual ValueListDefinitionUPtr copyList() const;

    template<class... _Types> inline
    static typename ValueListDefinitionUPtr MakeUPtr(_Types&&... _Args)
    {
        return (ValueListDefinitionUPtr(new ValueListDefinition(_STD forward<_Types>(_Args)...)));
    }
};

} // namespace Model
} // namespace Oak
