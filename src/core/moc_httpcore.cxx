/****************************************************************************
** Meta object code from reading C++ file 'httpcore.h'
**
** Created: Mon Feb 15 09:49:51 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "httpcore.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'httpcore.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_HttpCore[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x05,
      23,    9,    9,    9, 0x05,
      47,   45,    9,    9, 0x05,
      79,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
     113,   97,    9,    9, 0x08,
     158,  143,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_HttpCore[] = {
    "HttpCore\0\0pageReaded()\0requestError(QString)\0"
    ",\0updateDataReadProgress(int,int)\0"
    "stateChanged(int)\0requestId,error\0"
    "httpRequestFinished(int,bool)\0"
    "responseHeader\0readResponseHeader(QHttpResponseHeader)\0"
};

const QMetaObject HttpCore::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_HttpCore,
      qt_meta_data_HttpCore, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &HttpCore::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *HttpCore::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *HttpCore::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_HttpCore))
        return static_cast<void*>(const_cast< HttpCore*>(this));
    return QObject::qt_metacast(_clname);
}

int HttpCore::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: pageReaded(); break;
        case 1: requestError((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: updateDataReadProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: httpRequestFinished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 5: readResponseHeader((*reinterpret_cast< const QHttpResponseHeader(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void HttpCore::pageReaded()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void HttpCore::requestError(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void HttpCore::updateDataReadProgress(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void HttpCore::stateChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
