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

#include "ItemQuery.h"

namespace Oak {
namespace Model {

class ValueQuery;
typedef std::shared_ptr<ValueQuery> ValueQuerySPtr;
typedef std::weak_ptr<ValueQuery> ValueQueryWPtr;

// =============================================================================
// Class definition
// =============================================================================
class ValueQuery
{
protected:
    ValueQuery(const std::string &valueName = "");
    ValueQuery(ItemQueryUPtr itemQueryUPtr, const std::string &valueName = "");

public:
    ~ValueQuery();

    ValueQuerySPtr setValueName(const std::string &valueName);

    int count(const Item &item);

    void addValueList(const Item &item, std::vector<UnionValue> &valueList) const;
    std::vector<UnionValue> getValueList(const Item &item) const;

    template<typename T>
    std::vector<T> toValueList(const Item &item);

    std::vector<Item> toItemList(const Item &item);

    static ValueQuerySPtr create(const std::string &valueName = "");
    static ValueQuerySPtr create(ItemQueryUPtr itemQueryUPtr, const std::string &valueName = "");

protected:
    std::string m_valueName;
    ItemQueryUPtr m_itemQueryPtr = ItemQueryUPtr();

    ValueQueryWPtr m_thisWPtr;
};

// =============================================================================
// (public)
template<typename T>
std::vector<T> ValueQuery::toValueList(const Item &item)
{
    assert(!m_valueName.empty());

    std::vector<T> valueList;
    if (m_itemQueryPtr) {
        m_itemQueryPtr->reset(item);
        while(m_itemQueryPtr->moveNext()) {
            Item tempItem = m_itemQueryPtr->current();
            if (tempItem.hasValue(m_valueName)) {
                valueList.push_back(tempItem.value(m_valueName).value<T>());
            }
        }
    } else {
        if (item.hasValue(m_valueName)) {
            valueList.push_back(item.value(m_valueName).value<T>());
        }
    }

    return valueList;
}

} // namespace Model
} // namespace Oak
