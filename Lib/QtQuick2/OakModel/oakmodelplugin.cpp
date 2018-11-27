/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "oakmodelplugin.h"

#include "QuickOakBaseTypes.h"

#include "QuickContainerDef.h"
#include "QuickNodeDef.h"
#include "QuickOakModel.h"
#include "QuickOakModelBuilder.h"

#include "QuickValueDef.h"
#include "QuickValueOptions.h"


#include <qqml.h>

void OakModePlugin::registerTypes(const char *uri)
{
    Q_UNUSED(uri)
    qmlRegisterType<QuickTriState>("Oak.Model", 1, 0, "TriState");

    qmlRegisterType<QuickContainerDef>("Oak.Model", 1, 0, "ContainerDef");
    qmlRegisterType<QuickNodeDef>("Oak.Model", 1, 0, "NodeDef");
    qmlRegisterType<QuickOakModel>("Oak.Model", 1, 0, "OakModel");
    qmlRegisterType<QuickOakModelBuilder>("Oak.Model", 1, 0, "OakModelBuilder");
    qmlRegisterType<QuickValueDef>("Oak.Model", 1, 0, "ValueDef");
    qmlRegisterType<QuickValueOptions>("Oak.Model", 1, 0, "ValueOptions");
    qmlRegisterType<QuickValueSettings>("Oak.Model", 1, 0, "ValueSettings");
}
