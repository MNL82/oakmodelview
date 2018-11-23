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
#include <QString>

#include "../ServiceFunctions/PropertyHelper.h"

// =============================================================================
// Class definition
// =============================================================================
class QuickItemQuery : public QObject
{
    Q_OBJECT
    AUTO_PROPERTY_OBJECT(QuickItemQuery, childQuery)

public:
    QuickItemQuery(QObject *parent = nullptr);
    virtual ~QuickItemQuery() {}
};

