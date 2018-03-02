/****************************************************************************
** Meta object code from reading C++ file 'chess_timers.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../chess/chess_timers.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chess_timers.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ChessTimers_t {
    QByteArrayData data[7];
    char stringdata0[81];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ChessTimers_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ChessTimers_t qt_meta_stringdata_ChessTimers = {
    {
QT_MOC_LITERAL(0, 0, 11), // "ChessTimers"
QT_MOC_LITERAL(1, 12, 13), // "playerTimeOut"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 11), // "PLAYER_TYPE"
QT_MOC_LITERAL(4, 39, 6), // "Player"
QT_MOC_LITERAL(5, 46, 16), // "updateTimeLabels"
QT_MOC_LITERAL(6, 63, 17) // "timeOutStartQueue"

    },
    "ChessTimers\0playerTimeOut\0\0PLAYER_TYPE\0"
    "Player\0updateTimeLabels\0timeOutStartQueue"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ChessTimers[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x08 /* Private */,
       5,    0,   32,    2, 0x08 /* Private */,
       6,    0,   33,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ChessTimers::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ChessTimers *_t = static_cast<ChessTimers *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->playerTimeOut((*reinterpret_cast< PLAYER_TYPE(*)>(_a[1]))); break;
        case 1: _t->updateTimeLabels(); break;
        case 2: _t->timeOutStartQueue(); break;
        default: ;
        }
    }
}

const QMetaObject ChessTimers::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ChessTimers.data,
      qt_meta_data_ChessTimers,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ChessTimers::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChessTimers::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ChessTimers.stringdata0))
        return static_cast<void*>(const_cast< ChessTimers*>(this));
    return QObject::qt_metacast(_clname);
}

int ChessTimers::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_END_MOC_NAMESPACE
