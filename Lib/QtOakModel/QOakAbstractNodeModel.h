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

#include <QOakModel.h>

// =============================================================================
// Class definition
// =============================================================================
class QOakAbstractNodeModel : public QAbstractItemModel
{
    Q_OBJECT

    Q_PROPERTY(QModelIndex sourceItem READ sourceItem WRITE setSourceItem NOTIFY sourceItemChanged)

public:
    QOakAbstractNodeModel(QObject *parent = nullptr);
    //QOakAbstractNodeModel(const QOakAbstractNodeModel &other);
    virtual ~QOakAbstractNodeModel() override;

    virtual bool isNull() const;

    const Oak::Model::Node &node() const;
    const Oak::Model::NodeIndex *nodeIndex() const;
    const QModelIndex &sourceItem() const;

    const QOakModel *sourceModel() const;
    const Oak::Model::OakModel *sourceOakModel() const;

public slots:
    void setSourceItem(QModelIndex _sourceItem);

protected:
    void clearModel();
    virtual void sourceModelConnect() = 0;
    virtual void sourceModelDisconnect() = 0;
    virtual void sourceItemChanged();

signals:
    void sourceItemChanged(QModelIndex sourceItem);

protected:
    Oak::Model::Node m_node;
    Oak::Model::NodeIndexUPtr m_nodeIndexUPtr;
    QModelIndex m_sourceItem;

    const QOakModel * m_sourceModel = nullptr;
};

