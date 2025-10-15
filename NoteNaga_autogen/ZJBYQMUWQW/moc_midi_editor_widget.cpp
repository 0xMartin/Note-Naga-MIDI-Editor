/****************************************************************************
** Meta object code from reading C++ file 'midi_editor_widget.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/gui/editor/midi_editor_widget.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'midi_editor_widget.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN16MidiEditorWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto MidiEditorWidget::qt_create_metaobjectdata<qt_meta_tag_ZN16MidiEditorWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MidiEditorWidget",
        "positionSelected",
        "",
        "tick",
        "horizontalScrollChanged",
        "value",
        "verticalScrollChanged",
        "followModeChanged",
        "MidiEditorFollowMode",
        "mode",
        "timeScaleChanged",
        "scale",
        "keyHeightChanged",
        "height",
        "loopingChanged",
        "enabled",
        "notesModified",
        "setTimeScale",
        "setKeyHeight",
        "h",
        "refreshAll",
        "refreshMarker",
        "refreshTrack",
        "NoteNagaTrack*",
        "track",
        "refreshSequence",
        "NoteNagaMidiSeq*",
        "seq",
        "currentTickChanged",
        "selectFollowMode",
        "enableLooping"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'positionSelected'
        QtMocHelpers::SignalData<void(int)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Signal 'horizontalScrollChanged'
        QtMocHelpers::SignalData<void(int)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 5 },
        }}),
        // Signal 'verticalScrollChanged'
        QtMocHelpers::SignalData<void(int)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 5 },
        }}),
        // Signal 'followModeChanged'
        QtMocHelpers::SignalData<void(MidiEditorFollowMode)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 9 },
        }}),
        // Signal 'timeScaleChanged'
        QtMocHelpers::SignalData<void(double)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 11 },
        }}),
        // Signal 'keyHeightChanged'
        QtMocHelpers::SignalData<void(int)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 13 },
        }}),
        // Signal 'loopingChanged'
        QtMocHelpers::SignalData<void(bool)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 15 },
        }}),
        // Signal 'notesModified'
        QtMocHelpers::SignalData<void()>(16, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setTimeScale'
        QtMocHelpers::SlotData<void(double)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 11 },
        }}),
        // Slot 'setKeyHeight'
        QtMocHelpers::SlotData<void(int)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 19 },
        }}),
        // Slot 'refreshAll'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'refreshMarker'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'refreshTrack'
        QtMocHelpers::SlotData<void(NoteNagaTrack *)>(22, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 23, 24 },
        }}),
        // Slot 'refreshSequence'
        QtMocHelpers::SlotData<void(NoteNagaMidiSeq *)>(25, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 26, 27 },
        }}),
        // Slot 'currentTickChanged'
        QtMocHelpers::SlotData<void(int)>(28, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Slot 'selectFollowMode'
        QtMocHelpers::SlotData<void(MidiEditorFollowMode)>(29, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 8, 9 },
        }}),
        // Slot 'enableLooping'
        QtMocHelpers::SlotData<void(bool)>(30, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 15 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MidiEditorWidget, qt_meta_tag_ZN16MidiEditorWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MidiEditorWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QGraphicsView::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16MidiEditorWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16MidiEditorWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN16MidiEditorWidgetE_t>.metaTypes,
    nullptr
} };

void MidiEditorWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MidiEditorWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->positionSelected((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->horizontalScrollChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->verticalScrollChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->followModeChanged((*reinterpret_cast< std::add_pointer_t<MidiEditorFollowMode>>(_a[1]))); break;
        case 4: _t->timeScaleChanged((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 5: _t->keyHeightChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->loopingChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 7: _t->notesModified(); break;
        case 8: _t->setTimeScale((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 9: _t->setKeyHeight((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 10: _t->refreshAll(); break;
        case 11: _t->refreshMarker(); break;
        case 12: _t->refreshTrack((*reinterpret_cast< std::add_pointer_t<NoteNagaTrack*>>(_a[1]))); break;
        case 13: _t->refreshSequence((*reinterpret_cast< std::add_pointer_t<NoteNagaMidiSeq*>>(_a[1]))); break;
        case 14: _t->currentTickChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 15: _t->selectFollowMode((*reinterpret_cast< std::add_pointer_t<MidiEditorFollowMode>>(_a[1]))); break;
        case 16: _t->enableLooping((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 12:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< NoteNagaTrack* >(); break;
            }
            break;
        case 13:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< NoteNagaMidiSeq* >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (MidiEditorWidget::*)(int )>(_a, &MidiEditorWidget::positionSelected, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (MidiEditorWidget::*)(int )>(_a, &MidiEditorWidget::horizontalScrollChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (MidiEditorWidget::*)(int )>(_a, &MidiEditorWidget::verticalScrollChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (MidiEditorWidget::*)(MidiEditorFollowMode )>(_a, &MidiEditorWidget::followModeChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (MidiEditorWidget::*)(double )>(_a, &MidiEditorWidget::timeScaleChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (MidiEditorWidget::*)(int )>(_a, &MidiEditorWidget::keyHeightChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (MidiEditorWidget::*)(bool )>(_a, &MidiEditorWidget::loopingChanged, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (MidiEditorWidget::*)()>(_a, &MidiEditorWidget::notesModified, 7))
            return;
    }
}

const QMetaObject *MidiEditorWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MidiEditorWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16MidiEditorWidgetE_t>.strings))
        return static_cast<void*>(this);
    return QGraphicsView::qt_metacast(_clname);
}

int MidiEditorWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void MidiEditorWidget::positionSelected(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void MidiEditorWidget::horizontalScrollChanged(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void MidiEditorWidget::verticalScrollChanged(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void MidiEditorWidget::followModeChanged(MidiEditorFollowMode _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void MidiEditorWidget::timeScaleChanged(double _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void MidiEditorWidget::keyHeightChanged(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}

// SIGNAL 6
void MidiEditorWidget::loopingChanged(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}

// SIGNAL 7
void MidiEditorWidget::notesModified()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}
QT_WARNING_POP
