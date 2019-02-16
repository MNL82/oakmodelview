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

#include <QAbstractItemModel>
#include <QString>

#include "OakModel.h"

#include "../ServiceFunctions/PropertyHelper.h"

class QOakModelBuilderData;

// =============================================================================
// Class definition
// =============================================================================
class QOakModel : public QAbstractItemModel
{
    Q_OBJECT

    AUTO_PROPERTY(bool, newActionEnabled)
    AUTO_PROPERTY(bool, loadActionEnabled)
    AUTO_PROPERTY(bool, saveActionEnabled)
    AUTO_PROPERTY(bool, saveAsActionEnabled)

    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QOakModelBuilderData* builder READ builder WRITE setBuilder NOTIFY builderChanged)

public:
    enum Roles  {
        DisplayName = Qt::UserRole + 1,
        Name = Qt::UserRole + 2,
        KeyValue = Qt::UserRole + 3,
        VariantValue = Qt::UserRole + 4
    };
    Q_ENUM(Roles)

    QOakModel(QObject *parent = nullptr);

    QOakModelBuilderData* builder() const;
    QString name() const;

public slots:
    void setName(QString name);

    void newModel();
    bool loadModel(const QString &filePath);
    bool saveModel();
    bool saveModelAs(const QString &filePath);


private:
    void setBuilder(QOakModelBuilderData* rootNodeDef);
    void updateEnabledActions();

signals:
    void dataLoaded();
    void builderChanged();

private:
    Oak::Model::OakModel m_model;
    QOakModelBuilderData* m_builder;
    QString m_name;

    // QAbstractItemModel interface
public:
    virtual QModelIndex index(int row, int column, const QModelIndex& parent) const override;
    virtual QModelIndex parent(const QModelIndex& child) const override;
    virtual QModelIndex sibling(int row, int column, const QModelIndex& idx) const override;
    virtual int rowCount(const QModelIndex& parent) const override;
    virtual int columnCount(const QModelIndex& parent) const override;
    virtual bool hasChildren(const QModelIndex& parent) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;
//    virtual bool setData(const QModelIndex& index, const QVariant& value, int role) override;
//    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
//    virtual bool setHeaderData(int section, Qt::Orientation orientation, const QVariant& value, int role) override;
//    virtual QMap<int, QVariant> itemData(const QModelIndex& index) const override;
//    virtual bool setItemData(const QModelIndex& index, const QMap<int, QVariant>& roles) override;
//    virtual QStringList mimeTypes() const override;
//    virtual QMimeData*mimeData(const QModelIndexList& indexes) const override;
//    virtual bool canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) const override;
//    virtual bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) override;
//    virtual Qt::DropActions supportedDropActions() const override;
//    virtual Qt::DropActions supportedDragActions() const override;
//    virtual bool insertRows(int row, int count, const QModelIndex& parent) override;
//    virtual bool insertColumns(int column, int count, const QModelIndex& parent) override;
//    virtual bool removeRows(int row, int count, const QModelIndex& parent) override;
//    virtual bool removeColumns(int column, int count, const QModelIndex& parent) override;
//    virtual bool moveRows(const QModelIndex& sourceParent, int sourceRow, int count, const QModelIndex& destinationParent, int destinationChild) override;
//    virtual bool moveColumns(const QModelIndex& sourceParent, int sourceColumn, int count, const QModelIndex& destinationParent, int destinationChild) override;
//    virtual void fetchMore(const QModelIndex& parent) override;
//    virtual bool canFetchMore(const QModelIndex& parent) const override;
//    virtual Qt::ItemFlags flags(const QModelIndex& index) const override;
//    virtual void sort(int column, Qt::SortOrder order) override;
//    virtual QModelIndex buddy(const QModelIndex& index) const override;
//    virtual QModelIndexList match(const QModelIndex& start, int role, const QVariant& value, int hits, Qt::MatchFlags flags) const override;
//    virtual QSize span(const QModelIndex& index) const override;
    virtual QHash<int, QByteArray> roleNames() const override;


//public slots:
//    virtual bool submit() override;
//    virtual void revert() override;

public:
    QModelIndex createModelIndex(int row, int column, const Oak::Model::Node &node) const;
    Oak::Model::Node nodeFromIndex(const QModelIndex &index) const;
};
