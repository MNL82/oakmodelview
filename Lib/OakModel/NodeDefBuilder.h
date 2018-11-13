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

#include "NodeDef.h"
#include "ValueDefBuilder.h"


namespace Oak::Model {

class ContainerDefBuilder;
typedef std::shared_ptr<ContainerDefBuilder> ContainerDefBuilderSPtr;

class NodeDefBuilder;
typedef std::shared_ptr<NodeDefBuilder> NodeDefBuilderSPtr;
typedef std::weak_ptr<NodeDefBuilder> NodeDefBuilderWPtr;

// =============================================================================
// Class definition
// =============================================================================
class NodeDefBuilder
{
protected:
    NodeDefBuilder(const std::string &name);
    NodeDefBuilder(const std::string &name, const UnionRef &variantId);
    NodeDefBuilder(NodeDefSPtr variantRoot, const UnionRef &variantId);
    NodeDefBuilder(NodeDefSPtr nodeDef);

public:
    static NodeDefBuilderSPtr create(const std::string &name);
    static NodeDefBuilderSPtr createVariantRoot(const std::string &name, const UnionRef &variantId);
    static NodeDefBuilderSPtr createVariant(NodeDefBuilderSPtr variantRoot, const UnionRef &variantId);
    static NodeDefBuilderSPtr use(NodeDefSPtr nodeDef);

    NodeDefSPtr get();

    NodeDefBuilderSPtr addValueDef(ValueDefBuilderSPtr valueDef);
    NodeDefBuilderSPtr addValueKey(ValueDefBuilderSPtr valueDefKey);
    NodeDefBuilderSPtr addValueInheritanceId(ValueDefBuilderSPtr variantValueDef);

    NodeDefBuilderSPtr addContainerDef(ContainerDefBuilderSPtr cDef);

    NodeDefBuilderSPtr setDisplayName(const std::string& displayName);

    NodeDefBuilderSPtr setColor(int red, int green, int blue, int alpha = 255);
    NodeDefBuilderSPtr setImagePath(const std::string &imagePath);

#ifdef XML_BACKEND
    NodeDefBuilderSPtr setTagName(const std::string& tagName);
#endif // XML_BACKEND

    static ValueDefUPtr takeValueDef(NodeDefSPtr nodeDef, const std::string &valueName);
    static ContainerDefUPtr takeContainerDef(NodeDefSPtr nodeDef, const std::string &name);

protected:
    // ************* SERVICE FUNCTIONS *************
    // Set the keyValueDef or variantValueDef on all variants in the inheritance heiracky
    static void setKeyValueDefForAllVariants(NodeDefSPtr nodeDef, int index);
    static void setVariantValueDefForAllVariants(NodeDefSPtr nodeDef, int index);

#ifdef XML_BACKEND
    static void setTagNameForAllVariants(NodeDefSPtr nodeDef, const std::string& tagName);
#endif // XML_BACKEND

private:
    NodeDefSPtr m_nodeDef;
    NodeDefBuilderWPtr m_thisWPtr;
};

typedef NodeDefBuilder NDB;

} // namespace Oak::Model

