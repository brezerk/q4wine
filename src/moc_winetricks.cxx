/****************************************************************************
** Meta object code from reading C++ file 'winetricks.h'
**
** Created: Mon Jan 5 14:18:22 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "winetricks.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'winetricks.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_winetricks[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      23,   11,   11,   11, 0x08,
      34,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_winetricks[] = {
    "winetricks\0\0onreject()\0onaccept()\0"
    "instwinetricks()\0"
};

const QMetaObject winetricks::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_winetricks,
      qt_meta_data_winetricks, 0 }
};

const QMetaObject *winetricks::metaObject() const
{
    return &staticMetaObject;
}

void *winetricks::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_winetricks))
        return static_cast<void*>(const_cast< winetricks*>(this));
    return QDialog::qt_metacast(_clname);
}

int winetricks::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: onreject(); break;
        case 1: onaccept(); break;
        case 2: instwinetricks(); break;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
