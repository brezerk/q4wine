/********************************************************************************
** Form generated from reading ui file 'Proccess.ui'
**
** Created: Mon May 19 15:56:03 2008
**      by: Qt User Interface Compiler version 4.3.4
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_PROCCESS_H
#define UI_PROCCESS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

class Ui_Proccess
{
public:
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QVBoxLayout *vboxLayout;
    QLabel *lblSrc;
    QLabel *lblDst;
    QFrame *line;
    QSpacerItem *spacerItem;
    QProgressBar *progressBar;
    QSpacerItem *spacerItem1;
    QSpacerItem *spacerItem2;
    QPushButton *cmdAction;

    void setupUi(QDialog *Proccess)
    {
    if (Proccess->objectName().isEmpty())
        Proccess->setObjectName(QString::fromUtf8("Proccess"));
    Proccess->setWindowModality(Qt::NonModal);
    Proccess->resize(292, 167);
    Proccess->setSizeGripEnabled(false);
    Proccess->setModal(false);
    gridLayout = new QGridLayout(Proccess);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    groupBox = new QGroupBox(Proccess);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    vboxLayout = new QVBoxLayout(groupBox);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    lblSrc = new QLabel(groupBox);
    lblSrc->setObjectName(QString::fromUtf8("lblSrc"));

    vboxLayout->addWidget(lblSrc);

    lblDst = new QLabel(groupBox);
    lblDst->setObjectName(QString::fromUtf8("lblDst"));

    vboxLayout->addWidget(lblDst);

    line = new QFrame(groupBox);
    line->setObjectName(QString::fromUtf8("line"));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    vboxLayout->addWidget(line);

    spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout->addItem(spacerItem);

    progressBar = new QProgressBar(groupBox);
    progressBar->setObjectName(QString::fromUtf8("progressBar"));
    progressBar->setMinimum(0);
    progressBar->setMaximum(0);
    progressBar->setValue(-1);

    vboxLayout->addWidget(progressBar);

    spacerItem1 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout->addItem(spacerItem1);


    gridLayout->addWidget(groupBox, 0, 0, 1, 2);

    spacerItem2 = new QSpacerItem(546, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout->addItem(spacerItem2, 1, 0, 1, 1);

    cmdAction = new QPushButton(Proccess);
    cmdAction->setObjectName(QString::fromUtf8("cmdAction"));

    gridLayout->addWidget(cmdAction, 1, 1, 1, 1);


    retranslateUi(Proccess);

    QMetaObject::connectSlotsByName(Proccess);
    } // setupUi

    void retranslateUi(QDialog *Proccess)
    {
    Proccess->setWindowTitle(QApplication::translate("Proccess", "Mounting image...", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QString());
    lblSrc->setText(QApplication::translate("Proccess", "src: /home/brezerk/cdrom.iso", 0, QApplication::UnicodeUTF8));
    lblDst->setText(QApplication::translate("Proccess", "dst: /mnt/cdrom", 0, QApplication::UnicodeUTF8));
    cmdAction->setText(QApplication::translate("Proccess", "Cancel", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Proccess);
    } // retranslateUi

};

namespace Ui {
    class Proccess: public Ui_Proccess {};
} // namespace Ui

#endif // UI_PROCCESS_H
