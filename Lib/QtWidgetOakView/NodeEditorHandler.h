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

#include "Node.h"
#include "ValueEditorHandler.h"
#include "ContainerEditorHandler.h"

#include <QWidget>
#include <QMap>


namespace Oak::View::QtWidgets {

class NodeEditorHandler;
typedef std::unique_ptr<NodeEditorHandler> NodeEditorHandlerUPtr;

// =============================================================================
// Class definition
// =============================================================================
class NodeEditorHandler : public QObject
{
    Q_OBJECT
public:
    NodeEditorHandler(QObject* parent, const Model::Node &node);
    virtual ~NodeEditorHandler();

    bool operator==(const Model::NodeDef* nodeDef);
    bool operator!=(const Model::NodeDef* nodeDef);

    const Model::NodeDef* nodeDef() const;

    QWidget* getEditor();

    bool setNode(const Model::NodeData &nodeData);

    void updateEditorValue(const std::string &name);
    void updateEditorValues();
    void getEditorValues();

    void clearEditor();

protected:
    void createEditor();

protected slots:
    void onEditorDestroyed();

protected:
    Model::Node m_node;
    QWidget* m_editor = nullptr;
    QMap<std::string, ValueEditorHandler*> m_valueEditorMap;
    QMap<std::string, ContainerEditorHandler*> m_containerEditorMap;
};

} // namespace Oak::View::QtWidgets

