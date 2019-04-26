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

#include "QOakAbstractNodeModel.h"

// =============================================================================
// Class definition
// =============================================================================
class QOakNodeProxyModel : public QOakAbstractNodeModel
{
    Q_OBJECT

public:
    enum Roles  {
        DisplayName = Qt::UserRole + 10,
        LeafValidator = Qt::UserRole + 11,
        HasOptions = Qt::UserRole + 12,
        OptionsOnly = Qt::UserRole + 13,
        Options = Qt::UserRole +14,
        Unit = Qt::UserRole +15,
        Required = Qt::UserRole + 16
    };

    QOakNodeProxyModel(QObject *parent = nullptr);
    virtual ~QOakNodeProxyModel() override;

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

    const Oak::Model::Leaf& toLeaf(const QModelIndex &index) const;

protected:
    virtual void sourceModelConnect() override;
    virtual void sourceModelDisconnect() override;
    virtual void sourceItemChanged() override;

    void onLeafValueChanged(const Oak::Model::NodeIndex& nIndex, const std::string &str);
    void onVariantLeafChanged(const Oak::Model::NodeIndex& nIndex);
};
