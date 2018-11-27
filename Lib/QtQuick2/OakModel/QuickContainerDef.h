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

class QuickNodeDef;

#include "ContainerDefBuilderData.h"

#include "QuickPropertyHelpers.h"

// =============================================================================
// Class definition
// =============================================================================
class QuickContainerDef : public QObject
{
    Q_OBJECT
    BUILDER_PROPERTY_STRING(nodeDefName)
    BUILDER_PROPERTY_BASE(int, minCount)
    BUILDER_PROPERTY_BASE(int, maxCount)

public:
    QuickContainerDef(QObject *parent = nullptr);
    Oak::Model::ContainerDefBuilderDataUPtr takeBuilder() { return std::move(m_builderOwner); }

private:
    Oak::Model::ContainerDefBuilderData * m_builder;
    Oak::Model::ContainerDefBuilderDataUPtr m_builderOwner;
};


