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
#include <QString>

#include "QOakBaseTypes.h"
#include "LeafSettingsBuilderData.h"

#include "QPropertyHelpers.h"


// =============================================================================
// Class definition
// =============================================================================
class QLeafSettingsBuilderData : public QObject
{
    Q_OBJECT
    BUILDER_PROPERTY_ENUM(QTriState::TriStateEnum, Oak::Model::BoolState, readOnly)
    BUILDER_PROPERTY_STRING(unit)
    BUILDER_PROPERTY_ENUM(QTriState::TriStateEnum, Oak::Model::BoolState, unique)
    BUILDER_PROPERTY_ENUM(QTriState::TriStateEnum, Oak::Model::BoolState, required)

public:
    QLeafSettingsBuilderData(QObject *parent = nullptr);
    Oak::Model::LeafSettingsBuilderDataUPtr takeBuilder() { return std::move(m_builderOwner); }

private:
    Oak::Model::LeafSettingsBuilderData * m_builder;
    Oak::Model::LeafSettingsBuilderDataUPtr m_builderOwner;
};
