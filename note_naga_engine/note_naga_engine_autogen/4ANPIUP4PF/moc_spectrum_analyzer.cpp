/****************************************************************************
** Meta object code from reading C++ file 'spectrum_analyzer.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/note_naga_engine/module/spectrum_analyzer.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'spectrum_analyzer.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN24NoteNagaSpectrumAnalyzerE_t {};
} // unnamed namespace

template <> constexpr inline auto NoteNagaSpectrumAnalyzer::qt_create_metaobjectdata<qt_meta_tag_ZN24NoteNagaSpectrumAnalyzerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "NoteNagaSpectrumAnalyzer",
        "spectrumUpdated",
        "",
        "std::vector<float>",
        "spectrum"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'spectrumUpdated'
        QtMocHelpers::SignalData<void(const std::vector<float> &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<NoteNagaSpectrumAnalyzer, qt_meta_tag_ZN24NoteNagaSpectrumAnalyzerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject NoteNagaSpectrumAnalyzer::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN24NoteNagaSpectrumAnalyzerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN24NoteNagaSpectrumAnalyzerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN24NoteNagaSpectrumAnalyzerE_t>.metaTypes,
    nullptr
} };

void NoteNagaSpectrumAnalyzer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<NoteNagaSpectrumAnalyzer *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->spectrumUpdated((*reinterpret_cast< std::add_pointer_t<std::vector<float>>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (NoteNagaSpectrumAnalyzer::*)(const std::vector<float> & )>(_a, &NoteNagaSpectrumAnalyzer::spectrumUpdated, 0))
            return;
    }
}

const QMetaObject *NoteNagaSpectrumAnalyzer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NoteNagaSpectrumAnalyzer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN24NoteNagaSpectrumAnalyzerE_t>.strings))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "AsyncQueueComponent<NN_AsyncTriggerMessage_t,16>"))
        return static_cast< AsyncQueueComponent<NN_AsyncTriggerMessage_t,16>*>(this);
    return QObject::qt_metacast(_clname);
}

int NoteNagaSpectrumAnalyzer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void NoteNagaSpectrumAnalyzer::spectrumUpdated(const std::vector<float> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}
QT_WARNING_POP
