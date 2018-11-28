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

#include "ValueSettingsBuilderData.h"
#include "ValueOptionsBuilderData.h"

namespace Oak::Model {

class ValueDefBuilder;
typedef std::shared_ptr<ValueDefBuilder> ValueDefBuilderSPtr;

// =============================================================================
// Class definition
// =============================================================================
class ValueDefBuilderData
{
public:
    ValueDefBuilderData();
    virtual ~ValueDefBuilderData() {}

    void validate(std::vector<std::string> &errorMessages) const;
    ValueDefBuilderSPtr create() const;

    std::string name;
    std::string displayName;
    std::string tooltip;
    UnionValue defaultValue;
    UnionType dataType = UnionType::Undefined;
    ValueSettingsBuilderDataUPtr settings;
    ValueOptionsBuilderDataUPtr options;
};

typedef std::unique_ptr<ValueDefBuilderData> ValueDefBuilderDataUPtr;

} // namespace Oak::Model
