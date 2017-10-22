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

INCLUDEPATH += \
    . \
    .. \
    ../OakXML

include(Configure.pri)
include(Variants.pri)
include(Definitions.pri)
include(Builders.pri)
include(Reference.pri)
include(Models.pri)
include(Query.pri)

SOURCES += \
    Conversion.cpp \
    ServiceFunctions.cpp \
    Node.cpp \
    Item.cpp \
    ItemValue.cpp \
    CallbackFunctions.cpp \
    InputOptions.cpp

HEADERS += \
    Conversion.h \
    ServiceFunctions.h \
    Node.h \
    Item.h \
    ItemValue.h \
    CallbackFunctions.h \
    InputOptions.h

CONFIG(debug, debug|release) {
    TARGET = ../../OakModeld
    OBJECTS_DIR = ./debug
} else {
    TARGET = ../../OakModel
    OBJECTS_DIR = ./release
}
