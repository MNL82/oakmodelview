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

#include "OakModel.h"
#include "NodeEditorHandler.h"

#include <QStackedWidget>
#include <QMap>
#include <QTreeWidget>

namespace Oak {
namespace View {

// =============================================================================
// Class definition
// =============================================================================
class OakView : public QTreeWidget
{
    Q_OBJECT
public:
    OakView(QWidget* parent = nullptr);
    virtual ~OakView() override;

    void setOakModel(Model::OakModel* model);

    void currentItemChanged();

    void setCurrentItem(const Model::ItemIndex &itemIndex);

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void dragEnterEvent(QDragEnterEvent * event) override;
    virtual void dragLeaveEvent(QDragLeaveEvent * event) override;
    virtual void dragMoveEvent(QDragMoveEvent * event) override;
    virtual void dropEvent(QDropEvent * event) override;
    virtual void startDrag(Qt::DropActions supportedActions) override;

protected:
    void modelDestroyed();
    void clearTreeStructure();
    void updateTreeStructure();

    QTreeWidgetItem * getTreeItems(const Model::Item &item, QTreeWidgetItem *parentItem = nullptr);

    void onItemInserteAfter(const Model::ItemIndex& itemIndex);
    void onItemMoveAfter(const Model::ItemIndex& sourceItemIndex, const Model::ItemIndex& targetItemIndex);
    void onItemCloneAfter(const Model::ItemIndex& sourceItemIndex, const Model::ItemIndex& targetItemIndex);
    void onItemRemoveBefore(const Model::ItemIndex& itemIndex);

    void onEntryTypeChangeAfter(const Model::ItemIndex& itemIndex);
    void onEntryKeyChangeAfter(const Model::ItemIndex& itemIndex);

    QTreeWidgetItem * widgetFromIndex(const Model::ItemIndex &itemIndex, bool parentWidget = false);
    Model::ItemIndexUPtr indexFromWidget(QTreeWidgetItem *itemWidget);

protected slots:
    void createTreeStructure();
    void onCurrentQItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);

protected:
    Model::OakModel* m_model = nullptr;

    // Drag & drop
    QStringList m_acceptedDropNames;
    //XMLElementAbstractPtr m_dragSource;
    QPoint m_mousePressedPosition;
    Model::Item m_dragItem;
};

} // namespace View
} // namespace Oak
