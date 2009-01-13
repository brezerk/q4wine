/****************************************************************************
** Meta object code from reading C++ file 'run.h'
**
** Created: Tue Jan 13 11:41:18 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "run.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'run.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Run[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
       5,    4,    4,    4, 0x08,
      23,    4,    4,    4, 0x08,
      37,    4,    4,    4, 0x08,
      52,    4,    4,    4, 0x08,
      84,    4,    4,    4, 0x08,
     124,  115,    4,    4, 0x08,
     143,    4,    4,    4, 0x08,
     165,    4,    4,    4, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Run[] = {
    "Run\0\0cmdCancel_Click()\0cmdOk_Click()\0"
    "cmdAdd_Click()\0comboPrefixes_indexChanged(int)\0"
    "cbUseConsole_stateChanged(int)\0TabIndex\0"
    "ResizeContent(int)\0cmdGetProgram_Click()\0"
    "cmdGetWorkDir_Click()\0"
};

const QMetaObject Run::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Run,
      qt_meta_data_Run, 0 }
};

const QMetaObject *Run::metaObject() const
{
    return &staticMetaObject;
}

void *Run::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Run))
        return static_cast<void*>(const_cast< Run*>(this));
    if (!strcmp(_clname, "Ui::Run"))
        return static_cast< Ui::Run*>(const_cast< Run*>(this));
    return QDialog::qt_metacast(_clname);
}

int Run::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: cmdCancel_Click(); break;
        case 1: cmdOk_Click(); break;
        case 2: cmdAdd_Click(); break;
        case 3: comboPrefixes_indexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: cbUseConsole_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: ResizeContent((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: cmdGetProgram_Click(); break;
        case 7: cmdGetWorkDir_Click(); break;
        }
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
