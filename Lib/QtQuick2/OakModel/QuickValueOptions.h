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
#include <QQmlListProperty>
#include <QVariant>
#include <QVariantList>

#include "QuickValueQuery.h"

#include "../ServiceFunctions/PropertyHelper.h"

// =============================================================================
// Class definition
// =============================================================================
class QuickValueOptions : public QObject
{
    Q_OBJECT

    AUTO_PROPERTY_READONLY(QVariantList, values)
    AUTO_PROPERTY_READONLY(QVariantList, excludedValue)
    AUTO_PROPERTY_OBJECT_READONLY(QuickValueQuery, valueQuery)
    AUTO_PROPERTY_OBJECT_READONLY(QuickValueQuery, excludedValueQuery)

public:
    QuickValueOptions(QObject *parent = nullptr);

};
