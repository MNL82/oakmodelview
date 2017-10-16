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

#include <string>

namespace Oak {
namespace Model {

// =============================================================================
// Class definition
// =============================================================================
class ValueSettings
{
public:
    ValueSettings();

    ValueSettings& operator=(const ValueSettings& copy);

    bool optionsOnly() const;
    void setOptionsOnly(bool value);

    bool hasUnit() const;
    const std::string& unit() const;
    void setUnit(const std::string value);

    bool unique() const;
    void setUnique(bool value);
    bool isUniqueSet() const;

    bool required() const;
    void setRequired(bool value);
    bool isRequiredSet() const;

private:
    // x < 0 => false
    // x > 0 => true
    // x == 0 => undefined (default is false)
    int m_unique = 0;
    int m_required = 0;

    bool m_optionsOnly = false;
    std::string m_unit;
};

} // namespace Model
} // namespace Oak
