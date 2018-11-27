# oakmodelview - version 0.1.0
# --------------------------------------------------------
# Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
# Report bugs and download new versions at http://oakmodelview.com/
#
# This library is distributed under the MIT License.
# See accompanying file LICENSE in the root folder.

TEMPLATE = lib
CONFIG += plugin
CONFIG += c++1z
QT += qml quick

TARGET = oakmodelplugin

DEFINES += XML_BACKEND
DEFINES += NOMINMAX

DESTDIR += ../../Plugins/Oak/Model

INCLUDEPATH += \
    . \
    .. \
    ../../OakXML \
    ../../OakModel

	
HEADERS += \
    oakmodelplugin.h \
    QuickOakModel.h \
    QuickNodeDef.h \
    QuickContainerDef.h \
    QuickOakModelBuilder.h \
    QuickValueDef.h \
    QuickValueOptions.h \
    QuickValueSettings.h \
    QuickOakBaseTypes.h \
    QuickServiceFunctions.h \
    QuickPropertyHelpers.h


SOURCES += \
    oakmodelplugin.cpp \
    QuickOakModel.cpp \
    QuickNodeDef.cpp \
    QuickContainerDef.cpp \
    QuickOakModelBuilder.cpp \
    QuickValueDef.cpp \
    QuickValueOptions.cpp \
    QuickValueSettings.cpp \
    QuickServiceFunctions.cpp

# Make the qmldir file visible in the Project tree
OTHER_FILES += qmldir

# Copy the qmldir file to the same folder as the plugin binary
cpqmldir.files = qmldir
cpqmldir.path = $$DESTDIR
COPIES += cpqmldir




TARGET = $$qtLibraryTarget($$TARGET)

CONFIG(debug, debug|release) {
    win32:POST_TARGETDEPS = \
        ../../OakXMLd.lib \
        ../../OakModeld.lib
    win32:LIBS = \
        ../../OakXMLd.lib \
        ../../OakModeld.lib
    OBJECTS_DIR = ./debug
} else {
    win32:POST_TARGETDEPS = \
        ../../OakXML.lib \
        ../../OakModel.lib
    win32:LIBS = \
        ../../OakXML.lib \
        ../../OakModel.lib
    OBJECTS_DIR = ./release
}

##### Stuff needed by the installer #####

#DESTPATH = ../../installer

#target.files += files.qml
#target.path = $$DESTPATH

#qmldir.files = $$PWD/qmldir
#qmldir.path  =$$DESTPATH

#INSTALLS += target qmldir

#CONFIG += install_ok  # Do not cargo-cult this!


