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

#include "Item.h"
#include "ValueEditorHandler.h"
#include "ContainerEditorHandler.h"

#include <QWidget>
#include <QMap>

namespace Oak {
namespace View {

class NodeEditorHandler;
typedef std::unique_ptr<NodeEditorHandler> NodeEditorHandlerUPtr;

// =============================================================================
// Class definition
// =============================================================================
class NodeEditorHandler : public QObject
{
    Q_OBJECT
public:
    NodeEditorHandler(QObject* parent, Model::Item item);
    virtual ~NodeEditorHandler();

    bool operator==(const Model::NodeDefinition* nodeDefinition);
    bool operator!=(const Model::NodeDefinition* nodeDefinition);

    const Model::NodeDefinition* nodeDefinition() const;

    QWidget* getEditor();

    bool setNode(Model::Node node);

    void updateEditorValues();
    void getEditorValues();

    void clearEditor();

protected:
    void createEditor();

protected slots:
    void onEditorDestroyed();

protected:
    Model::Item m_item;
    QWidget* m_editor = nullptr;
    QMap<Model::VariantCRef, ValueEditorHandler*> m_valueEditorMap;
    QMap<Model::VariantCRef, ContainerEditorHandler*> m_containerEditorMap;
};

} // namespace View
} // namespace Oak
