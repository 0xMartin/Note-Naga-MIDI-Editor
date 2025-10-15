/****************************************************************************
** Meta object code from reading C++ file 'midi_control_bar_widget.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/gui/widgets/midi_control_bar_widget.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'midi_control_bar_widget.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN20MidiControlBarWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto MidiControlBarWidget::qt_create_metaobjectdata<qt_meta_tag_ZN20MidiControlBarWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MidiControlBarWidget",
        "playToggled",
        "",
        "goToStart",
        "goToEnd",
        "tempoChanged",
        "tempo",
        "metronomeToggled",
        "state",
        "playPositionChanged",
        "seconds",
        "tick_position",
        "setPlaying",
        "is_playing",
        "updateBPM",
        "updateProgressBar",
        "metronomeBtnClicked",
        "onProgressBarPositionPressed",
        "onProgressBarPositionDragged",
        "onProgressBarPositionReleased"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'playToggled'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'goToStart'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'goToEnd'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'tempoChanged'
        QtMocHelpers::SignalData<void(int)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 6 },
        }}),
        // Signal 'metronomeToggled'
        QtMocHelpers::SignalData<void(bool)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 8 },
        }}),
        // Signal 'playPositionChanged'
        QtMocHelpers::SignalData<void(float, int)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Float, 10 }, { QMetaType::Int, 11 },
        }}),
        // Slot 'setPlaying'
        QtMocHelpers::SlotData<void(bool)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 13 },
        }}),
        // Slot 'updateBPM'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'updateProgressBar'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'metronomeBtnClicked'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onProgressBarPositionPressed'
        QtMocHelpers::SlotData<void(float)>(17, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Float, 10 },
        }}),
        // Slot 'onProgressBarPositionDragged'
        QtMocHelpers::SlotData<void(float)>(18, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Float, 10 },
        }}),
        // Slot 'onProgressBarPositionReleased'
        QtMocHelpers::SlotData<void(float)>(19, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Float, 10 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MidiControlBarWidget, qt_meta_tag_ZN20MidiControlBarWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MidiControlBarWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QFrame::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20MidiControlBarWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20MidiControlBarWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN20MidiControlBarWidgetE_t>.metaTypes,
    nullptr
} };

void MidiControlBarWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MidiControlBarWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->playToggled(); break;
        case 1: _t->goToStart(); break;
        case 2: _t->goToEnd(); break;
        case 3: _t->tempoChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->metronomeToggled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 5: _t->playPositionChanged((*reinterpret_cast< std::add_pointer_t<float>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 6: _t->setPlaying((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 7: _t->updateBPM(); break;
        case 8: _t->updateProgressBar(); break;
        case 9: _t->metronomeBtnClicked(); break;
        case 10: _t->onProgressBarPositionPressed((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 11: _t->onProgressBarPositionDragged((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 12: _t->onProgressBarPositionReleased((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (MidiControlBarWidget::*)()>(_a, &MidiControlBarWidget::playToggled, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (MidiControlBarWidget::*)()>(_a, &MidiControlBarWidget::goToStart, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (MidiControlBarWidget::*)()>(_a, &MidiControlBarWidget::goToEnd, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (MidiControlBarWidget::*)(int )>(_a, &MidiControlBarWidget::tempoChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (MidiControlBarWidget::*)(bool )>(_a, &MidiControlBarWidget::metronomeToggled, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (MidiControlBarWidget::*)(float , int )>(_a, &MidiControlBarWidget::playPositionChanged, 5))
            return;
    }
}

const QMetaObject *MidiControlBarWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MidiControlBarWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20MidiControlBarWidgetE_t>.strings))
        return static_cast<void*>(this);
    return QFrame::qt_metacast(_clname);
}

int MidiControlBarWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void MidiControlBarWidget::playToggled()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void MidiControlBarWidget::goToStart()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void MidiControlBarWidget::goToEnd()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void MidiControlBarWidget::tempoChanged(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void MidiControlBarWidget::metronomeToggled(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void MidiControlBarWidget::playPositionChanged(float _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1, _t2);
}
QT_WARNING_POP
