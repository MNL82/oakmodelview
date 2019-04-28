TEMPLATE = subdirs
CONFIG += debug_and_release

SUBDIRS += sub_OakXML \
           sub_OakModel \
           sub_QtWidgetOakView \
           sub_ODataBrowser

sub_OakXML.file      = Lib/OakXML/OakXML.pro

sub_OakModel.file    = Lib/OakModel/OakModel.pro
sub_OakModel.depends = sub_OakXML

sub_QtWidgetOakView.file     = Lib/QtWidgetOakView/QtWidgetOakView.pro
sub_QtWidgetOakView.depends  = sub_OakXML sub_OakModel

sub_ODataBrowser.file    = ODataBrowser/ODataBrowser.pro
sub_ODataBrowser.depends = sub_OakXML sub_OakModel sub_QtWidgetOakView
