/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#pragma once

#include <QStringList>
#include <QVariantList>
#include <vector>
#include <string>
#include <QDateTime>
#include <QColor>

#include "UnionValue.h"
#include "Color.h"

Oak::Model::Color toOakColor(const QColor& v1);
QColor toQColor(const Oak::Model::Color& v1);

std::vector<std::string> toStdStringList(const QStringList& list1);
QStringList toQStringList(const std::vector<std::string>& list1);

Oak::Model::UnionValue toUnionValue(const QVariant& v1);
QVariant toQVariant(const Oak::Model::UnionValue& v1);

Oak::Model::UnionValueList toUnionValueList(const QVariantList& vList);
QVariantList toQVariantList(const Oak::Model::UnionValueList& sList);
