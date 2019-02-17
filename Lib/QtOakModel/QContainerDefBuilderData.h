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

class QNodeDefBuilderData;

#include "ContainerDefBuilderData.h"

#include "QPropertyHelpers.h"

// =============================================================================
// Class definition
// =============================================================================
class QContainerDefBuilderData : public QObject
{
    Q_OBJECT
    BUILDER_PROPERTY_STRING(name)
    BUILDER_PROPERTY_STRING(variantId)
    BUILDER_PROPERTY_BASE(int, minCount)
    BUILDER_PROPERTY_BASE(int, maxCount)

public:
    QContainerDefBuilderData(QObject *parent = nullptr);
    Oak::Model::ContainerDefBuilderDataUPtr takeBuilder() { return std::move(m_builderOwner); }

private:
    Oak::Model::ContainerDefBuilderData * m_builder;
    Oak::Model::ContainerDefBuilderDataUPtr m_builderOwner;
};
