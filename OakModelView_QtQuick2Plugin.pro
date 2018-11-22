TEMPLATE = subdirs
CONFIG += debug_and_release

SUBDIRS += sub_OakXML \
           sub_OakModel \
           sub_OakModel_QtQuick2Plugin

sub_OakXML.file      = Lib/OakXML/OakXML.pro

sub_OakModel.file    = Lib/OakModel/OakModel.pro
sub_OakModel.depends = sub_OakXML

sub_OakModel_QtQuick2Plugin.file     = Lib/QtQuick2/OakModel/OakModel_QtQuick2Plugin.pro
sub_OakModel_QtQuick2Plugin.depends  = sub_OakXML sub_OakModel
