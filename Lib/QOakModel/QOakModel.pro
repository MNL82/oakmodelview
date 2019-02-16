# oakmodelview - version 0.1.0
# --------------------------------------------------------
# Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
# Report bugs and download new versions at http://oakmodelview.com/
#
# This library is distributed under the MIT License.
# See accompanying file LICENSE in the root folder.

TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++1z

DEFINES += XML_BACKEND
DEFINES += NOMINMAX

INCLUDEPATH += \
    . \
    .. \
    ../OakXML \
    ../OakModel

	
HEADERS += \
    QContainerDefBuilderData.h \
    QPropertyHelpers.h \
    QLeafDefBuilderData.h \
    QLeafSettingsBuilderData.h \
    QNodeDefBuilderData.h \
    QOakBaseTypes.h \
    QServiceFunctions.h \
    QValueOptionsBuilderData.h \
    QOakModel.h \
    QOakModelBuilderData.h


SOURCES += \
    QContainerDefBuilderData.cpp \
    QLeafDefBuilderData.cpp \
    QLeafSettingsBuilderData.cpp \
    QNodeDefBuilderData.cpp \
    QServiceFunctions.cpp \
    QValueOptionsBuilderData.cpp \
    QOakModel.cpp \
    QOakModelBuilderData.cpp

CONFIG(debug, debug|release) {
    TARGET = ../../QOakModeld
    win32:POST_TARGETDEPS = \
        ../OakXMLd.lib \
        ../OakModeld.lib
    win32:LIBS = \
        ../OakXMLd.lib \
        ../OakModeld.lib
    OBJECTS_DIR = ./debug
} else {
    TARGET = ../../QOakModel
    win32:POST_TARGETDEPS = \
        ../OakXML.lib \
        ../OakModel.lib
    win32:LIBS = \
        ../OakXML.lib \
        ../OakModel.lib
    OBJECTS_DIR = ./release
}


