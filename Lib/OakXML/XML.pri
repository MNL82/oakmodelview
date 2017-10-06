# oakmodelview - version 0.1.0
# --------------------------------------------------------
# Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
# Report bugs and download new versions at http://oakmodelview.com/
#
# This library is distributed under the MIT License.
# See accompanying file LICENSE in the root folder.

SOURCES += \
    pugixml/pugixml.cpp \
    $$PWD/XMLElement.cpp \
    $$PWD/XMLElementListRef.cpp \
    $$PWD/XMLElementRef.cpp \
    $$PWD/XMLElementRefChild.cpp \
    $$PWD/XMLElementRefGroup.cpp \
    $$PWD/XMLElementRefParent.cpp \
    $$PWD/XMLValueRef.cpp \
    $$PWD/XMLElementRefChildGroup.cpp \
    $$PWD/XMLRefFactory.cpp \
    $$PWD/XMLDoc.cpp

HEADERS += \
    pugixml/pugiconfig.hpp \
    pugixml/pugixml.hpp \
    $$PWD/XMLElement.h \
    $$PWD/XMLElementListRef.h \
    $$PWD/XMLElementRef.h \
    $$PWD/XMLElementRefChild.h \
    $$PWD/XMLElementRefGroup.h \
    $$PWD/XMLElementRefParent.h \
    $$PWD/XMLValueRef.h \
    $$PWD/XMLElementRefChildGroup.h \
    $$PWD/XMLRefFactory.h \
    $$PWD/XMLDoc.h
