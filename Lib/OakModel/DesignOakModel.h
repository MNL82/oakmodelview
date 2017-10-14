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

#include "OakModel.h"

namespace Oak {
namespace Model {

// =============================================================================
// Class definition
// =============================================================================
class DesignOakModel : public OakModel
{
public:
    DesignOakModel();

    NodeDefinitionSPtr generateModelDefinition() const;

protected:
    NodeDefinitionSPtr createNodeDefinition(const Item& item) const;

    void createDesignDefinition();

    static std::string s_design;
    static std::string s_node;
    static std::string s_value;
    static std::string s_container;
    static std::string s_name;
    static std::string s_type;
    static std::string s_minCount;
    static std::string s_maxCount;
    static std::string s_default;
    static std::string s_keyValue;
    static std::string s_tagName;
    static std::string s_attributeName;

    // Value types
    static std::string s_string;
    static std::string s_integer;
    static std::string s_decimal;
};

} // namespace Model
} // namespace Oak
