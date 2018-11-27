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

#include "OakModel.h"

#include "../ServiceFunctions/PropertyHelper.h"

class QuickOakModelBuilder;

// =============================================================================
// Class definition
// =============================================================================
class QuickOakModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)

    Q_PROPERTY(QuickOakModelBuilder* builder READ builder WRITE setBuilder NOTIFY builderChanged)

public:
    QuickOakModel(QObject *parent = nullptr);

    QuickOakModelBuilder* builder() const;
    QString name() const;

public slots:
    bool loadData(const QString &filePath);
    void setName(QString name);

private:
    void setBuilder(QuickOakModelBuilder* rootNodeDef);

signals:
    void dataLoaded();
    void builderChanged();

private:
    Oak::Model::OakModel m_model;
    QuickOakModelBuilder* m_builder;
    QString m_name;
};
