/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "VariantCRef.h"

#include "Variant.h"
#include "VariantRef.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
VariantCRef::VariantCRef()
    : m_value(InvalidVariant())
{
}

#ifdef TDM_USE_CHAR_PTR_IN_VARIANT
// =============================================================================
// (public)
VariantCRef::VariantCRef(const char* value)
    : m_value(value)
{
}
#endif // TDM_USE_CHAR_PTR_IN_VARIANT

// =============================================================================
// (public)
VariantCRef::VariantCRef(const VariantCRef& copy)
    : m_value(copy.m_value)
{
}

// =============================================================================
// (public)
bool VariantCRef::operator<(const VariantCRef &value) const
{
    return boost::apply_visitor(SmallerVisitor(), m_value, value.m_value);
}

// =============================================================================
// (public)
VariantCRef::VariantCRef(const Variant& variant)
    : m_value(boost::apply_visitor(SetVariantCRefVisitor(), variant.m_value))
{
}

// =============================================================================
// (public)
VariantCRef::VariantCRef(const VariantRef& variant)
    : m_value(boost::apply_visitor(SetVariantCRefVisitor(), variant.m_value))
{
}

// =============================================================================
// (public)
bool VariantCRef::operator==(const VariantCRef& value) const
{
    return boost::apply_visitor(EqualVisitor(), m_value, value.m_value);
}

// =============================================================================
// (public)
bool VariantCRef::operator!=(const VariantCRef& value) const
{
    return !boost::apply_visitor(EqualVisitor(), m_value, value.m_value);
}

// =============================================================================
// (public)
bool VariantCRef::isEqual(const VariantCRef& value, bool allowConversion, Conversion* properties) const
{
    return boost::apply_visitor(EqualVisitor(allowConversion, properties), m_value,  value.m_value);
}

// =============================================================================
// (public)
bool VariantCRef::isBaseTypeEqual(const VariantCRef &value) const
{
    return boost::apply_visitor(TypeIdBaseEqualVisitor(), m_value, value.m_value);
}

// =============================================================================
// (public)
bool VariantCRef::isNull() const
{
    return m_value.which() == 0;
}

// =============================================================================
// (public)
const type_info& VariantCRef::typeId() const
{
    return m_value.type();
}

// =============================================================================
// (public)
int VariantCRef::witch() const
{
    return m_value.which();
}

// =============================================================================
// (public)
bool VariantCRef::canGet(VariantRef& value, bool allowConversion, Conversion* properties) const
{
    return boost::apply_visitor(CanConvertVisitor(allowConversion, properties), value.m_value, m_value);
}

// =============================================================================
// (public)
bool VariantCRef::get(VariantRef& value, bool allowConversion, Conversion* properties) const
{
    return boost::apply_visitor(ConvertVisitor(allowConversion, properties), value.m_value, m_value);
}

// =============================================================================
// (public)
Variant VariantCRef::value() const
{
    return Variant(*this);
}

} // namespace Model
} // namespace Oak
