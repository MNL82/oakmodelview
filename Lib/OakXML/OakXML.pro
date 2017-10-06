# oakmodelview - version 0.1.0
# --------------------------------------------------------
# Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
# Report bugs and download new versions at http://oakmodelview.com/
#
# This library is distributed under the MIT License.
# See accompanying file LICENSE in the root folder.

TEMPLATE = lib
CONFIG += staticlib

DEFINES += XML_BACKEND
DEFINES += NOMINMAX

unix::QMAKE_CXXFLAGS += -std=gnu++0x

SOURCES += \
    pugixml/pugixml.cpp \
    XMLElement.cpp \
    XMLValueRef.cpp \
    XMLRefFactory.cpp \
    XMLServiceFunctions.cpp \
    XMLDocument.cpp \
    XMLListRef.cpp \
    XMLRef.cpp \
    XMLChildRef.cpp \
    XMLChildRefGroup.cpp \
    XMLRefGroup.cpp \
    XMLParentRef.cpp

HEADERS += \
    pugixml/pugiconfig.hpp \
    pugixml/pugixml.hpp \
    XMLElement.h \
    XMLValueRef.h \
    XMLRefFactory.h \
    XMLServiceFunctions.h \
    XMLDocument.h \
    XMLListRef.h \
    XMLRef.h \
    XMLChildRef.h \
    XMLChildRefGroup.h \
    XMLRefGroup.h \
    XMLParentRef.h

CONFIG(debug, debug|release) {
    TARGET = ../../OakXMLd
    OBJECTS_DIR = ./debug
} else {
    TARGET = ../../OakXML
    OBJECTS_DIR = ./release
}
