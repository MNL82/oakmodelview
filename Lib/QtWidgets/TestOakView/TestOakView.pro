# oakmodelview - version 0.1.0
# --------------------------------------------------------
# Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
# Report bugs and download new versions at http://oakmodelview.com/
#
# This library is distributed under the MIT License.
# See accompanying file LICENSE in the root folder.

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test
TEMPLATE = app

DEFINES += RESOURCE_PATH=\\\"E:/oakmodelview/Lib/TestOakModel/resources/\\\"

INCLUDEPATH += \
    .. \
    ../OakXML \
    ../OakModel \
    ../OakView

include(../OakView/configure.pri)

SOURCES += \
    main.cpp\
    Test_ServiceFunctions.cpp \
    Test_OakView.cpp

HEADERS += \
    Test_ServiceFunctions.h \
    Test_OakView.h

FORMS += \
    Test_OakView.ui

CONFIG(debug, debug|release) {
    TARGET = testd
    win32:POST_TARGETDEPS = \
        ../OakXMLd.lib \
        ../OakModeld.lib \
        ../OakViewd.lib
    win32:LIBS = \
        ../OakXMLd.lib \
        ../OakModeld.lib \
        ../OakViewd.lib
    OBJECTS_DIR = ./debug

} else {
    TARGET = test
    win32:POST_TARGETDEPS = \
        ../OakXML.lib \
        ../OakModel.lib \
        ../OakView.lib
    win32:LIBS = \
        ../OakXML.lib \
        ../OakModel.lib \
        ../OakView.lib
    OBJECTS_DIR = ./release
}
