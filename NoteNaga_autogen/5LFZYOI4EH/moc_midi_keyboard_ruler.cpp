/****************************************************************************
** Meta object code from reading C++ file 'midi_keyboard_ruler.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/gui/widgets/midi_keyboard_ruler.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'midi_keyboard_ruler.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN17MidiKeyboardRulerE_t {};
} // unnamed namespace

template <> constexpr inline auto MidiKeyboardRuler::qt_create_metaobjectdata<qt_meta_tag_ZN17MidiKeyboardRulerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MidiKeyboardRuler",
        "notePressed",
        "",
        "NN_Note_t",
        "note",
        "noteReleased",
        "handleNotePlay",
        "clearHighlights",
        "setRowHeight",
        "height",
        "setVerticalScroll",
        "value"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'notePressed'
        QtMocHelpers::SignalData<void(const NN_Note_t &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'noteReleased'
        QtMocHelpers::SignalData<void(const NN_Note_t &)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Slot 'handleNotePlay'
        QtMocHelpers::SlotData<void(const NN_Note_t &)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Slot 'clearHighlights'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setRowHeight'
        QtMocHelpers::SlotData<void(int)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 9 },
        }}),
        // Slot 'setVerticalScroll'
        QtMocHelpers::SlotData<void(float)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Float, 11 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MidiKeyboardRuler, qt_meta_tag_ZN17MidiKeyboardRulerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MidiKeyboardRuler::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17MidiKeyboardRulerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17MidiKeyboardRulerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN17MidiKeyboardRulerE_t>.metaTypes,
    nullptr
} };

void MidiKeyboardRuler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MidiKeyboardRuler *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->notePressed((*reinterpret_cast< std::add_pointer_t<NN_Note_t>>(_a[1]))); break;
        case 1: _t->noteReleased((*reinterpret_cast< std::add_pointer_t<NN_Note_t>>(_a[1]))); break;
        case 2: _t->handleNotePlay((*reinterpret_cast< std::add_pointer_t<NN_Note_t>>(_a[1]))); break;
        case 3: _t->clearHighlights(); break;
        case 4: _t->setRowHeight((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->setVerticalScroll((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (MidiKeyboardRuler::*)(const NN_Note_t & )>(_a, &MidiKeyboardRuler::notePressed, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (MidiKeyboardRuler::*)(const NN_Note_t & )>(_a, &MidiKeyboardRuler::noteReleased, 1))
            return;
    }
}

const QMetaObject *MidiKeyboardRuler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MidiKeyboardRuler::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17MidiKeyboardRulerE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int MidiKeyboardRuler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void MidiKeyboardRuler::notePressed(const NN_Note_t & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void MidiKeyboardRuler::noteReleased(const NN_Note_t & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}
QT_WARNING_POP
