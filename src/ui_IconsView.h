/********************************************************************************
** Form generated from reading UI file 'IconsView.ui'
**
** Created: Mon Feb 15 09:49:50 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ICONSVIEW_H
#define UI_ICONSVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_IconsView
{
public:
    QVBoxLayout *vboxLayout;
    QGroupBox *groupBox;
    QVBoxLayout *vboxLayout1;
    QListWidget *lstIcons;
    QCheckBox *cbDefaultExport;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *cmdCancel;
    QPushButton *cmdOk;

    void setupUi(QDialog *IconsView)
    {
        if (IconsView->objectName().isEmpty())
            IconsView->setObjectName(QString::fromUtf8("IconsView"));
        IconsView->resize(419, 370);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/data/q4wine.png"), QSize(), QIcon::Normal, QIcon::Off);
        IconsView->setWindowIcon(icon);
        vboxLayout = new QVBoxLayout(IconsView);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        groupBox = new QGroupBox(IconsView);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        vboxLayout1 = new QVBoxLayout(groupBox);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        lstIcons = new QListWidget(groupBox);
        lstIcons->setObjectName(QString::fromUtf8("lstIcons"));
        lstIcons->setContextMenuPolicy(Qt::CustomContextMenu);
        lstIcons->setSelectionMode(QAbstractItemView::ExtendedSelection);
        lstIcons->setIconSize(QSize(64, 64));
        lstIcons->setTextElideMode(Qt::ElideNone);
        lstIcons->setMovement(QListView::Snap);
        lstIcons->setResizeMode(QListView::Adjust);
        lstIcons->setGridSize(QSize(80, 80));
        lstIcons->setViewMode(QListView::IconMode);
        lstIcons->setUniformItemSizes(false);
        lstIcons->setWordWrap(false);
        lstIcons->setSelectionRectVisible(true);

        vboxLayout1->addWidget(lstIcons);

        cbDefaultExport = new QCheckBox(groupBox);
        cbDefaultExport->setObjectName(QString::fromUtf8("cbDefaultExport"));
        cbDefaultExport->setChecked(true);

        vboxLayout1->addWidget(cbDefaultExport);


        vboxLayout->addWidget(groupBox);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        cmdCancel = new QPushButton(IconsView);
        cmdCancel->setObjectName(QString::fromUtf8("cmdCancel"));

        hboxLayout->addWidget(cmdCancel);

        cmdOk = new QPushButton(IconsView);
        cmdOk->setObjectName(QString::fromUtf8("cmdOk"));

        hboxLayout->addWidget(cmdOk);


        vboxLayout->addLayout(hboxLayout);


        retranslateUi(IconsView);

        QMetaObject::connectSlotsByName(IconsView);
    } // setupUi

    void retranslateUi(QDialog *IconsView)
    {
        IconsView->setWindowTitle(QApplication::translate("IconsView", "Icons Export", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("IconsView", "Avalible icons:", 0, QApplication::UnicodeUTF8));
        cbDefaultExport->setText(QApplication::translate("IconsView", "Export icons to default place", 0, QApplication::UnicodeUTF8));
        cmdCancel->setText(QApplication::translate("IconsView", "Cancel", 0, QApplication::UnicodeUTF8));
        cmdOk->setText(QApplication::translate("IconsView", "Ok", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class IconsView: public Ui_IconsView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ICONSVIEW_H
