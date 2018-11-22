# oakmodelview - version 0.1.0
# --------------------------------------------------------
# Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
# Report bugs and download new versions at http://oakmodelview.com/
#
# This library is distributed under the MIT License.
# See accompanying file LICENSE in the root folder.

TEMPLATE = subdirs
CONFIG += debug_and_release

SUBDIRS += sub_OakXML \
           sub_OakModel \
           sub_OakView \
           sub_TestOakView

sub_OakXML.file         = OakXML/OakXML.pro

sub_OakModel.file       = OakModel/OakModel.pro
sub_OakMoedl.depends    = sub_OakXML

sub_OakView.file        = QtWidgets/OakView/OakView.pro
sub_OakView.depends     = sub_OakXML sub_OakModel

sub_TestOakView.file    = QtWidgets/TestOakView/TestOakView.pro
sub_TestOakView.depends = sub_OakXML sub_OakModel sub_OakView
