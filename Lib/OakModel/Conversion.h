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
#include <memory>
#include <vector>

#ifndef UNUSED
#define UNUSED(x) (void)x;
#endif

namespace Oak {
namespace Model {

typedef std::vector<int>::size_type vSize;

class Conversion;
typedef std::shared_ptr<Conversion> ConversionSPtr;

// =============================================================================
// Class definition
// =============================================================================
class Conversion
{
public:
    enum DoubleToInt {
        DoubleToInt_Round = 0,
        DoubleToInt_Floor,
        DoubleToInt_Ceil,
        DoubleToInt_Trunc
    };

    enum DoubleToString {
        DoubleToString_Default = 0,
        DoubleToString_Fixed,
        DoubleToString_Scientific
    };

public:
    Conversion();

    const std::string &boolTrue() const { return m_boolTrue; }
    const std::string &boolFalse() const { return m_boolFalse; }

    double equalTolerance() const { return m_equalTolerance; }

    static ConversionSPtr globalDefault();
    static Conversion* globalDefault2();

    DoubleToInt roundRules() const { return m_roundRules; }

    int doubleToStringPrecision() const { return m_doubleToStringPrecision; }
    DoubleToString doubleToStringMode() const { return m_doubleToStringMode; }
private:
    // Bool to string and back
    std::string m_boolTrue;
    std::string m_boolFalse;

    int m_doubleToStringPrecision;
    DoubleToString m_doubleToStringMode = DoubleToString_Default;

    // Double to integer
    DoubleToInt m_roundRules;

    //ToDo: Double to string rules

    std::string m_listSeperator;

    double m_equalTolerance;
};

} // namespace Model
} // namespace Oak
