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

    AUTO_PROPERTY(bool, newActionEnabled)
    AUTO_PROPERTY(bool, loadActionEnabled)
    AUTO_PROPERTY(bool, saveActionEnabled)
    AUTO_PROPERTY(bool, saveAsActionEnabled)

    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QuickOakModelBuilder* builder READ builder WRITE setBuilder NOTIFY builderChanged)

public:
    QuickOakModel(QObject *parent = nullptr);

    QuickOakModelBuilder* builder() const;
    QString name() const;

public slots:
    void setName(QString name);

    void newModel();
    bool loadModel(const QString &filePath);
    bool saveModel();
    bool saveModelAs(const QString &filePath);


private:
    void setBuilder(QuickOakModelBuilder* rootNodeDef);
    void updateEnabledActions();

signals:
    void dataLoaded();
    void builderChanged();

private:
    Oak::Model::OakModel m_model;
    QuickOakModelBuilder* m_builder;
    QString m_name;
};
