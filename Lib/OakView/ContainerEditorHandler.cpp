/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ContainerEditorHandler.h"

#include "NodeDef.h"

#include <QPushButton>

namespace Oak {
namespace View {

// =============================================================================
// (public)
ContainerEditorHandler::ContainerEditorHandler(Model::Item item, const std::string &name, QObject* parent)
    : QObject(parent), m_item(item), m_name(name)
{

}

// =============================================================================
// (public)
QString ContainerEditorHandler::listDisplayName() const
{
    return QString::fromStdString(m_item.def()->childDef(m_name)->displayName());
}

// =============================================================================
// (public)
int ContainerEditorHandler::entryCount() const
{
    return m_item.childCount(m_name);
}

// =============================================================================
// (public)
QStringList ContainerEditorHandler::entryList() const
{
    int count = m_item.childCount(m_name);
    QStringList eList;
    std::string keyValue;
    QString lName = listDisplayName();
    QString entryId;
    for (int i = 0; i < count; i++)
    {
        Model::Item cItem = m_item.childAt(m_name, i);
        if (cItem.hasKey()) {
            cItem.valueKey().value().get(keyValue);
            entryId = QString::fromStdString(keyValue);
        } else {
            entryId = QString::number(i+1);
        }
        //eList.append(QString("%1[%2]").arg(lName).arg(entryId));
        eList.append(QString("%1").arg(entryId));
    }

    return eList;
}

// =============================================================================
// (public)
bool ContainerEditorHandler::canInsert(int index) const
{
    return m_item.canInsertChild(m_name, index);
}

// =============================================================================
// (public)
bool ContainerEditorHandler::canRemove(int index) const
{
    return m_item.canRemoveChild(m_name, index);
}

// =============================================================================
// (public)
QWidget* ContainerEditorHandler::getEditor()
{
    createEditor();
    return m_editor;
}

// =============================================================================
// (public)
bool ContainerEditorHandler::setNode(const Model::Node& node)
{
    if (!m_item.def()->validateForThis(node)) { return false; }

    m_item = Model::Item(m_item.def(), node, m_item.model());

    updateEditor();

    return true;
}

// =============================================================================
// (public)
void ContainerEditorHandler::updateEditor()
{
    if (m_editor == nullptr) { return; }

    // Todo: Update the Editor...
    ContainerEditor* cEditor = dynamic_cast<ContainerEditor*>(m_editor);
    if (cEditor != nullptr) {
        cEditor->updateEditor();
    }
}

// =============================================================================
// (public)
void ContainerEditorHandler::clearEditor()
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
void ContainerEditorHandler::createEditor()
{
    if (m_editor != nullptr) { return; }

    ContainerEditor* cEditor = new ContainerEditor(this);
    connect(cEditor, SIGNAL(destroyed(QObject*)), this, SLOT(onEditorDestroyed()));
    connect(cEditor, SIGNAL(entryClicked(int)), this, SLOT(onEntryClicked(int)));
    connect(cEditor, SIGNAL(entryInserted(int)), this, SLOT(onEntryInsert(int)));
    connect(cEditor, SIGNAL(entryRemoved(int)), this, SLOT(onEntryRemove(int)));

    m_editor = cEditor;
    updateEditor();
}

// =============================================================================
// (protected slots)
void ContainerEditorHandler::onEditorDestroyed()
{
    m_editor = nullptr;
}

// =============================================================================
// (protected slots)
void ContainerEditorHandler::onEntryClicked(int index)
{
    m_item.childAt(m_name, index).setCurrent();
}

// =============================================================================
// (protected slots)
void ContainerEditorHandler::onEntryInsert(int index)
{
    m_item.insertChild(m_name, index);
    updateEditor();
}

// =============================================================================
// (protected slots)
void ContainerEditorHandler::onEntryRemove(int index)
{
    m_item.removeChild(m_name, index);
    updateEditor();
}

} // namespace View
} // namespace Oak
