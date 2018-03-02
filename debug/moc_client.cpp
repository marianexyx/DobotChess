/****************************************************************************
** Meta object code from reading C++ file 'client.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../client.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'client.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Clients_t {
    QByteArrayData data[9];
    char stringdata0[110];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Clients_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Clients_t qt_meta_stringdata_Clients = {
    {
QT_MOC_LITERAL(0, 0, 7), // "Clients"
QT_MOC_LITERAL(1, 8, 15), // "addTextToLogPTE"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 3), // "LOG"
QT_MOC_LITERAL(4, 29, 15), // "showClientsList"
QT_MOC_LITERAL(5, 45, 13), // "QList<Client>"
QT_MOC_LITERAL(6, 59, 17), // "setBoardDataLabel"
QT_MOC_LITERAL(7, 77, 16), // "BOARD_DATA_LABEL"
QT_MOC_LITERAL(8, 94, 15) // "showClientsInUI"

    },
    "Clients\0addTextToLogPTE\0\0LOG\0"
    "showClientsList\0QList<Client>\0"
    "setBoardDataLabel\0BOARD_DATA_LABEL\0"
    "showClientsInUI"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Clients[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   34,    2, 0x06 /* Public */,
       4,    1,   39,    2, 0x06 /* Public */,
       6,    2,   42,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   47,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 3,    2,    2,
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 7,    2,    2,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void Clients::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Clients *_t = static_cast<Clients *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->addTextToLogPTE((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< LOG(*)>(_a[2]))); break;
        case 1: _t->showClientsList((*reinterpret_cast< QList<Client>(*)>(_a[1]))); break;
        case 2: _t->setBoardDataLabel((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< BOARD_DATA_LABEL(*)>(_a[2]))); break;
        case 3: _t->showClientsInUI(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Clients::*_t)(QString , LOG );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Clients::addTextToLogPTE)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Clients::*_t)(QList<Client> );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Clients::showClientsList)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (Clients::*_t)(QString , BOARD_DATA_LABEL );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Clients::setBoardDataLabel)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject Clients::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Clients.data,
      qt_meta_data_Clients,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Clients::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Clients::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Clients.stringdata0))
        return static_cast<void*>(const_cast< Clients*>(this));
    return QObject::qt_metacast(_clname);
}

int Clients::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Clients::addTextToLogPTE(QString _t1, LOG _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Clients::showClientsList(QList<Client> _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Clients::setBoardDataLabel(QString _t1, BOARD_DATA_LABEL _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
