/********************************************************************************
** Form generated from reading UI file 'IconSettings.ui'
**
** Created: Mon Feb 15 09:49:50 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ICONSETTINGS_H
#define UI_ICONSETTINGS_H

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

class Ui_IconSettings
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
    QGroupBox *groupBox;
    QVBoxLayout *_21;
    QWidget *widget_2;
    QHBoxLayout *hboxLayout1;
    QLabel *label;
    QLineEdit *txtProgramPath;
    QPushButton *cmdGetProgram;
    QWidget *widget_4;
    QHBoxLayout *hboxLayout2;
    QLabel *label_3;
    QLineEdit *txtCmdArgs;
    QWidget *widget_12;
    QHBoxLayout *hboxLayout3;
    QLabel *label_12;
    QLineEdit *txtWorkDir;
    QPushButton *cmdGetWorkDir;
    QGroupBox *groupBox_7;
    QGridLayout *gridLayout;
    QWidget *widget_10;
    QHBoxLayout *hboxLayout4;
    QLabel *label_8;
    QLineEdit *txtName;
    QWidget *widget_3;
    QGridLayout *gridLayout1;
    QPushButton *cmdGetIcon;
    QWidget *widget_11;
    QHBoxLayout *hboxLayout5;
    QLabel *label_11;
    QLineEdit *txtDesc;
    QGroupBox *groupBox_8;
    QVBoxLayout *verticalLayout;
    QWidget *widget_13;
    QHBoxLayout *_2;
    QLabel *label_7;
    QComboBox *cboxDesktopSize;
    QSpacerItem *verticalSpacer;
    QWidget *tab_3;
    QVBoxLayout *vboxLayout2;
    QGroupBox *groupBox_5;
    QHBoxLayout *hboxLayout6;
    QComboBox *cboxDlls;
    QSpacerItem *spacerItem1;
    QComboBox *cboxOveride;
    QPushButton *cmdAdd;
    QGroupBox *groupBox_6;
    QGridLayout *gridLayout2;
    QTableWidget *twDlls;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox_4;
    QVBoxLayout *vboxLayout3;
    QWidget *widget_9;
    QHBoxLayout *hboxLayout7;
    QLabel *label_10;
    QLineEdit *txtDisplay;
    QGroupBox *groupBox_3;
    QVBoxLayout *vboxLayout4;
    QCheckBox *cbUseConsole;
    QWidget *widget_8;
    QHBoxLayout *hboxLayout8;
    QLabel *label_9;
    QLineEdit *txtWinedebug;
    QGroupBox *groupBox_9;
    QVBoxLayout *_3;
    QWidget *widget_14;
    QHBoxLayout *_4;
    QLabel *label_13;
    QSpinBox *spinNice;
    QSpacerItem *spacerItem2;
    QHBoxLayout *hboxLayout9;
    QPushButton *cmdHelp;
    QSpacerItem *spacerItem3;
    QPushButton *cmdCancel;
    QPushButton *cmdOk;

    void setupUi(QDialog *IconSettings)
    {
        if (IconSettings->objectName().isEmpty())
            IconSettings->setObjectName(QString::fromUtf8("IconSettings"));
        IconSettings->setWindowModality(Qt::WindowModal);
        IconSettings->resize(638, 418);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/data/q4wine.png"), QSize(), QIcon::Normal, QIcon::Off);
        IconSettings->setWindowIcon(icon);
        vboxLayout = new QVBoxLayout(IconSettings);
        vboxLayout->setSpacing(0);
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        widget_7 = new QWidget(IconSettings);
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

        line = new QFrame(IconSettings);
        line->setObjectName(QString::fromUtf8("line"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(line->sizePolicy().hasHeightForWidth());
        line->setSizePolicy(sizePolicy);
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        vboxLayout->addWidget(line);

        widget = new QWidget(IconSettings);
        widget->setObjectName(QString::fromUtf8("widget"));
        vboxLayout1 = new QVBoxLayout(widget);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        twbGeneral = new QTabWidget(widget);
        twbGeneral->setObjectName(QString::fromUtf8("twbGeneral"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_2 = new QVBoxLayout(tab);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox = new QGroupBox(tab);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        _21 = new QVBoxLayout(groupBox);
        _21->setObjectName(QString::fromUtf8("_21"));
        widget_2 = new QWidget(groupBox);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        hboxLayout1 = new QHBoxLayout(widget_2);
        hboxLayout1->setContentsMargins(0, 0, 0, 0);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        label = new QLabel(widget_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(140, 0));
        label->setMaximumSize(QSize(140, 16777215));

        hboxLayout1->addWidget(label);

        txtProgramPath = new QLineEdit(widget_2);
        txtProgramPath->setObjectName(QString::fromUtf8("txtProgramPath"));

        hboxLayout1->addWidget(txtProgramPath);

        cmdGetProgram = new QPushButton(widget_2);
        cmdGetProgram->setObjectName(QString::fromUtf8("cmdGetProgram"));
        cmdGetProgram->setIconSize(QSize(16, 16));

        hboxLayout1->addWidget(cmdGetProgram);


        _21->addWidget(widget_2);

        widget_4 = new QWidget(groupBox);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        hboxLayout2 = new QHBoxLayout(widget_4);
        hboxLayout2->setContentsMargins(0, 0, 0, 0);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        label_3 = new QLabel(widget_4);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(140, 0));
        label_3->setMaximumSize(QSize(140, 16777215));

        hboxLayout2->addWidget(label_3);

        txtCmdArgs = new QLineEdit(widget_4);
        txtCmdArgs->setObjectName(QString::fromUtf8("txtCmdArgs"));

        hboxLayout2->addWidget(txtCmdArgs);


        _21->addWidget(widget_4);

        widget_12 = new QWidget(groupBox);
        widget_12->setObjectName(QString::fromUtf8("widget_12"));
        hboxLayout3 = new QHBoxLayout(widget_12);
        hboxLayout3->setContentsMargins(0, 0, 0, 0);
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        label_12 = new QLabel(widget_12);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setMinimumSize(QSize(140, 0));
        label_12->setMaximumSize(QSize(140, 16777215));

        hboxLayout3->addWidget(label_12);

        txtWorkDir = new QLineEdit(widget_12);
        txtWorkDir->setObjectName(QString::fromUtf8("txtWorkDir"));

        hboxLayout3->addWidget(txtWorkDir);

        cmdGetWorkDir = new QPushButton(widget_12);
        cmdGetWorkDir->setObjectName(QString::fromUtf8("cmdGetWorkDir"));
        cmdGetWorkDir->setIconSize(QSize(16, 16));

        hboxLayout3->addWidget(cmdGetWorkDir);


        _21->addWidget(widget_12);


        verticalLayout_2->addWidget(groupBox);

        groupBox_7 = new QGroupBox(tab);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        gridLayout = new QGridLayout(groupBox_7);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        widget_10 = new QWidget(groupBox_7);
        widget_10->setObjectName(QString::fromUtf8("widget_10"));
        hboxLayout4 = new QHBoxLayout(widget_10);
        hboxLayout4->setContentsMargins(0, 0, 0, 0);
        hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
        label_8 = new QLabel(widget_10);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMinimumSize(QSize(140, 0));
        label_8->setMaximumSize(QSize(140, 16777215));

        hboxLayout4->addWidget(label_8);

        txtName = new QLineEdit(widget_10);
        txtName->setObjectName(QString::fromUtf8("txtName"));

        hboxLayout4->addWidget(txtName);


        gridLayout->addWidget(widget_10, 0, 0, 1, 1);

        widget_3 = new QWidget(groupBox_7);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget_3->sizePolicy().hasHeightForWidth());
        widget_3->setSizePolicy(sizePolicy1);
        gridLayout1 = new QGridLayout(widget_3);
        gridLayout1->setContentsMargins(0, 0, 0, 0);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        cmdGetIcon = new QPushButton(widget_3);
        cmdGetIcon->setObjectName(QString::fromUtf8("cmdGetIcon"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(cmdGetIcon->sizePolicy().hasHeightForWidth());
        cmdGetIcon->setSizePolicy(sizePolicy2);
        cmdGetIcon->setIconSize(QSize(32, 32));

        gridLayout1->addWidget(cmdGetIcon, 0, 0, 1, 1);


        gridLayout->addWidget(widget_3, 0, 1, 2, 1);

        widget_11 = new QWidget(groupBox_7);
        widget_11->setObjectName(QString::fromUtf8("widget_11"));
        hboxLayout5 = new QHBoxLayout(widget_11);
        hboxLayout5->setContentsMargins(0, 0, 0, 0);
        hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
        label_11 = new QLabel(widget_11);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setMinimumSize(QSize(140, 0));
        label_11->setMaximumSize(QSize(140, 16777215));

        hboxLayout5->addWidget(label_11);

        txtDesc = new QLineEdit(widget_11);
        txtDesc->setObjectName(QString::fromUtf8("txtDesc"));

        hboxLayout5->addWidget(txtDesc);


        gridLayout->addWidget(widget_11, 1, 0, 1, 1);


        verticalLayout_2->addWidget(groupBox_7);

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

        verticalSpacer = new QSpacerItem(20, 11, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        twbGeneral->addTab(tab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        vboxLayout2 = new QVBoxLayout(tab_3);
        vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
        groupBox_5 = new QGroupBox(tab_3);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        hboxLayout6 = new QHBoxLayout(groupBox_5);
        hboxLayout6->setObjectName(QString::fromUtf8("hboxLayout6"));
        cboxDlls = new QComboBox(groupBox_5);
        cboxDlls->setObjectName(QString::fromUtf8("cboxDlls"));
        cboxDlls->setMinimumSize(QSize(300, 0));
        cboxDlls->setMaximumSize(QSize(300, 16777215));
        cboxDlls->setMaxVisibleItems(5);

        hboxLayout6->addWidget(cboxDlls);

        spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout6->addItem(spacerItem1);

        cboxOveride = new QComboBox(groupBox_5);
        cboxOveride->setObjectName(QString::fromUtf8("cboxOveride"));

        hboxLayout6->addWidget(cboxOveride);

        cmdAdd = new QPushButton(groupBox_5);
        cmdAdd->setObjectName(QString::fromUtf8("cmdAdd"));

        hboxLayout6->addWidget(cmdAdd);


        vboxLayout2->addWidget(groupBox_5);

        groupBox_6 = new QGroupBox(tab_3);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        gridLayout2 = new QGridLayout(groupBox_6);
        gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
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

        gridLayout2->addWidget(twDlls, 0, 0, 1, 1);


        vboxLayout2->addWidget(groupBox_6);

        twbGeneral->addTab(tab_3, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_3 = new QVBoxLayout(tab_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        groupBox_4 = new QGroupBox(tab_2);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        vboxLayout3 = new QVBoxLayout(groupBox_4);
        vboxLayout3->setSpacing(2);
        vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
        widget_9 = new QWidget(groupBox_4);
        widget_9->setObjectName(QString::fromUtf8("widget_9"));
        hboxLayout7 = new QHBoxLayout(widget_9);
        hboxLayout7->setContentsMargins(0, 0, 0, 0);
        hboxLayout7->setObjectName(QString::fromUtf8("hboxLayout7"));
        label_10 = new QLabel(widget_9);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setMinimumSize(QSize(140, 0));
        label_10->setMaximumSize(QSize(140, 16777215));

        hboxLayout7->addWidget(label_10);

        txtDisplay = new QLineEdit(widget_9);
        txtDisplay->setObjectName(QString::fromUtf8("txtDisplay"));

        hboxLayout7->addWidget(txtDisplay);


        vboxLayout3->addWidget(widget_9);


        verticalLayout_3->addWidget(groupBox_4);

        groupBox_3 = new QGroupBox(tab_2);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        vboxLayout4 = new QVBoxLayout(groupBox_3);
        vboxLayout4->setObjectName(QString::fromUtf8("vboxLayout4"));
        vboxLayout4->setContentsMargins(-1, 0, -1, -1);
        cbUseConsole = new QCheckBox(groupBox_3);
        cbUseConsole->setObjectName(QString::fromUtf8("cbUseConsole"));

        vboxLayout4->addWidget(cbUseConsole);

        widget_8 = new QWidget(groupBox_3);
        widget_8->setObjectName(QString::fromUtf8("widget_8"));
        hboxLayout8 = new QHBoxLayout(widget_8);
        hboxLayout8->setContentsMargins(0, 0, 0, 0);
        hboxLayout8->setObjectName(QString::fromUtf8("hboxLayout8"));
        label_9 = new QLabel(widget_8);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setMinimumSize(QSize(140, 0));
        label_9->setMaximumSize(QSize(140, 16777215));

        hboxLayout8->addWidget(label_9);

        txtWinedebug = new QLineEdit(widget_8);
        txtWinedebug->setObjectName(QString::fromUtf8("txtWinedebug"));
        txtWinedebug->setEnabled(false);

        hboxLayout8->addWidget(txtWinedebug);


        vboxLayout4->addWidget(widget_8);


        verticalLayout_3->addWidget(groupBox_3);

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

        spinNice = new QSpinBox(widget_14);
        spinNice->setObjectName(QString::fromUtf8("spinNice"));
        spinNice->setMinimum(0);
        spinNice->setMaximum(20);

        _4->addWidget(spinNice);


        _3->addWidget(widget_14);


        verticalLayout_3->addWidget(groupBox_9);

        spacerItem2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(spacerItem2);

        twbGeneral->addTab(tab_2, QString());

        vboxLayout1->addWidget(twbGeneral);

        hboxLayout9 = new QHBoxLayout();
        hboxLayout9->setObjectName(QString::fromUtf8("hboxLayout9"));
        cmdHelp = new QPushButton(widget);
        cmdHelp->setObjectName(QString::fromUtf8("cmdHelp"));

        hboxLayout9->addWidget(cmdHelp);

        spacerItem3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout9->addItem(spacerItem3);

        cmdCancel = new QPushButton(widget);
        cmdCancel->setObjectName(QString::fromUtf8("cmdCancel"));

        hboxLayout9->addWidget(cmdCancel);

        cmdOk = new QPushButton(widget);
        cmdOk->setObjectName(QString::fromUtf8("cmdOk"));

        hboxLayout9->addWidget(cmdOk);


        vboxLayout1->addLayout(hboxLayout9);


        vboxLayout->addWidget(widget);


        retranslateUi(IconSettings);

        twbGeneral->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(IconSettings);
    } // setupUi

    void retranslateUi(QDialog *IconSettings)
    {
        IconSettings->setWindowTitle(QApplication::translate("IconSettings", "Icon settings", 0, QApplication::UnicodeUTF8));
        lblCaption->setText(QApplication::translate("IconSettings", "Iconn settings", 0, QApplication::UnicodeUTF8));
        lblLogo->setText(QString());
        groupBox->setTitle(QApplication::translate("IconSettings", "General", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("IconSettings", "Program:", 0, QApplication::UnicodeUTF8));
        cmdGetProgram->setText(QString());
        label_3->setText(QApplication::translate("IconSettings", "Cmd Args:", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("IconSettings", "Working dir:", 0, QApplication::UnicodeUTF8));
        cmdGetWorkDir->setText(QString());
        groupBox_7->setTitle(QApplication::translate("IconSettings", "Icon options", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("IconSettings", "Name:", 0, QApplication::UnicodeUTF8));
        cmdGetIcon->setText(QString());
        label_11->setText(QApplication::translate("IconSettings", "Description:", 0, QApplication::UnicodeUTF8));
        groupBox_8->setTitle(QApplication::translate("IconSettings", "Virtua desktop", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("IconSettings", "Size:", 0, QApplication::UnicodeUTF8));
        cboxDesktopSize->setItemText(0, QApplication::translate("IconSettings", "No virtual desktop", 0, QApplication::UnicodeUTF8));

        twbGeneral->setTabText(twbGeneral->indexOf(tab), QApplication::translate("IconSettings", "General", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("IconSettings", "Present wine dlls", 0, QApplication::UnicodeUTF8));
        cboxOveride->clear();
        cboxOveride->insertItems(0, QStringList()
         << QApplication::translate("IconSettings", "Native", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("IconSettings", "Buildin", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("IconSettings", "Native, Buildin", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("IconSettings", "Buildin, Native", 0, QApplication::UnicodeUTF8)
        );
        cmdAdd->setText(QApplication::translate("IconSettings", "Add", 0, QApplication::UnicodeUTF8));
        groupBox_6->setTitle(QApplication::translate("IconSettings", "Overrided dlls", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = twDlls->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("IconSettings", "Dll name", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = twDlls->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("IconSettings", "Override algorithm", 0, QApplication::UnicodeUTF8));
        twbGeneral->setTabText(twbGeneral->indexOf(tab_3), QApplication::translate("IconSettings", "Override Dll", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("IconSettings", "Display", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("IconSettings", "DISPLAY:", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("IconSettings", "Debug options", 0, QApplication::UnicodeUTF8));
        cbUseConsole->setText(QApplication::translate("IconSettings", "Run with output in console", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("IconSettings", "WINEDEBUG:", 0, QApplication::UnicodeUTF8));
        groupBox_9->setTitle(QApplication::translate("IconSettings", "Nice level priority", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("IconSettings", "Priority:", 0, QApplication::UnicodeUTF8));
        twbGeneral->setTabText(twbGeneral->indexOf(tab_2), QApplication::translate("IconSettings", "Advanced", 0, QApplication::UnicodeUTF8));
        cmdHelp->setText(QApplication::translate("IconSettings", "Help", 0, QApplication::UnicodeUTF8));
        cmdCancel->setText(QApplication::translate("IconSettings", "Cancel", 0, QApplication::UnicodeUTF8));
        cmdOk->setText(QApplication::translate("IconSettings", "Ok", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class IconSettings: public Ui_IconSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ICONSETTINGS_H
