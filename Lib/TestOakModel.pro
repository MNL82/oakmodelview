TEMPLATE = subdirs
CONFIG += debug_and_release

SUBDIRS += sub_OakXML \
           sub_OakModel \
           sub_TestOakModel

sub_OakXML.file          = OakXML/OakXML.pro

sub_OakModel.file        = OakModel/OakModel.pro
sub_OakModel.depends     = sub_OakXML

sub_TestOakModel.file    = TestOakModel/TestOakModel.pro
sub_TestOakModel.depends = sub_OakXML sub_OakModel
