/****************************************************************************
** Meta object code from reading C++ file 'chess.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../chess.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chess.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Chess_t {
    QByteArrayData data[12];
    char stringdata0[160];
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
QT_MOC_LITERAL(3, 29, 20), // "addTextToCoreConsole"
QT_MOC_LITERAL(4, 50, 19), // "checkMsgFromChenard"
QT_MOC_LITERAL(5, 70, 10), // "tcpRespond"
QT_MOC_LITERAL(6, 81, 22), // "checkMsgFromWebsockets"
QT_MOC_LITERAL(7, 104, 9), // "msgFromWs"
QT_MOC_LITERAL(8, 114, 12), // "AIEnemyStart"
QT_MOC_LITERAL(9, 127, 11), // "AIEnemySend"
QT_MOC_LITERAL(10, 139, 4), // "QsFT"
QT_MOC_LITERAL(11, 144, 15) // "AIFirstIgorMove"

    },
    "Chess\0addTextToDobotConsole\0\0"
    "addTextToCoreConsole\0checkMsgFromChenard\0"
    "tcpRespond\0checkMsgFromWebsockets\0"
    "msgFromWs\0AIEnemyStart\0AIEnemySend\0"
    "QsFT\0AIFirstIgorMove"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Chess[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   49,    2, 0x06 /* Public */,
       3,    2,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   59,    2, 0x0a /* Public */,
       6,    1,   62,    2, 0x0a /* Public */,
       8,    0,   65,    2, 0x0a /* Public */,
       9,    1,   66,    2, 0x0a /* Public */,
      11,    0,   69,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Char,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::Char,    2,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::QString, QMetaType::QString,   10,
    QMetaType::Void,

       0        // eod
};

void Chess::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Chess *_t = static_cast<Chess *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->addTextToDobotConsole((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< char(*)>(_a[2]))); break;
        case 1: _t->addTextToCoreConsole((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< char(*)>(_a[2]))); break;
        case 2: _t->checkMsgFromChenard((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->checkMsgFromWebsockets((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->AIEnemyStart(); break;
        case 5: { QString _r = _t->AIEnemySend((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 6: _t->AIFirstIgorMove(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Chess::*_t)(QString , char );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Chess::addTextToDobotConsole)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Chess::*_t)(QString , char );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Chess::addTextToCoreConsole)) {
                *result = 1;
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
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void Chess::addTextToDobotConsole(QString _t1, char _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Chess::addTextToCoreConsole(QString _t1, char _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
