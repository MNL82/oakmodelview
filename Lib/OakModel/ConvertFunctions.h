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

#include <string>
#include <vector>

#include "Conversion.h"
#include "VariantRelations.h"

namespace Oak {
namespace Model {

template<typename T> bool convert(InvalidVariant&, const InvalidVariant&, Conversion* properties = nullptr) { UNUSED(properties); return false; }
template<typename T> bool convert(InvalidVariant&, const T&, Conversion* properties = nullptr)              { UNUSED(properties); return false; }
template<typename T> bool convert(T&, const InvalidVariant&, Conversion* properties = nullptr)              { UNUSED(properties); return false; }
template<typename T> bool convert(std::vector<T>&, const InvalidVariant&, Conversion* properties = nullptr) { UNUSED(properties); return false; }
template<typename T> bool convert(InvalidVariant&, const std::vector<T>&, Conversion* properties = nullptr) { UNUSED(properties); return false; }

// =============================================================================
// convert()
// =============================================================================
bool convert(Bool& dest, int src, Conversion* properties = nullptr);
bool convert(int& dest, Bool src, Conversion* properties = nullptr);

bool convert(Bool&, double, Conversion* properties = nullptr);
bool convert(double&, Bool, Conversion* properties = nullptr);

bool convert(Bool& dest, const std::string& src, Conversion* properties = nullptr);
bool convert(std::string& dest, Bool src, Conversion* properties = nullptr);

bool convert(int& dest, double src, Conversion* properties = nullptr);
bool convert(double& dest, int src, Conversion* properties = nullptr);

bool convert(int& dest, const std::string& src, Conversion* properties = nullptr);
bool convert(std::string& dest, int src, Conversion* properties = nullptr);

bool convert(double& dest, const std::string& src, Conversion* properties = nullptr);
bool convert(std::string& dest, double src, Conversion* properties = nullptr);

template<typename T1, typename T2>
bool convert(std::vector<T1>&, const T2&, Conversion* properties = nullptr)
{
    UNUSED(properties);
    return false;
}

template<typename T1, typename T2>
bool convert(T1&, const std::vector<T2>&, Conversion* properties = nullptr)
{
    UNUSED(properties);
    return false;
}

template<typename T1, typename T2>
bool convert(std::vector<T1>& dest, const std::vector<T2>& src, Conversion* properties = nullptr)
{
    const int size = static_cast<int>(src.size());
    dest.resize(size);
    if (size == 0) { return true; }
    bool success = true;
    T1* destPtr = dest.data();
    const T2* srcPtr = src.data();
    for (int i = 0; i < size; ++i)
    {
        if (!convert(*destPtr++, *srcPtr++, properties)) {
            success = false;
        }
    }
    return success;
}

template<typename T>
bool convert(std::vector<Bool>& dest, const std::vector<T>& src, Conversion* properties = nullptr)
{
    const int size = static_cast<int>(src.size());
    dest.resize(size);
    if (size == 0) { return true; }
    bool success = true;
    const T* srcPtr = src.data();
    Bool temp;
    for (int i = 0; i < size; ++i)
    {
        if (convert(temp, *srcPtr++, properties)) {
            dest[i] = temp;
        } else {
            success = false;
        }
    }
    return success;
}

template<typename T>
bool convert(std::vector<T>& dest, const std::vector<Bool>& src, Conversion* properties = nullptr)
{
    const int size = static_cast<int>(src.size());
    dest.resize(size);
    if (size == 0) { return true; }
    bool success = true;
    T* srcPtr = dest.data();
    Bool temp;
    for (int i = 0; i < size; ++i)
    {
        temp = src[i];
        if (!convert(*srcPtr++, temp, properties)) {
            success = false;
        }
    }
    return success;
}

template<typename T1, typename T2>
bool convert(T1&, const T2& , Conversion* properties = nullptr)
{
    UNUSED(properties);
    return false;
}

// =============================================================================
// canConvert()
// =============================================================================
bool canConvert(Bool& dest, int& src, Conversion* properties = nullptr);
bool canConvert(int& dest, const Bool& src, Conversion* properties = nullptr);

bool canConvert(Bool& dest, double src, Conversion* properties = nullptr);
bool canConvert(double& dest, Bool src, Conversion* properties = nullptr);

bool canConvert(std::string& dest, Bool src, Conversion* properties = nullptr);

bool canConvert(int& dest, double src, Conversion* properties = nullptr);
bool canConvert(double& dest, int src, Conversion* properties = nullptr);

bool canConvert(std::string& dest, const int& src, Conversion* properties = nullptr);
bool canConvert(std::string& dest, const double& src, Conversion* properties = nullptr);

template<typename T1, typename T2>
bool canConvert(T1&, const T2& src, Conversion* properties = nullptr)
{
    T1 tempDest;
    return convert(tempDest, src, properties);
}

// =============================================================================
// compare typeid
// =============================================================================

template<typename T1, typename T2>
bool isTypeIdBaseEqual(const T1&, const T2&)
{
    if (typeid(T1) == typeid(InvalidVariant) || typeid(T2) == typeid(InvalidVariant)) { return false; }
    if (typeid(T1) == typeid(T2)) { return true; }
    if (typeid(T1) == typeid(std::vector<T2>)) { return true; }
    if (typeid(std::vector<T1>) == typeid(T2)) { return true; }
    return false;
}

} // namespace Model
} // namespace Oak
