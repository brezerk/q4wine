/****************************************************************************
** Meta object code from reading C++ file 'imagemanager.h'
**
** Created: Tue Jan 13 11:39:03 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/gui/imagemanager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'imagemanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ImageManager[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x08,
      28,   13,   13,   13, 0x08,
      45,   13,   13,   13, 0x08,
      65,   13,   13,   13, 0x08,
      90,   13,   13,   13, 0x08,
     125,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ImageManager[] = {
    "ImageManager\0\0cmdOk_Click()\0"
    "actionAddImage()\0actionRemoveImage()\0"
    "actionRefreshImageList()\0"
    "tableImage_showContextMenu(QPoint)\0"
    "update_lblPathInfo(QModelIndex)\0"
};

const QMetaObject ImageManager::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ImageManager,
      qt_meta_data_ImageManager, 0 }
};

const QMetaObject *ImageManager::metaObject() const
{
    return &staticMetaObject;
}

void *ImageManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ImageManager))
        return static_cast<void*>(const_cast< ImageManager*>(this));
    if (!strcmp(_clname, "Ui::ImageManager"))
        return static_cast< Ui::ImageManager*>(const_cast< ImageManager*>(this));
    return QDialog::qt_metacast(_clname);
}

int ImageManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: cmdOk_Click(); break;
        case 1: actionAddImage(); break;
        case 2: actionRemoveImage(); break;
        case 3: actionRefreshImageList(); break;
        case 4: tableImage_showContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 5: update_lblPathInfo((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        }
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
