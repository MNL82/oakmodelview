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

#include <QObject>
#include "QuickServiceFunctions.h"

// Creates a read/write property and all the needed functions
#define BUILDER_PROPERTY_BASE(TYPE, NAME) \
private: \
    Q_PROPERTY(TYPE NAME READ NAME WRITE set_ ## NAME NOTIFY NAME ## _changed ) \
public: \
    TYPE NAME() const { return m_builder->NAME; } \
    void set_ ## NAME(TYPE value) { \
        if (m_builder->NAME == value) { return; } \
        m_builder->NAME = value; \
        emit NAME ## _changed(m_builder->NAME); \
    } \
    Q_SIGNAL void NAME ## _changed(const TYPE & value); \


// Creates a read/write property and all the needed functions
#define BUILDER_PROPERTY_STRING(NAME) \
private: \
    Q_PROPERTY(QString NAME READ NAME WRITE set_ ## NAME NOTIFY NAME ## _changed ) \
public: \
    QString NAME() const { return QString::fromStdString(m_builder->NAME); } \
    void set_ ## NAME(const QString &value) { \
        std::string v = value.toStdString(); \
        if (m_builder->NAME == v) { return; } \
        m_builder->NAME = v; \
        emit NAME ## _changed(value); \
    } \
    Q_SIGNAL void NAME ## _changed(QString value); \


// Creates a read/write property and all the needed functions
#define BUILDER_PROPERTY_STRING_LIST(NAME) \
private: \
    Q_PROPERTY(QStringList NAME READ NAME WRITE set_ ## NAME NOTIFY NAME ## _changed ) \
public: \
    QStringList NAME() const { return toQStringList(m_builder->NAME); } \
    void set_ ## NAME(const QStringList &value) { \
        std::vector<std::string> v = toStdStringList(value); \
        if (m_builder->NAME == v) { return; } \
        m_builder->NAME = v; \
        emit NAME ## _changed(value); \
    } \
    Q_SIGNAL void NAME ## _changed(QStringList value); \

// Creates a read/write property and all the needed functions
#define BUILDER_PROPERTY_VARIANT(NAME) \
private: \
    Q_PROPERTY(QVariant NAME READ NAME WRITE set_ ## NAME NOTIFY NAME ## _changed ) \
public: \
    QVariant NAME() const { return toQVariant(m_builder->NAME); } \
    void set_ ## NAME(const QVariant &value) { \
        Oak::Model::UnionValue v = toUnionValue(value); \
        if (m_builder->NAME == v) { return; } \
        m_builder->NAME = v; \
        emit NAME ## _changed(value); \
    } \
    Q_SIGNAL void NAME ## _changed(QVariant value); \

// Creates a read/write property and all the needed functions
#define BUILDER_PROPERTY_COLOR(NAME) \
private: \
    Q_PROPERTY(QColor NAME READ NAME WRITE set_ ## NAME NOTIFY NAME ## _changed ) \
public: \
    QColor NAME() const { return toQColor(m_builder->NAME); } \
    void set_ ## NAME(const QColor &value) { \
        Oak::Model::Color v = toOakColor(value); \
        if (m_builder->NAME == v) { return; } \
        m_builder->NAME = v; \
        emit NAME ## _changed(value); \
    } \
    Q_SIGNAL void NAME ## _changed(QColor value); \


// Creates a read/write property and all the needed functions
#define BUILDER_PROPERTY_VARIANT_LIST(NAME) \
private: \
    Q_PROPERTY(QVariantList NAME READ NAME WRITE set_ ## NAME NOTIFY NAME ## _changed ) \
public: \
    QVariantList NAME() const { return toQVariantList(m_builder->NAME); } \
    void set_ ## NAME(const QVariantList &value) { \
        std::vector<Oak::Model::UnionValue> v = toUnionValueList(value); \
        if (m_builder->NAME == v) { return; } \
        m_builder->NAME = v; \
        emit NAME ## _changed(value); \
    } \
    Q_SIGNAL void NAME ## _changed(QVariantList value); \


// Creates a read/write property and all the needed functions
#define BUILDER_PROPERTY_ENUM(ENUM1, ENUM2, NAME) \
private: \
    Q_PROPERTY(ENUM1 NAME READ NAME WRITE set_ ## NAME NOTIFY NAME ## _changed ) \
public: \
    ENUM1 NAME() const { return ENUM1(static_cast<int>(m_builder->NAME)); } \
    void set_ ## NAME(const ENUM1 &value) { \
        ENUM2 v = ENUM2(static_cast<int>(value)); \
        if (m_builder->NAME == v) { return; } \
        m_builder->NAME = v; \
        emit NAME ## _changed(value); \
    } \
    Q_SIGNAL void NAME ## _changed(ENUM1 value); \


// Creates a read/write property and all the needed functions
#define BUILDER_PROPERTY_OBJECT(TYPE, NAME) \
private: \
    Q_PROPERTY(TYPE* NAME READ NAME WRITE set_ ## NAME NOTIFY NAME ## _changed ) \
    public: \
    const TYPE* NAME() const { return m_ ## NAME ; } \
    TYPE* NAME() { return m_ ## NAME ; } \
    void set_ ## NAME(TYPE *value) { \
        if (m_ ## NAME == value) { return; } \
        if (m_ ## NAME != nullptr) { m_ ## NAME->setParent(nullptr); } \
        m_ ## NAME = value; \
        m_ ## NAME->setParent(this); \
        m_builder->NAME = m_ ## NAME->takeBuilder(); \
        emit NAME ## _changed(m_ ## NAME); \
    } \
    Q_SIGNAL void NAME ## _changed(const TYPE* value); \
private: \
    TYPE* m_ ## NAME = nullptr;


// Creates a list property and all the needed functions
#define BUILDER_PROPERTY_OBJECT_LIST(OWNERTYPE, TYPE, NAMES, NAME) \
private: \
    Q_PROPERTY(QQmlListProperty<TYPE> NAMES READ NAMES) \
    QList<TYPE*> m_ ## NAMES; \
    static void append_ ## NAME(QQmlListProperty<TYPE>* list, TYPE *NAME) { \
        OWNERTYPE *owner = qobject_cast<OWNERTYPE*>(list->object); \
        if (owner) { \
            NAME->setParent(owner); \
            owner->m_builder->NAMES.push_back(NAME->takeBuilder()); \
            owner->m_ ## NAMES.append(NAME); \
        } \
    } \
    static int NAME ## _count(QQmlListProperty<TYPE>* list) { \
        OWNERTYPE *owner = qobject_cast<OWNERTYPE*>(list->object); \
        return owner->m_ ## NAMES.count(); \
    } \
    static TYPE* NAME(QQmlListProperty<TYPE>* list, int index) { \
        OWNERTYPE *owner = qobject_cast<OWNERTYPE*>(list->object); \
        return owner->m_ ## NAMES[index]; \
    } \
    static void clear_ ## NAME(QQmlListProperty<TYPE>* list) { \
        OWNERTYPE *owner = qobject_cast<OWNERTYPE*>(list->object); \
        return owner->m_ ## NAMES.clear(); \
    } \
public: \
    QQmlListProperty<TYPE> NAMES() { \
        return QQmlListProperty<TYPE>(this, this, \
                &OWNERTYPE::append_ ## NAME, \
                &OWNERTYPE::NAME ## _count, \
                &OWNERTYPE::NAME, \
                &OWNERTYPE::clear_ ## NAME); \
    } \
private: \

