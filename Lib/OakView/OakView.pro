# oakmodelview - version 0.1.0
# --------------------------------------------------------
# Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
# Report bugs and download new versions at http://oakmodelview.com/
#
# This library is distributed under the MIT License.
# See accompanying file LICENSE in the root folder.

QT       += widgets

TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++11

INCLUDEPATH += \
    . \
    .. \
    ../OakXML \
    ../OakModel

include(Configure.pri)

SOURCES += \
    ValueEditorHandler.cpp \
    NodeEditorHandler.cpp \
    ParentDataView.cpp \
    ContainerEditorHandler.cpp \
    ContainerEditor.cpp \
    ServiceFunctions.cpp \
    NodeDataView.cpp \
    OakView.cpp \
    ListView.cpp \
    TableView.cpp \
    ListViewItem.cpp \
    ActionToolBar.cpp

HEADERS += \
    ValueEditorHandler.h \
    NodeEditorHandler.h \
    ParentDataView.h \
    ContainerEditorHandler.h \
    ContainerEditor.h \
    ServiceFunctions.h \
    NodeDataView.h \
    OakView.h \
    ListView.h \
    TableView.h \
    ListViewItem.h \
    ActionToolBar.h


CONFIG(debug, debug|release) {
    TARGET = ../../OakViewd
    OBJECTS_DIR = ./debug
} else {
    TARGET = ../../OakView
    OBJECTS_DIR = ./release
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    OakViewResources.qrc
