/********************************************************************************
** Form generated from reading UI file 'Run.ui'
**
** Created: Mon Feb 15 09:49:50 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RUN_H
#define UI_RUN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Run
{
public:
    QVBoxLayout *vboxLayout;
    QWidget *widget_7;
    QHBoxLayout *hboxLayout;
    QLabel *lblCaption;
    QSpacerItem *spacerItem;
    QLabel *lblLogo;
    QFrame *line;
    QWidget *widget;
    QVBoxLayout *vboxLayout1;
    QTabWidget *twbGeneral;
    QWidget *tab;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox_7;
    QVBoxLayout *vboxLayout2;
    QWidget *widget_29;
    QHBoxLayout *hboxLayout1;
    QLabel *label_28;
    QComboBox *comboPrefixes;
    QGroupBox *groupBox;
    QVBoxLayout *vboxLayout3;
    QWidget *widget_2;
    QHBoxLayout *hboxLayout2;
    QLabel *label;
    QLineEdit *txtProgramBin;
    QPushButton *cmdGetProgramBin;
    QWidget *widget_4;
    QHBoxLayout *hboxLayout3;
    QLabel *label_3;
    QLineEdit *txtCmdArgs;
    QWidget *widget_12;
    QHBoxLayout *hboxLayout4;
    QLabel *label_12;
    QLineEdit *txtWorkDir;
    QPushButton *cmdGetWorkDir;
    QGroupBox *groupBox_8;
    QVBoxLayout *verticalLayout;
    QWidget *widget_13;
    QHBoxLayout *_2;
    QLabel *label_7;
    QComboBox *cboxDesktopSize;
    QSpacerItem *spacerItem1;
    QWidget *tab_3;
    QVBoxLayout *vboxLayout4;
    QGroupBox *groupBox_5;
    QHBoxLayout *hboxLayout5;
    QComboBox *cboxDlls;
    QSpacerItem *spacerItem2;
    QComboBox *cboxOveride;
    QPushButton *cmdAdd;
    QGroupBox *groupBox_6;
    QGridLayout *gridLayout;
    QTableWidget *twDlls;
    QWidget *tab_2;
    QVBoxLayout *vboxLayout5;
    QGroupBox *groupBox_4;
    QVBoxLayout *vboxLayout6;
    QWidget *widget_9;
    QHBoxLayout *hboxLayout6;
    QLabel *label_10;
    QLineEdit *txtDisplay;
    QGroupBox *groupBox_3;
    QVBoxLayout *vboxLayout7;
    QCheckBox *cbUseConsole;
    QWidget *widget_8;
    QHBoxLayout *hboxLayout7;
    QLabel *label_9;
    QLineEdit *txtWinedebug;
    QGroupBox *groupBox_9;
    QVBoxLayout *_3;
    QWidget *widget_14;
    QHBoxLayout *_4;
    QLabel *label_13;
    QSpinBox *txtNice;
    QSpacerItem *spacerItem3;
    QHBoxLayout *hboxLayout8;
    QPushButton *cmdHelp;
    QSpacerItem *spacerItem4;
    QPushButton *cmdCancel;
    QPushButton *cmdOk;

    void setupUi(QDialog *Run)
    {
        if (Run->objectName().isEmpty())
            Run->setObjectName(QString::fromUtf8("Run"));
        Run->setWindowModality(Qt::WindowModal);
        Run->resize(638, 418);
        Run->setMinimumSize(QSize(340, 370));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/data/q4wine.png"), QSize(), QIcon::Normal, QIcon::Off);
        Run->setWindowIcon(icon);
        vboxLayout = new QVBoxLayout(Run);
        vboxLayout->setSpacing(0);
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        widget_7 = new QWidget(Run);
        widget_7->setObjectName(QString::fromUtf8("widget_7"));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        widget_7->setPalette(palette);
        widget_7->setAutoFillBackground(true);
        hboxLayout = new QHBoxLayout(widget_7);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        hboxLayout->setContentsMargins(18, -1, -1, -1);
        lblCaption = new QLabel(widget_7);
        lblCaption->setObjectName(QString::fromUtf8("lblCaption"));
        QFont font;
        font.setPointSize(11);
        font.setBold(true);
        font.setWeight(75);
        lblCaption->setFont(font);

        hboxLayout->addWidget(lblCaption);

        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        lblLogo = new QLabel(widget_7);
        lblLogo->setObjectName(QString::fromUtf8("lblLogo"));

        hboxLayout->addWidget(lblLogo);


        vboxLayout->addWidget(widget_7);

        line = new QFrame(Run);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        vboxLayout->addWidget(line);

        widget = new QWidget(Run);
        widget->setObjectName(QString::fromUtf8("widget"));
        vboxLayout1 = new QVBoxLayout(widget);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        twbGeneral = new QTabWidget(widget);
        twbGeneral->setObjectName(QString::fromUtf8("twbGeneral"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_2 = new QVBoxLayout(tab);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox_7 = new QGroupBox(tab);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        vboxLayout2 = new QVBoxLayout(groupBox_7);
        vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
        widget_29 = new QWidget(groupBox_7);
        widget_29->setObjectName(QString::fromUtf8("widget_29"));
        hboxLayout1 = new QHBoxLayout(widget_29);
        hboxLayout1->setContentsMargins(0, 0, 0, 0);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        label_28 = new QLabel(widget_29);
        label_28->setObjectName(QString::fromUtf8("label_28"));
        label_28->setMinimumSize(QSize(140, 0));
        label_28->setMaximumSize(QSize(140, 16777215));

        hboxLayout1->addWidget(label_28);

        comboPrefixes = new QComboBox(widget_29);
        comboPrefixes->setObjectName(QString::fromUtf8("comboPrefixes"));

        hboxLayout1->addWidget(comboPrefixes);


        vboxLayout2->addWidget(widget_29);


        verticalLayout_2->addWidget(groupBox_7);

        groupBox = new QGroupBox(tab);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        vboxLayout3 = new QVBoxLayout(groupBox);
        vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
        widget_2 = new QWidget(groupBox);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        hboxLayout2 = new QHBoxLayout(widget_2);
        hboxLayout2->setContentsMargins(0, 0, 0, 0);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        label = new QLabel(widget_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(140, 0));
        label->setMaximumSize(QSize(140, 16777215));

        hboxLayout2->addWidget(label);

        txtProgramBin = new QLineEdit(widget_2);
        txtProgramBin->setObjectName(QString::fromUtf8("txtProgramBin"));

        hboxLayout2->addWidget(txtProgramBin);

        cmdGetProgramBin = new QPushButton(widget_2);
        cmdGetProgramBin->setObjectName(QString::fromUtf8("cmdGetProgramBin"));
        cmdGetProgramBin->setIconSize(QSize(16, 16));

        hboxLayout2->addWidget(cmdGetProgramBin);


        vboxLayout3->addWidget(widget_2);

        widget_4 = new QWidget(groupBox);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        hboxLayout3 = new QHBoxLayout(widget_4);
        hboxLayout3->setContentsMargins(0, 0, 0, 0);
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        label_3 = new QLabel(widget_4);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(140, 0));
        label_3->setMaximumSize(QSize(140, 16777215));

        hboxLayout3->addWidget(label_3);

        txtCmdArgs = new QLineEdit(widget_4);
        txtCmdArgs->setObjectName(QString::fromUtf8("txtCmdArgs"));

        hboxLayout3->addWidget(txtCmdArgs);


        vboxLayout3->addWidget(widget_4);

        widget_12 = new QWidget(groupBox);
        widget_12->setObjectName(QString::fromUtf8("widget_12"));
        hboxLayout4 = new QHBoxLayout(widget_12);
        hboxLayout4->setContentsMargins(0, 0, 0, 0);
        hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
        label_12 = new QLabel(widget_12);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setMinimumSize(QSize(140, 0));
        label_12->setMaximumSize(QSize(140, 16777215));

        hboxLayout4->addWidget(label_12);

        txtWorkDir = new QLineEdit(widget_12);
        txtWorkDir->setObjectName(QString::fromUtf8("txtWorkDir"));

        hboxLayout4->addWidget(txtWorkDir);

        cmdGetWorkDir = new QPushButton(widget_12);
        cmdGetWorkDir->setObjectName(QString::fromUtf8("cmdGetWorkDir"));
        cmdGetWorkDir->setIconSize(QSize(16, 16));

        hboxLayout4->addWidget(cmdGetWorkDir);


        vboxLayout3->addWidget(widget_12);


        verticalLayout_2->addWidget(groupBox);

        groupBox_8 = new QGroupBox(tab);
        groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
        verticalLayout = new QVBoxLayout(groupBox_8);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        widget_13 = new QWidget(groupBox_8);
        widget_13->setObjectName(QString::fromUtf8("widget_13"));
        _2 = new QHBoxLayout(widget_13);
        _2->setContentsMargins(0, 0, 0, 0);
        _2->setObjectName(QString::fromUtf8("_2"));
        label_7 = new QLabel(widget_13);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setMinimumSize(QSize(140, 0));
        label_7->setMaximumSize(QSize(140, 16777215));

        _2->addWidget(label_7);

        cboxDesktopSize = new QComboBox(widget_13);
        cboxDesktopSize->addItem(QString());
        cboxDesktopSize->addItem(QString::fromUtf8("320x240"));
        cboxDesktopSize->addItem(QString::fromUtf8("640x480"));
        cboxDesktopSize->addItem(QString::fromUtf8("800x600"));
        cboxDesktopSize->addItem(QString::fromUtf8("1024x768"));
        cboxDesktopSize->addItem(QString::fromUtf8("1600x1200"));
        cboxDesktopSize->addItem(QString::fromUtf8("1440x900"));
        cboxDesktopSize->addItem(QString::fromUtf8("1680x1050"));
        cboxDesktopSize->addItem(QString::fromUtf8("1920x1200"));
        cboxDesktopSize->addItem(QString::fromUtf8("2560x1600"));
        cboxDesktopSize->setObjectName(QString::fromUtf8("cboxDesktopSize"));

        _2->addWidget(cboxDesktopSize);


        verticalLayout->addWidget(widget_13);


        verticalLayout_2->addWidget(groupBox_8);

        spacerItem1 = new QSpacerItem(680, 54, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(spacerItem1);

        twbGeneral->addTab(tab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        vboxLayout4 = new QVBoxLayout(tab_3);
        vboxLayout4->setObjectName(QString::fromUtf8("vboxLayout4"));
        groupBox_5 = new QGroupBox(tab_3);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        hboxLayout5 = new QHBoxLayout(groupBox_5);
        hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
        cboxDlls = new QComboBox(groupBox_5);
        cboxDlls->setObjectName(QString::fromUtf8("cboxDlls"));
        cboxDlls->setMinimumSize(QSize(300, 0));
        cboxDlls->setMaximumSize(QSize(300, 16777215));
        cboxDlls->setMaxVisibleItems(5);

        hboxLayout5->addWidget(cboxDlls);

        spacerItem2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout5->addItem(spacerItem2);

        cboxOveride = new QComboBox(groupBox_5);
        cboxOveride->setObjectName(QString::fromUtf8("cboxOveride"));

        hboxLayout5->addWidget(cboxOveride);

        cmdAdd = new QPushButton(groupBox_5);
        cmdAdd->setObjectName(QString::fromUtf8("cmdAdd"));

        hboxLayout5->addWidget(cmdAdd);


        vboxLayout4->addWidget(groupBox_5);

        groupBox_6 = new QGroupBox(tab_3);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        gridLayout = new QGridLayout(groupBox_6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        twDlls = new QTableWidget(groupBox_6);
        if (twDlls->columnCount() < 2)
            twDlls->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        twDlls->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        twDlls->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        twDlls->setObjectName(QString::fromUtf8("twDlls"));
        twDlls->setSelectionMode(QAbstractItemView::SingleSelection);
        twDlls->setSelectionBehavior(QAbstractItemView::SelectRows);
        twDlls->setSortingEnabled(false);

        gridLayout->addWidget(twDlls, 0, 0, 1, 1);


        vboxLayout4->addWidget(groupBox_6);

        twbGeneral->addTab(tab_3, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        vboxLayout5 = new QVBoxLayout(tab_2);
        vboxLayout5->setObjectName(QString::fromUtf8("vboxLayout5"));
        groupBox_4 = new QGroupBox(tab_2);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        vboxLayout6 = new QVBoxLayout(groupBox_4);
        vboxLayout6->setSpacing(2);
        vboxLayout6->setObjectName(QString::fromUtf8("vboxLayout6"));
        widget_9 = new QWidget(groupBox_4);
        widget_9->setObjectName(QString::fromUtf8("widget_9"));
        hboxLayout6 = new QHBoxLayout(widget_9);
        hboxLayout6->setContentsMargins(0, 0, 0, 0);
        hboxLayout6->setObjectName(QString::fromUtf8("hboxLayout6"));
        label_10 = new QLabel(widget_9);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setMinimumSize(QSize(140, 0));
        label_10->setMaximumSize(QSize(140, 16777215));

        hboxLayout6->addWidget(label_10);

        txtDisplay = new QLineEdit(widget_9);
        txtDisplay->setObjectName(QString::fromUtf8("txtDisplay"));

        hboxLayout6->addWidget(txtDisplay);


        vboxLayout6->addWidget(widget_9);


        vboxLayout5->addWidget(groupBox_4);

        groupBox_3 = new QGroupBox(tab_2);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        vboxLayout7 = new QVBoxLayout(groupBox_3);
        vboxLayout7->setObjectName(QString::fromUtf8("vboxLayout7"));
        vboxLayout7->setContentsMargins(-1, 0, -1, -1);
        cbUseConsole = new QCheckBox(groupBox_3);
        cbUseConsole->setObjectName(QString::fromUtf8("cbUseConsole"));

        vboxLayout7->addWidget(cbUseConsole);

        widget_8 = new QWidget(groupBox_3);
        widget_8->setObjectName(QString::fromUtf8("widget_8"));
        hboxLayout7 = new QHBoxLayout(widget_8);
        hboxLayout7->setContentsMargins(0, 0, 0, 0);
        hboxLayout7->setObjectName(QString::fromUtf8("hboxLayout7"));
        label_9 = new QLabel(widget_8);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setMinimumSize(QSize(140, 0));
        label_9->setMaximumSize(QSize(140, 16777215));

        hboxLayout7->addWidget(label_9);

        txtWinedebug = new QLineEdit(widget_8);
        txtWinedebug->setObjectName(QString::fromUtf8("txtWinedebug"));
        txtWinedebug->setEnabled(false);

        hboxLayout7->addWidget(txtWinedebug);


        vboxLayout7->addWidget(widget_8);


        vboxLayout5->addWidget(groupBox_3);

        groupBox_9 = new QGroupBox(tab_2);
        groupBox_9->setObjectName(QString::fromUtf8("groupBox_9"));
        _3 = new QVBoxLayout(groupBox_9);
        _3->setObjectName(QString::fromUtf8("_3"));
        _3->setContentsMargins(-1, 0, -1, -1);
        widget_14 = new QWidget(groupBox_9);
        widget_14->setObjectName(QString::fromUtf8("widget_14"));
        _4 = new QHBoxLayout(widget_14);
        _4->setContentsMargins(0, 0, 0, 0);
        _4->setObjectName(QString::fromUtf8("_4"));
        label_13 = new QLabel(widget_14);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setMinimumSize(QSize(140, 0));
        label_13->setMaximumSize(QSize(140, 16777215));

        _4->addWidget(label_13);

        txtNice = new QSpinBox(widget_14);
        txtNice->setObjectName(QString::fromUtf8("txtNice"));
        txtNice->setMinimum(0);
        txtNice->setMaximum(20);

        _4->addWidget(txtNice);


        _3->addWidget(widget_14);


        vboxLayout5->addWidget(groupBox_9);

        spacerItem3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout5->addItem(spacerItem3);

        twbGeneral->addTab(tab_2, QString());

        vboxLayout1->addWidget(twbGeneral);

        hboxLayout8 = new QHBoxLayout();
        hboxLayout8->setObjectName(QString::fromUtf8("hboxLayout8"));
        cmdHelp = new QPushButton(widget);
        cmdHelp->setObjectName(QString::fromUtf8("cmdHelp"));

        hboxLayout8->addWidget(cmdHelp);

        spacerItem4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout8->addItem(spacerItem4);

        cmdCancel = new QPushButton(widget);
        cmdCancel->setObjectName(QString::fromUtf8("cmdCancel"));

        hboxLayout8->addWidget(cmdCancel);

        cmdOk = new QPushButton(widget);
        cmdOk->setObjectName(QString::fromUtf8("cmdOk"));

        hboxLayout8->addWidget(cmdOk);


        vboxLayout1->addLayout(hboxLayout8);


        vboxLayout->addWidget(widget);


        retranslateUi(Run);

        twbGeneral->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Run);
    } // setupUi

    void retranslateUi(QDialog *Run)
    {
        Run->setWindowTitle(QApplication::translate("Run", "Run program", 0, QApplication::UnicodeUTF8));
        lblCaption->setText(QApplication::translate("Run", "Run program", 0, QApplication::UnicodeUTF8));
        lblLogo->setText(QString());
        groupBox_7->setTitle(QApplication::translate("Run", "Prefix", 0, QApplication::UnicodeUTF8));
        label_28->setText(QApplication::translate("Run", "Name:", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("Run", "General", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Run", "Program:", 0, QApplication::UnicodeUTF8));
        cmdGetProgramBin->setText(QString());
        label_3->setText(QApplication::translate("Run", "Cmd Args:", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("Run", "Working dir:", 0, QApplication::UnicodeUTF8));
        cmdGetWorkDir->setText(QString());
        groupBox_8->setTitle(QApplication::translate("Run", "Virtual desktop", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("Run", "Size:", 0, QApplication::UnicodeUTF8));
        cboxDesktopSize->setItemText(0, QApplication::translate("Run", "No virtual desktop", 0, QApplication::UnicodeUTF8));

        twbGeneral->setTabText(twbGeneral->indexOf(tab), QApplication::translate("Run", "General", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("Run", "Present wine dlls", 0, QApplication::UnicodeUTF8));
        cboxOveride->clear();
        cboxOveride->insertItems(0, QStringList()
         << QApplication::translate("Run", "Native", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Run", "Buildin", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Run", "Native, Buildin", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Run", "Buildin, Native", 0, QApplication::UnicodeUTF8)
        );
        cmdAdd->setText(QApplication::translate("Run", "Add", 0, QApplication::UnicodeUTF8));
        groupBox_6->setTitle(QApplication::translate("Run", "Overrided dlls", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = twDlls->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("Run", "Dll name", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = twDlls->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("Run", "Override algoritm", 0, QApplication::UnicodeUTF8));
        twbGeneral->setTabText(twbGeneral->indexOf(tab_3), QApplication::translate("Run", "Override Dll", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("Run", "Display", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("Run", "DISPLAY:", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("Run", "Debug options", 0, QApplication::UnicodeUTF8));
        cbUseConsole->setText(QApplication::translate("Run", "Run with output in console", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("Run", "WINEDEBUG:", 0, QApplication::UnicodeUTF8));
        groupBox_9->setTitle(QApplication::translate("Run", "Nice level priority", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("Run", "Priority:", 0, QApplication::UnicodeUTF8));
        twbGeneral->setTabText(twbGeneral->indexOf(tab_2), QApplication::translate("Run", "Advanced", 0, QApplication::UnicodeUTF8));
        cmdHelp->setText(QApplication::translate("Run", "Help", 0, QApplication::UnicodeUTF8));
        cmdCancel->setText(QApplication::translate("Run", "Cancel", 0, QApplication::UnicodeUTF8));
        cmdOk->setText(QApplication::translate("Run", "Ok", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Run: public Ui_Run {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RUN_H
