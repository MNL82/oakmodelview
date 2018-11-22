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

#include <QObject>
#include <QtQuick/QQuickItem>

#include "QuickContainerDef.h"

#include "../ServiceFunctions/PropertyHelper.h"

// =============================================================================
// Class definition
// =============================================================================
class QuickNodeDef :public QObject
{
    Q_OBJECT
    AUTO_PROPERTY_READONLY(QString, name)
    AUTO_PROPERTY_OBJECT_LIST(QuickNodeDef, QuickContainerDef, containers, container)

public:
    QuickNodeDef(QObject *parent = nullptr);

};
