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

#include "Node.h"
#include "NodeIndex.h"


namespace Oak::Model {

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
class Callback_NodeIntNodeInt
{
public:
    Callback_NodeIntNodeInt();

    template<typename T>
    void add(T* funcObj, void (T::*func)(const Node&, int, const Node&, int))
    {
        if (funcObj == nullptr) {
            assert(false);
            return;
        }
        m_functionMap[funcObj] = std::bind(func, funcObj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    }

    void remove(void* funcObj = nullptr);

    void trigger(const Node& sourceNode, int sourceIndex, const Node& targetNode, int targetIndex) const;

protected:
    std::map<void*, std::function<void(const Node&, int, const Node&, int)>> m_functionMap;
};

// =============================================================================
// Class definition
// =============================================================================
class Callback_NodeInt
{
public:
    Callback_NodeInt();

    template<typename T>
    void add(T* funcObj, void (T::*func)(const Node&, int))
    {
        if (funcObj == nullptr) {
            assert(false);
            return;
        }
        m_functionMap[funcObj] = std::bind(func, funcObj, std::placeholders::_1, std::placeholders::_2);
    }

    void remove(void* funcObj = nullptr);

    void trigger(const Node &parentNode, int index) const;

protected:
    std::map<void*, std::function<void(const Node&, int)>> m_functionMap;
};

// =============================================================================
// Class definition
// =============================================================================
class Callback_Node
{
public:
    Callback_Node();

    template<typename T>
    void add(T* funcObj, void (T::*func)(const Node&))
    {
        if (funcObj == nullptr) {
            assert(false);
            return;
        }
        m_functionMap[funcObj] = std::bind(func, funcObj, std::placeholders::_1);
    }

    void remove(void* funcObj = nullptr);

    void trigger(const Node &parentNode) const;

protected:
    std::map<void*, std::function<void(const Node&)>> m_functionMap;
};

// =============================================================================
// Class definition
// =============================================================================
class Callback_NodeIndex
{
public:
    Callback_NodeIndex();

    template<typename T>
    void add(T* funcObj, void (T::*func)(const NodeIndex&))
    {
        if (funcObj == nullptr) {
            assert(false);
            return;
        }
        m_functionMap[funcObj] = std::bind(func, funcObj, std::placeholders::_1);
    }

    void remove(void* funcObj = nullptr);

    void trigger(const NodeIndex &nodeIndex) const;

protected:
    std::map<void*, std::function<void(const NodeIndex&)>> m_functionMap;
};

// =============================================================================
// Class definition
// =============================================================================
class Callback_NodeIndexNodeIndex
{
public:
    Callback_NodeIndexNodeIndex();

    template<typename T>
    void add(T* funcObj, void (T::*func)(const NodeIndex&, const NodeIndex&))
    {
        if (funcObj == nullptr) {
            assert(false);
            return;
        }
        m_functionMap[funcObj] = std::bind(func, funcObj, std::placeholders::_1, std::placeholders::_2);
    }

    void remove(void* funcObj = nullptr);

    void trigger(const NodeIndex &nodeIndex1, const NodeIndex &nodeIndex2) const;

protected:
    std::map<void*, std::function<void(const NodeIndex&, const NodeIndex&)>> m_functionMap;
};

// =============================================================================
// Class definition
// =============================================================================
class Callback_NodeIndexInt
{
public:
    Callback_NodeIndexInt();

    template<typename T>
    void add(T* funcObj, void (T::*func)(const NodeIndex&, int))
    {
        if (funcObj == nullptr) {
            assert(false);
            return;
        }
        m_functionMap[funcObj] = std::bind(func, funcObj, std::placeholders::_1, std::placeholders::_2);
    }

    void remove(void* funcObj = nullptr);

    void trigger(const NodeIndex &nodeIndex, int index) const;

protected:
    std::map<void*, std::function<void(const NodeIndex&, int)>> m_functionMap;
};

// =============================================================================
// Class definition
// =============================================================================
class Callback_NodeIndexString
{
public:
    Callback_NodeIndexString();

    template<typename T>
    void add(T* funcObj, void (T::*func)(const NodeIndex&, const std::string &))
    {
        if (funcObj == nullptr) {
            assert(false);
            return;
        }
        m_functionMap[funcObj] = std::bind(func, funcObj, std::placeholders::_1, std::placeholders::_2);
    }

    void remove(void* funcObj = nullptr);

    void trigger(const NodeIndex &nodeIndex, const std::string & name) const;

protected:
    std::map<void*, std::function<void(const NodeIndex&, const std::string &)>> m_functionMap;
};

} // namespace Oak::Model

