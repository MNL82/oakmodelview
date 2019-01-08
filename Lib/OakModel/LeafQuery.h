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


namespace Oak::Model {

class LeafQuery;
typedef std::shared_ptr<LeafQuery> LeafQuerySPtr;
typedef std::weak_ptr<LeafQuery> LeafQueryWPtr;

// =============================================================================
// Class definition
// =============================================================================
class LeafQuery
{
protected:
    LeafQuery(const std::string &leafName = "");
    LeafQuery(ItemQueryUPtr itemQueryUPtr, const std::string &leafName = "");

public:
    ~LeafQuery();

    const std::string &valueName() const;
    LeafQuerySPtr setValueName(const std::string &leafName);

    int count(const Item &item);

    const Leaf &leaf(const Item &item, int index) const;

    void getValueList(const Item &item, std::vector<UnionValue> &valueList) const;
    std::vector<UnionValue> valueList(const Item &item) const;

    void getValue(const Item &item, int index, UnionValue value) const;


    template<typename T>
    T value(const Item &item, int index) const;

    template<typename T>
    std::vector<T> toValueList(const Item &item);

    std::vector<Item> toItemList(const Item &item);

    const ItemQuery &itemQuery() const;

    static LeafQuerySPtr create(const std::string &valueName = "");
    static LeafQuerySPtr create(ItemQueryUPtr itemQueryUPtr, const std::string &leafName = "");

protected:
    std::string m_leafName;
    ItemQueryUPtr m_itemQueryPtr = ItemQueryUPtr();

    LeafQueryWPtr m_thisWPtr;

public:
    // Iterator navigation implementation
    class Iterator : public ItemQuery::Iterator {

    public:
        Iterator(const LeafQuery &leafQuery, const Item *refItem = nullptr);

        virtual ~Iterator() override;

        void getValue(UnionValue value) const;

        template<typename T>
        T value() const;

        const Leaf &leaf() const;

    protected:
        const LeafQuery *m_leafQuery;

        friend class LeafQuery;
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
T LeafQuery::value(const Item &item, int index) const
{
    assert(!m_leafName.empty());

    if (m_itemQueryPtr) {

        int i = 0;
        auto it = m_itemQueryPtr->iterator(item);
        while(it->next()) {
            if (i == index) {
                auto tempItem = it->item();
                if (tempItem.hasLeaf(m_leafName)) {
                    return item.leaf(m_leafName).value<T>();
                }
                return T();
            }
            i++;
        }
        assert(false);
    } else {
        assert(index == 0);
        if (item.hasLeaf(m_leafName)) {
            return item.leaf(m_leafName).value<T>();
        }
    }
    return T();
}

// =============================================================================
// (public)
template<typename T>
std::vector<T> LeafQuery::toValueList(const Item &item)
{
    assert(!m_leafName.empty());

    std::vector<T> valueList;
    if (m_itemQueryPtr) {
        auto it = m_itemQueryPtr->iterator(item);
        while (it->next()) {
            Item tempItem = it->item();
            if (tempItem.hasLeaf(m_leafName)) {
                valueList.push_back(tempItem.leaf(m_leafName).value<T>());
            }
        }
    } else {
        if (item.hasLeaf(m_leafName)) {
            valueList.push_back(item.leaf(m_leafName).value<T>());
        }
    }

    return valueList;
}

// =============================================================================
// (public)
template<typename T>
T LeafQuery::Iterator::value() const
{
    return leaf().value<T>();
}

} // namespace Oak::Model

