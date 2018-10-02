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

namespace Oak {
namespace View {

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

    void currentItemChanged();

    void setCurrentItem(const Model::Item& item);

    NodeEditorHandler* getEditorHandler(const Model::Item& item);

protected:
    void modelDestroyed();
    void setCurrentEditor(QWidget* editor);
    void setCurrentWidget(int index);

    void onEntryChangeAfter(const Model::ItemIndex &itemIndex, int valueIndex);

protected:
    Model::OakModel* m_model = nullptr;

    QStackedWidget* m_stackedWidget;
    int m_initialWidgetCount;

    std::vector<NodeEditorHandlerUPtr> m_editorList;
};

} // namespace View
} // namespace Oak
