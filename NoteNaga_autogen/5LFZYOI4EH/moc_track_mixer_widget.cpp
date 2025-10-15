/****************************************************************************
** Meta object code from reading C++ file 'track_mixer_widget.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/gui/widgets/track_mixer_widget.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'track_mixer_widget.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN16TrackMixerWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto TrackMixerWidget::qt_create_metaobjectdata<qt_meta_tag_ZN16TrackMixerWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "TrackMixerWidget",
        "refresh_routing_table",
        "",
        "onSynthesizerAdded",
        "NoteNagaSynthesizer*",
        "synth",
        "onSynthesizerRemoved",
        "onSynthesizerUpdated",
        "onMinNoteChanged",
        "value",
        "onMaxNoteChanged",
        "onGlobalOffsetChanged",
        "onGlobalVolumeChanged",
        "onGlobalPanChanged",
        "onAddEntry",
        "onRemoveSelectedEntry",
        "onReassignSynth",
        "onClearRoutingTable",
        "onDefaultEntries",
        "onMaxVolumeAllTracks",
        "onMinVolumeAllTracks",
        "handlePlayingNote",
        "NN_Note_t",
        "note",
        "std::string",
        "device_name",
        "channel",
        "onSynthesizerSelectionChanged",
        "index"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'refresh_routing_table'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onSynthesizerAdded'
        QtMocHelpers::SlotData<void(NoteNagaSynthesizer *)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 4, 5 },
        }}),
        // Slot 'onSynthesizerRemoved'
        QtMocHelpers::SlotData<void(NoteNagaSynthesizer *)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 4, 5 },
        }}),
        // Slot 'onSynthesizerUpdated'
        QtMocHelpers::SlotData<void(NoteNagaSynthesizer *)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 4, 5 },
        }}),
        // Slot 'onMinNoteChanged'
        QtMocHelpers::SlotData<void(float)>(8, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Float, 9 },
        }}),
        // Slot 'onMaxNoteChanged'
        QtMocHelpers::SlotData<void(float)>(10, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Float, 9 },
        }}),
        // Slot 'onGlobalOffsetChanged'
        QtMocHelpers::SlotData<void(float)>(11, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Float, 9 },
        }}),
        // Slot 'onGlobalVolumeChanged'
        QtMocHelpers::SlotData<void(float)>(12, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Float, 9 },
        }}),
        // Slot 'onGlobalPanChanged'
        QtMocHelpers::SlotData<void(float)>(13, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Float, 9 },
        }}),
        // Slot 'onAddEntry'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onRemoveSelectedEntry'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onReassignSynth'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onClearRoutingTable'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDefaultEntries'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onMaxVolumeAllTracks'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onMinVolumeAllTracks'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'handlePlayingNote'
        QtMocHelpers::SlotData<void(const NN_Note_t &, const std::string &, int)>(21, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 22, 23 }, { 0x80000000 | 24, 25 }, { QMetaType::Int, 26 },
        }}),
        // Slot 'onSynthesizerSelectionChanged'
        QtMocHelpers::SlotData<void(int)>(27, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 28 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<TrackMixerWidget, qt_meta_tag_ZN16TrackMixerWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject TrackMixerWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16TrackMixerWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16TrackMixerWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN16TrackMixerWidgetE_t>.metaTypes,
    nullptr
} };

void TrackMixerWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<TrackMixerWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->refresh_routing_table(); break;
        case 1: _t->onSynthesizerAdded((*reinterpret_cast< std::add_pointer_t<NoteNagaSynthesizer*>>(_a[1]))); break;
        case 2: _t->onSynthesizerRemoved((*reinterpret_cast< std::add_pointer_t<NoteNagaSynthesizer*>>(_a[1]))); break;
        case 3: _t->onSynthesizerUpdated((*reinterpret_cast< std::add_pointer_t<NoteNagaSynthesizer*>>(_a[1]))); break;
        case 4: _t->onMinNoteChanged((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 5: _t->onMaxNoteChanged((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 6: _t->onGlobalOffsetChanged((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 7: _t->onGlobalVolumeChanged((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 8: _t->onGlobalPanChanged((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 9: _t->onAddEntry(); break;
        case 10: _t->onRemoveSelectedEntry(); break;
        case 11: _t->onReassignSynth(); break;
        case 12: _t->onClearRoutingTable(); break;
        case 13: _t->onDefaultEntries(); break;
        case 14: _t->onMaxVolumeAllTracks(); break;
        case 15: _t->onMinVolumeAllTracks(); break;
        case 16: _t->handlePlayingNote((*reinterpret_cast< std::add_pointer_t<NN_Note_t>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::string>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3]))); break;
        case 17: _t->onSynthesizerSelectionChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< NoteNagaSynthesizer* >(); break;
            }
            break;
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
        }
    }
}

const QMetaObject *TrackMixerWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TrackMixerWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16TrackMixerWidgetE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int TrackMixerWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}
QT_WARNING_POP
