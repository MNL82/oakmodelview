# oakmodelview - version 0.1.0
# --------------------------------------------------------
# Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
# Report bugs and download new versions at http://oakmodelview.com/
#
# This library is distributed under the MIT License.
# See accompanying file LICENSE in the root folder.

CONFIG += \
    debug_and_release

DEFINES += RESOURCE_PATH=\\\"E:/dev/oakmodelview/Lib/TestOakModel/resources/\\\"

INCLUDEPATH +=  \
    .. \
    ../OakXML \
    ../OakModel

include(../OakModel/Configure.pri)

SOURCES += \
    main.cpp \
    Test_ServiceFunctions.cpp

HEADERS += \
    Test_XML.h \
    Test_XMLReferences.h \
    Test_XMLDoc.h \
    Test_Item.h \
    Test_ServiceFunctions.h \
    Test_Variant.h \
    Test_NodeDefinition.h \
    Test_ValueDefinition.h \
    Test_ItemQuery.h

win32:QMAKE_CXXFLAGS_EXCEPTIONS_ON = /EHa
win32:QMAKE_CXXFLAGS_STL_ON = /EHa

CONFIG(debug, debug|release) {
    TARGET = TDMTestd
    win32:POST_TARGETDEPS = \
        ../OakXMLd.lib \
        ../OakModeld.lib
    win32:LIBS = \
        ../OakXMLd.lib \
        ../OakModeld.lib
    OBJECTS_DIR = ./debug

} else {
    TARGET = TDMTest
    win32:POST_TARGETDEPS = \
        ../OakXML.lib \
        ../OakModel.lib
    win32:LIBS = \
        ../OakXML.lib \
        ../OakModel.lib
    OBJECTS_DIR = ./release
}

RESOURCES +=
