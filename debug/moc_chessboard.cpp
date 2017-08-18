/****************************************************************************
** Meta object code from reading C++ file 'chessboard.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../chessboard.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chessboard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Chessboard_t {
    QByteArrayData data[13];
    char stringdata0[200];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Chessboard_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Chessboard_t qt_meta_stringdata_Chessboard = {
    {
QT_MOC_LITERAL(0, 0, 10), // "Chessboard"
QT_MOC_LITERAL(1, 11, 16), // "addTextToConsole"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 17), // "changeWindowTitle"
QT_MOC_LITERAL(4, 47, 9), // "showBoard"
QT_MOC_LITERAL(5, 57, 18), // "setBoardDataLabels"
QT_MOC_LITERAL(6, 76, 17), // "BOARD_DATA_LABELS"
QT_MOC_LITERAL(7, 94, 14), // "showLegalMoves"
QT_MOC_LITERAL(8, 109, 29), // "msgFromChessboardToWebsockets"
QT_MOC_LITERAL(9, 139, 12), // "timeOutWhite"
QT_MOC_LITERAL(10, 152, 12), // "timeOutBlack"
QT_MOC_LITERAL(11, 165, 16), // "updateTimeLabels"
QT_MOC_LITERAL(12, 182, 17) // "timeOutStartQueue"

    },
    "Chessboard\0addTextToConsole\0\0"
    "changeWindowTitle\0showBoard\0"
    "setBoardDataLabels\0BOARD_DATA_LABELS\0"
    "showLegalMoves\0msgFromChessboardToWebsockets\0"
    "timeOutWhite\0timeOutBlack\0updateTimeLabels\0"
    "timeOutStartQueue"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Chessboard[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       3,    1,   67,    2, 0x06 /* Public */,
       4,    1,   70,    2, 0x06 /* Public */,
       5,    2,   73,    2, 0x06 /* Public */,
       7,    1,   78,    2, 0x06 /* Public */,
       8,    1,   81,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   84,    2, 0x08 /* Private */,
      10,    0,   85,    2, 0x08 /* Private */,
      11,    0,   86,    2, 0x08 /* Private */,
      12,    0,   87,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 6,    2,    2,
    QMetaType::Void, QMetaType::QStringList,    2,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Chessboard::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Chessboard *_t = static_cast<Chessboard *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->addTextToConsole((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->changeWindowTitle((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->showBoard((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->setBoardDataLabels((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< BOARD_DATA_LABELS(*)>(_a[2]))); break;
        case 4: _t->showLegalMoves((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 5: _t->msgFromChessboardToWebsockets((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->timeOutWhite(); break;
        case 7: _t->timeOutBlack(); break;
        case 8: _t->updateTimeLabels(); break;
        case 9: _t->timeOutStartQueue(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Chessboard::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Chessboard::addTextToConsole)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Chessboard::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Chessboard::changeWindowTitle)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (Chessboard::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Chessboard::showBoard)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (Chessboard::*_t)(QString , BOARD_DATA_LABELS );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Chessboard::setBoardDataLabels)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (Chessboard::*_t)(QStringList );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Chessboard::showLegalMoves)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (Chessboard::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Chessboard::msgFromChessboardToWebsockets)) {
                *result = 5;
                return;
            }
        }
    }
}

const QMetaObject Chessboard::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Chessboard.data,
      qt_meta_data_Chessboard,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Chessboard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Chessboard::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Chessboard.stringdata0))
        return static_cast<void*>(const_cast< Chessboard*>(this));
    return QObject::qt_metacast(_clname);
}

int Chessboard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void Chessboard::addTextToConsole(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Chessboard::changeWindowTitle(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Chessboard::showBoard(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Chessboard::setBoardDataLabels(QString _t1, BOARD_DATA_LABELS _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Chessboard::showLegalMoves(QStringList _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Chessboard::msgFromChessboardToWebsockets(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
