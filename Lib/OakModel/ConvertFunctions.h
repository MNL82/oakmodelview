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
#include <chrono>
#include <vector>

#include "Conversion.h"

namespace Oak {
namespace Model {

// =============================================================================
// convert()
// =============================================================================
bool convert(bool& dest, int src, Conversion* properties = nullptr);
bool convert(int& dest, bool src, Conversion* properties = nullptr);

bool convert(bool&, double, Conversion* properties = nullptr);
bool convert(double&, bool, Conversion* properties = nullptr);

bool convert(bool& dest, const std::string& src, Conversion* properties = nullptr);
bool convert(std::string& dest, bool src, Conversion* properties = nullptr);

bool convert(int& dest, double src, Conversion* properties = nullptr);
bool convert(double& dest, int src, Conversion* properties = nullptr);

bool convert(int& dest, const std::string& src, Conversion* properties = nullptr);
bool convert(std::string& dest, int src, Conversion* properties = nullptr);

bool convert(double& dest, const std::string& src, Conversion* properties = nullptr);
bool convert(std::string& dest, double src, Conversion* properties = nullptr);

bool convert(std::string& dest, const char * source, Conversion* properties = nullptr);

bool convert(std::string& dest, const std::chrono::system_clock::time_point &src, Conversion* properties = nullptr);
bool convert(std::chrono::system_clock::time_point& dest, const std::string &src, Conversion* properties = nullptr);

template<typename T>
bool convert(T& dest, const T& source, Conversion* properties = nullptr)
{
    UNUSED(properties);
    dest = source;
    return true;
}

template<typename T>
bool convert(T& dest, const char * source, Conversion* properties = nullptr)
{
    if (source == nullptr) { return false; }
    return convert(dest, std::string(source), properties);
}

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

template<typename T1, typename T2>
bool convert(T1 &dest, const T2 &source, Conversion* properties = nullptr)
{
    UNUSED(dest);
    UNUSED(source);
    UNUSED(properties);
    return false;
}

// =============================================================================
// canConvert()
// =============================================================================
bool canConvert(bool& dest, int& src, Conversion* properties = nullptr);
bool canConvert(int& dest, const bool& src, Conversion* properties = nullptr);

bool canConvert(bool& dest, double src, Conversion* properties = nullptr);
bool canConvert(double& dest, bool src, Conversion* properties = nullptr);

bool canConvert(std::string& dest, bool src, Conversion* properties = nullptr);

bool canConvert(int& dest, double src, Conversion* properties = nullptr);
bool canConvert(double& dest, int src, Conversion* properties = nullptr);

bool canConvert(std::string& dest, const int& src, Conversion* properties = nullptr);
bool canConvert(std::string& dest, const double& src, Conversion* properties = nullptr);

bool canConvert(std::string& dest, const char * source, Conversion* properties = nullptr);

template<typename T>
bool canConvert(T& dest, const char * source, Conversion* properties = nullptr)
{
    if (source == nullptr) { return false; }
    return canConvert(dest, std::string(source), properties);
}

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
    if (typeid(T1) == typeid(T2)) { return true; }
    if (typeid(T1) == typeid(std::vector<T2>)) { return true; }
    if (typeid(std::vector<T1>) == typeid(T2)) { return true; }
    return false;
}

} // namespace Model
} // namespace Oak
