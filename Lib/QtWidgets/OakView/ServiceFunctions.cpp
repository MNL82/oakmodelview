/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ServiceFunctions.h"


namespace Oak::View::QtWidgets {

QStringList toQStringList(const std::vector<std::string>& stringList)
{
    QStringList qStringList;
    for (auto string: stringList) {
        qStringList.append(QString::fromStdString(string));
    }
    return qStringList;
}

SignalBlocker::SignalBlocker(QObject *object)
    : m_object(object), m_alreadyBlocked(m_object->signalsBlocked())
{
    if (!m_alreadyBlocked) {
        m_object->blockSignals(true);
    }
}

SignalBlocker::~SignalBlocker()
{
    if (!m_alreadyBlocked) {
        m_object->blockSignals(false);
    }
}

} // namespace Oak::View::QtWidgets

