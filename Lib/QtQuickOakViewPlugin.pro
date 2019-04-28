TEMPLATE = subdirs
CONFIG += debug_and_release

SUBDIRS += sub_OakXML \
           sub_OakModel \
           sub_QtOakModel \
           sub_QtQuickOakModelPlugin \
           sub_QtQuickOakViewPlugin

sub_OakXML.file      = OakXML/OakXML.pro

sub_OakModel.file    = OakModel/OakModel.pro
sub_OakModel.depends = sub_OakXML

sub_QtOakModel.file    = QtOakModel/QtOakModel.pro
sub_QtOakModel.depends = sub_OakXML sub_OakModel

sub_QtQuickOakModelPlugin.file = QtQuickOakModelPlugin/QtQuickOakModelPlugin.pro
sub_QtQuickOakModelPlugin = sub_OakXML sub_OakModel sub_QtOakModel

sub_QtQuickOakViewPlugin.file     = QtQuickOakViewPlugin/QtQuickOakViewPlugin.pro
sub_QtQuickOakViewPlugin.depends  = sub_OakXML sub_OakModel sub_QtQuickOakModelPlugin
