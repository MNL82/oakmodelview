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


namespace Oak::View::QtWidgets {

// =============================================================================
// Class definition
// =============================================================================
class NodeDataView : public QWidget
{
    Q_OBJECT
public:
    NodeDataView(QWidget* parent = nullptr);
    virtual ~NodeDataView();

    void setModel(Model::OakModel* model);

    void clearEditorCash();

    void currentNodeChanged();

    void setCurrentNode(const Model::Node& node);

    NodeEditorHandler* getEditorHandler(const Model::Node& node);

protected:
    void modelDestroyed();
    void setCurrentEditor(QWidget* editor);
    void setCurrentWidget(int index);

    void onLeafChangeAfter(const Model::NodeIndex &nodeIndex, const std::string &valueName);

protected:
    Model::OakModel* m_model = nullptr;

    QStackedWidget* m_stackedWidget;
    int m_initialWidgetCount;

    std::vector<NodeEditorHandlerUPtr> m_editorList;
};

} // namespace Oak::View::QtWidgets

