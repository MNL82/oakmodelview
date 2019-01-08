/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ValueOptions.h"

#include "UnionRef.h"
#include "LeafQuery.h"


namespace Oak::Model {

// =============================================================================
// (public)
ValueOptions::ValueOptions()
{

}

// =============================================================================
// (public)
ValueOptions::ValueOptions(const ValueOptions &copy)
{
    m_options.assign(copy.m_options.begin(), copy.m_options.end());
    m_excluded.assign(copy.m_excluded.begin(), copy.m_excluded.end());
}

// =============================================================================
// (public)
bool ValueOptions::isUsed() const
{
    return !m_options.empty() || m_query;
}

// =============================================================================
// (public)
bool ValueOptions::getOptions(std::vector<UnionValue> &options, const Node *node, bool allowConversion, ConversionSPtr conversion) const
{
    options.resize(m_options.size());
    if (!isUsed()) { return false; }

    // Add static options
    for (std::vector<UnionValue>::size_type i = 0; i < m_options.size(); i++)
    {
        options[i] = m_options[i];
    }

    std::vector<UnionValue>::const_iterator it;
    if (node && m_query) {
        std::vector<UnionValue> oList = m_query->valueList(*node);
        for(const UnionValue &option: oList)
        {
            it = std::find(options.begin(), options.end(), option);
            if (it == options.end()) {
                options.push_back(option);
            }
        }
    }

    if (node && m_queryExcluded) {
        std::vector<UnionValue> oList = m_queryExcluded->valueList(*node);
        for(const UnionValue &option: oList)
        {
            it = std::find(options.begin(), options.end(), option);
            if (it != options.end()) {
                options.erase(it);
            }
        }
    }

    // Remove excluded options
    UnionValue option;
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

// =============================================================================
// (public)
bool ValueOptions::hasQuery() const
{
    return m_query.operator bool();
}

// =============================================================================
// (public)
bool ValueOptions::hasQueryExcluded() const
{
    return m_queryExcluded.operator bool();
}

// =============================================================================
// (public)
const LeafQuery *ValueOptions::query() const
{
    return m_query.get();
}

// =============================================================================
// (public)
const LeafQuery *ValueOptions::queryExcluded() const
{
    return m_queryExcluded.get();
}

// =============================================================================
// (public)
const ValueOptions &ValueOptions::empty()
{
    static ValueOptions vo;
    return vo;
}

//// =============================================================================
//// (public)
//bool ValueOptions::getOptions(std::vector<VariantCRef>& options) const
//{
//    options.clear();
//    if (m_options.empty()) { return false; }

//    for (const auto& option: m_options) {
//        options.push_back(option);
//    }
//    return true;
//}

} // namespace Oak::Model

