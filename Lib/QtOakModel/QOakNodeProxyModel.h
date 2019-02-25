#ifndef OAKNODEPROXYMODEL_H
#define OAKNODEPROXYMODEL_H

#include <QAbstractItemModel>

class OakNodeProxyModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit OakNodeProxyModel(QObject *parent = nullptr);

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

private:
};

#endif // OAKNODEPROXYMODEL_H