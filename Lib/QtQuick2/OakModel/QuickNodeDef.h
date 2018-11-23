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
#include <QColor>

#include "QuickContainerDef.h"
#include "QuickValueDef.h"

#include "../ServiceFunctions/PropertyHelper.h"

// =============================================================================
// Class definition
// =============================================================================
class QuickNodeDef :public QObject
{
    Q_OBJECT
    AUTO_PROPERTY_READONLY(QString, name)
    AUTO_PROPERTY_READONLY(QString, displayName)
    AUTO_PROPERTY_READONLY(QColor, color)
    AUTO_PROPERTY_READONLY(QString, imagePath)
    AUTO_PROPERTY_READONLY(QString, variantId)
    AUTO_PROPERTY_OBJECT(QuickValueDef, keyValue)
    AUTO_PROPERTY_OBJECT(QuickValueDef, variantValue)
    AUTO_PROPERTY_OBJECT_LIST(QuickNodeDef, QuickValueDef, values, value)
    AUTO_PROPERTY_OBJECT_LIST(QuickNodeDef, QuickContainerDef, containers, container)
    AUTO_PROPERTY_OBJECT_LIST(QuickNodeDef, QuickNodeDef, derivedNodeDefs, derivedNodeDef)

public:
    QuickNodeDef(QObject *parent = nullptr);

};
