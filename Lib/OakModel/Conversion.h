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

#ifndef UNUSED
#define UNUSED(x) (void)x;
#endif

namespace Oak {
namespace Model {

class Conversion;
typedef std::shared_ptr<Conversion> ConversionSPtr;

// =============================================================================
// Class definition
// =============================================================================
class Conversion
{
public:
    enum DoubleToInt {
        DoubleToInt_Round,
        DoubleToInt_Floor,
        DoubleToInt_Ceil,
        DoubleToInt_Trunc
    };

public:
    Conversion();

    const std::string &boolTrue() const { return m_boolTrue; }
    const std::string &boolFalse() const { return m_boolFalse; }

    double equalTolerance() const { return m_equalTolerance; }

    static ConversionSPtr globalDefault();
    static Conversion* globalDefault2();

    DoubleToInt roundRules() const { return m_roundRules; }

private:
    // Bool to string and back
    std::string m_boolTrue;
    std::string m_boolFalse;

    // Double to integer
    DoubleToInt m_roundRules;

    //ToDo: Double to string rules

    std::string m_listSeperator;

    double m_equalTolerance;
};

} // namespace Model
} // namespace Oak
