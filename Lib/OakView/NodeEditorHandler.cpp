/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "NodeEditorHandler.h"

#include <QLabel>
#include <QGridLayout>
#include <QSpacerItem>

namespace Oak {
namespace View {

// =============================================================================
// (public)
NodeEditorHandler::NodeEditorHandler(QObject *parent, Model::Item item)
    : QObject(parent), m_item(item)
{
}

// =============================================================================
// (public)
NodeEditorHandler::~NodeEditorHandler()
{
    clearEditor();
}

// =============================================================================
// (public)
bool NodeEditorHandler::operator==(const Model::NodeDefinition* nodeDefinition)
{
    return m_item.definition() == nodeDefinition;
}

// =============================================================================
// (public)
bool NodeEditorHandler::operator!=(const Model::NodeDefinition* nodeDefinition)
{
    return m_item.definition() != nodeDefinition;
}

// =============================================================================
// (public)
const Model::NodeDefinition* NodeEditorHandler::nodeDefinition() const
{
    return m_item.definition();
}

// =============================================================================
// (public)
QWidget* NodeEditorHandler::getEditor()
{
    createEditor();
    return m_editor;
}

// =============================================================================
// (public)
bool NodeEditorHandler::setNode(Model::Node node)
{
    if (!m_item.definition()->validateForThis(node)) { return false; }

    m_item = Model::Item(m_item.definition(), node, m_item.model());

    foreach (ValueEditorHandler* vHandler, m_valueEditorMap) {
        vHandler->setNode(node);
    }

    foreach (ContainerEditorHandler* cHandler, m_containerEditorMap) {
        cHandler->setNode(node);
    }

    return true;
}

// =============================================================================
// (public)
void NodeEditorHandler::updateEditorValues()
{
    foreach (ValueEditorHandler* vHandler, m_valueEditorMap) {
        vHandler->updateLabelValue();
        vHandler->updateEditorValue();
    }

    foreach (ContainerEditorHandler* cHandler, m_containerEditorMap) {
        cHandler->updateEditor();
    }
}

// =============================================================================
// (public)
void NodeEditorHandler::getEditorValues()
{
    foreach (ValueEditorHandler* vHandler, m_valueEditorMap) {
        vHandler->getEditorValue();
    }
}

// =============================================================================
// (public)
void NodeEditorHandler::clearEditor()
{
    if (m_editor) {
        if (m_editor->parent() == nullptr) {
            // Delete the editor if it has no parent
            delete m_editor;
        } else {
            // Disconnect any signal from the m_editor to this object
            disconnect(m_editor, 0, this, 0);
        }
        m_editor = nullptr;
    }
}

// =============================================================================
// (protected)
void NodeEditorHandler::createEditor()
{
    if (m_editor != nullptr) { return; }

    m_editor = new QWidget();
    QGridLayout* layout = new QGridLayout();

    int row = 0;

    Model::Item::ValueIterator vIt = m_item.valueBegin();
    Model::Item::ValueIterator vItEnd = m_item.valueEnd();
    while (vIt != vItEnd) {
        Model::VariantCRef id = vIt->valueId();
        if (!m_valueEditorMap.contains(id)) {
            auto vEditor = new ValueEditorHandler(this, *vIt);
            m_valueEditorMap.insert(id, vEditor);
            // TODO: Connect signals
        }
        ValueEditorHandler* vEditor = m_valueEditorMap[id];

        if (vEditor->getLabel()) {
            layout->addWidget(vEditor->getLabel(), row, 0);
            layout->addWidget(vEditor->getEditor(), row, 1);
        } else {
            layout->addWidget(vEditor->getEditor(), row, 0, 1, 2);
        }
        row++;
        vIt++;
    }

    std::vector<const Model::ContainerDefinition*> cList;
    m_item.definition()->getContainerList(cList);

    for (const Model::ContainerDefinition* containerPtr: cList)
    {
        Model::VariantCRef id = containerPtr->containerDefinition()->primaryKey();
        if (!m_containerEditorMap.contains(id)) {
            auto cHandler = new ContainerEditorHandler(m_item, id, this);
            m_containerEditorMap.insert(id, cHandler);
        }

        layout->addWidget(m_containerEditorMap[id]->getEditor(), row++, 0, 1, 2);
    }

    if (row == 0) {
        std::string nodeName = m_item.definition()->name();
        QLabel* emptyLabel = new QLabel(QString("Item %1 is empty").arg(QString::fromStdString(nodeName)));
        layout->addWidget(emptyLabel);
    } else {
        layout->addItem(new QSpacerItem(0,0, QSizePolicy::Preferred, QSizePolicy::Expanding), layout->rowCount(), 0);
    }

    m_editor->setLayout(layout);

    connect(m_editor, SIGNAL(destroyed(QObject*)), this, SLOT(onEditorDestroyed()));
}

// =============================================================================
// (protected slots)
void NodeEditorHandler::onEditorDestroyed()
{
    m_editor = nullptr;
}

} // namespace View
} // namespace Oak
