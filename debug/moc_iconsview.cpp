/****************************************************************************
** Meta object code from reading C++ file 'iconsview.h'
**
** Created: Tue Jan 13 11:38:56 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/gui/iconsview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'iconsview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_IconsView[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,
      29,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_IconsView[] = {
    "IconsView\0\0cmdCancel_Click()\0cmdOk_Click()\0"
};

const QMetaObject IconsView::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_IconsView,
      qt_meta_data_IconsView, 0 }
};

const QMetaObject *IconsView::metaObject() const
{
    return &staticMetaObject;
}

void *IconsView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_IconsView))
        return static_cast<void*>(const_cast< IconsView*>(this));
    if (!strcmp(_clname, "Ui::IconsView"))
        return static_cast< Ui::IconsView*>(const_cast< IconsView*>(this));
    return QDialog::qt_metacast(_clname);
}

int IconsView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: cmdCancel_Click(); break;
        case 1: cmdOk_Click(); break;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
