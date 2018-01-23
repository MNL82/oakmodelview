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

namespace Oak {
namespace Model {

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

} // namespace Model
} // namespace Oak
