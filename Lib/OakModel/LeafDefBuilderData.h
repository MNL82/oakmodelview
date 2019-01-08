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

#include "LeafSettingsBuilderData.h"
#include "ValueOptionsBuilderData.h"

namespace Oak::Model {

class LeafDefBuilder;
typedef std::shared_ptr<LeafDefBuilder> LeafDefBuilderSPtr;

// =============================================================================
// Class definition
// =============================================================================
class LeafDefBuilderData
{
public:
    LeafDefBuilderData();
    virtual ~LeafDefBuilderData() {}

    void validate(std::vector<std::string> &errorMessages) const;
    LeafDefBuilderSPtr create() const;

    std::string name;
    std::string displayName;
    std::string tooltip;
    UnionValue defaultValue;
    UnionType dataType = UnionType::Undefined;
    LeafSettingsBuilderDataUPtr settings;
    ValueOptionsBuilderDataUPtr options;
};

typedef std::unique_ptr<LeafDefBuilderData> LeafDefBuilderDataUPtr;

} // namespace Oak::Model
