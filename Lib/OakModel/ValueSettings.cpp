/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ValueSettings.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
ValueSettings::ValueSettings()
{

}

// =============================================================================
// (public)
ValueSettings& ValueSettings::operator=(const ValueSettings& copy)
{
    m_optionsOnly = copy.m_optionsOnly;
    return *this;
}

// =============================================================================
// (public)
bool ValueSettings::optionsOnly() const
{
    return m_optionsOnly;
}

// =============================================================================
// (public)
void ValueSettings::setOptionsOnly(bool value)
{
    // Todo: Notify about change?
    m_optionsOnly = value;
}

// =============================================================================
// (public)
bool ValueSettings::hasUnit() const
{
    return !m_unit.empty();
}

// =============================================================================
// (public)
const std::string &ValueSettings::unit() const
{
    return m_unit;
}

// =============================================================================
// (public)
void ValueSettings::setUnit(const std::string value)
{
    m_unit = value;
}

} // namespace Model
} // namespace Oak
