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

TARGET = oaktreeviewplugin

DEFINES += XML_BACKEND
DEFINES += NOMINMAX

DESTDIR += ../Plugins/Oak/TreeView
	
HEADERS += \
    OakTreeViewInternalModel.h \
    OakTreeViewNodeData.h \
    oaktreeviewplugin.h \
    Assert.h \
    Trace.h

SOURCES += \
    OakTreeViewInternalModel.cpp \
    OakTreeViewNodeData.cpp \
    oaktreeviewplugin.cpp

DISTFILES += \
    OakTreeView.qml \
    expandArrow.png

# Make the qmldir file visible in the Project tree
OTHER_FILES += qmldir

# Copy the qmldir file to the same folder as the plugin binary
cpqmldir.files = qmldir
cpqmldir.path = $$DESTDIR

cpqml.files = $$DISTFILES
cpqml.path = $$DESTDIR

COPIES += \
    cpqmldir \
    cppicsdir \
    cpqml

TARGET = $$qtLibraryTarget($$TARGET)

CONFIG(debug, debug|release) {
    OBJECTS_DIR = ./debug
} else {
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


