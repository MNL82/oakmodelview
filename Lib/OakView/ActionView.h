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

#include <QObject>

#include "OakModel.h"

namespace Oak {
namespace View {

// =============================================================================
// Class definition
// =============================================================================
class ActionView : public QObject
{
    Q_OBJECT
public:
    ActionView(QObject * parent = nullptr);

    void setModel(Model::OakModel* model);

    void currentItemChanged();


protected:
    Model::OakModel* m_model = nullptr;


};

} // namespace View
} // namespace Oak
