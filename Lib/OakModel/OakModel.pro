# oakmodelview - version 0.1.0
# --------------------------------------------------------
# Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
# Report bugs and download new versions at http://oakmodelview.com/
#
# This library is distributed under the MIT License.
# See accompanying file LICENSE in the root folder.

TEMPLATE = lib
CONFIG += staticlib

unix::QMAKE_CXXFLAGS += -std=gnu++0x

include(../Configure.pri)

INCLUDEPATH += \
    . \
    .. \
    ../OakXML

include(Definitions.pri)
include(Builders.pri)
include(Reference.pri)
include(Models.pri)
include(Query.pri)
include(Nodes.pri)
include(Union.pri)
include(Observers.pri)

SOURCES += \
    Conversion.cpp \
    CallbackFunctions.cpp \
    Color.cpp \
    OakModelServiceFunctions.cpp \
    NodeData.cpp

HEADERS += \
    Conversion.h \
    CallbackFunctions.h \
    Color.h \
    OakModelServiceFunctions.h \
    NodeData.h

CONFIG(debug, debug|release) {
    TARGET = ../../OakModeld
    OBJECTS_DIR = ./debug
} else {
    TARGET = ../../OakModel
    OBJECTS_DIR = ./release
}
