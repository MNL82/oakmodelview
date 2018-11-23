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
#include <QVariant>

#include "QuickValueOptions.h"
#include "QuickValueSettings.h"

#include "Union.h"
#include "../ServiceFunctions/PropertyHelper.h"

// =============================================================================
// Class definition
// =============================================================================
class QuickValueDef : public QObject
{
    Q_OBJECT
public:
    enum dataTypeEnum {
        Boolean = 0,
        Integer = 1,
        Double = 2,
        String = 3,
        DateTime = 4
    };
    Q_ENUM(dataTypeEnum)

private:
    AUTO_PROPERTY_READONLY(QString, name)
    AUTO_PROPERTY_READONLY(QString, displayName)
    AUTO_PROPERTY_READONLY(QVariant, defaultValue)
    AUTO_PROPERTY_READONLY(dataTypeEnum, dataType)
    AUTO_PROPERTY_OBJECT_READONLY(QuickValueSettings, settings)
    AUTO_PROPERTY_OBJECT_READONLY(QuickValueOptions, options)

public:
    QuickValueDef(QObject* parent = nullptr);
};
