/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ParentDataView.h"


namespace Oak::View::QtWidgets {

// =============================================================================
// (public)
ParentDataView::ParentDataView(QWidget* parent)
    : QWidget(parent)
{
    m_layout = new QHBoxLayout();
    m_layout->setMargin(0);
    m_layout->addItem(new QSpacerItem(0,0, QSizePolicy::Expanding, QSizePolicy::Preferred));
    setLayout(m_layout);
}

// =============================================================================
// (public)
void ParentDataView::setModel(Model::OakModel* model)
{
    if (m_model == model) { return; }

    if (m_model) {
        // Disconnect the old model
        m_model->notifier_currentNodeChanged.remove(this);
    }

    // Change the model
    m_model = model;

    if (m_model) {
        // connect the new mobel
        m_model->notifier_currentNodeChanged.add(this, &ParentDataView::currentNodeChanged);
        if (!m_model->currentNode().isNull()) {
            currentNodeChanged();
        }
    }

    // todo: Reload widget if loaded
}

// =============================================================================
// (public)
void ParentDataView::currentNodeChanged()
{
    setCurrentNode(m_model->currentNode());
}

// =============================================================================
// (public)
void ParentDataView::setCurrentNode(const Model::Node& node)
{
    m_nodeList.clear();
    Model::Node bNode(node);
    while (!bNode.isNull()) {
        m_nodeList.prepend(bNode);
        bNode = bNode.parent();
    }

    int index = 0;
    QLayoutItem* layoutNode;
    QPushButton* button;
    while (index < m_nodeList.count()) {
        bNode = m_nodeList.at(index);
        QString nodeName = QString::fromStdString(bNode.def()->displayName());
        if (index < m_layout->count()-1) {
            layoutNode = m_layout->itemAt(index);
            button = dynamic_cast<QPushButton*>(layoutNode->widget());
            if (button) {
                button->setText(nodeName);
                index++;
                continue;
            }
        }
        button = new QPushButton(nodeName);
        connect(button, SIGNAL(pressed()), this, SLOT(onButtonClicked()));
        m_layout->insertWidget(index, button);
        index++;
    }

    while (index < m_layout->count()-1) {
        delete m_layout->takeAt(index)->widget();
    }
    m_layout->invalidate();
}

// =============================================================================
// (protected slots)
void ParentDataView::onButtonClicked()
{
    QPushButton* button = dynamic_cast<QPushButton*>(sender());
    if (button) {
        int index = m_layout->indexOf(button);
        if (index >= 0 && index < m_nodeList.count()) {
            m_model->setCurrentNode(m_nodeList.at(index));
        }
    }
}

} // namespace Oak::View::QtWidgets

