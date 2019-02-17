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

#include "QOakBaseTypes.h"

#include "QContainerDefBuilderData.h"
#include "QNodeDefBuilderData.h"
#include "QOakModel.h"
#include "QOakModelBuilderData.h"

#include "QLeafDefBuilderData.h"
#include "QValueOptionsBuilderData.h"

#include <qqml.h>

void OakModePlugin::registerTypes(const char *uri)
{
    Q_UNUSED(uri)
    qmlRegisterType<QTriState>("Oak.Model", 1, 0, "TriState");

    qmlRegisterType<QContainerDefBuilderData>("Oak.Model", 1, 0, "ContainerDef");
    qmlRegisterType<QNodeDefBuilderData>("Oak.Model", 1, 0, "NodeDef");
    qmlRegisterType<QOakModel>("Oak.Model", 1, 0, "OakModel");
    qmlRegisterType<QOakModelBuilderData>("Oak.Model", 1, 0, "OakModelBuilder");
    qmlRegisterType<QLeafDefBuilderData>("Oak.Model", 1, 0, "LeafDef");
    qmlRegisterType<QLeafSettingsBuilderData>("Oak.Model", 1, 0, "LeafSettings");
    qmlRegisterType<QValueOptionsBuilderData>("Oak.Model", 1, 0, "ValueOptions");
}
