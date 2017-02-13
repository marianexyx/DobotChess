/****************************************************************************
** Meta object code from reading C++ file 'chess.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../chess.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chess.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Chess_t {
    QByteArrayData data[8];
    char stringdata0[100];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Chess_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Chess_t qt_meta_stringdata_Chess = {
    {
QT_MOC_LITERAL(0, 0, 5), // "Chess"
QT_MOC_LITERAL(1, 6, 21), // "addTextToDobotConsole"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 6), // "QS_msg"
QT_MOC_LITERAL(4, 36, 19), // "checkMsgFromChenard"
QT_MOC_LITERAL(5, 56, 10), // "tcpRespond"
QT_MOC_LITERAL(6, 67, 22), // "checkMsgFromWebsockets"
QT_MOC_LITERAL(7, 90, 9) // "msgFromWs"

    },
    "Chess\0addTextToDobotConsole\0\0QS_msg\0"
    "checkMsgFromChenard\0tcpRespond\0"
    "checkMsgFromWebsockets\0msgFromWs"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Chess[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   32,    2, 0x0a /* Public */,
       6,    1,   35,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    7,

       0        // eod
};

void Chess::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Chess *_t = static_cast<Chess *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->addTextToDobotConsole((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->checkMsgFromChenard((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->checkMsgFromWebsockets((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Chess::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Chess::addTextToDobotConsole)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject Chess::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Chess.data,
      qt_meta_data_Chess,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Chess::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Chess::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Chess.stringdata0))
        return static_cast<void*>(const_cast< Chess*>(this));
    return QObject::qt_metacast(_clname);
}

int Chess::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void Chess::addTextToDobotConsole(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
