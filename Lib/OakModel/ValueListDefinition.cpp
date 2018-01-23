/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ValueListDefinition.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
ValueListDefinition::ValueListDefinition(const UnionRef &valueTemplate)
    : ValueDefinition(valueTemplate)
{
}

// =============================================================================
// (public)
ValueListDefinition::ValueListDefinition(const ValueListDefinition &copy)
    : ValueDefinition(copy)
{
}

// =============================================================================
// (public)
ValueListDefinition::ValueListDefinition(ValueListDefinition&& move)
    : ValueDefinition(std::move(move))
{
}

// =============================================================================
// (public)
ValueDefinitionUPtr ValueListDefinition::copy() const
{
    return MakeUPtr(*this);
}

// =============================================================================
// (public)
ValueListDefinitionUPtr ValueListDefinition::copyList() const
{
    return MakeUPtr(*this);
}

} // namespace Model
} // namespace Oak
