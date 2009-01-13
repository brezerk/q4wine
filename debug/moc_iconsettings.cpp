/****************************************************************************
** Meta object code from reading C++ file 'iconsettings.h'
**
** Created: Tue Jan 13 11:38:55 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/gui/iconsettings.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'iconsettings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_IconSettings[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x08,
      33,   13,   13,   13, 0x08,
      48,   13,   13,   13, 0x08,
      70,   13,   13,   13, 0x08,
      89,   13,   13,   13, 0x08,
     111,   13,   13,   13, 0x08,
     142,   13,   13,   13, 0x08,
     160,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_IconSettings[] = {
    "IconSettings\0\0ResizeContent(int)\0"
    "cmdAdd_Click()\0cmdGetProgram_Click()\0"
    "cmdGetIcon_Click()\0cmdGetWorkDir_Click()\0"
    "cbUseConsole_stateChanged(int)\0"
    "cmdCancel_Click()\0cmdOk_Click()\0"
};

const QMetaObject IconSettings::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_IconSettings,
      qt_meta_data_IconSettings, 0 }
};

const QMetaObject *IconSettings::metaObject() const
{
    return &staticMetaObject;
}

void *IconSettings::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_IconSettings))
        return static_cast<void*>(const_cast< IconSettings*>(this));
    if (!strcmp(_clname, "Ui::IconSettings"))
        return static_cast< Ui::IconSettings*>(const_cast< IconSettings*>(this));
    return QDialog::qt_metacast(_clname);
}

int IconSettings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: ResizeContent((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: cmdAdd_Click(); break;
        case 2: cmdGetProgram_Click(); break;
        case 3: cmdGetIcon_Click(); break;
        case 4: cmdGetWorkDir_Click(); break;
        case 5: cbUseConsole_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: cmdCancel_Click(); break;
        case 7: cmdOk_Click(); break;
        }
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
