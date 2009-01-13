/********************************************************************************
** Form generated from reading ui file 'AppSettings.ui'
**
** Created: Tue Jan 13 11:38:31 2009
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_APPSETTINGS_H
#define UI_APPSETTINGS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AppSettings
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
    QVBoxLayout *vboxLayout2;
    QGroupBox *groupBox;
    QVBoxLayout *vboxLayout3;
    QWidget *widget_2;
    QHBoxLayout *hboxLayout1;
    QLabel *label;
    QLineEdit *txtWineBin;
    QPushButton *cmdGetWineBin;
    QWidget *widget_4;
    QHBoxLayout *hboxLayout2;
    QLabel *label_2;
    QLineEdit *txtWineServerBin;
    QPushButton *cmdGetWineServerBin;
    QWidget *widget_10;
    QHBoxLayout *hboxLayout3;
    QLabel *label_3;
    QLineEdit *txtWineLoaderBin;
    QPushButton *cmdGetWineLoaderBin;
    QWidget *widget_11;
    QHBoxLayout *hboxLayout4;
    QLabel *label_11;
    QLineEdit *txtWineLibs;
    QPushButton *cmdGetWineLibs;
    QGroupBox *groupBox_4;
    QVBoxLayout *vboxLayout4;
    QCheckBox *chShowTrarey;
    QSpacerItem *spacerItem1;
    QWidget *tab_4;
    QVBoxLayout *vboxLayout5;
    QGroupBox *groupBox_7;
    QVBoxLayout *vboxLayout6;
    QWidget *widget_14;
    QHBoxLayout *hboxLayout5;
    QLabel *label_13;
    QLineEdit *txtTarBin;
    QPushButton *cmdGetTarBin;
    QWidget *widget_17;
    QHBoxLayout *hboxLayout6;
    QLabel *label_16;
    QLineEdit *txtMountBin;
    QPushButton *cmdGetMountBin;
    QWidget *widget_18;
    QHBoxLayout *hboxLayout7;
    QLabel *label_17;
    QLineEdit *txtUmountBin;
    QPushButton *cmdGetUmountBin;
    QWidget *widget_19;
    QHBoxLayout *hboxLayout8;
    QLabel *label_19;
    QLineEdit *txtSudoBin;
    QPushButton *cmdGetSudoBin;
    QWidget *widget_24;
    QHBoxLayout *hboxLayout9;
    QLabel *label_23;
    QLineEdit *txtNiceBin;
    QPushButton *cmdGetNiceBin;
    QWidget *widget_25;
    QHBoxLayout *hboxLayout10;
    QLabel *label_24;
    QLineEdit *txtReniceBin;
    QPushButton *cmdGetReniceBin;
    QWidget *widget_28;
    QHBoxLayout *hboxLayout11;
    QLabel *label_27;
    QLineEdit *txtShBin;
    QPushButton *cmdGetShBin;
    QSpacerItem *spacerItem2;
    QWidget *tab_3;
    QVBoxLayout *vboxLayout7;
    QGroupBox *groupBox_2;
    QVBoxLayout *vboxLayout8;
    QWidget *widget_12;
    QHBoxLayout *hboxLayout12;
    QLabel *label_6;
    QLineEdit *txtConsoleBin;
    QPushButton *cmdGetConsoleBin;
    QWidget *widget_13;
    QHBoxLayout *hboxLayout13;
    QLabel *label_12;
    QLineEdit *txtConsoleArgs;
    QGroupBox *groupBox_5;
    QVBoxLayout *vboxLayout9;
    QWidget *widget_26;
    QHBoxLayout *hboxLayout14;
    QLabel *label_25;
    QLineEdit *txtWrestoolBin;
    QPushButton *cmdGetWrestoolBin;
    QWidget *widget_27;
    QHBoxLayout *hboxLayout15;
    QLabel *label_26;
    QLineEdit *txtIcotoolBin;
    QPushButton *cmdGetIcotoolBin;
    QSpacerItem *spacerItem3;
    QWidget *tab_2;
    QVBoxLayout *vboxLayout10;
    QGroupBox *groupBox_9;
    QVBoxLayout *vboxLayout11;
    QWidget *widget_30;
    QHBoxLayout *hboxLayout16;
    QLabel *label_29;
    QComboBox *comboLangs;
    QGroupBox *groupBox_3;
    QVBoxLayout *vboxLayout12;
    QListWidget *listThemesView;
    QWidget *tab_5;
    QVBoxLayout *vboxLayout13;
    QGroupBox *groupBox_8;
    QVBoxLayout *vboxLayout14;
    QWidget *widget_29;
    QHBoxLayout *hboxLayout17;
    QLabel *label_28;
    QComboBox *comboProxyType;
    QWidget *widget_20;
    QHBoxLayout *hboxLayout18;
    QLabel *label_18;
    QLineEdit *txtProxyHost;
    QWidget *widget_21;
    QHBoxLayout *hboxLayout19;
    QLabel *label_20;
    QLineEdit *txtProxyPort;
    QWidget *widget_22;
    QHBoxLayout *hboxLayout20;
    QLabel *label_21;
    QLineEdit *txtProxyUser;
    QWidget *widget_23;
    QHBoxLayout *hboxLayout21;
    QLabel *label_22;
    QLineEdit *txtProxyPass;
    QSpacerItem *spacerItem4;
    QHBoxLayout *hboxLayout22;
    QPushButton *cmdHelp;
    QSpacerItem *spacerItem5;
    QPushButton *cmdCancel;
    QPushButton *cmdOk;

    void setupUi(QDialog *AppSettings)
    {
    if (AppSettings->objectName().isEmpty())
        AppSettings->setObjectName(QString::fromUtf8("AppSettings"));
    AppSettings->resize(720, 439);
    AppSettings->setMinimumSize(QSize(340, 370));
    vboxLayout = new QVBoxLayout(AppSettings);
    vboxLayout->setSpacing(0);
    vboxLayout->setMargin(0);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    widget_7 = new QWidget(AppSettings);
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
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(lblLogo->sizePolicy().hasHeightForWidth());
    lblLogo->setSizePolicy(sizePolicy);
    lblLogo->setMaximumSize(QSize(48, 48));

    hboxLayout->addWidget(lblLogo);


    vboxLayout->addWidget(widget_7);

    line = new QFrame(AppSettings);
    line->setObjectName(QString::fromUtf8("line"));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    vboxLayout->addWidget(line);

    widget = new QWidget(AppSettings);
    widget->setObjectName(QString::fromUtf8("widget"));
    vboxLayout1 = new QVBoxLayout(widget);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    twbGeneral = new QTabWidget(widget);
    twbGeneral->setObjectName(QString::fromUtf8("twbGeneral"));
    tab = new QWidget();
    tab->setObjectName(QString::fromUtf8("tab"));
    vboxLayout2 = new QVBoxLayout(tab);
    vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
    groupBox = new QGroupBox(tab);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    vboxLayout3 = new QVBoxLayout(groupBox);
    vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
    widget_2 = new QWidget(groupBox);
    widget_2->setObjectName(QString::fromUtf8("widget_2"));
    hboxLayout1 = new QHBoxLayout(widget_2);
    hboxLayout1->setMargin(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    label = new QLabel(widget_2);
    label->setObjectName(QString::fromUtf8("label"));
    label->setMinimumSize(QSize(140, 0));
    label->setMaximumSize(QSize(140, 16777215));

    hboxLayout1->addWidget(label);

    txtWineBin = new QLineEdit(widget_2);
    txtWineBin->setObjectName(QString::fromUtf8("txtWineBin"));

    hboxLayout1->addWidget(txtWineBin);

    cmdGetWineBin = new QPushButton(widget_2);
    cmdGetWineBin->setObjectName(QString::fromUtf8("cmdGetWineBin"));
    cmdGetWineBin->setIconSize(QSize(16, 16));

    hboxLayout1->addWidget(cmdGetWineBin);


    vboxLayout3->addWidget(widget_2);

    widget_4 = new QWidget(groupBox);
    widget_4->setObjectName(QString::fromUtf8("widget_4"));
    hboxLayout2 = new QHBoxLayout(widget_4);
    hboxLayout2->setMargin(0);
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    label_2 = new QLabel(widget_4);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setMinimumSize(QSize(140, 0));
    label_2->setMaximumSize(QSize(140, 16777215));

    hboxLayout2->addWidget(label_2);

    txtWineServerBin = new QLineEdit(widget_4);
    txtWineServerBin->setObjectName(QString::fromUtf8("txtWineServerBin"));

    hboxLayout2->addWidget(txtWineServerBin);

    cmdGetWineServerBin = new QPushButton(widget_4);
    cmdGetWineServerBin->setObjectName(QString::fromUtf8("cmdGetWineServerBin"));
    cmdGetWineServerBin->setIconSize(QSize(16, 16));

    hboxLayout2->addWidget(cmdGetWineServerBin);


    vboxLayout3->addWidget(widget_4);

    widget_10 = new QWidget(groupBox);
    widget_10->setObjectName(QString::fromUtf8("widget_10"));
    hboxLayout3 = new QHBoxLayout(widget_10);
    hboxLayout3->setMargin(0);
    hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
    label_3 = new QLabel(widget_10);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    label_3->setMinimumSize(QSize(140, 0));
    label_3->setMaximumSize(QSize(140, 16777215));

    hboxLayout3->addWidget(label_3);

    txtWineLoaderBin = new QLineEdit(widget_10);
    txtWineLoaderBin->setObjectName(QString::fromUtf8("txtWineLoaderBin"));

    hboxLayout3->addWidget(txtWineLoaderBin);

    cmdGetWineLoaderBin = new QPushButton(widget_10);
    cmdGetWineLoaderBin->setObjectName(QString::fromUtf8("cmdGetWineLoaderBin"));
    cmdGetWineLoaderBin->setIconSize(QSize(16, 16));

    hboxLayout3->addWidget(cmdGetWineLoaderBin);


    vboxLayout3->addWidget(widget_10);

    widget_11 = new QWidget(groupBox);
    widget_11->setObjectName(QString::fromUtf8("widget_11"));
    hboxLayout4 = new QHBoxLayout(widget_11);
    hboxLayout4->setMargin(0);
    hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
    label_11 = new QLabel(widget_11);
    label_11->setObjectName(QString::fromUtf8("label_11"));
    label_11->setMinimumSize(QSize(140, 0));
    label_11->setMaximumSize(QSize(140, 16777215));

    hboxLayout4->addWidget(label_11);

    txtWineLibs = new QLineEdit(widget_11);
    txtWineLibs->setObjectName(QString::fromUtf8("txtWineLibs"));

    hboxLayout4->addWidget(txtWineLibs);

    cmdGetWineLibs = new QPushButton(widget_11);
    cmdGetWineLibs->setObjectName(QString::fromUtf8("cmdGetWineLibs"));
    cmdGetWineLibs->setIconSize(QSize(16, 16));

    hboxLayout4->addWidget(cmdGetWineLibs);


    vboxLayout3->addWidget(widget_11);


    vboxLayout2->addWidget(groupBox);

    groupBox_4 = new QGroupBox(tab);
    groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
    vboxLayout4 = new QVBoxLayout(groupBox_4);
    vboxLayout4->setObjectName(QString::fromUtf8("vboxLayout4"));
    chShowTrarey = new QCheckBox(groupBox_4);
    chShowTrarey->setObjectName(QString::fromUtf8("chShowTrarey"));
    chShowTrarey->setChecked(true);

    vboxLayout4->addWidget(chShowTrarey);


    vboxLayout2->addWidget(groupBox_4);

    spacerItem1 = new QSpacerItem(680, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout2->addItem(spacerItem1);

    twbGeneral->addTab(tab, QString());
    tab_4 = new QWidget();
    tab_4->setObjectName(QString::fromUtf8("tab_4"));
    vboxLayout5 = new QVBoxLayout(tab_4);
    vboxLayout5->setObjectName(QString::fromUtf8("vboxLayout5"));
    groupBox_7 = new QGroupBox(tab_4);
    groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
    vboxLayout6 = new QVBoxLayout(groupBox_7);
    vboxLayout6->setObjectName(QString::fromUtf8("vboxLayout6"));
    widget_14 = new QWidget(groupBox_7);
    widget_14->setObjectName(QString::fromUtf8("widget_14"));
    hboxLayout5 = new QHBoxLayout(widget_14);
    hboxLayout5->setMargin(0);
    hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
    label_13 = new QLabel(widget_14);
    label_13->setObjectName(QString::fromUtf8("label_13"));
    label_13->setMinimumSize(QSize(140, 0));
    label_13->setMaximumSize(QSize(140, 16777215));

    hboxLayout5->addWidget(label_13);

    txtTarBin = new QLineEdit(widget_14);
    txtTarBin->setObjectName(QString::fromUtf8("txtTarBin"));

    hboxLayout5->addWidget(txtTarBin);

    cmdGetTarBin = new QPushButton(widget_14);
    cmdGetTarBin->setObjectName(QString::fromUtf8("cmdGetTarBin"));
    cmdGetTarBin->setIconSize(QSize(16, 16));

    hboxLayout5->addWidget(cmdGetTarBin);


    vboxLayout6->addWidget(widget_14);

    widget_17 = new QWidget(groupBox_7);
    widget_17->setObjectName(QString::fromUtf8("widget_17"));
    hboxLayout6 = new QHBoxLayout(widget_17);
    hboxLayout6->setMargin(0);
    hboxLayout6->setObjectName(QString::fromUtf8("hboxLayout6"));
    label_16 = new QLabel(widget_17);
    label_16->setObjectName(QString::fromUtf8("label_16"));
    label_16->setMinimumSize(QSize(140, 0));
    label_16->setMaximumSize(QSize(140, 16777215));

    hboxLayout6->addWidget(label_16);

    txtMountBin = new QLineEdit(widget_17);
    txtMountBin->setObjectName(QString::fromUtf8("txtMountBin"));

    hboxLayout6->addWidget(txtMountBin);

    cmdGetMountBin = new QPushButton(widget_17);
    cmdGetMountBin->setObjectName(QString::fromUtf8("cmdGetMountBin"));
    cmdGetMountBin->setIconSize(QSize(16, 16));

    hboxLayout6->addWidget(cmdGetMountBin);


    vboxLayout6->addWidget(widget_17);

    widget_18 = new QWidget(groupBox_7);
    widget_18->setObjectName(QString::fromUtf8("widget_18"));
    hboxLayout7 = new QHBoxLayout(widget_18);
    hboxLayout7->setMargin(0);
    hboxLayout7->setObjectName(QString::fromUtf8("hboxLayout7"));
    label_17 = new QLabel(widget_18);
    label_17->setObjectName(QString::fromUtf8("label_17"));
    label_17->setMinimumSize(QSize(140, 0));
    label_17->setMaximumSize(QSize(140, 16777215));

    hboxLayout7->addWidget(label_17);

    txtUmountBin = new QLineEdit(widget_18);
    txtUmountBin->setObjectName(QString::fromUtf8("txtUmountBin"));

    hboxLayout7->addWidget(txtUmountBin);

    cmdGetUmountBin = new QPushButton(widget_18);
    cmdGetUmountBin->setObjectName(QString::fromUtf8("cmdGetUmountBin"));
    cmdGetUmountBin->setIconSize(QSize(16, 16));

    hboxLayout7->addWidget(cmdGetUmountBin);


    vboxLayout6->addWidget(widget_18);

    widget_19 = new QWidget(groupBox_7);
    widget_19->setObjectName(QString::fromUtf8("widget_19"));
    hboxLayout8 = new QHBoxLayout(widget_19);
    hboxLayout8->setMargin(0);
    hboxLayout8->setObjectName(QString::fromUtf8("hboxLayout8"));
    label_19 = new QLabel(widget_19);
    label_19->setObjectName(QString::fromUtf8("label_19"));
    label_19->setMinimumSize(QSize(140, 0));
    label_19->setMaximumSize(QSize(140, 16777215));

    hboxLayout8->addWidget(label_19);

    txtSudoBin = new QLineEdit(widget_19);
    txtSudoBin->setObjectName(QString::fromUtf8("txtSudoBin"));

    hboxLayout8->addWidget(txtSudoBin);

    cmdGetSudoBin = new QPushButton(widget_19);
    cmdGetSudoBin->setObjectName(QString::fromUtf8("cmdGetSudoBin"));
    cmdGetSudoBin->setIconSize(QSize(16, 16));

    hboxLayout8->addWidget(cmdGetSudoBin);


    vboxLayout6->addWidget(widget_19);

    widget_24 = new QWidget(groupBox_7);
    widget_24->setObjectName(QString::fromUtf8("widget_24"));
    hboxLayout9 = new QHBoxLayout(widget_24);
    hboxLayout9->setMargin(0);
    hboxLayout9->setObjectName(QString::fromUtf8("hboxLayout9"));
    label_23 = new QLabel(widget_24);
    label_23->setObjectName(QString::fromUtf8("label_23"));
    label_23->setMinimumSize(QSize(140, 0));
    label_23->setMaximumSize(QSize(140, 16777215));

    hboxLayout9->addWidget(label_23);

    txtNiceBin = new QLineEdit(widget_24);
    txtNiceBin->setObjectName(QString::fromUtf8("txtNiceBin"));

    hboxLayout9->addWidget(txtNiceBin);

    cmdGetNiceBin = new QPushButton(widget_24);
    cmdGetNiceBin->setObjectName(QString::fromUtf8("cmdGetNiceBin"));
    cmdGetNiceBin->setIconSize(QSize(16, 16));

    hboxLayout9->addWidget(cmdGetNiceBin);


    vboxLayout6->addWidget(widget_24);

    widget_25 = new QWidget(groupBox_7);
    widget_25->setObjectName(QString::fromUtf8("widget_25"));
    hboxLayout10 = new QHBoxLayout(widget_25);
    hboxLayout10->setMargin(0);
    hboxLayout10->setObjectName(QString::fromUtf8("hboxLayout10"));
    label_24 = new QLabel(widget_25);
    label_24->setObjectName(QString::fromUtf8("label_24"));
    label_24->setMinimumSize(QSize(140, 0));
    label_24->setMaximumSize(QSize(140, 16777215));

    hboxLayout10->addWidget(label_24);

    txtReniceBin = new QLineEdit(widget_25);
    txtReniceBin->setObjectName(QString::fromUtf8("txtReniceBin"));

    hboxLayout10->addWidget(txtReniceBin);

    cmdGetReniceBin = new QPushButton(widget_25);
    cmdGetReniceBin->setObjectName(QString::fromUtf8("cmdGetReniceBin"));
    cmdGetReniceBin->setIconSize(QSize(16, 16));

    hboxLayout10->addWidget(cmdGetReniceBin);


    vboxLayout6->addWidget(widget_25);

    widget_28 = new QWidget(groupBox_7);
    widget_28->setObjectName(QString::fromUtf8("widget_28"));
    hboxLayout11 = new QHBoxLayout(widget_28);
    hboxLayout11->setMargin(0);
    hboxLayout11->setObjectName(QString::fromUtf8("hboxLayout11"));
    label_27 = new QLabel(widget_28);
    label_27->setObjectName(QString::fromUtf8("label_27"));
    label_27->setMinimumSize(QSize(140, 0));
    label_27->setMaximumSize(QSize(140, 16777215));

    hboxLayout11->addWidget(label_27);

    txtShBin = new QLineEdit(widget_28);
    txtShBin->setObjectName(QString::fromUtf8("txtShBin"));

    hboxLayout11->addWidget(txtShBin);

    cmdGetShBin = new QPushButton(widget_28);
    cmdGetShBin->setObjectName(QString::fromUtf8("cmdGetShBin"));
    cmdGetShBin->setIconSize(QSize(16, 16));

    hboxLayout11->addWidget(cmdGetShBin);


    vboxLayout6->addWidget(widget_28);


    vboxLayout5->addWidget(groupBox_7);

    spacerItem2 = new QSpacerItem(680, 21, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout5->addItem(spacerItem2);

    twbGeneral->addTab(tab_4, QString());
    tab_3 = new QWidget();
    tab_3->setObjectName(QString::fromUtf8("tab_3"));
    vboxLayout7 = new QVBoxLayout(tab_3);
    vboxLayout7->setObjectName(QString::fromUtf8("vboxLayout7"));
    groupBox_2 = new QGroupBox(tab_3);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    vboxLayout8 = new QVBoxLayout(groupBox_2);
    vboxLayout8->setObjectName(QString::fromUtf8("vboxLayout8"));
    widget_12 = new QWidget(groupBox_2);
    widget_12->setObjectName(QString::fromUtf8("widget_12"));
    hboxLayout12 = new QHBoxLayout(widget_12);
    hboxLayout12->setMargin(0);
    hboxLayout12->setObjectName(QString::fromUtf8("hboxLayout12"));
    label_6 = new QLabel(widget_12);
    label_6->setObjectName(QString::fromUtf8("label_6"));
    label_6->setMinimumSize(QSize(140, 0));
    label_6->setMaximumSize(QSize(140, 16777215));

    hboxLayout12->addWidget(label_6);

    txtConsoleBin = new QLineEdit(widget_12);
    txtConsoleBin->setObjectName(QString::fromUtf8("txtConsoleBin"));

    hboxLayout12->addWidget(txtConsoleBin);

    cmdGetConsoleBin = new QPushButton(widget_12);
    cmdGetConsoleBin->setObjectName(QString::fromUtf8("cmdGetConsoleBin"));
    cmdGetConsoleBin->setIconSize(QSize(16, 16));

    hboxLayout12->addWidget(cmdGetConsoleBin);


    vboxLayout8->addWidget(widget_12);

    widget_13 = new QWidget(groupBox_2);
    widget_13->setObjectName(QString::fromUtf8("widget_13"));
    hboxLayout13 = new QHBoxLayout(widget_13);
    hboxLayout13->setMargin(0);
    hboxLayout13->setObjectName(QString::fromUtf8("hboxLayout13"));
    label_12 = new QLabel(widget_13);
    label_12->setObjectName(QString::fromUtf8("label_12"));
    label_12->setMinimumSize(QSize(140, 0));
    label_12->setMaximumSize(QSize(140, 16777215));

    hboxLayout13->addWidget(label_12);

    txtConsoleArgs = new QLineEdit(widget_13);
    txtConsoleArgs->setObjectName(QString::fromUtf8("txtConsoleArgs"));

    hboxLayout13->addWidget(txtConsoleArgs);


    vboxLayout8->addWidget(widget_13);


    vboxLayout7->addWidget(groupBox_2);

    groupBox_5 = new QGroupBox(tab_3);
    groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
    vboxLayout9 = new QVBoxLayout(groupBox_5);
    vboxLayout9->setObjectName(QString::fromUtf8("vboxLayout9"));
    widget_26 = new QWidget(groupBox_5);
    widget_26->setObjectName(QString::fromUtf8("widget_26"));
    hboxLayout14 = new QHBoxLayout(widget_26);
    hboxLayout14->setMargin(0);
    hboxLayout14->setObjectName(QString::fromUtf8("hboxLayout14"));
    label_25 = new QLabel(widget_26);
    label_25->setObjectName(QString::fromUtf8("label_25"));
    label_25->setMinimumSize(QSize(140, 0));
    label_25->setMaximumSize(QSize(140, 16777215));

    hboxLayout14->addWidget(label_25);

    txtWrestoolBin = new QLineEdit(widget_26);
    txtWrestoolBin->setObjectName(QString::fromUtf8("txtWrestoolBin"));

    hboxLayout14->addWidget(txtWrestoolBin);

    cmdGetWrestoolBin = new QPushButton(widget_26);
    cmdGetWrestoolBin->setObjectName(QString::fromUtf8("cmdGetWrestoolBin"));
    cmdGetWrestoolBin->setIconSize(QSize(16, 16));

    hboxLayout14->addWidget(cmdGetWrestoolBin);


    vboxLayout9->addWidget(widget_26);

    widget_27 = new QWidget(groupBox_5);
    widget_27->setObjectName(QString::fromUtf8("widget_27"));
    hboxLayout15 = new QHBoxLayout(widget_27);
    hboxLayout15->setMargin(0);
    hboxLayout15->setObjectName(QString::fromUtf8("hboxLayout15"));
    label_26 = new QLabel(widget_27);
    label_26->setObjectName(QString::fromUtf8("label_26"));
    label_26->setMinimumSize(QSize(140, 0));
    label_26->setMaximumSize(QSize(140, 16777215));

    hboxLayout15->addWidget(label_26);

    txtIcotoolBin = new QLineEdit(widget_27);
    txtIcotoolBin->setObjectName(QString::fromUtf8("txtIcotoolBin"));

    hboxLayout15->addWidget(txtIcotoolBin);

    cmdGetIcotoolBin = new QPushButton(widget_27);
    cmdGetIcotoolBin->setObjectName(QString::fromUtf8("cmdGetIcotoolBin"));
    cmdGetIcotoolBin->setIconSize(QSize(16, 16));

    hboxLayout15->addWidget(cmdGetIcotoolBin);


    vboxLayout9->addWidget(widget_27);


    vboxLayout7->addWidget(groupBox_5);

    spacerItem3 = new QSpacerItem(680, 21, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout7->addItem(spacerItem3);

    twbGeneral->addTab(tab_3, QString());
    tab_2 = new QWidget();
    tab_2->setObjectName(QString::fromUtf8("tab_2"));
    vboxLayout10 = new QVBoxLayout(tab_2);
    vboxLayout10->setObjectName(QString::fromUtf8("vboxLayout10"));
    groupBox_9 = new QGroupBox(tab_2);
    groupBox_9->setObjectName(QString::fromUtf8("groupBox_9"));
    vboxLayout11 = new QVBoxLayout(groupBox_9);
    vboxLayout11->setObjectName(QString::fromUtf8("vboxLayout11"));
    widget_30 = new QWidget(groupBox_9);
    widget_30->setObjectName(QString::fromUtf8("widget_30"));
    hboxLayout16 = new QHBoxLayout(widget_30);
    hboxLayout16->setMargin(0);
    hboxLayout16->setObjectName(QString::fromUtf8("hboxLayout16"));
    label_29 = new QLabel(widget_30);
    label_29->setObjectName(QString::fromUtf8("label_29"));
    label_29->setMinimumSize(QSize(140, 0));
    label_29->setMaximumSize(QSize(140, 16777215));

    hboxLayout16->addWidget(label_29);

    comboLangs = new QComboBox(widget_30);
    comboLangs->setObjectName(QString::fromUtf8("comboLangs"));

    hboxLayout16->addWidget(comboLangs);


    vboxLayout11->addWidget(widget_30);


    vboxLayout10->addWidget(groupBox_9);

    groupBox_3 = new QGroupBox(tab_2);
    groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
    vboxLayout12 = new QVBoxLayout(groupBox_3);
    vboxLayout12->setObjectName(QString::fromUtf8("vboxLayout12"));
    listThemesView = new QListWidget(groupBox_3);
    listThemesView->setObjectName(QString::fromUtf8("listThemesView"));
    listThemesView->setIconSize(QSize(32, 32));

    vboxLayout12->addWidget(listThemesView);


    vboxLayout10->addWidget(groupBox_3);

    twbGeneral->addTab(tab_2, QString());
    tab_5 = new QWidget();
    tab_5->setObjectName(QString::fromUtf8("tab_5"));
    vboxLayout13 = new QVBoxLayout(tab_5);
    vboxLayout13->setObjectName(QString::fromUtf8("vboxLayout13"));
    groupBox_8 = new QGroupBox(tab_5);
    groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
    vboxLayout14 = new QVBoxLayout(groupBox_8);
    vboxLayout14->setObjectName(QString::fromUtf8("vboxLayout14"));
    widget_29 = new QWidget(groupBox_8);
    widget_29->setObjectName(QString::fromUtf8("widget_29"));
    hboxLayout17 = new QHBoxLayout(widget_29);
    hboxLayout17->setMargin(0);
    hboxLayout17->setObjectName(QString::fromUtf8("hboxLayout17"));
    label_28 = new QLabel(widget_29);
    label_28->setObjectName(QString::fromUtf8("label_28"));
    label_28->setMinimumSize(QSize(140, 0));
    label_28->setMaximumSize(QSize(140, 16777215));

    hboxLayout17->addWidget(label_28);

    comboProxyType = new QComboBox(widget_29);
    comboProxyType->setObjectName(QString::fromUtf8("comboProxyType"));

    hboxLayout17->addWidget(comboProxyType);


    vboxLayout14->addWidget(widget_29);

    widget_20 = new QWidget(groupBox_8);
    widget_20->setObjectName(QString::fromUtf8("widget_20"));
    hboxLayout18 = new QHBoxLayout(widget_20);
    hboxLayout18->setMargin(0);
    hboxLayout18->setObjectName(QString::fromUtf8("hboxLayout18"));
    label_18 = new QLabel(widget_20);
    label_18->setObjectName(QString::fromUtf8("label_18"));
    label_18->setMinimumSize(QSize(140, 0));
    label_18->setMaximumSize(QSize(140, 16777215));

    hboxLayout18->addWidget(label_18);

    txtProxyHost = new QLineEdit(widget_20);
    txtProxyHost->setObjectName(QString::fromUtf8("txtProxyHost"));
    txtProxyHost->setEnabled(false);

    hboxLayout18->addWidget(txtProxyHost);


    vboxLayout14->addWidget(widget_20);

    widget_21 = new QWidget(groupBox_8);
    widget_21->setObjectName(QString::fromUtf8("widget_21"));
    hboxLayout19 = new QHBoxLayout(widget_21);
    hboxLayout19->setMargin(0);
    hboxLayout19->setObjectName(QString::fromUtf8("hboxLayout19"));
    label_20 = new QLabel(widget_21);
    label_20->setObjectName(QString::fromUtf8("label_20"));
    label_20->setMinimumSize(QSize(140, 0));
    label_20->setMaximumSize(QSize(140, 16777215));

    hboxLayout19->addWidget(label_20);

    txtProxyPort = new QLineEdit(widget_21);
    txtProxyPort->setObjectName(QString::fromUtf8("txtProxyPort"));
    txtProxyPort->setEnabled(false);

    hboxLayout19->addWidget(txtProxyPort);


    vboxLayout14->addWidget(widget_21);

    widget_22 = new QWidget(groupBox_8);
    widget_22->setObjectName(QString::fromUtf8("widget_22"));
    hboxLayout20 = new QHBoxLayout(widget_22);
    hboxLayout20->setMargin(0);
    hboxLayout20->setObjectName(QString::fromUtf8("hboxLayout20"));
    label_21 = new QLabel(widget_22);
    label_21->setObjectName(QString::fromUtf8("label_21"));
    label_21->setMinimumSize(QSize(140, 0));
    label_21->setMaximumSize(QSize(140, 16777215));

    hboxLayout20->addWidget(label_21);

    txtProxyUser = new QLineEdit(widget_22);
    txtProxyUser->setObjectName(QString::fromUtf8("txtProxyUser"));
    txtProxyUser->setEnabled(false);

    hboxLayout20->addWidget(txtProxyUser);


    vboxLayout14->addWidget(widget_22);

    widget_23 = new QWidget(groupBox_8);
    widget_23->setObjectName(QString::fromUtf8("widget_23"));
    hboxLayout21 = new QHBoxLayout(widget_23);
    hboxLayout21->setMargin(0);
    hboxLayout21->setObjectName(QString::fromUtf8("hboxLayout21"));
    label_22 = new QLabel(widget_23);
    label_22->setObjectName(QString::fromUtf8("label_22"));
    label_22->setMinimumSize(QSize(140, 0));
    label_22->setMaximumSize(QSize(140, 16777215));

    hboxLayout21->addWidget(label_22);

    txtProxyPass = new QLineEdit(widget_23);
    txtProxyPass->setObjectName(QString::fromUtf8("txtProxyPass"));
    txtProxyPass->setEnabled(false);

    hboxLayout21->addWidget(txtProxyPass);


    vboxLayout14->addWidget(widget_23);


    vboxLayout13->addWidget(groupBox_8);

    spacerItem4 = new QSpacerItem(680, 31, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout13->addItem(spacerItem4);

    twbGeneral->addTab(tab_5, QString());

    vboxLayout1->addWidget(twbGeneral);

    hboxLayout22 = new QHBoxLayout();
    hboxLayout22->setObjectName(QString::fromUtf8("hboxLayout22"));
    cmdHelp = new QPushButton(widget);
    cmdHelp->setObjectName(QString::fromUtf8("cmdHelp"));

    hboxLayout22->addWidget(cmdHelp);

    spacerItem5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout22->addItem(spacerItem5);

    cmdCancel = new QPushButton(widget);
    cmdCancel->setObjectName(QString::fromUtf8("cmdCancel"));

    hboxLayout22->addWidget(cmdCancel);

    cmdOk = new QPushButton(widget);
    cmdOk->setObjectName(QString::fromUtf8("cmdOk"));

    hboxLayout22->addWidget(cmdOk);


    vboxLayout1->addLayout(hboxLayout22);


    vboxLayout->addWidget(widget);


    retranslateUi(AppSettings);

    twbGeneral->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(AppSettings);
    } // setupUi

    void retranslateUi(QDialog *AppSettings)
    {
    AppSettings->setWindowTitle(QApplication::translate("AppSettings", "%1 settings", 0, QApplication::UnicodeUTF8));
    lblCaption->setText(QApplication::translate("AppSettings", "%1 settings", 0, QApplication::UnicodeUTF8));
    lblLogo->setText(QString());
    groupBox->setTitle(QApplication::translate("AppSettings", "Wine settings", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("AppSettings", "Bin:", 0, QApplication::UnicodeUTF8));
    cmdGetWineBin->setText(QString());
    label_2->setText(QApplication::translate("AppSettings", "Server:", 0, QApplication::UnicodeUTF8));
    cmdGetWineServerBin->setText(QString());
    label_3->setText(QApplication::translate("AppSettings", "Loader:", 0, QApplication::UnicodeUTF8));
    cmdGetWineLoaderBin->setText(QString());
    label_11->setText(QApplication::translate("AppSettings", "Libs:", 0, QApplication::UnicodeUTF8));
    cmdGetWineLibs->setText(QString());
    groupBox_4->setTitle(QApplication::translate("AppSettings", "Trarey Icon", 0, QApplication::UnicodeUTF8));
    chShowTrarey->setText(QApplication::translate("AppSettings", "Use trarey icon", 0, QApplication::UnicodeUTF8));
    twbGeneral->setTabText(twbGeneral->indexOf(tab), QApplication::translate("AppSettings", "General", 0, QApplication::UnicodeUTF8));
    groupBox_7->setTitle(QApplication::translate("AppSettings", "System utils", 0, QApplication::UnicodeUTF8));
    label_13->setText(QApplication::translate("AppSettings", "tar:", 0, QApplication::UnicodeUTF8));
    cmdGetTarBin->setText(QString());
    label_16->setText(QApplication::translate("AppSettings", "mount:", 0, QApplication::UnicodeUTF8));
    cmdGetMountBin->setText(QString());
    label_17->setText(QApplication::translate("AppSettings", "umount:", 0, QApplication::UnicodeUTF8));
    cmdGetUmountBin->setText(QString());
    label_19->setText(QApplication::translate("AppSettings", "sudo:", 0, QApplication::UnicodeUTF8));
    cmdGetSudoBin->setText(QString());
    label_23->setText(QApplication::translate("AppSettings", "nice:", 0, QApplication::UnicodeUTF8));
    cmdGetNiceBin->setText(QString());
    label_24->setText(QApplication::translate("AppSettings", "renice:", 0, QApplication::UnicodeUTF8));
    cmdGetReniceBin->setText(QString());
    label_27->setText(QApplication::translate("AppSettings", "sh:", 0, QApplication::UnicodeUTF8));
    cmdGetShBin->setText(QString());
    twbGeneral->setTabText(twbGeneral->indexOf(tab_4), QApplication::translate("AppSettings", "System", 0, QApplication::UnicodeUTF8));
    groupBox_2->setTitle(QApplication::translate("AppSettings", "Console", 0, QApplication::UnicodeUTF8));
    label_6->setText(QApplication::translate("AppSettings", "Bin:", 0, QApplication::UnicodeUTF8));
    cmdGetConsoleBin->setText(QString());
    label_12->setText(QApplication::translate("AppSettings", "Args:", 0, QApplication::UnicodeUTF8));
    groupBox_5->setTitle(QApplication::translate("AppSettings", "Icotool", 0, QApplication::UnicodeUTF8));
    label_25->setText(QApplication::translate("AppSettings", "wrestool:", 0, QApplication::UnicodeUTF8));
    cmdGetWrestoolBin->setText(QString());
    label_26->setText(QApplication::translate("AppSettings", "icotool:", 0, QApplication::UnicodeUTF8));
    cmdGetIcotoolBin->setText(QString());
    twbGeneral->setTabText(twbGeneral->indexOf(tab_3), QApplication::translate("AppSettings", "Utils", 0, QApplication::UnicodeUTF8));
    groupBox_9->setTitle(QApplication::translate("AppSettings", "Language", 0, QApplication::UnicodeUTF8));
    label_29->setText(QApplication::translate("AppSettings", "Languale:", 0, QApplication::UnicodeUTF8));
    comboLangs->clear();
    comboLangs->insertItems(0, QStringList()
     << QApplication::translate("AppSettings", "System Default", 0, QApplication::UnicodeUTF8)
    );
    groupBox_3->setTitle(QApplication::translate("AppSettings", "Themes", 0, QApplication::UnicodeUTF8));
    twbGeneral->setTabText(twbGeneral->indexOf(tab_2), QApplication::translate("AppSettings", "Customize", 0, QApplication::UnicodeUTF8));
    groupBox_8->setTitle(QApplication::translate("AppSettings", "Proxy settings", 0, QApplication::UnicodeUTF8));
    label_28->setText(QApplication::translate("AppSettings", "Password:", 0, QApplication::UnicodeUTF8));
    comboProxyType->clear();
    comboProxyType->insertItems(0, QStringList()
     << QApplication::translate("AppSettings", "No Proxy", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("AppSettings", "HTTP", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("AppSettings", "Socks v5", 0, QApplication::UnicodeUTF8)
    );
    label_18->setText(QApplication::translate("AppSettings", "Host:", 0, QApplication::UnicodeUTF8));
    txtProxyHost->setInputMask(QString());
    label_20->setText(QApplication::translate("AppSettings", "Port:", 0, QApplication::UnicodeUTF8));
    txtProxyPort->setInputMask(QString());
    label_21->setText(QApplication::translate("AppSettings", "Username:", 0, QApplication::UnicodeUTF8));
    label_22->setText(QApplication::translate("AppSettings", "Password:", 0, QApplication::UnicodeUTF8));
    twbGeneral->setTabText(twbGeneral->indexOf(tab_5), QApplication::translate("AppSettings", "Network", 0, QApplication::UnicodeUTF8));
    cmdHelp->setText(QApplication::translate("AppSettings", "Help", 0, QApplication::UnicodeUTF8));
    cmdCancel->setText(QApplication::translate("AppSettings", "Cancel", 0, QApplication::UnicodeUTF8));
    cmdOk->setText(QApplication::translate("AppSettings", "Ok", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(AppSettings);
    } // retranslateUi

};

namespace Ui {
    class AppSettings: public Ui_AppSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_APPSETTINGS_H
