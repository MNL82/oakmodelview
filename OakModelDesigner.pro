TEMPLATE = subdirs
CONFIG += debug_and_release

SUBDIRS += sub_OakXML \
           sub_OakModel \
           sub_OakView \
           sub_OakModelDesigner

sub_OakXML.file      = Lib/OakXML/OakXML.pro

sub_OakModel.file    = Lib/OakModel/OakModel.pro
sub_OakModel.depends = sub_OakXML

sub_OakView.file     = Lib/OakView/OakView.pro
sub_OakView.depends  = sub_OakXML sub_OakModel

sub_OakModelDesigner.file    = Designer/OakModelDesigner.pro
sub_OakModelDesigner.depends = sub_OakXML sub_OakModel sub_OakView
