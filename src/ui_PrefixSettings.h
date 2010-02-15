/********************************************************************************
** Form generated from reading UI file 'PrefixSettings.ui'
**
** Created: Mon Feb 15 09:49:50 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREFIXSETTINGS_H
#define UI_PREFIXSETTINGS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PrefixSettings
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
    QWidget *tab_4;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_5;
    QVBoxLayout *vboxLayout2;
    QWidget *widget_15;
    QHBoxLayout *hboxLayout1;
    QLabel *label_14;
    QLineEdit *txtPrefixName;
    QWidget *widget_16;
    QHBoxLayout *hboxLayout2;
    QLabel *label_15;
    QLineEdit *txtPrefixPath;
    QPushButton *cmdGetPrefixPath;
    QGroupBox *groupBox_7;
    QVBoxLayout *vboxLayout3;
    QWidget *widget_17;
    QHBoxLayout *hboxLayout3;
    QLabel *label_16;
    QComboBox *comboDeviceList;
    QWidget *widget_18;
    QHBoxLayout *hboxLayout4;
    QLabel *label_17;
    QLineEdit *txtMountPoint;
    QPushButton *cmdGetMountPoint;
    QSpacerItem *verticalSpacer;
    QWidget *tab;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QVBoxLayout *vboxLayout4;
    QLabel *label_4;
    QWidget *widget_2;
    QHBoxLayout *hboxLayout5;
    QLabel *label;
    QLineEdit *txtWineBin;
    QPushButton *cmdGetWineBin;
    QWidget *widget_4;
    QHBoxLayout *hboxLayout6;
    QLabel *label_2;
    QLineEdit *txtWineServerBin;
    QPushButton *cmdGetWineServerBin;
    QWidget *widget_10;
    QHBoxLayout *hboxLayout7;
    QLabel *label_3;
    QLineEdit *txtWineLoaderBin;
    QPushButton *cmdGetWineLoaderBin;
    QWidget *widget_11;
    QHBoxLayout *hboxLayout8;
    QLabel *label_11;
    QLineEdit *txtWineLibs;
    QPushButton *cmdGetWineLibs;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *hboxLayout9;
    QPushButton *cmdHelp;
    QSpacerItem *spacerItem1;
    QPushButton *cmdCancel;
    QPushButton *cmdOk;

    void setupUi(QDialog *PrefixSettings)
    {
        if (PrefixSettings->objectName().isEmpty())
            PrefixSettings->setObjectName(QString::fromUtf8("PrefixSettings"));
        PrefixSettings->resize(650, 400);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/data/q4wine.png"), QSize(), QIcon::Normal, QIcon::Off);
        PrefixSettings->setWindowIcon(icon);
        vboxLayout = new QVBoxLayout(PrefixSettings);
        vboxLayout->setSpacing(0);
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        widget_7 = new QWidget(PrefixSettings);
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
        lblLogo->setPixmap(QPixmap(QString::fromUtf8(":/data/exec.png")));

        hboxLayout->addWidget(lblLogo);


        vboxLayout->addWidget(widget_7);

        line = new QFrame(PrefixSettings);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        vboxLayout->addWidget(line);

        widget = new QWidget(PrefixSettings);
        widget->setObjectName(QString::fromUtf8("widget"));
        vboxLayout1 = new QVBoxLayout(widget);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        twbGeneral = new QTabWidget(widget);
        twbGeneral->setObjectName(QString::fromUtf8("twbGeneral"));
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        verticalLayout = new QVBoxLayout(tab_4);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox_5 = new QGroupBox(tab_4);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        vboxLayout2 = new QVBoxLayout(groupBox_5);
        vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
        widget_15 = new QWidget(groupBox_5);
        widget_15->setObjectName(QString::fromUtf8("widget_15"));
        hboxLayout1 = new QHBoxLayout(widget_15);
        hboxLayout1->setContentsMargins(0, 0, 0, 0);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        label_14 = new QLabel(widget_15);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setMinimumSize(QSize(140, 0));
        label_14->setMaximumSize(QSize(140, 16777215));

        hboxLayout1->addWidget(label_14);

        txtPrefixName = new QLineEdit(widget_15);
        txtPrefixName->setObjectName(QString::fromUtf8("txtPrefixName"));

        hboxLayout1->addWidget(txtPrefixName);


        vboxLayout2->addWidget(widget_15);

        widget_16 = new QWidget(groupBox_5);
        widget_16->setObjectName(QString::fromUtf8("widget_16"));
        hboxLayout2 = new QHBoxLayout(widget_16);
        hboxLayout2->setContentsMargins(0, 0, 0, 0);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        label_15 = new QLabel(widget_16);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setMinimumSize(QSize(140, 0));
        label_15->setMaximumSize(QSize(140, 16777215));

        hboxLayout2->addWidget(label_15);

        txtPrefixPath = new QLineEdit(widget_16);
        txtPrefixPath->setObjectName(QString::fromUtf8("txtPrefixPath"));

        hboxLayout2->addWidget(txtPrefixPath);

        cmdGetPrefixPath = new QPushButton(widget_16);
        cmdGetPrefixPath->setObjectName(QString::fromUtf8("cmdGetPrefixPath"));
        cmdGetPrefixPath->setIconSize(QSize(16, 16));

        hboxLayout2->addWidget(cmdGetPrefixPath);


        vboxLayout2->addWidget(widget_16);


        verticalLayout->addWidget(groupBox_5);

        groupBox_7 = new QGroupBox(tab_4);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        vboxLayout3 = new QVBoxLayout(groupBox_7);
        vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
        widget_17 = new QWidget(groupBox_7);
        widget_17->setObjectName(QString::fromUtf8("widget_17"));
        hboxLayout3 = new QHBoxLayout(widget_17);
        hboxLayout3->setContentsMargins(0, 0, 0, 0);
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        label_16 = new QLabel(widget_17);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setMinimumSize(QSize(140, 0));
        label_16->setMaximumSize(QSize(140, 16777215));

        hboxLayout3->addWidget(label_16);

        comboDeviceList = new QComboBox(widget_17);
        comboDeviceList->setObjectName(QString::fromUtf8("comboDeviceList"));

        hboxLayout3->addWidget(comboDeviceList);


        vboxLayout3->addWidget(widget_17);

        widget_18 = new QWidget(groupBox_7);
        widget_18->setObjectName(QString::fromUtf8("widget_18"));
        hboxLayout4 = new QHBoxLayout(widget_18);
        hboxLayout4->setContentsMargins(0, 0, 0, 0);
        hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
        label_17 = new QLabel(widget_18);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setMinimumSize(QSize(140, 0));
        label_17->setMaximumSize(QSize(140, 16777215));

        hboxLayout4->addWidget(label_17);

        txtMountPoint = new QLineEdit(widget_18);
        txtMountPoint->setObjectName(QString::fromUtf8("txtMountPoint"));

        hboxLayout4->addWidget(txtMountPoint);

        cmdGetMountPoint = new QPushButton(widget_18);
        cmdGetMountPoint->setObjectName(QString::fromUtf8("cmdGetMountPoint"));
        cmdGetMountPoint->setIconSize(QSize(16, 16));

        hboxLayout4->addWidget(cmdGetMountPoint);


        vboxLayout3->addWidget(widget_18);


        verticalLayout->addWidget(groupBox_7);

        verticalSpacer = new QSpacerItem(20, 84, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        twbGeneral->addTab(tab_4, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_2 = new QVBoxLayout(tab);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox = new QGroupBox(tab);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        vboxLayout4 = new QVBoxLayout(groupBox);
        vboxLayout4->setObjectName(QString::fromUtf8("vboxLayout4"));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);
        label_4->setScaledContents(false);
        label_4->setWordWrap(true);

        vboxLayout4->addWidget(label_4);

        widget_2 = new QWidget(groupBox);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        hboxLayout5 = new QHBoxLayout(widget_2);
        hboxLayout5->setContentsMargins(0, 0, 0, 0);
        hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
        label = new QLabel(widget_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(140, 0));
        label->setMaximumSize(QSize(140, 16777215));

        hboxLayout5->addWidget(label);

        txtWineBin = new QLineEdit(widget_2);
        txtWineBin->setObjectName(QString::fromUtf8("txtWineBin"));

        hboxLayout5->addWidget(txtWineBin);

        cmdGetWineBin = new QPushButton(widget_2);
        cmdGetWineBin->setObjectName(QString::fromUtf8("cmdGetWineBin"));
        cmdGetWineBin->setIconSize(QSize(16, 16));

        hboxLayout5->addWidget(cmdGetWineBin);


        vboxLayout4->addWidget(widget_2);

        widget_4 = new QWidget(groupBox);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        hboxLayout6 = new QHBoxLayout(widget_4);
        hboxLayout6->setContentsMargins(0, 0, 0, 0);
        hboxLayout6->setObjectName(QString::fromUtf8("hboxLayout6"));
        label_2 = new QLabel(widget_4);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(140, 0));
        label_2->setMaximumSize(QSize(140, 16777215));

        hboxLayout6->addWidget(label_2);

        txtWineServerBin = new QLineEdit(widget_4);
        txtWineServerBin->setObjectName(QString::fromUtf8("txtWineServerBin"));

        hboxLayout6->addWidget(txtWineServerBin);

        cmdGetWineServerBin = new QPushButton(widget_4);
        cmdGetWineServerBin->setObjectName(QString::fromUtf8("cmdGetWineServerBin"));
        cmdGetWineServerBin->setIconSize(QSize(16, 16));

        hboxLayout6->addWidget(cmdGetWineServerBin);


        vboxLayout4->addWidget(widget_4);

        widget_10 = new QWidget(groupBox);
        widget_10->setObjectName(QString::fromUtf8("widget_10"));
        hboxLayout7 = new QHBoxLayout(widget_10);
        hboxLayout7->setContentsMargins(0, 0, 0, 0);
        hboxLayout7->setObjectName(QString::fromUtf8("hboxLayout7"));
        label_3 = new QLabel(widget_10);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(140, 0));
        label_3->setMaximumSize(QSize(140, 16777215));

        hboxLayout7->addWidget(label_3);

        txtWineLoaderBin = new QLineEdit(widget_10);
        txtWineLoaderBin->setObjectName(QString::fromUtf8("txtWineLoaderBin"));

        hboxLayout7->addWidget(txtWineLoaderBin);

        cmdGetWineLoaderBin = new QPushButton(widget_10);
        cmdGetWineLoaderBin->setObjectName(QString::fromUtf8("cmdGetWineLoaderBin"));
        cmdGetWineLoaderBin->setIconSize(QSize(16, 16));

        hboxLayout7->addWidget(cmdGetWineLoaderBin);


        vboxLayout4->addWidget(widget_10);

        widget_11 = new QWidget(groupBox);
        widget_11->setObjectName(QString::fromUtf8("widget_11"));
        hboxLayout8 = new QHBoxLayout(widget_11);
        hboxLayout8->setContentsMargins(0, 0, 0, 0);
        hboxLayout8->setObjectName(QString::fromUtf8("hboxLayout8"));
        label_11 = new QLabel(widget_11);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setMinimumSize(QSize(140, 0));
        label_11->setMaximumSize(QSize(140, 16777215));

        hboxLayout8->addWidget(label_11);

        txtWineLibs = new QLineEdit(widget_11);
        txtWineLibs->setObjectName(QString::fromUtf8("txtWineLibs"));

        hboxLayout8->addWidget(txtWineLibs);

        cmdGetWineLibs = new QPushButton(widget_11);
        cmdGetWineLibs->setObjectName(QString::fromUtf8("cmdGetWineLibs"));
        cmdGetWineLibs->setIconSize(QSize(16, 16));

        hboxLayout8->addWidget(cmdGetWineLibs);


        vboxLayout4->addWidget(widget_11);


        verticalLayout_2->addWidget(groupBox);

        verticalSpacer_2 = new QSpacerItem(20, 74, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        twbGeneral->addTab(tab, QString());

        vboxLayout1->addWidget(twbGeneral);

        hboxLayout9 = new QHBoxLayout();
        hboxLayout9->setObjectName(QString::fromUtf8("hboxLayout9"));
        cmdHelp = new QPushButton(widget);
        cmdHelp->setObjectName(QString::fromUtf8("cmdHelp"));

        hboxLayout9->addWidget(cmdHelp);

        spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout9->addItem(spacerItem1);

        cmdCancel = new QPushButton(widget);
        cmdCancel->setObjectName(QString::fromUtf8("cmdCancel"));

        hboxLayout9->addWidget(cmdCancel);

        cmdOk = new QPushButton(widget);
        cmdOk->setObjectName(QString::fromUtf8("cmdOk"));

        hboxLayout9->addWidget(cmdOk);


        vboxLayout1->addLayout(hboxLayout9);


        vboxLayout->addWidget(widget);


        retranslateUi(PrefixSettings);

        twbGeneral->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(PrefixSettings);
    } // setupUi

    void retranslateUi(QDialog *PrefixSettings)
    {
        PrefixSettings->setWindowTitle(QApplication::translate("PrefixSettings", "Prefix settings", 0, QApplication::UnicodeUTF8));
        lblCaption->setText(QApplication::translate("PrefixSettings", "Prefix settings", 0, QApplication::UnicodeUTF8));
        lblLogo->setText(QString());
        groupBox_5->setTitle(QApplication::translate("PrefixSettings", "Prefix settings", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("PrefixSettings", "Name:", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("PrefixSettings", "Prefix path:", 0, QApplication::UnicodeUTF8));
        cmdGetPrefixPath->setText(QString());
        groupBox_7->setTitle(QApplication::translate("PrefixSettings", "Quick mount", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("PrefixSettings", "Device:", 0, QApplication::UnicodeUTF8));
        comboDeviceList->clear();
        comboDeviceList->insertItems(0, QStringList()
         << QApplication::translate("PrefixSettings", "<none>", 0, QApplication::UnicodeUTF8)
        );
        label_17->setText(QApplication::translate("PrefixSettings", "Mount point:", 0, QApplication::UnicodeUTF8));
        cmdGetMountPoint->setText(QString());
        twbGeneral->setTabText(twbGeneral->indexOf(tab_4), QApplication::translate("PrefixSettings", "General", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("PrefixSettings", "Wine settings", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("PrefixSettings", "If you wish to use different wine versions, you may wish set custom wine paths here.\n"
"Note: leave it blank for default.", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("PrefixSettings", "Bin:", 0, QApplication::UnicodeUTF8));
        cmdGetWineBin->setText(QString());
        label_2->setText(QApplication::translate("PrefixSettings", "Server:", 0, QApplication::UnicodeUTF8));
        cmdGetWineServerBin->setText(QString());
        label_3->setText(QApplication::translate("PrefixSettings", "Loader:", 0, QApplication::UnicodeUTF8));
        cmdGetWineLoaderBin->setText(QString());
        label_11->setText(QApplication::translate("PrefixSettings", "Libs:", 0, QApplication::UnicodeUTF8));
        cmdGetWineLibs->setText(QString());
        twbGeneral->setTabText(twbGeneral->indexOf(tab), QApplication::translate("PrefixSettings", "Wine settings", 0, QApplication::UnicodeUTF8));
        cmdHelp->setText(QApplication::translate("PrefixSettings", "Help", 0, QApplication::UnicodeUTF8));
        cmdCancel->setText(QApplication::translate("PrefixSettings", "Cancel", 0, QApplication::UnicodeUTF8));
        cmdOk->setText(QApplication::translate("PrefixSettings", "Ok", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PrefixSettings: public Ui_PrefixSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFIXSETTINGS_H
