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
#include "UnionValue.h"

#include <algorithm>

namespace Oak {
namespace Model {

class Item;
class EntryQuery;
typedef std::shared_ptr<EntryQuery> EntryQuerySPtr;
typedef std::weak_ptr<EntryQuery> EntryQueryWPtr;

// =============================================================================
// Class definition
// =============================================================================
class ValueOptions
{
public:
    ValueOptions();
    ValueOptions(const ValueOptions &copy);

    bool isUsed() const;

    bool getOptions(std::vector<UnionValue>& options, const Item *item, bool allowConversion, ConversionSPtr conversion) const;

    template<typename T>
    bool getOptions(std::vector<T>& options, const Item *item, bool allowConversion = false, ConversionSPtr conversion = ConversionSPtr()) const;

    static const ValueOptions& empty();

protected:
    std::vector<UnionValue> m_options;
    std::vector<UnionValue> m_excluded;

    EntryQuerySPtr m_query;
    EntryQuerySPtr m_queryExcluded;

    friend class ValueDef;
    friend class ValueDefBuilder;
};

// =============================================================================
// (public)
template<typename T>
bool ValueOptions::getOptions(std::vector<T>& options, const Item *item, bool allowConversion, ConversionSPtr conversion) const
{
    std::vector<UnionValue> oList;
    getOptions(oList, item, allowConversion, conversion);
    options.resize(oList.size());
    for (std::vector<UnionValue>::size_type i = 0; i < oList.size(); i++)
    {
        oList[i].get(options[i], allowConversion, conversion.get());
    }
    return true;
}

} // namespace Model
} // namespace Oak
