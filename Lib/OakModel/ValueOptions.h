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

#include "UnionRef.h"
#include "QueryRef.h"

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
    bool getOptions(std::vector<T>& options, const Item *item, bool allowConversion = false, ConversionSPtr conversion = ConversionSPtr()) const;
    //bool getOptions(std::vector<VariantCRef>& options) const;

    static const ValueOptions& empty();

protected:
    std::vector<UnionValue> m_options;
    std::vector<UnionValue> m_excluded;

    QueryRefSPtr m_query;
    QueryRefSPtr m_queryExcluded;

    friend class ValueDef;
    friend class ValueDefBuilder;
};

// =============================================================================
// (public)
template<typename T>
bool ValueOptions::getOptions(std::vector<T>& options, const Item *item, bool allowConversion, ConversionSPtr conversion) const
{
    options.resize(m_options.size());
    if (!isUsed()) { return false; }

    // Add static options
    for (int i = 0; i < m_options.size(); i++)
    {
        m_options.at(i).get(options[i], allowConversion, conversion.get());
    }

    std::vector<T>::const_iterator it;
    if (item && m_query) {
        std::vector<T> oList = m_query->toValueList<T>(*item);
        for(const T &option: oList)
        {
            it = std::find(options.begin(), options.end(), option);
            if (it == options.end()) {
                options.push_back(option);
            }
        }
    }

    if (item && m_queryExcluded) {
        std::vector<T> oList = m_queryExcluded->toValueList<T>(*item);
        for(const T &option: oList)
        {
            it = std::find(options.begin(), options.end(), option);
            if (it != options.end()) {
                options.erase(it);
            }
        }
    }


    // Remove excluded options
    T option;
    for (const UnionValue& vo: m_excluded)
    {
        if (vo.get(option, allowConversion, conversion.get())) {
            it = std::find(options.begin(), options.end(), option);
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
