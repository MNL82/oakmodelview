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

class LeafDefBuilder;
typedef std::shared_ptr<LeafDefBuilder> LeafDefBuilderSPtr;

// =============================================================================
// Class definition
// =============================================================================
class LeafSettingsBuilderData
{
public:
    LeafSettingsBuilderData();
    virtual ~LeafSettingsBuilderData() {}

    void validate(std::vector<std::string> &errorMessages) const;
    void set(LeafDefBuilderSPtr builder) const;

    BoolState readOnly = BoolState_Undefined;
    std::string unit;
    BoolState unique = BoolState_Undefined;
    BoolState required = BoolState_Undefined;
};

typedef std::unique_ptr<LeafSettingsBuilderData> LeafSettingsBuilderDataUPtr;

} // namespace Oak::Model
