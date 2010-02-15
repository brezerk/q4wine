/********************************************************************************
** Form generated from reading UI file 'WineDriveDialog.ui'
**
** Created: Mon Feb 15 09:49:50 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WINEDRIVEDIALOG_H
#define UI_WINEDRIVEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WineDriveDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QWidget *widget_14;
    QHBoxLayout *_5;
    QLabel *label_8;
    QComboBox *cbDriveLetter;
    QWidget *widget_2;
    QHBoxLayout *_3;
    QLabel *label;
    QLineEdit *txtDrivePath;
    QPushButton *cmdGetDrivePath;
    QWidget *widget_13;
    QHBoxLayout *_2;
    QLabel *label_7;
    QComboBox *cbDriveType;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *cmdCancel;
    QPushButton *cmdOk;

    void setupUi(QDialog *WineDriveDialog)
    {
        if (WineDriveDialog->objectName().isEmpty())
            WineDriveDialog->setObjectName(QString::fromUtf8("WineDriveDialog"));
        WineDriveDialog->resize(599, 169);
        verticalLayout_2 = new QVBoxLayout(WineDriveDialog);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox = new QGroupBox(WineDriveDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
#ifndef Q_OS_MAC
        verticalLayout->setSpacing(-1);
#endif
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(6, 3, 6, 3);
        widget_14 = new QWidget(groupBox);
        widget_14->setObjectName(QString::fromUtf8("widget_14"));
        _5 = new QHBoxLayout(widget_14);
        _5->setContentsMargins(0, 0, 0, 0);
        _5->setObjectName(QString::fromUtf8("_5"));
        label_8 = new QLabel(widget_14);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMinimumSize(QSize(140, 0));
        label_8->setMaximumSize(QSize(140, 16777215));

        _5->addWidget(label_8);

        cbDriveLetter = new QComboBox(widget_14);
        cbDriveLetter->insertItems(0, QStringList()
         << QString::fromUtf8("A:")
         << QString::fromUtf8("B:")
         << QString::fromUtf8("C:")
         << QString::fromUtf8("D:")
         << QString::fromUtf8("E:")
         << QString::fromUtf8("F:")
         << QString::fromUtf8("G:")
         << QString::fromUtf8("H:")
         << QString::fromUtf8("I:")
         << QString::fromUtf8("J:")
         << QString::fromUtf8("K:")
         << QString::fromUtf8("L:")
         << QString::fromUtf8("M:")
         << QString::fromUtf8("N:")
         << QString::fromUtf8("O:")
         << QString::fromUtf8("P:")
         << QString::fromUtf8("Q:")
         << QString::fromUtf8("R:")
         << QString::fromUtf8("S:")
         << QString::fromUtf8("T:")
         << QString::fromUtf8("U:")
         << QString::fromUtf8("V:")
         << QString::fromUtf8("W:")
         << QString::fromUtf8("X:")
         << QString::fromUtf8("Y:")
         << QString::fromUtf8("Z:")
        );
        cbDriveLetter->setObjectName(QString::fromUtf8("cbDriveLetter"));

        _5->addWidget(cbDriveLetter);


        verticalLayout->addWidget(widget_14);

        widget_2 = new QWidget(groupBox);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        _3 = new QHBoxLayout(widget_2);
        _3->setContentsMargins(0, 0, 0, 0);
        _3->setObjectName(QString::fromUtf8("_3"));
        label = new QLabel(widget_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(140, 0));
        label->setMaximumSize(QSize(140, 16777215));

        _3->addWidget(label);

        txtDrivePath = new QLineEdit(widget_2);
        txtDrivePath->setObjectName(QString::fromUtf8("txtDrivePath"));

        _3->addWidget(txtDrivePath);

        cmdGetDrivePath = new QPushButton(widget_2);
        cmdGetDrivePath->setObjectName(QString::fromUtf8("cmdGetDrivePath"));
        cmdGetDrivePath->setIconSize(QSize(16, 16));

        _3->addWidget(cmdGetDrivePath);


        verticalLayout->addWidget(widget_2);

        widget_13 = new QWidget(groupBox);
        widget_13->setObjectName(QString::fromUtf8("widget_13"));
        _2 = new QHBoxLayout(widget_13);
        _2->setContentsMargins(0, 0, 0, 0);
        _2->setObjectName(QString::fromUtf8("_2"));
        label_7 = new QLabel(widget_13);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setMinimumSize(QSize(140, 0));
        label_7->setMaximumSize(QSize(140, 16777215));

        _2->addWidget(label_7);

        cbDriveType = new QComboBox(widget_13);
        cbDriveType->insertItems(0, QStringList()
         << QString::fromUtf8("auto")
         << QString::fromUtf8("hd")
         << QString::fromUtf8("floppy")
         << QString::fromUtf8("network")
         << QString::fromUtf8("cdrom")
        );
        cbDriveType->setObjectName(QString::fromUtf8("cbDriveType"));

        _2->addWidget(cbDriveType);


        verticalLayout->addWidget(widget_13);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        verticalLayout_2->addWidget(groupBox);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        cmdCancel = new QPushButton(WineDriveDialog);
        cmdCancel->setObjectName(QString::fromUtf8("cmdCancel"));

        hboxLayout->addWidget(cmdCancel);

        cmdOk = new QPushButton(WineDriveDialog);
        cmdOk->setObjectName(QString::fromUtf8("cmdOk"));

        hboxLayout->addWidget(cmdOk);


        verticalLayout_2->addLayout(hboxLayout);


        retranslateUi(WineDriveDialog);

        QMetaObject::connectSlotsByName(WineDriveDialog);
    } // setupUi

    void retranslateUi(QDialog *WineDriveDialog)
    {
        WineDriveDialog->setWindowTitle(QApplication::translate("WineDriveDialog", "Wine drive settings", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("WineDriveDialog", "Wine drive", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("WineDriveDialog", "Drive letter:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("WineDriveDialog", "Path:", 0, QApplication::UnicodeUTF8));
        cmdGetDrivePath->setText(QString());
        label_7->setText(QApplication::translate("WineDriveDialog", "Type:", 0, QApplication::UnicodeUTF8));
        cmdCancel->setText(QApplication::translate("WineDriveDialog", "Cancel", 0, QApplication::UnicodeUTF8));
        cmdOk->setText(QApplication::translate("WineDriveDialog", "Ok", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class WineDriveDialog: public Ui_WineDriveDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINEDRIVEDIALOG_H
