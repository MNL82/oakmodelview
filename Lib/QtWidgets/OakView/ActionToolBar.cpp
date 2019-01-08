/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ActionToolBar.h"

#include <QMessageBox>
#include <QAction>
#include <QIcon>
#include <QImage>


namespace Oak::View::QtWidgets {

// =============================================================================
// (public)
ActionToolBar::ActionToolBar(QWidget *parent)
    : QToolBar("Model Actions", parent)
{
    m_actionInsert = new QAction(QPixmap(":/OakView/Resources/add_32.png"), "Insert node before");
    m_actionInsert->setEnabled(false);
    connect(m_actionInsert, SIGNAL(triggered()), this, SLOT(onActionInsert()));
    addAction(m_actionInsert);

    m_actionDelete = new QAction(QPixmap(":/OakView/Resources/delete_32.png"), "Delete node");
    m_actionDelete->setEnabled(false);
    m_actionDelete->setShortcut(QKeySequence::Delete);
    connect(m_actionDelete, SIGNAL(triggered()), this, SLOT(onActionDelete()));
    addAction(m_actionDelete);

    addSeparator();

    m_actionCut = new QAction(QPixmap(":/OakView/Resources/cut_32.png"), "Cut node");
    m_actionCut->setEnabled(false);
    m_actionCut->setShortcut(QKeySequence::Cut);
    connect(m_actionCut, SIGNAL(triggered()), this, SLOT(onActionCut()));
    addAction(m_actionCut);

    m_actionCopy = new QAction(QPixmap(":/OakView/Resources/copy_32.png"), "Copy node");
    m_actionCopy->setEnabled(false);
    m_actionCopy->setShortcut(QKeySequence::Copy);
    connect(m_actionCopy, SIGNAL(triggered()), this, SLOT(onActionCopy()));
    addAction(m_actionCopy);

    m_actionPaste = new QAction(QPixmap(":/OakView/Resources/paste_32.png"), "Paste node before");
    m_actionPaste->setEnabled(false);
    m_actionPaste->setShortcut(QKeySequence::Paste);
    connect(m_actionPaste, SIGNAL(triggered()), this, SLOT(onActionPaste()));
    addAction(m_actionPaste);

    addSeparator();

    m_actionUp = new QAction(QPixmap(":/OakView/Resources/up_32.png"), "Move node up");
    m_actionUp->setEnabled(false);
    connect(m_actionUp, SIGNAL(triggered()), this, SLOT(onActionUp()));
    addAction(m_actionUp);

    m_actionDown = new QAction(QPixmap(":/OakView/Resources/down_32.png"), "Move node down");
    m_actionDown->setEnabled(false);
    connect(m_actionDown, SIGNAL(triggered()), this, SLOT(onActionDown()));
    addAction(m_actionDown);
}

// =============================================================================
// (public)
void ActionToolBar::setModel(Model::OakModel *model)
{
    if (m_model == model) { return; }

    if (m_model) {
        // Disconnect the old model
        m_model->notifier_currentNodeChanged.remove(this);
        m_model->notifier_nodeRemoveBefore.remove(this);
    }

    // Change the model
    m_model = model;

    if (m_model) {
        // connect the new mobel
        m_model->notifier_currentNodeChanged.add(this, &ActionToolBar::currentNodeChanged);
        m_model->notifier_nodeRemoveBefore.add(this, &ActionToolBar::nodeRemoveBefore);
    }
}

// =============================================================================
// (protected)
void ActionToolBar::currentNodeChanged()
{
    Model::Node node = m_model->currentNode();
    if (node.isNull()) {
        disableAllActions();
        return;
    }

    Model::Node pNode = node.parent();
    if (!pNode.isNodeNull()) {
        int index = pNode.childIndex(node);
        m_actionInsert->setEnabled(pNode.canInsertChild(node.def()->name(), index));
        m_actionDelete->setEnabled(pNode.canRemoveChild(index));
        int index2 = index-1;
        m_actionUp->setEnabled(index2 >= 0 && pNode.canMoveChild(index2, node));
        index2 = index+1;
        m_actionDown->setEnabled(pNode.canMoveChild(index2, node));
        m_actionCut->setEnabled(true);
        m_actionCopy->setEnabled(true);

        if (!m_copyNode.isNull()) {
            index2 = index;
            if (pNode.canCloneChild(index2, m_copyNode)) {
                m_actionPaste->setEnabled(true);
            } else {
                index2 = -1;
                m_actionPaste->setEnabled(node.canCloneChild(index2, m_copyNode));
            }
        } else if (!m_cutNode.isNull()) {
            index2 = index;
            if (pNode.canMoveChild(index2, m_cutNode)) {
                m_actionPaste->setEnabled(true);
            } else {
                index2 = -1;
                m_actionPaste->setEnabled(node.canMoveChild(index2, m_cutNode));
            }
        } else {
            m_actionPaste->setEnabled(false);
        }
    } else{
        disableAllActions();
    }

}

// =============================================================================
// (protected)
void ActionToolBar::nodeRemoveBefore(const Model::NodeIndex &nodeIndex)
{
    Model::Node node = nodeIndex.node(m_model->rootNode());
    Model::Node cNode = m_cutNode;
    while (!cNode.isNull()) {
        if (cNode == node) {
            m_cutNode = Model::Node();
            m_actionPaste->setEnabled(false);
            return;
        }
        cNode = cNode.parent();
    }

    cNode = m_copyNode;
    while (!cNode.isNull()) {
        if (cNode == node) {
            m_copyNode = Model::Node();
            m_actionPaste->setEnabled(false);
            return;
        }
        cNode = cNode.parent();
    }
}

// =============================================================================
// (protected)
void ActionToolBar::disableAllActions()
{
    m_actionInsert->setEnabled(false);
    m_actionDelete->setEnabled(false);
    m_actionUp->setEnabled(false);
    m_actionDown->setEnabled(false);
    m_actionCut->setEnabled(false);
    m_actionCopy->setEnabled(false);
    m_actionPaste->setEnabled(false);
}

// =============================================================================
// (protected slots)
void ActionToolBar::onActionInsert()
{
    Model::Node node = m_model->currentNode();
    Model::Node pNode = node.parent();
    int index = pNode.childIndex(node);
    if (pNode.canInsertChild(node.def()->name(), index)) {
        Model::Node newNode = pNode.insertChild(node.def()->name(), index);
        m_model->setCurrentNode(newNode);
        setFocus(); // Return focus to the action tool bar from whatever view inserted the node
    }
}

// =============================================================================
// (protected slots)
void ActionToolBar::onActionDelete()
{
    if (QMessageBox::question(this, "Delete", "Are you sure you want to delete current node?") == QMessageBox::Yes) {
        Model::Node node = m_model->currentNode();
        Model::Node pNode = node.parent();
        int index = pNode.childIndex(node.def()->name(), node);
        pNode.removeChild(node.def()->name(), index);
        setFocus(); // Return focus to the action tool bar from whatever view deleted the node
    }
}

// =============================================================================
// (protected slots)
void ActionToolBar::onActionUp()
{
    Model::Node node = m_model->currentNode();
    Model::Node pNode = node.parent();
    int index = pNode.childIndex(node)-1;
    pNode.moveChild(index, node);
    setFocus(); // Return focus to the action tool bar from whatever view moved the node
}

// =============================================================================
// (protected slots)
void ActionToolBar::onActionDown()
{
    Model::Node node = m_model->currentNode();
    Model::Node pNode = node.parent();
    int index = pNode.childIndex(node)+1;
    pNode.moveChild(index, node);
    setFocus(); // Return focus to the action tool bar from whatever view moved the node
}

// =============================================================================
// (protected slots)
void ActionToolBar::onActionCut()
{
    m_cutNode = m_model->currentNode();
    m_copyNode = Model::Node();
}

// =============================================================================
// (protected slots)
void ActionToolBar::onActionCopy()
{
    m_cutNode = Model::Node();
    m_copyNode = m_model->currentNode();
}

// =============================================================================
// (protected slots)
void ActionToolBar::onActionPaste()
{
    if (!m_copyNode.isNull()) {
        Model::Node node = m_model->currentNode();
        Model::Node pNode = node.parent();
        int index = pNode.childIndex(node);
        if (pNode.canCloneChild(index, m_copyNode)) {
            Model::Node newNode = pNode.cloneChild(index, m_copyNode);
            m_model->setCurrentNode(newNode);
        } else {
            index = -1;
            Model::Node newNode = node.cloneChild(index, m_copyNode);
            m_model->setCurrentNode(newNode);
        }
        setFocus(); // Return focus to the action tool bar from whatever view copied the node
    } else if (!m_cutNode.isNull()) {
        Model::Node node = m_model->currentNode();
        Model::Node pNode = node.parent();
        int index = pNode.childIndex(node);
        if (pNode.canMoveChild(index, m_cutNode)) {
            Model::Node newNode = pNode.moveChild(index, m_cutNode);
            m_model->setCurrentNode(newNode);
        } else {
            index = -1;
            Model::Node newNode = node.moveChild(index, m_cutNode);
            m_model->setCurrentNode(newNode);
        }
        m_cutNode = Model::Node();
        m_actionPaste->setEnabled(false);
        setFocus(); // Return focus to the action tool bar from whatever view moved the node
    }
}

} // namespace Oak::View::QtWidgets

