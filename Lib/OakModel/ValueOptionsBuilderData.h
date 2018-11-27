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

#include <UnionValue.h>

namespace Oak::Model {

class ValueDef;

// =============================================================================
// Class definition
// =============================================================================
class ValueOptionsBuilderData
{
public:
    ValueOptionsBuilderData();
    virtual ~ValueOptionsBuilderData() {}

    void validate(std::vector<std::string> &errorMessages) const;
    bool createDerived(ValueDef* valueDef) const;

    std::vector<UnionValue> values;
    std::vector<UnionValue> excludedValues;
    std::string valueQuery;
    std::string excludedValueQuery;
};

typedef std::unique_ptr<ValueOptionsBuilderData> ValueOptionsBuilderDataUPtr;

} // namespace Oak::Model
