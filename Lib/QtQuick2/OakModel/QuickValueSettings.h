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
#include "ValueSettingsBuilderData.h"

#include "QuickPropertyHelpers.h"


// =============================================================================
// Class definition
// =============================================================================
class QuickValueSettings : public QObject
{
    Q_OBJECT
    BUILDER_PROPERTY_BASE(bool, readOnly)
    BUILDER_PROPERTY_STRING(unit)
    BUILDER_PROPERTY_ENUM(QuickTriState::TriStateEnum, Oak::Model::BoolState, unique)
    BUILDER_PROPERTY_ENUM(QuickTriState::TriStateEnum, Oak::Model::BoolState, required)

public:
    QuickValueSettings(QObject *parent = nullptr);
    Oak::Model::ValueSettingsBuilderDataUPtr takeBuilder() { return std::move(m_builderOwner); }

private:
    Oak::Model::ValueSettingsBuilderData * m_builder;
    Oak::Model::ValueSettingsBuilderDataUPtr m_builderOwner;
};
