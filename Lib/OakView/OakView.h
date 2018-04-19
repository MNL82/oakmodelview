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

    void setCurrentItem(const Model::Item& item);

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

    QTreeWidgetItem * getTreeItems(Model::Item item, QTreeWidgetItem *parentItem = nullptr);

    void onItemInserted(const Model::Item& parentItem, int index);
    void onItemMoved(const Model::Item& sourceParentItem, int sourceIndex, const Model::Item& targetParentItem, int targetIndex);
    void onItemCloned(const Model::Item& sourceParentItem, int sourceIndex, const Model::Item& targetParentItem, int targetIndex);
    void onItemRemoved(const Model::Item& parentItem, int index);

    void onItemValueChanged(const Model::Item& item, int valueIndex);

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
