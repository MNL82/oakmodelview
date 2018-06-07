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

namespace Oak {
namespace View {

// =============================================================================
// Class definition
// =============================================================================
class ActionToolBar : public QToolBar
{
    Q_OBJECT
public:
    ActionToolBar(QWidget * parent = nullptr);

    void setModel(Model::OakModel* model);

    void currentItemChanged();

protected:
    void disableAllActions();

protected slots:
    void onActionDelete();

protected:
    Model::OakModel* m_model = nullptr;
    QAction * m_actionRemove;
};

} // namespace View
} // namespace Oak
