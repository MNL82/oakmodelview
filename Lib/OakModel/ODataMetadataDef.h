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

class ODataMetadataDef;
typedef std::shared_ptr<ODataMetadataDef> ODataMetadataDefSPtr;
typedef std::shared_ptr<ODataMetadataDef> ODataMetadataDefSPtr;
typedef std::weak_ptr<ODataMetadataDef> ODataMetadataDefWPtr;
typedef std::weak_ptr<ODataMetadataDef> ODataMetadataDefWPtr;

// =============================================================================
// Class definition
// =============================================================================
class ODataMetadataDef : public NodeDef
{
protected:
    ODataMetadataDef();
    void createModelDesign();

public:
    static NodeDefSPtr MakeSPtr();
};

} // namespace Oak::Model

