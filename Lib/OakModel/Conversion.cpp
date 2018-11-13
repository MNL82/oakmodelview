/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "Conversion.h"


namespace Oak::Model {

// =============================================================================
// (public)
Conversion::Conversion()
{
}

// =============================================================================
// (public)
ConversionSPtr Conversion::globalDefault()
{
    static ConversionSPtr globalDefault(new Conversion());

    // Initialize the global default conversion
    if (globalDefault->m_boolTrue.empty()) {
        globalDefault->m_boolTrue = "true";
        globalDefault->m_boolFalse = "false";
        globalDefault->m_listSeperator = ";";
        globalDefault->m_roundRules = DoubleToInt_Round;
        globalDefault->m_equalTolerance = 1.0e-12;
        globalDefault->m_doubleToStringPrecision = 5;
        globalDefault->m_doubleToStringMode = DoubleToString_Default;
    }
    return globalDefault;
}

// =============================================================================
// (public)
Conversion*Conversion::globalDefault2()
{
    static Conversion* s_default = new Conversion();

    // Initialize the global default conversion
    if (s_default->m_boolTrue.empty()) {
        s_default->m_boolTrue = "true";
        s_default->m_boolFalse = "false";
        s_default->m_listSeperator = ";";
        s_default->m_roundRules = DoubleToInt_Round;
        s_default->m_equalTolerance = 1.0e-12;
        s_default->m_doubleToStringPrecision = 5;
        s_default->m_doubleToStringMode = DoubleToString_Default;
    }

    return s_default;
}

} // namespace Oak::Model

