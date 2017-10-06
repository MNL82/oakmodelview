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

#include <cstring>

#include "ConvertFunctions.h"
#include "VariantRelations.h"

namespace Oak {
namespace Model {

// =============================================================================
// Class definition
// =============================================================================
struct ConvertVisitor : public boost::static_visitor<bool>
{
    bool m_allowConversion;
    Conversion * m_properties;

public:
    ConvertVisitor(bool allowConversion = false, Conversion* properties = nullptr)
        : boost::static_visitor<bool>(),
          m_allowConversion(allowConversion),
          m_properties(properties)
    {}

    //******************** No Conversion ********************
    template<typename T>
    bool operator()(T& dest, const T& src) const
    {
        dest = src;
        return true;
    }

#ifdef TDM_USE_CHAR_PTR_IN_VARIANT
    bool operator()(std::string& dest, const char* src) const
    {
        dest.assign(src);
        return true;
    }

    bool operator()(InvalidVariant&, const char*) const { return false; }

    template<typename T>
    bool operator()(T& dest, const char* src) const
    {
        if (m_allowConversion) {
            std::string temp(src);
            return convert(dest, temp, m_properties);
        }
        return false;
    }
#endif // TDM_USE_CHAR_PTR_IN_VARIANT

    //******************** Generic conversion ********************
    template<typename T1, typename T2>
    bool operator()(T1& dest, const T2& src) const
    {
        if (m_allowConversion) {
            return convert(dest, src, m_properties);
        }
        return false;
    }

    //******************** No Conversion ********************
    template<typename T>
    bool operator()(std::vector<T>& dest, const std::vector<T>& src) const
    {
        dest.assign(src.begin(), src.end());
        return true;
    }
};

// =============================================================================
// Class definition
// =============================================================================
struct CanConvertVisitor : public boost::static_visitor<bool>
{
    bool m_allowConversion;
    Conversion * m_properties;

public:
    CanConvertVisitor(bool allowConversion = false, Conversion* properties = nullptr)
        : boost::static_visitor<bool>(),
          m_allowConversion(allowConversion),
          m_properties(properties)
    {}

    //******************** No Conversion ********************
    template<typename T>
    bool operator()(T&, const T&) const
    {
        return true;
    }

#ifdef TDM_USE_CHAR_PTR_IN_VARIANT
    template<typename T>
    bool operator()(T& dest, const char* src) const
    {
        return operator()(dest, std::string(src));
    }
#endif // TDM_USE_CHAR_PTR_IN_VARIANT

    //******************** Generic conversion ********************
    template<typename T1, typename T2>
    bool operator()(T1& dest, const T2& src) const
    {
        if (m_allowConversion) {
            return canConvert(dest, src, m_properties);
        }
        return false;
    }
};

// =============================================================================
// Class definition
// =============================================================================
struct SetVariantCRefVisitor : public boost::static_visitor<BaseTypeCRef>
{
public:
    template<typename T>
    BaseTypeCRef operator()(const T& value) const
    {
        return BaseTypeCRef(value);
    }
};

// =============================================================================
// Class definition
// =============================================================================
struct SetVariantRefVisitor : public boost::static_visitor<BaseTypeRef>
{
public:
    template<typename T>
    BaseTypeRef operator()(T& value) const
    {
        return BaseTypeRef(value);
    }
};

// =============================================================================
// Class definition
// =============================================================================
struct SetVariantVisitor : public boost::static_visitor<BaseType>
{
public:
#ifdef TDM_USE_CHAR_PTR_IN_VARIANT
    BaseType operator()(const char* value) const
    {
        return BaseType(std::string(value));
    }
#endif // TDM_USE_CHAR_PTR_IN_VARIANT

    template<typename T>
    BaseType operator()(const T& value) const
    {
        return BaseType(value);
    }
};

// =============================================================================
// Class definition
// =============================================================================
struct EqualVisitor : public boost::static_visitor<bool>
{
private:
    bool m_allowConversion;
    Conversion* m_properties;

public:
    EqualVisitor(bool allowConversion = false, Conversion* properties = nullptr)
        : boost::static_visitor<bool>(),
          m_allowConversion(allowConversion),
          m_properties(properties)
    {}

    //******************** No Conversion ********************
    template<typename T>
    bool operator()(const T& v1, const T& v2) const
    {
        return v1 == v2;
    }

    //******************** Generic conversion ********************
    template<typename T1, typename T2>
    bool operator()(const T1& v1, const T2& v2) const
    {
        if (m_allowConversion) {
            T2 tempValue;
            if (convert(tempValue, v1, m_properties)) {
                return tempValue == v2;
            }
        }
        return false;
    }

#ifdef TDM_USE_CHAR_PTR_IN_VARIANT
    //*********************** const char* ***********************
    bool operator()(const char* v1, const char* v2) const
    {
        return std::strcmp(v1, v2) == 0;
    }

    template<typename T>
    bool operator()(const char* v1, const T& v2) const
    {
        return operator()(std::string(v1), v2);
    }

    template<typename T>
    bool operator()(const T& v1, const char* v2) const
    {
        return operator()(v1, std::string(v2));
    }
#endif // TDM_USE_CHAR_PTR_IN_VARIANT
};

// =============================================================================
// Class definition
// =============================================================================
struct SmallerVisitor : public boost::static_visitor<bool>
{
private:
    bool m_allowConversion;
    Conversion* m_properties;

public:
    SmallerVisitor(bool allowConversion = false, Conversion* properties = nullptr)
        : boost::static_visitor<bool>(),
          m_allowConversion(allowConversion),
          m_properties(properties)
    {}

    //******************** No Conversion ********************
    template<typename T>
    bool operator()(const T& v1, const T& v2) const
    {
        return v1 < v2;
    }

    //******************** Generic conversion ********************
    template<typename T1, typename T2>
    bool operator()(const T1& v1, const T2& v2) const
    {
        if (m_allowConversion) {
            T2 tempValue;
            if (convert(tempValue, v1, m_properties)) {
                return operator()(tempValue, v2);
            }
        }
        return false;
    }

#ifdef TDM_USE_CHAR_PTR_IN_VARIANT
    //*********************** const char* ***********************
    bool operator()(const char* v1, const char* v2) const
    {
        return operator()(std::string(v1), std::string(v2));
    }

    template<typename T>
    bool operator()(const char* v1, const T& v2) const
    {
        return operator()(std::string(v1), v2);
    }

    template<typename T>
    bool operator()(const T& v1, const char* v2) const
    {
        return operator()(v1, std::string(v2));
    }
#endif // TDM_USE_CHAR_PTR_IN_VARIANT
};

// =============================================================================
// Class definition
// =============================================================================
struct EqualBaseVisitor : public boost::static_visitor<bool>
{

public:
    //******************** No Conversion ********************
    template<typename T>
    bool operator()(const T& v1, const T& v2) const
    {
        return true;
    }

    //******************** Generic conversion ********************
    template<typename T1, typename T2>
    bool operator()(const T1& v1, const T2& v2) const
    {
        auto vec1 = dynamic_cast<std::vector<T2>*>(v1);
        if (vec1 != nullptr) { return true; }
        auto vec2 = dynamic_cast<std::vector<T1>*>(v2);
        if (vec2 != nullptr) { return true; }
        return false;
    }

#ifdef TDM_USE_CHAR_PTR_IN_VARIANT
    //*********************** const char* ***********************
    bool operator()(const char*, const char*) const
    {
        return true;
    }

    template<typename T>
    bool operator()(const char*, const T&) const
    {
        return typeid(T) == typeid(std::string);
    }

    template<typename T>
    bool operator()(const T&, const char*) const
    {
        return typeid(T) == typeid(std::string);
    }
#endif // TDM_USE_CHAR_PTR_IN_VARIANT
};

// =============================================================================
// Class definition
// =============================================================================
struct TypeIdBaseEqualVisitor : public boost::static_visitor<bool>
{

public:
    template<typename T1, typename T2>
    bool operator()(const T1& v1, const T2& v2) const
    {
        return isTypeIdBaseEqual(v1, v2);
    }

#ifdef TDM_USE_CHAR_PTR_IN_VARIANT
    //*********************** const char* ***********************
    bool operator()(const char*, const char*) const
    {
        return true;
    }

    template<typename T>
    bool operator()(const char*, const T&) const
    {
        return typeid(T) == typeid(std::string) || typeid(T) == typeid(std::vector<std::string>);
    }

    template<typename T>
    bool operator()(const T&, const char*) const
    {
        return typeid(T) == typeid(std::string) || typeid(T) == typeid(std::vector<std::string>);
    }
#endif // TDM_USE_CHAR_PTR_IN_VARIANT
};

} // namespace Model
} // namespace Oak
