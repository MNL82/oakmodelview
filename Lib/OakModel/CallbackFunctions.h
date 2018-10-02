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

#include <map>
#include <functional>

#include "Item.h"
#include "ItemIndex.h"

namespace Oak {
namespace Model {

// =============================================================================
// Class definition
// =============================================================================
class Callback
{
public:
    Callback();

    template<typename T>
    void add(T* funcObj, void (T::*func)(void))
    {
        if (funcObj == nullptr) {
            assert(false);
            return;
        }
        m_functionMap[funcObj] = std::bind(func, funcObj);
    }

    void remove(void* funcObj = nullptr);

    void trigger() const;

protected:
    std::map<void*, std::function<void(void)>> m_functionMap;
};

// =============================================================================
// Class definition
// =============================================================================
class Callback_ItemIntItemInt
{
public:
    Callback_ItemIntItemInt();

    template<typename T>
    void add(T* funcObj, void (T::*func)(const Item&, int, const Item&, int))
    {
        if (funcObj == nullptr) {
            assert(false);
            return;
        }
        m_functionMap[funcObj] = std::bind(func, funcObj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    }

    void remove(void* funcObj = nullptr);

    void trigger(const Item& sourceItem, int sourceIndex, const Item& targetItem, int targetIndex) const;

protected:
    std::map<void*, std::function<void(const Item&, int, const Item&, int)>> m_functionMap;
};

// =============================================================================
// Class definition
// =============================================================================
class Callback_ItemInt
{
public:
    Callback_ItemInt();

    template<typename T>
    void add(T* funcObj, void (T::*func)(const Item&, int))
    {
        if (funcObj == nullptr) {
            assert(false);
            return;
        }
        m_functionMap[funcObj] = std::bind(func, funcObj, std::placeholders::_1, std::placeholders::_2);
    }

    void remove(void* funcObj = nullptr);

    void trigger(const Item &parentItem, int index) const;

protected:
    std::map<void*, std::function<void(const Item&, int)>> m_functionMap;
};

// =============================================================================
// Class definition
// =============================================================================
class Callback_Item
{
public:
    Callback_Item();

    template<typename T>
    void add(T* funcObj, void (T::*func)(const Item&))
    {
        if (funcObj == nullptr) {
            assert(false);
            return;
        }
        m_functionMap[funcObj] = std::bind(func, funcObj, std::placeholders::_1);
    }

    void remove(void* funcObj = nullptr);

    void trigger(const Item &parentItem) const;

protected:
    std::map<void*, std::function<void(const Item&)>> m_functionMap;
};

// =============================================================================
// Class definition
// =============================================================================
class Callback_ItemIndex
{
public:
    Callback_ItemIndex();

    template<typename T>
    void add(T* funcObj, void (T::*func)(const ItemIndex&))
    {
        if (funcObj == nullptr) {
            assert(false);
            return;
        }
        m_functionMap[funcObj] = std::bind(func, funcObj, std::placeholders::_1);
    }

    void remove(void* funcObj = nullptr);

    void trigger(const ItemIndex &itemIndex) const;

protected:
    std::map<void*, std::function<void(const ItemIndex&)>> m_functionMap;
};

// =============================================================================
// Class definition
// =============================================================================
class Callback_ItemIndexItemIndex
{
public:
    Callback_ItemIndexItemIndex();

    template<typename T>
    void add(T* funcObj, void (T::*func)(const ItemIndex&, const ItemIndex&))
    {
        if (funcObj == nullptr) {
            assert(false);
            return;
        }
        m_functionMap[funcObj] = std::bind(func, funcObj, std::placeholders::_1, std::placeholders::_2);
    }

    void remove(void* funcObj = nullptr);

    void trigger(const ItemIndex &itemIndex1, const ItemIndex &itemIndex2) const;

protected:
    std::map<void*, std::function<void(const ItemIndex&, const ItemIndex&)>> m_functionMap;
};

// =============================================================================
// Class definition
// =============================================================================
class Callback_ItemIndexInt
{
public:
    Callback_ItemIndexInt();

    template<typename T>
    void add(T* funcObj, void (T::*func)(const ItemIndex&, int))
    {
        if (funcObj == nullptr) {
            assert(false);
            return;
        }
        m_functionMap[funcObj] = std::bind(func, funcObj, std::placeholders::_1, std::placeholders::_2);
    }

    void remove(void* funcObj = nullptr);

    void trigger(const ItemIndex &itemIndex, int index) const;

protected:
    std::map<void*, std::function<void(const ItemIndex&, int)>> m_functionMap;
};

} // namespace Model
} // namespace Oak
