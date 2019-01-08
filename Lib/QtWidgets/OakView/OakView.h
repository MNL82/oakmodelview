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


namespace Oak::View::QtWidgets {

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

    void currentNodeChanged();

    void setCurrentNode(const Model::NodeIndex &nodeIndex);

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

    QTreeWidgetItem * getTreeNodes(const Model::Node &node, QTreeWidgetItem *parentNode = nullptr);

    void onNodeInserteAfter(const Model::NodeIndex& nodeIndex);
    void onNodeMoveAfter(const Model::NodeIndex& sourceNodeIndex, const Model::NodeIndex& targetNodeIndex);
    void onNodeCloneAfter(const Model::NodeIndex& sourceNodeIndex, const Model::NodeIndex& targetNodeIndex);
    void onNodeRemoveBefore(const Model::NodeIndex& nodeIndex);

    void onVariantLeafChangeAfter(const Model::NodeIndex& nodeIndex);
    void onKeyLeafChangeAfter(const Model::NodeIndex& nodeIndex);

    QTreeWidgetItem * widgetFromIndex(const Model::NodeIndex &nodeIndex, bool parentWidget = false);
    Model::NodeIndexUPtr indexFromWidget(QTreeWidgetItem *nodeWidget);

protected slots:
    void createTreeStructure();
    void onCurrentQNodeChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);

protected:
    Model::OakModel* m_model = nullptr;

    // Drag & drop
    QStringList m_acceptedDropNames;
    //XMLElementAbstractPtr m_dragSource;
    QPoint m_mousePressedPosition;
    Model::Node m_dragNode;
};

} // namespace Oak::View::QtWidgets

