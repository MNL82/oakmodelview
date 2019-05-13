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

#include <QtQml/QQmlListProperty>

#include "QOakAbstractNodeModel.h"
#include "QOakModel.h"
#include "QOakToolButton.h"

// =============================================================================
// Class definition
// =============================================================================
class QOakToolBarModel : public QOakAbstractNodeModel
{
    Q_OBJECT

    AUTO_PROPERTY_OBJECT_LIST(QOakToolBarModel, QOakToolButton, buttons, button)

public:
    enum Roles  {
        ImagePath = Qt::UserRole + 11,
        Color = Qt::UserRole + 12,
        Enabled = Qt::UserRole + 13
    };
    Q_ENUM(Roles)

    QOakToolBarModel(QObject *parent = nullptr);

public slots:
    void trigger(int index);

protected:
    virtual void sourceModelConnect() override;
    virtual void sourceModelDisconnect() override;
    virtual void sourceItemChanged() override;

protected slots:
    void onToolButtonChange(int index, QVector<int> roles);

public:
    virtual QModelIndex index(int row, int column, const QModelIndex& parent) const override;
    virtual QModelIndex parent(const QModelIndex& child) const override;
    virtual int rowCount(const QModelIndex& parent) const override;
    virtual int columnCount(const QModelIndex& parent) const override;
    virtual bool hasChildren(const QModelIndex& parent) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;


    virtual Qt::ItemFlags flags(const QModelIndex& index) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

};



