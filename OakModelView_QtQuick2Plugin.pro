TEMPLATE = subdirs
CONFIG += debug_and_release

SUBDIRS += sub_OakXML \
           sub_OakModel \
           sub_QOakModel \
           sub_QQuickOakModelPlugin \
           sub_QQuickOakViewPlugin

sub_OakXML.file      = Lib/OakXML/OakXML.pro

sub_OakModel.file    = Lib/OakModel/OakModel.pro
sub_OakModel.depends = sub_OakXML

sub_QOakModel.file    = Lib/QOakModel/QOakModel.pro
sub_QOakModel.depends = sub_OakXML sub_OakModel

sub_QQuickOakModelPlugin.file = Lib/QQuickOakModelPlugin/QQuickOakModelPlugin.pro
sub_QQuickOakModelPlugin = sub_OakXML sub_OakModel sub_QOakModel

sub_QQuickOakViewPlugin.file     = Lib/QQuickOakViewPlugin/QQuickOakViewPlugin.pro
sub_QQuickOakViewPlugin.depends  = sub_OakXML sub_OakModel sub_QQuickOakModelPlugin
