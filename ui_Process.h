/********************************************************************************
** Form generated from reading ui file 'Process.ui'
**
** Created: Tue Jan 13 11:38:31 2009
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_PROCESS_H
#define UI_PROCESS_H

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

QT_BEGIN_NAMESPACE

class Ui_Process
{
public:
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QVBoxLayout *vboxLayout;
    QLabel *lblInfo;
    QFrame *line;
    QSpacerItem *spacerItem;
    QProgressBar *progressBar;
    QSpacerItem *spacerItem1;
    QSpacerItem *spacerItem2;
    QPushButton *cmdCancel;

    void setupUi(QDialog *Process)
    {
    if (Process->objectName().isEmpty())
        Process->setObjectName(QString::fromUtf8("Process"));
    Process->setWindowModality(Qt::NonModal);
    Process->resize(280, 116);
    Process->setSizeGripEnabled(false);
    Process->setModal(false);
    gridLayout = new QGridLayout(Process);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setHorizontalSpacing(3);
    gridLayout->setVerticalSpacing(3);
    gridLayout->setContentsMargins(6, 6, 6, 6);
    groupBox = new QGroupBox(Process);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    vboxLayout = new QVBoxLayout(groupBox);
    vboxLayout->setSpacing(3);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    vboxLayout->setContentsMargins(6, 6, 6, 6);
    lblInfo = new QLabel(groupBox);
    lblInfo->setObjectName(QString::fromUtf8("lblInfo"));

    vboxLayout->addWidget(lblInfo);

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

    cmdCancel = new QPushButton(Process);
    cmdCancel->setObjectName(QString::fromUtf8("cmdCancel"));

    gridLayout->addWidget(cmdCancel, 1, 1, 1, 1);


    retranslateUi(Process);

    QMetaObject::connectSlotsByName(Process);
    } // setupUi

    void retranslateUi(QDialog *Process)
    {
    Process->setWindowTitle(QApplication::translate("Process", "Mounting image...", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QString());
    lblInfo->setText(QApplication::translate("Process", "src: /home/brezerk/cdrom.iso", 0, QApplication::UnicodeUTF8));
    cmdCancel->setText(QApplication::translate("Process", "Cancel", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Process);
    } // retranslateUi

};

namespace Ui {
    class Process: public Ui_Process {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROCESS_H
