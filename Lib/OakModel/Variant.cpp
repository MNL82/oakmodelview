/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "Variant.h"

#include "VariantRef.h"
#include "VariantCRef.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
Variant::Variant()
{

}

// =============================================================================
// (public)
Variant::Variant(const char* value)
    : m_value(std::string(value))
{

}

// =============================================================================
// (public)
Variant::Variant(const VariantRef& variant)
    : m_value(boost::apply_visitor(SetVariantVisitor(), variant.m_value))
{

}

// =============================================================================
// (public)
Variant::Variant(const VariantCRef& variant)
    : m_value(boost::apply_visitor(SetVariantVisitor(), variant.m_value))
{

}

// =============================================================================
// (public)
Variant::Variant(const Variant& copy)
    : m_value(copy.m_value)
{

}

// =============================================================================
// (public)
Variant::Variant(Variant&& move)
    : m_value(std::move(move.m_value))
{

}

// =============================================================================
// (public)
Variant& Variant::operator=(const VariantCRef &variant)
{
    m_value = boost::apply_visitor(SetVariantVisitor(), variant.m_value);
    return *this;
}

// =============================================================================
// (public)
Variant& Variant::operator=(const VariantRef &variant)
{
    m_value = boost::apply_visitor(SetVariantVisitor(), variant.m_value);
    return *this;
}

// =============================================================================
// (public)
Variant& Variant::operator=(const Variant &variant)
{
    m_value = variant.m_value;
    return *this;
}

// =============================================================================
// (public)
bool Variant::operator==(const VariantCRef& value) const
{
    return boost::apply_visitor(EqualVisitor(), m_value, value.m_value);
}

// =============================================================================
// (public)
bool Variant::operator!=(const VariantCRef& value) const
{
    return !boost::apply_visitor(EqualVisitor(), m_value, value.m_value);
}

// =============================================================================
// (public)
bool Variant::isEqual(const VariantCRef& value, bool allowConversion, Conversion* properties) const
{
    return boost::apply_visitor(EqualVisitor(allowConversion, properties), m_value,  value.m_value);
}

// =============================================================================
// (public)
bool Variant::isNull() const
{
    return m_value.which() == 0;
}

// =============================================================================
// (public)
const type_info& Variant::typeId() const
{
    return m_value.type();
}

// =============================================================================
// (public)
int Variant::witch() const
{
    return m_value.which();
}

// =============================================================================
// (public)
bool Variant::canGet(VariantRef& value, bool allowConversion, Conversion* properties) const
{
    return boost::apply_visitor(CanConvertVisitor(allowConversion, properties), value.m_value, m_value);
}

// =============================================================================
// (public)
bool Variant::get(VariantRef& value, bool allowConversion, Conversion* properties) const
{
    return boost::apply_visitor(ConvertVisitor(allowConversion, properties), value.m_value, m_value);
}

// =============================================================================
// (public)
bool Variant::canSet(const VariantCRef& value, bool allowConversion, Conversion* properties)
{
    return boost::apply_visitor(CanConvertVisitor(allowConversion, properties), m_value, value.m_value);
}

// =============================================================================
// (public)
bool Variant::set(const VariantCRef& value, bool allowConversion, Conversion* properties)
{
    return boost::apply_visitor(ConvertVisitor(allowConversion, properties), m_value, value.m_value);
}

} // namespace Model
} // namespace Oak
