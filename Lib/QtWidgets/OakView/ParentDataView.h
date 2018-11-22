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

#include <QPushButton>
#include <QHBoxLayout>


namespace Oak::View::QtWidgets {

// =============================================================================
// Class definition
// =============================================================================
class ParentDataView : public QWidget
{
    Q_OBJECT
public:
    ParentDataView(QWidget* parent = nullptr);

    void setModel(Model::OakModel* model);

    void currentItemChanged();

    void setCurrentItem(const Model::Item& item);

protected slots:
    void onButtonClicked();

protected:
    Model::OakModel* m_model = nullptr;

    QList<Model::Item> m_itemList;
    QHBoxLayout* m_layout;
};

} // namespace Oak::View::QtWidgets

