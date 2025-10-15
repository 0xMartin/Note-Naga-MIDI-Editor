/****************************************************************************
** Meta object code from reading C++ file 'mixer.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/note_naga_engine/module/mixer.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mixer.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN13NoteNagaMixerE_t {};
} // unnamed namespace

template <> constexpr inline auto NoteNagaMixer::qt_create_metaobjectdata<qt_meta_tag_ZN13NoteNagaMixerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "NoteNagaMixer",
        "routingEntryStackChanged",
        "",
        "noteInSignal",
        "NN_Note_t",
        "note",
        "noteOutSignal",
        "std::string",
        "device_name",
        "channel"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'routingEntryStackChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'noteInSignal'
        QtMocHelpers::SignalData<void(const NN_Note_t &)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 4, 5 },
        }}),
        // Signal 'noteOutSignal'
        QtMocHelpers::SignalData<void(const NN_Note_t &, const std::string &, int)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 4, 5 }, { 0x80000000 | 7, 8 }, { QMetaType::Int, 9 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<NoteNagaMixer, qt_meta_tag_ZN13NoteNagaMixerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject NoteNagaMixer::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13NoteNagaMixerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13NoteNagaMixerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13NoteNagaMixerE_t>.metaTypes,
    nullptr
} };

void NoteNagaMixer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<NoteNagaMixer *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->routingEntryStackChanged(); break;
        case 1: _t->noteInSignal((*reinterpret_cast< std::add_pointer_t<NN_Note_t>>(_a[1]))); break;
        case 2: _t->noteOutSignal((*reinterpret_cast< std::add_pointer_t<NN_Note_t>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::string>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (NoteNagaMixer::*)()>(_a, &NoteNagaMixer::routingEntryStackChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (NoteNagaMixer::*)(const NN_Note_t & )>(_a, &NoteNagaMixer::noteInSignal, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (NoteNagaMixer::*)(const NN_Note_t & , const std::string & , int )>(_a, &NoteNagaMixer::noteOutSignal, 2))
            return;
    }
}

const QMetaObject *NoteNagaMixer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NoteNagaMixer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13NoteNagaMixerE_t>.strings))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "AsyncQueueComponent<NN_MixerMessage_t,1024>"))
        return static_cast< AsyncQueueComponent<NN_MixerMessage_t,1024>*>(this);
    return QObject::qt_metacast(_clname);
}

int NoteNagaMixer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void NoteNagaMixer::routingEntryStackChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void NoteNagaMixer::noteInSignal(const NN_Note_t & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void NoteNagaMixer::noteOutSignal(const NN_Note_t & _t1, const std::string & _t2, int _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2, _t3);
}
QT_WARNING_POP
