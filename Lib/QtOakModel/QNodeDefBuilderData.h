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

#include "QContainerDefBuilderData.h"
#include "QLeafDefBuilderData.h"
#include "QPropertyHelpers.h"

#include "NodeDefBuilderData.h"

// =============================================================================
// Class definition
// =============================================================================
class QNodeDefBuilderData : public QObject
{
    Q_OBJECT
    BUILDER_PROPERTY_STRING(name)
    BUILDER_PROPERTY_STRING(displayName)
    BUILDER_PROPERTY_STRING(tooltip)
    BUILDER_PROPERTY_COLOR(color)
    BUILDER_PROPERTY_STRING(imagePath)
    BUILDER_PROPERTY_STRING(variantId)
    BUILDER_PROPERTY_STRING(tagName)
    BUILDER_PROPERTY_OBJECT(QLeafDefBuilderData, keyLeaf)
    BUILDER_PROPERTY_OBJECT(QLeafDefBuilderData, variantLeaf)
    BUILDER_PROPERTY_OBJECT_LIST(QNodeDefBuilderData, QLeafDefBuilderData, leafs, leaf)
    BUILDER_PROPERTY_OBJECT_LIST(QNodeDefBuilderData, QContainerDefBuilderData, containers, container)
    BUILDER_PROPERTY_OBJECT_LIST(QNodeDefBuilderData, QNodeDefBuilderData, derivedNodeDefs, derivedNodeDef)

public:
    QNodeDefBuilderData(QObject *parent = nullptr);
    Oak::Model::NodeDefBuilderDataUPtr takeBuilder() { return std::move(m_builderOwner); }

private:
    Oak::Model::NodeDefBuilderData * m_builder;
    Oak::Model::NodeDefBuilderDataUPtr m_builderOwner;
};
