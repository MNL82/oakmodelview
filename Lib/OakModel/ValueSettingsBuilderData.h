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

#include "OakModelServiceFunctions.h"

namespace Oak::Model {

class ValueDefBuilder;
typedef std::shared_ptr<ValueDefBuilder> ValueDefBuilderSPtr;

// =============================================================================
// Class definition
// =============================================================================
class ValueSettingsBuilderData
{
public:
    ValueSettingsBuilderData();
    virtual ~ValueSettingsBuilderData() {}

    void validate(std::vector<std::string> &errorMessages) const;
    void set(ValueDefBuilderSPtr builder) const;

    BoolState readOnly = BoolState_Undefined;
    std::string unit;
    BoolState unique = BoolState_Undefined;
    BoolState required = BoolState_Undefined;
};

typedef std::unique_ptr<ValueSettingsBuilderData> ValueSettingsBuilderDataUPtr;

} // namespace Oak::Model
