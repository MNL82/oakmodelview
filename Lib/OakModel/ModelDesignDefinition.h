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

#include "NodeDefinition.h"

namespace Oak {
namespace Model {

class DesignStr {
public:
    // Node tag names
    static std::string Design;
    static std::string Node;
    static std::string Value;
    static std::string Container;

    // Value tag and attribute names
    static std::string Name;
    static std::string Key;
    static std::string Type;
};

class ModelDesignDefinition;
typedef std::shared_ptr<ModelDesignDefinition> ModelDesignDefinitionSPtr;
typedef std::shared_ptr<ModelDesignDefinition> ModelDesignDefSPtr;
typedef std::weak_ptr<ModelDesignDefinition> ModelDesignDefinitionWPtr;
typedef std::weak_ptr<ModelDesignDefinition> ModelDesignDefWPtr;

// =============================================================================
// Class definition
// =============================================================================
class ModelDesignDefinition : public NodeDefinition
{
protected:
    ModelDesignDefinition();
    void createModelDesign();

public:
    static NodeDefinitionSPtr MakeSPtr();
};

} // namespace Model
} // namespace Oak
