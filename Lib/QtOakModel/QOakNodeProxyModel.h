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

#include "QOakModel.h"

// =============================================================================
// Class definition
// =============================================================================
class QOakNodeProxyModel : public QAbstractItemModel
{
    Q_OBJECT

    Q_PROPERTY(QModelIndex sourceModelItem READ sourceModelItem WRITE setSourceModelItem NOTIFY sourceModelItemChanged)

public:
    QOakNodeProxyModel(QObject *parent = nullptr);
    virtual ~QOakNodeProxyModel() override;

    bool isNull() const;

    // Basic functiona6lity:
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    virtual QModelIndex sibling(int row, int column, const QModelIndex& index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual bool hasChildren(const QModelIndex& parent) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    QModelIndex sourceModelItem() const;

    const Oak::Model::Leaf& toLeaf(const QModelIndex &index) const;

public slots:
    void setSourceModelItem(QModelIndex sourceModelItem);

signals:
    void sourceModelItemChanged(QModelIndex sourceModelItem);

protected:
    void onLeafValueChanged(const Oak::Model::NodeIndex& nIndex, const std::string &str);
    void clearModel();

protected:
    QModelIndex m_sourceModelItem;
    const QOakModel * m_sourceModel = nullptr;
    Oak::Model::Node m_node;
    Oak::Model::NodeIndexUPtr m_nodeIndexUPtr;
};
