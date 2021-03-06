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

#include <QToolBar>

#include "OakModel.h"

class QAction;


namespace Oak::View::QtWidgets {

// =============================================================================
// Class definition
// =============================================================================
class ActionToolBar : public QToolBar
{
    Q_OBJECT
public:
    ActionToolBar(QWidget * parent = nullptr);

    void setModel(Model::OakModel* model);

protected:
    void currentNodeChanged();
    void nodeRemoveBefore(const Model::NodeIndex &nodeIndex);

    void disableAllActions();

protected slots:
    void onActionInsert();
    void onActionDelete();
    void onActionUp();
    void onActionDown();
    void onActionCut();
    void onActionCopy();
    void onActionPaste();

protected:
    Model::OakModel* m_model = nullptr;

    Model::Node m_cutNode;
    Model::Node m_copyNode;

    QAction * m_actionInsert;
    QAction * m_actionDelete;
    QAction * m_actionUp;
    QAction * m_actionDown;
    QAction * m_actionCut;
    QAction * m_actionCopy;
    QAction * m_actionPaste;
};

} // namespace Oak::View::QtWidgets

