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

#include "QuickOakBaseTypes.h"
#include "LeafSettingsBuilderData.h"

#include "QuickPropertyHelpers.h"


// =============================================================================
// Class definition
// =============================================================================
class QuickLeafSettings : public QObject
{
    Q_OBJECT
    BUILDER_PROPERTY_ENUM(QuickTriState::TriStateEnum, Oak::Model::BoolState, readOnly)
    BUILDER_PROPERTY_STRING(unit)
    BUILDER_PROPERTY_ENUM(QuickTriState::TriStateEnum, Oak::Model::BoolState, unique)
    BUILDER_PROPERTY_ENUM(QuickTriState::TriStateEnum, Oak::Model::BoolState, required)

public:
    QuickLeafSettings(QObject *parent = nullptr);
    Oak::Model::LeafSettingsBuilderDataUPtr takeBuilder() { return std::move(m_builderOwner); }

private:
    Oak::Model::LeafSettingsBuilderData * m_builder;
    Oak::Model::LeafSettingsBuilderDataUPtr m_builderOwner;
};
