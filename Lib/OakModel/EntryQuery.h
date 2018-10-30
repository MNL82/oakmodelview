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

class EntryQuery;
typedef std::shared_ptr<EntryQuery> EntryQuerySPtr;
typedef std::weak_ptr<EntryQuery> EntryQueryWPtr;

// =============================================================================
// Class definition
// =============================================================================
class EntryQuery
{
protected:
    EntryQuery(const std::string &valueName = "");
    EntryQuery(ItemQueryUPtr itemQueryUPtr, const std::string &entryName = "");

public:
    ~EntryQuery();

    const std::string &valueName() const;
    EntryQuerySPtr setValueName(const std::string &entryName);

    int count(const Item &item);

    const Entry &entry(const Item &item, int index) const;

    void getValueList(const Item &item, std::vector<UnionValue> &valueList) const;
    std::vector<UnionValue> valueList(const Item &item) const;

    void getValue(const Item &item, int index, UnionValue value) const;


    template<typename T>
    T value(const Item &item, int index) const;

    template<typename T>
    std::vector<T> toValueList(const Item &item);

    std::vector<Item> toItemList(const Item &item);

    const ItemQuery &itemQuery() const;

    static EntryQuerySPtr create(const std::string &valueName = "");
    static EntryQuerySPtr create(ItemQueryUPtr itemQueryUPtr, const std::string &valueName = "");

protected:
    std::string m_entryName;
    ItemQueryUPtr m_itemQueryPtr = ItemQueryUPtr();

    EntryQueryWPtr m_thisWPtr;

public:
    // Iterator navigation implementation
    class Iterator : public ItemQuery::Iterator {

    public:
        Iterator(const EntryQuery &entryQuery, const Item *refItem = nullptr);

        virtual ~Iterator() override;

        void getValue(UnionValue value) const;

        template<typename T>
        T value() const;

        const Entry &entry() const;

    protected:
        const EntryQuery *m_entryQuery;

        friend class EntryQuery;
    };
    typedef std::unique_ptr<Iterator> IteratorUPtr;

    IteratorUPtr iterator(const Item &refItem) const;
//    IteratorUPtr begin(const Item &refItem) const;
//    IteratorUPtr rBegin(const Item &refItem) const;

    friend class Iterator;
};

// =============================================================================
// (public)
template<typename T>
T EntryQuery::value(const Item &item, int index) const
{
    assert(!m_entryName.empty());

    if (m_itemQueryPtr) {

        int i = 0;
        auto it = m_itemQueryPtr->iterator(item);
        while(it->next()) {
            if (i == index) {
                auto tempItem = it->item();
                if (tempItem.hasEntry(m_entryName)) {
                    return item.entry(m_entryName).value<T>();
                }
                return T();
            }
            i++;
        }
        assert(false);
    } else {
        assert(index == 0);
        if (item.hasEntry(m_entryName)) {
            return item.entry(m_entryName).value<T>();
        }
    }
    return T();
}

// =============================================================================
// (public)
template<typename T>
std::vector<T> EntryQuery::toValueList(const Item &item)
{
    assert(!m_entryName.empty());

    std::vector<T> valueList;
    if (m_itemQueryPtr) {
        auto it = m_itemQueryPtr->iterator(item);
        while (it->next()) {
            Item tempItem = it->item();
            if (tempItem.hasEntry(m_entryName)) {
                valueList.push_back(tempItem.entry(m_entryName).value<T>());
            }
        }
    } else {
        if (item.hasEntry(m_entryName)) {
            valueList.push_back(item.entry(m_entryName).value<T>());
        }
    }

    return valueList;
}

// =============================================================================
// (public)
template<typename T>
T EntryQuery::Iterator::value() const
{
    return entry().value<T>();
}

} // namespace Model
} // namespace Oak
