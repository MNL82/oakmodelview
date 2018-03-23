/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ConvertFunctions.h"

#include <iomanip>
#include <sstream>
#include <assert.h>

namespace Oak {
namespace Model {

// =============================================================================
// convert()
// =============================================================================

// =============================================================================
// (public)
bool convert(bool& dest, int src, Conversion* )
{
    dest = src != 0;
    return true;
}

// =============================================================================
// (public)
bool convert(int& dest, bool src, Conversion* )
{
    dest = src ? 1 : 0;
    return true;
}

// =============================================================================
// (public)
bool convert(bool&, double, Conversion* )
{
    return false;
}

// =============================================================================
// (public)
bool convert(double&, bool, Conversion* )
{
    return false;
}

// =============================================================================
// (public)
bool convert(bool& dest, const std::string& src, Conversion* properties)
{
    if (properties == nullptr) { properties = Conversion::globalDefault2(); }
    if (src.compare(properties->boolTrue()) == 0) {
        dest = true;
        return true;
    } else if (src.compare(properties->boolFalse()) == 0) {
        dest = false;
        return true;
    }
    return false;
}

// =============================================================================
// (public)
bool convert(std::string& dest, bool src, Conversion* properties)
{
    if (properties == nullptr) { properties = Conversion::globalDefault2(); }
    if (src) {
        dest = properties->boolTrue();
    } else {
        dest = properties->boolFalse();
    }
    return true;
}

// =============================================================================
// (public)
bool convert(int& dest, double src, Conversion* properties)
{
    if (properties == nullptr) { properties = Conversion::globalDefault2(); }
    if (properties->roundRules() == Conversion::DoubleToInt_Round) {
        dest = static_cast<int>(std::round(src));
    } else if (properties->roundRules() == Conversion::DoubleToInt_Floor) {
        dest = static_cast<int>(std::floor(src));
    } else if (properties->roundRules() == Conversion::DoubleToInt_Ceil) {
        dest = static_cast<int>(std::ceil(src));
    } else if (properties->roundRules() == Conversion::DoubleToInt_Trunc) {
        dest = static_cast<int>(std::trunc(src));
    }
    return true;
}

// =============================================================================
// (public)
bool convert(double& dest, int src, Conversion* )
{
    dest = src;
    return true;
}

// =============================================================================
// (public)
bool convert(int& dest, const std::string& src, Conversion* )
{
    try {
        dest = std::stoi(src);
    } catch(...) {
        return false;
    }
    return true;
}

// =============================================================================
// (public)
bool convert(std::string& dest, int src, Conversion* )
{
    dest = std::to_string(src);
    return true;
}

// =============================================================================
// (public)
bool convert(double& dest, const std::string& src, Conversion* )
{
    try {
        dest = std::stod(src);
    } catch(...) {
        return false;
    }
    return true;
}

// =============================================================================
// (public)
bool convert(std::string& dest, double src, Conversion* properties)
{
    if (properties == nullptr) { properties = Conversion::globalDefault2(); }
    std::stringstream stream;
    switch (properties->doubleToStringMode()) {
    case Conversion::DoubleToString_Default:
        stream << std::defaultfloat;
        break;
    case Conversion::DoubleToString_Scientific:
        stream << std::scientific;
        break;
    case Conversion::DoubleToString_Fixed:
        stream << std::fixed;
        break;
    default:
        assert(false);
        break;
    }
    stream << std::setprecision(properties->doubleToStringPrecision()) << src;
    dest = stream.str();
    return true;
}

// =============================================================================
// (public)
bool convert(std::string &dest, const char *source, Conversion* )
{
    if (source == nullptr) { return false; }
    dest.assign(source);
    return true;
}

// =============================================================================
// canConvert()
// =============================================================================

// =============================================================================
// (public)
bool canConvert(bool&, int, Conversion*)
{
    return true;
}

// =============================================================================
// (public)
bool canConvert(int&, const bool&, Conversion*)
{
    return true;
}

// =============================================================================
// (public)
bool canConvert(bool &, double, Conversion*)
{
    return false;
}

// =============================================================================
// (public)
bool canConvert(double&, bool, Conversion*)
{
    return false;
}

// =============================================================================
// (public)
bool canConvert(std::string&, bool, Conversion*)
{
    return true;
}

// =============================================================================
// (public)
bool canConvert(int&, double, Conversion*)
{
    return true;
}

// =============================================================================
// (public)
bool canConvert(double&, int, Conversion*)
{
    return true;
}

// =============================================================================
// (public)
bool canConvert(std::string&, const int&, Conversion*)
{
    return true;
}

// =============================================================================
// (public)
bool canConvert(std::string&, const double&, Conversion*)
{
    return true;
}

// =============================================================================
// (public)
bool canConvert(std::string &, const char *source, Conversion*)
{
    return source != nullptr;
}

} // namespace Model
} // namespace Oak
