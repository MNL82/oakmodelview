/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "VariantRef.h"

#include "Variant.h"
#include "VariantCRef.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
VariantRef::VariantRef(Variant& variant)
    : m_value(boost::apply_visitor(SetVariantRefVisitor(), variant.m_value))
{

}

// =============================================================================
// (public)
VariantRef::VariantRef(VariantRef& copy)
    : m_value(copy.m_value)
{

}

// =============================================================================
// (public)
bool VariantRef::operator==(const VariantCRef& value) const
{
    return boost::apply_visitor(EqualVisitor(), m_value, value.m_value);
}
// =============================================================================
// (public)
bool VariantRef::operator!=(const VariantCRef& value) const
{
    return !boost::apply_visitor(EqualVisitor(), m_value, value.m_value);
}

// =============================================================================
// (public)
bool VariantRef::isEqual(const VariantCRef& value, bool allowConversion, Conversion* properties) const
{
    return boost::apply_visitor(EqualVisitor(allowConversion, properties), m_value,  value.m_value);
}

// =============================================================================
// (public)
bool VariantRef::isNull() const
{
    return m_value.which() == 0;
}

// =============================================================================
// (public)
const type_info& VariantRef::typeId() const
{
    return m_value.type();
}

// =============================================================================
// (public)
int VariantRef::witch() const
{
    return m_value.which();
}

// =============================================================================
// (public)
bool VariantRef::canGet(VariantRef& value, bool allowConversion, Conversion* properties) const
{
    return boost::apply_visitor(CanConvertVisitor(allowConversion, properties), value.m_value, m_value);
}

// =============================================================================
// (public)
bool VariantRef::get(VariantRef& value, bool allowConversion, Conversion* properties) const
{
    return boost::apply_visitor(ConvertVisitor(allowConversion, properties), value.m_value, m_value);
}

// =============================================================================
// (public)
bool VariantRef::canSet(const VariantCRef& value, bool allowConversion, Conversion* properties)
{
    return boost::apply_visitor(CanConvertVisitor(allowConversion, properties), m_value, value.m_value);
}

// =============================================================================
// (public)
bool VariantRef::set(const VariantCRef& value, bool allowConversion, Conversion* properties)
{
    return boost::apply_visitor(ConvertVisitor(allowConversion, properties), m_value, value.m_value);
}

// =============================================================================
// (public)
Variant VariantRef::value() const
{
    return Variant(*this);
}

} // namespace Model
} // namespace Oak
