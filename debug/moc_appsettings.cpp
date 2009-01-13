/****************************************************************************
** Meta object code from reading C++ file 'appsettings.h'
**
** Created: Tue Jan 13 11:38:54 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/gui/appsettings.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'appsettings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppSettings[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,
      31,   12,   12,   12, 0x08,
      50,   45,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_AppSettings[] = {
    "AppSettings\0\0cmdCancel_Click()\0"
    "cmdOk_Click()\0text\0"
    "comboProxyType_indexChanged(QString)\0"
};

const QMetaObject AppSettings::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_AppSettings,
      qt_meta_data_AppSettings, 0 }
};

const QMetaObject *AppSettings::metaObject() const
{
    return &staticMetaObject;
}

void *AppSettings::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppSettings))
        return static_cast<void*>(const_cast< AppSettings*>(this));
    if (!strcmp(_clname, "Ui::AppSettings"))
        return static_cast< Ui::AppSettings*>(const_cast< AppSettings*>(this));
    return QDialog::qt_metacast(_clname);
}

int AppSettings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: cmdCancel_Click(); break;
        case 1: cmdOk_Click(); break;
        case 2: comboProxyType_indexChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
