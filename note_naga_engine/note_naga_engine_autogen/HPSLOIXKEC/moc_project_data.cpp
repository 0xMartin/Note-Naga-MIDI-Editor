/****************************************************************************
** Meta object code from reading C++ file 'project_data.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/note_naga_engine/core/project_data.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'project_data.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN15NoteNagaProjectE_t {};
} // unnamed namespace

template <> constexpr inline auto NoteNagaProject::qt_create_metaobjectdata<qt_meta_tag_ZN15NoteNagaProjectE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "NoteNagaProject",
        "projectFileLoaded",
        "",
        "currentTickChanged",
        "tick",
        "sequenceMetadataChanged",
        "NoteNagaMidiSeq*",
        "seq",
        "std::string",
        "param",
        "trackMetaChanged",
        "NoteNagaTrack*",
        "track",
        "activeSequenceChanged",
        "activeSequenceTrackListChanged"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'projectFileLoaded'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'currentTickChanged'
        QtMocHelpers::SignalData<void(int)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 4 },
        }}),
        // Signal 'sequenceMetadataChanged'
        QtMocHelpers::SignalData<void(NoteNagaMidiSeq *, const std::string &)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 6, 7 }, { 0x80000000 | 8, 9 },
        }}),
        // Signal 'trackMetaChanged'
        QtMocHelpers::SignalData<void(NoteNagaTrack *, const std::string &)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 11, 12 }, { 0x80000000 | 8, 9 },
        }}),
        // Signal 'activeSequenceChanged'
        QtMocHelpers::SignalData<void(NoteNagaMidiSeq *)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 6, 7 },
        }}),
        // Signal 'activeSequenceTrackListChanged'
        QtMocHelpers::SignalData<void(NoteNagaMidiSeq *)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 6, 7 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<NoteNagaProject, qt_meta_tag_ZN15NoteNagaProjectE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject NoteNagaProject::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15NoteNagaProjectE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15NoteNagaProjectE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN15NoteNagaProjectE_t>.metaTypes,
    nullptr
} };

void NoteNagaProject::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<NoteNagaProject *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->projectFileLoaded(); break;
        case 1: _t->currentTickChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->sequenceMetadataChanged((*reinterpret_cast< std::add_pointer_t<NoteNagaMidiSeq*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::string>>(_a[2]))); break;
        case 3: _t->trackMetaChanged((*reinterpret_cast< std::add_pointer_t<NoteNagaTrack*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::string>>(_a[2]))); break;
        case 4: _t->activeSequenceChanged((*reinterpret_cast< std::add_pointer_t<NoteNagaMidiSeq*>>(_a[1]))); break;
        case 5: _t->activeSequenceTrackListChanged((*reinterpret_cast< std::add_pointer_t<NoteNagaMidiSeq*>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< NoteNagaMidiSeq* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< NoteNagaTrack* >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< NoteNagaMidiSeq* >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< NoteNagaMidiSeq* >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (NoteNagaProject::*)()>(_a, &NoteNagaProject::projectFileLoaded, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (NoteNagaProject::*)(int )>(_a, &NoteNagaProject::currentTickChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (NoteNagaProject::*)(NoteNagaMidiSeq * , const std::string & )>(_a, &NoteNagaProject::sequenceMetadataChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (NoteNagaProject::*)(NoteNagaTrack * , const std::string & )>(_a, &NoteNagaProject::trackMetaChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (NoteNagaProject::*)(NoteNagaMidiSeq * )>(_a, &NoteNagaProject::activeSequenceChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (NoteNagaProject::*)(NoteNagaMidiSeq * )>(_a, &NoteNagaProject::activeSequenceTrackListChanged, 5))
            return;
    }
}

const QMetaObject *NoteNagaProject::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NoteNagaProject::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15NoteNagaProjectE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NoteNagaProject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void NoteNagaProject::projectFileLoaded()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void NoteNagaProject::currentTickChanged(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void NoteNagaProject::sequenceMetadataChanged(NoteNagaMidiSeq * _t1, const std::string & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2);
}

// SIGNAL 3
void NoteNagaProject::trackMetaChanged(NoteNagaTrack * _t1, const std::string & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2);
}

// SIGNAL 4
void NoteNagaProject::activeSequenceChanged(NoteNagaMidiSeq * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void NoteNagaProject::activeSequenceTrackListChanged(NoteNagaMidiSeq * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}
QT_WARNING_POP
