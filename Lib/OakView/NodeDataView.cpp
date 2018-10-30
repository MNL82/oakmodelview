/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "NodeDataView.h"

#include <QVBoxLayout>
#include <QStackedWidget>
#include <QLabel>
#include <QDebug>

namespace Oak {
namespace View {

// =============================================================================
// (public)
NodeDataView::NodeDataView(QWidget* parent)
    : QWidget(parent)
{
    //qDebug() << "NodeDataView()";
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setMargin(0);
    m_stackedWidget = new QStackedWidget();

    QLabel* emptyLabel = new QLabel("Oak Model not Loaded");
    emptyLabel->setAlignment(Qt::AlignCenter);

    m_stackedWidget->addWidget(emptyLabel);
    // remove all other widgets
    while (m_stackedWidget->count() > 1) {
        m_stackedWidget->removeWidget(m_stackedWidget->widget(0));
    }

    layout->addWidget(m_stackedWidget);
    setLayout(layout);

    m_initialWidgetCount = m_stackedWidget->count();
}

// =============================================================================
// (public)
NodeDataView::~NodeDataView()
{
    //qDebug() << "~NodeDataView()";
    setModel(nullptr);
}

// =============================================================================
// (public)
void NodeDataView::setModel(Model::OakModel* model)
{
    if (m_model == model) { return; }

    if (m_model) {
        // Disconnect the old model
        m_model->notifier_currentItemChanged.remove(this);
        m_model->notifier_rootNodeDefChanged.remove(this);

        m_model->notifier_destroyed.remove(this);

        m_model->notifier_entryChangeAfter.remove(this);
    }

    // Change the model
    m_model = model;

    if (m_model) {
        // connect the new mobel
        m_model->notifier_currentItemChanged.add(this, &NodeDataView::currentItemChanged);
        m_model->notifier_rootNodeDefChanged.add(this, &NodeDataView::clearEditorCash);

        m_model->notifier_destroyed.add(this, &NodeDataView::modelDestroyed);
        m_model->notifier_entryChangeAfter.add(this, &NodeDataView::onEntryChangeAfter);

        if (!m_model->currentItem().isNull()) {
            currentItemChanged();
        }
    }

    // todo: Reload widget if loaded
}

// =============================================================================
// (public)
void NodeDataView::clearEditorCash()
{
    setCurrentWidget(0);
    m_editorList.clear();
    for(int i = m_stackedWidget->count()-1; i >= m_initialWidgetCount; i--)
    {
        QWidget* widget = m_stackedWidget->widget(i);
        m_stackedWidget->removeWidget(widget);
        widget->deleteLater();
    }
}

// =============================================================================
// (public)
void NodeDataView::currentItemChanged()
{
    setCurrentItem(m_model->currentItem());
}

// =============================================================================
// (public)
void NodeDataView::setCurrentItem(const Model::Item& item)
{
    if (item.isNull()) {
        setCurrentWidget(0);
        return;
    }

    NodeEditorHandler* eHandler = getEditorHandler(item);
    if (eHandler == nullptr) { return; }

    eHandler->setNode(item.node());
    QWidget* editor = eHandler->getEditor();
    setCurrentEditor(editor);
}

// =============================================================================
// (public)
NodeEditorHandler* NodeDataView::getEditorHandler(const Model::Item& item)
{
    if (item.isDefNull()) { return nullptr; }

    for (const auto& eHandler: m_editorList) {
        if (eHandler->nodeDef() == item.def()) {
            return eHandler.get();
        }
    }
    m_editorList.push_back(std::make_unique<NodeEditorHandler>(this, item));
    NodeEditorHandler* nHandler = m_editorList.back().get();
    return nHandler;
}

// =============================================================================
// (protected)
void NodeDataView::modelDestroyed()
{
    m_model = nullptr;
}

// =============================================================================
// (protected)
void NodeDataView::setCurrentEditor(QWidget* editor)
{
    for (int i = 0; i < m_stackedWidget->count(); i++) {
        if (m_stackedWidget->widget(i) == editor) {
            setCurrentWidget(i);
            return;
        }
    }
    int index = m_stackedWidget->count();
    m_stackedWidget->addWidget(editor);
    setCurrentWidget(index);
}

// =============================================================================
// (protected)
void NodeDataView::setCurrentWidget(int index)
{
    if (m_stackedWidget->currentIndex() != index) {
        m_stackedWidget->currentWidget()->hide();
        m_stackedWidget->setCurrentIndex(index);
        m_stackedWidget->currentWidget()->show();
    }
}

// =============================================================================
// (protected)
void NodeDataView::onEntryChangeAfter(const Model::ItemIndex &itemIndex, const std::string &valueName)
{

    if (m_model->currentItemIndex().equal(itemIndex)) {
        // Pass on the event to the current editor
        getEditorHandler(m_model->currentItem())->updateEditorValue(valueName);
    }
}

} // namespace View
} // namespace Oak
