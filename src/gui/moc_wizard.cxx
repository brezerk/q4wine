/****************************************************************************
** Meta object code from reading C++ file 'wizard.h'
**
** Created: Tue Jan 13 11:41:18 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "wizard.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wizard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Wizard[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x08,
      25,    7,    7,    7, 0x08,
      51,   45,    7,    7, 0x08,
      76,   71,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Wizard[] = {
    "Wizard\0\0nextWizardPage()\0previosWizardPage()\0"
    "state\0changeBoxState(int)\0text\0"
    "comboProxyType_indexChanged(QString)\0"
};

const QMetaObject Wizard::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Wizard,
      qt_meta_data_Wizard, 0 }
};

const QMetaObject *Wizard::metaObject() const
{
    return &staticMetaObject;
}

void *Wizard::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Wizard))
        return static_cast<void*>(const_cast< Wizard*>(this));
    if (!strcmp(_clname, "Ui::Wizard"))
        return static_cast< Ui::Wizard*>(const_cast< Wizard*>(this));
    return QDialog::qt_metacast(_clname);
}

int Wizard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: nextWizardPage(); break;
        case 1: previosWizardPage(); break;
        case 2: changeBoxState((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: comboProxyType_indexChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
