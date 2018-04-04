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
#include <map>

#include "UnionValue.h"

namespace Oak {
namespace Model {

#define OPTION_ONLY     "OptionsOnly"
#define UNIT            "Unit"
#define UNIQUE          "Unique"
#define REQUIRED        "Required"

// =============================================================================
// Class definition
// =============================================================================
class ValueSettings
{
public:
    ValueSettings();

    ValueSettings& operator=(const ValueSettings& copy);

    const UnionValue &value(const std::string &settingName, const UnionValue &defaultValue = UnionValue()) const;
    void setValue(const std::string &settingName, const UnionRef &value);

private:
    // x < 0 => false
    // x > 0 => true
    // x == 0 => undefined (default is false)

    std::map<std::string, UnionValue> m_settingsMap;
};

} // namespace Model
} // namespace Oak
