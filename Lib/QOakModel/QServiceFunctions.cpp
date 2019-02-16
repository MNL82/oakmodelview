/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "QServiceFunctions.h"

// =============================================================================
// (global)
Oak::Model::Color toOakColor(const QColor& v1)
{
    return Oak::Model::Color(v1.red(), v1.green(), v1.blue(), v1.alpha());
}

// =============================================================================
// (global)
QColor toQColor(const Oak::Model::Color& v1)
{
    return QColor(v1.red(), v1.green(), v1.blue(), v1.alpha());
}

// =============================================================================
// (global)
std::vector<std::string> toStdStringList(const QStringList& list1)
{
    std::vector<std::string> list2;
    list2.reserve(static_cast<size_t>(list1.size()));
    foreach (const QString& str, list1) {
        list2.push_back(str.toStdString());
    }

    return list2;
}

// =============================================================================
// (global)
QStringList toQStringList(const std::vector<std::string>& list1)
{
    QStringList list2;
    list2.reserve(static_cast<int>(list1.size()));
    for (const std::string& str: list1) {
        list2.push_back(QString::fromStdString(str));
    }

    return list2;
}

// =============================================================================
// (global)
Oak::Model::UnionValue toUnionValue(const QVariant& v1)
{
    switch (v1.type()) {
    case QVariant::Type::Invalid: {
        return Oak::Model::UnionValue();
    }
    case QVariant::Type::Bool: {
        return Oak::Model::UnionValue(v1.toBool());
    }
    case QVariant::Type::Int: {
        return Oak::Model::UnionValue(v1.toInt());
    }
    case QVariant::Type::Double: {
        return Oak::Model::UnionValue(v1.toDouble());
    }
    case QVariant::Type::String: {
        return Oak::Model::UnionValue(v1.toString().toStdString());
    }
    case QVariant::Type::DateTime: {
        Oak::Model::DateTime dt;
        dt.setMSecsSinceEpoch(v1.toDateTime().toMSecsSinceEpoch());
        return Oak::Model::UnionValue(dt);
    }
    default: {
        assert(true);
        return Oak::Model::UnionValue();
    }
    }
}

// =============================================================================
// (global)
QVariant toQVariant(const Oak::Model::UnionValue& v1)
{
    QVariant v2;
    switch (v1.type()) {
    case Oak::Model::UnionType::Undefined:{
        return QVariant();
    }
    case Oak::Model::UnionType::Char:{
        return QVariant(v1.value<char*>());
    }
    case Oak::Model::UnionType::Bool:{
        return QVariant(v1.value<bool>());
    }
    case Oak::Model::UnionType::Integer:{
        return QVariant(v1.value<int>());
    }
    case Oak::Model::UnionType::Double:{
        return QVariant(v1.value<double>());
    }
    case Oak::Model::UnionType::String:{
        return QVariant(QString::fromStdString(v1.value<std::string>()));
    }
    case Oak::Model::UnionType::DateTime:{
        QDateTime dt;
        dt.setMSecsSinceEpoch(v1.value<Oak::Model::DateTime>().mSecsSinceEpoch());
        return QVariant::fromValue(dt);
    }
    default: {
        assert(false);
        return QVariant();
    }
    }
}

// =============================================================================
// (global)
Oak::Model::UnionValueList toUnionValueList(const QVariantList& vList)
{
    Oak::Model::UnionValueList list;
    list.reserve(static_cast<size_t>(vList.size()));
    foreach (const QVariant& v, vList) {
        list.push_back(toUnionValue(v));
    }

    return list;
}

// =============================================================================
// (global)
QVariantList toQVariantList(const Oak::Model::UnionValueList& sList)
{
    QVariantList list;
    list.reserve(static_cast<int>(sList.size()));
    for (const Oak::Model::UnionValue& v: sList) {
        list.push_back(toQVariant(v));
    }

    return list;
}
