/****************************************************************************
** Meta object code from reading C++ file 'types.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/note_naga_engine/core/types.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'types.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN13NoteNagaTrackE_t {};
} // unnamed namespace

template <> constexpr inline auto NoteNagaTrack::qt_create_metaobjectdata<qt_meta_tag_ZN13NoteNagaTrackE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "NoteNagaTrack",
        "metadataChanged",
        "",
        "NoteNagaTrack*",
        "track",
        "std::string",
        "param"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'metadataChanged'
        QtMocHelpers::SignalData<void(NoteNagaTrack *, const std::string &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 }, { 0x80000000 | 5, 6 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<NoteNagaTrack, qt_meta_tag_ZN13NoteNagaTrackE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject NoteNagaTrack::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13NoteNagaTrackE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13NoteNagaTrackE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13NoteNagaTrackE_t>.metaTypes,
    nullptr
} };

void NoteNagaTrack::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<NoteNagaTrack *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->metadataChanged((*reinterpret_cast< std::add_pointer_t<NoteNagaTrack*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::string>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< NoteNagaTrack* >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (NoteNagaTrack::*)(NoteNagaTrack * , const std::string & )>(_a, &NoteNagaTrack::metadataChanged, 0))
            return;
    }
}

const QMetaObject *NoteNagaTrack::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NoteNagaTrack::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13NoteNagaTrackE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NoteNagaTrack::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void NoteNagaTrack::metadataChanged(NoteNagaTrack * _t1, const std::string & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2);
}
namespace {
struct qt_meta_tag_ZN15NoteNagaMidiSeqE_t {};
} // unnamed namespace

template <> constexpr inline auto NoteNagaMidiSeq::qt_create_metaobjectdata<qt_meta_tag_ZN15NoteNagaMidiSeqE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "NoteNagaMidiSeq",
        "metadataChanged",
        "",
        "NoteNagaMidiSeq*",
        "seq",
        "std::string",
        "param",
        "trackMetadataChanged",
        "NoteNagaTrack*",
        "track",
        "activeTrackChanged",
        "trackListChanged"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'metadataChanged'
        QtMocHelpers::SignalData<void(NoteNagaMidiSeq *, const std::string &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 }, { 0x80000000 | 5, 6 },
        }}),
        // Signal 'trackMetadataChanged'
        QtMocHelpers::SignalData<void(NoteNagaTrack *, const std::string &)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 9 }, { 0x80000000 | 5, 6 },
        }}),
        // Signal 'activeTrackChanged'
        QtMocHelpers::SignalData<void(NoteNagaTrack *)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 9 },
        }}),
        // Signal 'trackListChanged'
        QtMocHelpers::SignalData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<NoteNagaMidiSeq, qt_meta_tag_ZN15NoteNagaMidiSeqE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject NoteNagaMidiSeq::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15NoteNagaMidiSeqE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15NoteNagaMidiSeqE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN15NoteNagaMidiSeqE_t>.metaTypes,
    nullptr
} };

void NoteNagaMidiSeq::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<NoteNagaMidiSeq *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->metadataChanged((*reinterpret_cast< std::add_pointer_t<NoteNagaMidiSeq*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::string>>(_a[2]))); break;
        case 1: _t->trackMetadataChanged((*reinterpret_cast< std::add_pointer_t<NoteNagaTrack*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::string>>(_a[2]))); break;
        case 2: _t->activeTrackChanged((*reinterpret_cast< std::add_pointer_t<NoteNagaTrack*>>(_a[1]))); break;
        case 3: _t->trackListChanged(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< NoteNagaMidiSeq* >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< NoteNagaTrack* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< NoteNagaTrack* >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (NoteNagaMidiSeq::*)(NoteNagaMidiSeq * , const std::string & )>(_a, &NoteNagaMidiSeq::metadataChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (NoteNagaMidiSeq::*)(NoteNagaTrack * , const std::string & )>(_a, &NoteNagaMidiSeq::trackMetadataChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (NoteNagaMidiSeq::*)(NoteNagaTrack * )>(_a, &NoteNagaMidiSeq::activeTrackChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (NoteNagaMidiSeq::*)()>(_a, &NoteNagaMidiSeq::trackListChanged, 3))
            return;
    }
}

const QMetaObject *NoteNagaMidiSeq::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NoteNagaMidiSeq::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15NoteNagaMidiSeqE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NoteNagaMidiSeq::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void NoteNagaMidiSeq::metadataChanged(NoteNagaMidiSeq * _t1, const std::string & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2);
}

// SIGNAL 1
void NoteNagaMidiSeq::trackMetadataChanged(NoteNagaTrack * _t1, const std::string & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2);
}

// SIGNAL 2
void NoteNagaMidiSeq::activeTrackChanged(NoteNagaTrack * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void NoteNagaMidiSeq::trackListChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
