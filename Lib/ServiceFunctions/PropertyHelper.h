#pragma once

#include <QObject>


// Creates a read/write property and all the needed functions
#define AUTO_PROPERTY(TYPE, NAME) \
private: \
    Q_PROPERTY(TYPE NAME READ NAME WRITE set_ ## NAME NOTIFY NAME ## _changed ) \
public: \
    const TYPE &NAME() const { return a_ ## NAME ; } \
    void set_ ## NAME(const TYPE &value) { \
        if (a_ ## NAME == value) { return; } \
        a_ ## NAME = value; \
        emit NAME ## _changed(a_ ## NAME); \
    } \
    Q_SIGNAL void NAME ## _changed(const TYPE & value); \
private: \
   TYPE a_ ## NAME;


// Creates a readonly property and all the needed functions
#define AUTO_PROPERTY_READONLY(TYPE, NAME) \
private: \
    Q_PROPERTY(TYPE NAME READ NAME MEMBER a_ ## NAME WRITE set_ ## NAME ) \
public: \
    TYPE NAME() const { return a_ ## NAME ; } \
private: \
    void set_ ## NAME(TYPE value) { a_ ## NAME = value; } \
    TYPE a_ ## NAME;


// Creates a list property and all the needed functions
#define AUTO_PROPERTY_OBJECT_LIST(OWNERTYPE, TYPE, NAMES, NAME) \
private: \
    Q_PROPERTY(QQmlListProperty<TYPE> NAMES READ NAMES) \
    QList<TYPE*> a_ ## NAMES; \
    static void append_ ## NAME(QQmlListProperty<TYPE>* list, TYPE *NAME) { \
        OWNERTYPE *owner = qobject_cast<OWNERTYPE*>(list->object); \
        NAME->setParent(owner); \
        if (owner) { \
            owner->a_ ## NAMES.append(NAME); \
        } \
    } \
    static int NAME ##_count(QQmlListProperty<TYPE>* list) { \
        OWNERTYPE *owner = qobject_cast<OWNERTYPE*>(list->object); \
        return owner->a_ ## NAMES.count(); \
    } \
    static TYPE* NAME(QQmlListProperty<TYPE>* list, int index) { \
        OWNERTYPE *owner = qobject_cast<OWNERTYPE*>(list->object); \
        return owner->a_ ## NAMES[index]; \
    } \
    static void clear_ ## NAME(QQmlListProperty<TYPE>* list) { \
        OWNERTYPE *owner = qobject_cast<OWNERTYPE*>(list->object); \
        return owner->a_ ## NAMES.clear(); \
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


// Creates a list property and all the needed functions
#define AUTO_PROPERTY_LIST(OWNERTYPE, TYPE, NAMES, NAME) \
private: \
    Q_PROPERTY(QQmlListProperty<TYPE> NAMES READ NAMES) \
    QList<TYPE*> a_ ## NAMES; \
    static void append_ ## NAME(QQmlListProperty<TYPE>* list, TYPE *NAME) { \
        OWNERTYPE *owner = qobject_cast<OWNERTYPE*>(list->object); \
        if (owner) { \
            owner->a_ ## NAMES.append(NAME); \
        } \
    } \
    static int NAME ##_count(QQmlListProperty<TYPE>* list) { \
        OWNERTYPE *owner = qobject_cast<OWNERTYPE*>(list->object); \
        return owner->a_ ## NAMES.count(); \
    } \
    static TYPE* NAME(QQmlListProperty<TYPE>* list, int index) { \
        OWNERTYPE *owner = qobject_cast<OWNERTYPE*>(list->object); \
        return owner->a_ ## NAMES[index]; \
    } \
    static void clear_ ## NAME(QQmlListProperty<TYPE>* list) { \
        OWNERTYPE *owner = qobject_cast<OWNERTYPE*>(list->object); \
        return owner->a_ ## NAMES.clear(); \
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
