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

#include <vector>

#include <QStringList>
#include <QObject>


namespace Oak::View::QtWidgets {

QStringList toQStringList(const std::vector<std::string>& stringList);

class SignalBlocker{
    public:
        SignalBlocker(QObject *object);
        ~SignalBlocker();

    private:
        QObject *m_object;
        bool m_alreadyBlocked;
};

} // namespace Oak::View::QtWidgets

