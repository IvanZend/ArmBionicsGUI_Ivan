/****************************************************************************
** Meta object code from reading C++ file 'emgwidget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../emgwidget.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'emgwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.2. It"
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

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSEMGWidgetENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSEMGWidgetENDCLASS = QtMocHelpers::stringData(
    "EMGWidget",
    "read_data",
    "",
    "handleSerialPortError",
    "QSerialPort::SerialPortError",
    "error",
    "on_btn_ConnectDisconnect_clicked",
    "on_actionSave_triggered",
    "on_actionOpen_triggered",
    "on_actionPlot_color_triggered",
    "on_actionDevice_info_triggered",
    "on_sensorNumber_triggered",
    "on_actionClear_plot_triggered",
    "on_actionClear_log_triggered",
    "on_actionClear_all_triggered"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSEMGWidgetENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   80,    2, 0x08,    1 /* Private */,
       3,    1,   81,    2, 0x08,    2 /* Private */,
       6,    0,   84,    2, 0x08,    4 /* Private */,
       7,    0,   85,    2, 0x08,    5 /* Private */,
       8,    0,   86,    2, 0x08,    6 /* Private */,
       9,    0,   87,    2, 0x08,    7 /* Private */,
      10,    0,   88,    2, 0x08,    8 /* Private */,
      11,    0,   89,    2, 0x08,    9 /* Private */,
      12,    0,   90,    2, 0x08,   10 /* Private */,
      13,    0,   91,    2, 0x08,   11 /* Private */,
      14,    0,   92,    2, 0x08,   12 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject EMGWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSEMGWidgetENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSEMGWidgetENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSEMGWidgetENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<EMGWidget, std::true_type>,
        // method 'read_data'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleSerialPortError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QSerialPort::SerialPortError, std::false_type>,
        // method 'on_btn_ConnectDisconnect_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionSave_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionOpen_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionPlot_color_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionDevice_info_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_sensorNumber_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionClear_plot_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionClear_log_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionClear_all_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void EMGWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<EMGWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->read_data(); break;
        case 1: _t->handleSerialPortError((*reinterpret_cast< std::add_pointer_t<QSerialPort::SerialPortError>>(_a[1]))); break;
        case 2: _t->on_btn_ConnectDisconnect_clicked(); break;
        case 3: _t->on_actionSave_triggered(); break;
        case 4: _t->on_actionOpen_triggered(); break;
        case 5: _t->on_actionPlot_color_triggered(); break;
        case 6: _t->on_actionDevice_info_triggered(); break;
        case 7: _t->on_sensorNumber_triggered(); break;
        case 8: _t->on_actionClear_plot_triggered(); break;
        case 9: _t->on_actionClear_log_triggered(); break;
        case 10: _t->on_actionClear_all_triggered(); break;
        default: ;
        }
    }
}

const QMetaObject *EMGWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EMGWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSEMGWidgetENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int EMGWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
