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


namespace Oak::Model {

class ModelDesignDef;
typedef std::shared_ptr<ModelDesignDef> ModelDesignDefSPtr;
typedef std::shared_ptr<ModelDesignDef> ModelDesignDefSPtr;
typedef std::weak_ptr<ModelDesignDef> ModelDesignDefWPtr;
typedef std::weak_ptr<ModelDesignDef> ModelDesignDefWPtr;

// =============================================================================
// Class definition
// =============================================================================
class ModelDesignDef : public NodeDef
{
protected:
    ModelDesignDef();
    void createModelDesign();

public:
    static NodeDefSPtr MakeSPtr();
};

} // namespace Oak::Model

