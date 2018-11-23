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

#include "QuickContainerDef.h"
#include "QuickNodeDef.h"
#include "QuickOakModel.h"
#include "QuickOakModelBuilder.h"

#include "QuickValueDef.h"
#include "QuickValueOptions.h"
#include "QuickValueQuery.h"
#include "QuickItemQuery.h"
#include "QuickChildrenQuery.h"
#include "QuickParentQuery.h"
#include "QuickSiblingsQuery.h"

#include <qqml.h>

void OakModePlugin::registerTypes(const char *uri)
{
    Q_UNUSED(uri)
    qmlRegisterType<QuickContainerDef>("Oak.Model", 1, 0, "ContainerDef");
    qmlRegisterType<QuickNodeDef>("Oak.Model", 1, 0, "NodeDef");
    qmlRegisterType<QuickOakModel>("Oak.Model", 1, 0, "OakModel");
    qmlRegisterType<QuickOakModelBuilder>("Oak.Model", 1, 0, "OakModelBuilder");
    qmlRegisterType<QuickValueDef>("Oak.Model", 1, 0, "ValueDef");
    qmlRegisterType<QuickValueOptions>("Oak.Model", 1, 0, "ValueOptions");
    qmlRegisterType<QuickValueQuery>("Oak.Model", 1, 0, "ValueQuery");
    qmlRegisterType<QuickItemQuery>("Oak.Model", 1, 0, "ItemQuery");
    qmlRegisterType<QuickChildrenQuery>("Oak.Model", 1, 0, "ChildrenQuery");
    qmlRegisterType<QuickParentQuery>("Oak.Model", 1, 0, "ParentQuery");
    qmlRegisterType<QuickSiblingsQuery>("Oak.Model", 1, 0, "SiblingsQuery");
}
