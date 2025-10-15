/****************************************************************************
** Meta object code from reading C++ file 'note_naga_engine.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/note_naga_engine/note_naga_engine.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'note_naga_engine.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN14NoteNagaEngineE_t {};
} // unnamed namespace

template <> constexpr inline auto NoteNagaEngine::qt_create_metaobjectdata<qt_meta_tag_ZN14NoteNagaEngineE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "NoteNagaEngine",
        "playbackStarted",
        "",
        "playbackStopped",
        "synthAdded",
        "NoteNagaSynthesizer*",
        "synth",
        "synthRemoved",
        "synthUpdated"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'playbackStarted'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'playbackStopped'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'synthAdded'
        QtMocHelpers::SignalData<void(NoteNagaSynthesizer *)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 5, 6 },
        }}),
        // Signal 'synthRemoved'
        QtMocHelpers::SignalData<void(NoteNagaSynthesizer *)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 5, 6 },
        }}),
        // Signal 'synthUpdated'
        QtMocHelpers::SignalData<void(NoteNagaSynthesizer *)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 5, 6 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<NoteNagaEngine, qt_meta_tag_ZN14NoteNagaEngineE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject NoteNagaEngine::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14NoteNagaEngineE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14NoteNagaEngineE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN14NoteNagaEngineE_t>.metaTypes,
    nullptr
} };

void NoteNagaEngine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<NoteNagaEngine *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->playbackStarted(); break;
        case 1: _t->playbackStopped(); break;
        case 2: _t->synthAdded((*reinterpret_cast< std::add_pointer_t<NoteNagaSynthesizer*>>(_a[1]))); break;
        case 3: _t->synthRemoved((*reinterpret_cast< std::add_pointer_t<NoteNagaSynthesizer*>>(_a[1]))); break;
        case 4: _t->synthUpdated((*reinterpret_cast< std::add_pointer_t<NoteNagaSynthesizer*>>(_a[1]))); break;
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
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< NoteNagaSynthesizer* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< NoteNagaSynthesizer* >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< NoteNagaSynthesizer* >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (NoteNagaEngine::*)()>(_a, &NoteNagaEngine::playbackStarted, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (NoteNagaEngine::*)()>(_a, &NoteNagaEngine::playbackStopped, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (NoteNagaEngine::*)(NoteNagaSynthesizer * )>(_a, &NoteNagaEngine::synthAdded, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (NoteNagaEngine::*)(NoteNagaSynthesizer * )>(_a, &NoteNagaEngine::synthRemoved, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (NoteNagaEngine::*)(NoteNagaSynthesizer * )>(_a, &NoteNagaEngine::synthUpdated, 4))
            return;
    }
}

const QMetaObject *NoteNagaEngine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NoteNagaEngine::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14NoteNagaEngineE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NoteNagaEngine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void NoteNagaEngine::playbackStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void NoteNagaEngine::playbackStopped()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void NoteNagaEngine::synthAdded(NoteNagaSynthesizer * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void NoteNagaEngine::synthRemoved(NoteNagaSynthesizer * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void NoteNagaEngine::synthUpdated(NoteNagaSynthesizer * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}
QT_WARNING_POP
