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

#include "NodeDefBuilderData.h"

#include "QuickPropertyHelpers.h"

// =============================================================================
// Class definition
// =============================================================================
class QuickNodeDef : public QObject
{
    Q_OBJECT
    BUILDER_PROPERTY_STRING(name)
    BUILDER_PROPERTY_STRING(displayName)
    BUILDER_PROPERTY_COLOR(color)
    BUILDER_PROPERTY_STRING(imagePath)
    BUILDER_PROPERTY_STRING(variantId)
    BUILDER_PROPERTY_OBJECT(QuickValueDef, keyValue)
    BUILDER_PROPERTY_OBJECT(QuickValueDef, variantValue)
    BUILDER_PROPERTY_OBJECT_LIST(QuickNodeDef, QuickValueDef, values, value)
    BUILDER_PROPERTY_OBJECT_LIST(QuickNodeDef, QuickContainerDef, containers, container)
    BUILDER_PROPERTY_OBJECT_LIST(QuickNodeDef, QuickNodeDef, derivedNodeDefs, derivedNodeDef)

public:
    QuickNodeDef(QObject *parent = nullptr);
    Oak::Model::NodeDefBuilderDataUPtr takeBuilder() { return std::move(m_builderOwner); }

private:
    Oak::Model::NodeDefBuilderData * m_builder;
    Oak::Model::NodeDefBuilderDataUPtr m_builderOwner;
};
