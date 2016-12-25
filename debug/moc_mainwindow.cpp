/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[22];
    char stringdata0[323];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 19), // "onPTPsendBtnClicked"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 19), // "writeInDobotConsole"
QT_MOC_LITERAL(4, 52, 7), // "QStrMsg"
QT_MOC_LITERAL(5, 60, 18), // "setDobotValidators"
QT_MOC_LITERAL(6, 79, 17), // "setJointLabelText"
QT_MOC_LITERAL(7, 97, 16), // "QSJointLabelText"
QT_MOC_LITERAL(8, 114, 6), // "sJoint"
QT_MOC_LITERAL(9, 121, 16), // "setAxisLabelText"
QT_MOC_LITERAL(10, 138, 15), // "QSAxisLabelText"
QT_MOC_LITERAL(11, 154, 6), // "chAxis"
QT_MOC_LITERAL(12, 161, 20), // "setConnectButtonText"
QT_MOC_LITERAL(13, 182, 14), // "bConnectButton"
QT_MOC_LITERAL(14, 197, 15), // "setDeviceLabels"
QT_MOC_LITERAL(15, 213, 10), // "QSdeviceSN"
QT_MOC_LITERAL(16, 224, 12), // "QSdeviceName"
QT_MOC_LITERAL(17, 237, 15), // "QSdeviceVersion"
QT_MOC_LITERAL(18, 253, 20), // "showDobotErrorMsgBox"
QT_MOC_LITERAL(19, 274, 17), // "writeInTcpConsole"
QT_MOC_LITERAL(20, 292, 6), // "QS_msg"
QT_MOC_LITERAL(21, 299, 23) // "writeInWebsocketConsole"

    },
    "MainWindow\0onPTPsendBtnClicked\0\0"
    "writeInDobotConsole\0QStrMsg\0"
    "setDobotValidators\0setJointLabelText\0"
    "QSJointLabelText\0sJoint\0setAxisLabelText\0"
    "QSAxisLabelText\0chAxis\0setConnectButtonText\0"
    "bConnectButton\0setDeviceLabels\0"
    "QSdeviceSN\0QSdeviceName\0QSdeviceVersion\0"
    "showDobotErrorMsgBox\0writeInTcpConsole\0"
    "QS_msg\0writeInWebsocketConsole"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x08 /* Private */,
       3,    1,   65,    2, 0x08 /* Private */,
       5,    0,   68,    2, 0x08 /* Private */,
       6,    2,   69,    2, 0x08 /* Private */,
       9,    2,   74,    2, 0x08 /* Private */,
      12,    1,   79,    2, 0x08 /* Private */,
      14,    3,   82,    2, 0x08 /* Private */,
      18,    0,   89,    2, 0x08 /* Private */,
      19,    1,   90,    2, 0x08 /* Private */,
      21,    1,   93,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Short,    7,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::Char,   10,   11,
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   15,   16,   17,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void, QMetaType::QString,    4,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onPTPsendBtnClicked(); break;
        case 1: _t->writeInDobotConsole((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->setDobotValidators(); break;
        case 3: _t->setJointLabelText((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< short(*)>(_a[2]))); break;
        case 4: _t->setAxisLabelText((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< char(*)>(_a[2]))); break;
        case 5: _t->setConnectButtonText((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->setDeviceLabels((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 7: _t->showDobotErrorMsgBox(); break;
        case 8: _t->writeInTcpConsole((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->writeInWebsocketConsole((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
