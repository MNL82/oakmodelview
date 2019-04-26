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
#include <QVariant>

#include "Node.h"

class QOakToolBarModel;

// =============================================================================
// Class definition
// =============================================================================
class QOakToolButton : public QObject
{
    Q_OBJECT
public:
    QOakToolButton(QObject *parent = nullptr);

    virtual ~QOakToolButton();

    virtual int count() const;

    virtual QVariant data(int index, int role) const;

    virtual void sourceModelConnect();
    virtual void sourceModelDisconnect();

    void setToolBarModel(const QOakToolBarModel *toolBarModel);

    virtual void trigger(int index);

signals:
    void buttonChanged(int index, QVector<int> roles);

protected:
    const QOakToolBarModel * m_toolBarModel;
};
