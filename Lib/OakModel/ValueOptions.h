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

#include "VariantRef.h"
#include <algorithm>

namespace Oak {
namespace Model {

// =============================================================================
// Class definition
// =============================================================================
class ValueOptions
{
public:
    ValueOptions();
    ValueOptions(const ValueOptions &copy);

    bool isUsed() const;
    template<typename T>
    bool getOptions(std::vector<T>& options, bool allowConversion = false, ConversionSPtr conversion = ConversionSPtr()) const;
    //bool getOptions(std::vector<VariantCRef>& options) const;

    static const ValueOptions& empty();

protected:
    std::vector<Variant> m_options;
    std::vector<Variant> m_excluded;

    friend class ValueDefinition;
    friend class ValueDefinitionBuilder;
};

// =============================================================================
// (public)
template<typename T>
bool ValueOptions::getOptions(std::vector<T>& options, bool allowConversion, ConversionSPtr conversion) const
{
    options.resize(m_options.size());
    if (m_options.empty()) { return false; }

    // Add static options
    for (int i = 0; i < m_options.size(); i++)
    {
        m_options.at(i).get(options[i], allowConversion, conversion.get());
    }

    //m_queryBase->toList

    // Remove excluded options
    std::vector<T>::const_iterator it;
    T o;
    for (const Variant & option: m_excluded)
    {
        if (option.get(o, allowConversion, conversion.get())) {
            it = std::find(options.begin(), options.end(), o);
            if (it != options.end()) {
                options.erase(it);
            }
        }

    }

    std::sort(options.begin(), options.end());

    return true;
}

} // namespace Model
} // namespace Oak
