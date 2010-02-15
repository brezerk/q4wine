/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created: Mon Feb 15 09:49:50 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *mainRun;
    QAction *mainExit;
    QAction *mainAbout;
    QAction *mainAboutQt;
    QAction *mainInstall;
    QAction *mainPrograms;
    QAction *mainProcess;
    QAction *mainSetup;
    QAction *mainPrefix;
    QAction *mainExportIcons;
    QAction *mainOptions;
    QAction *mainImageManage;
    QAction *mainFirstSteps;
    QAction *mainFAQ;
    QAction *mainIndex;
    QAction *mainWebsite;
    QAction *mainDonate;
    QAction *mainBugs;
    QAction *mainAppDB;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QTabWidget *tbwGeneral;
    QWidget *tabPrograms;
    QWidget *widgetFilter;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *cmdClearFilter;
    QLineEdit *txtIconFilter;
    QFrame *gbInfo;
    QGridLayout *gridLayout_3;
    QLabel *lblIconInfo0;
    QLabel *lblIconInfo1;
    QLabel *lblIconInfo2;
    QWidget *tabProcess;
    QVBoxLayout *tabProcessLayout;
    QWidget *tabPrefixSeup;
    QFrame *frame;
    QGridLayout *gridLayout1;
    QTabWidget *twWineSetup;
    QWidget *tab_4;
    QVBoxLayout *_2;
    QLabel *label;
    QPushButton *cmdCreateFake;
    QPushButton *cmdUpdateFake;
    QSpacerItem *spacerItem;
    QWidget *tab_5;
    QVBoxLayout *_3;
    QLabel *label_2;
    QSpacerItem *verticalSpacer;
    QWidget *tabPrefix;
    QVBoxLayout *tabPrefixLayout;
    QWidget *tabAppDB;
    QVBoxLayout *tabAppDBLayout;
    QMenuBar *menubar;
    QMenu *menu_File;
    QMenu *menuHelp;
    QMenu *menuHandbook;
    QMenu *menuTools;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(597, 509);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/data/q4wine.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        mainRun = new QAction(MainWindow);
        mainRun->setObjectName(QString::fromUtf8("mainRun"));
        mainExit = new QAction(MainWindow);
        mainExit->setObjectName(QString::fromUtf8("mainExit"));
        mainAbout = new QAction(MainWindow);
        mainAbout->setObjectName(QString::fromUtf8("mainAbout"));
        mainAboutQt = new QAction(MainWindow);
        mainAboutQt->setObjectName(QString::fromUtf8("mainAboutQt"));
        mainInstall = new QAction(MainWindow);
        mainInstall->setObjectName(QString::fromUtf8("mainInstall"));
        mainInstall->setEnabled(false);
        mainPrograms = new QAction(MainWindow);
        mainPrograms->setObjectName(QString::fromUtf8("mainPrograms"));
        mainProcess = new QAction(MainWindow);
        mainProcess->setObjectName(QString::fromUtf8("mainProcess"));
        mainSetup = new QAction(MainWindow);
        mainSetup->setObjectName(QString::fromUtf8("mainSetup"));
        mainPrefix = new QAction(MainWindow);
        mainPrefix->setObjectName(QString::fromUtf8("mainPrefix"));
        mainExportIcons = new QAction(MainWindow);
        mainExportIcons->setObjectName(QString::fromUtf8("mainExportIcons"));
        mainOptions = new QAction(MainWindow);
        mainOptions->setObjectName(QString::fromUtf8("mainOptions"));
        mainImageManage = new QAction(MainWindow);
        mainImageManage->setObjectName(QString::fromUtf8("mainImageManage"));
        mainFirstSteps = new QAction(MainWindow);
        mainFirstSteps->setObjectName(QString::fromUtf8("mainFirstSteps"));
        mainFAQ = new QAction(MainWindow);
        mainFAQ->setObjectName(QString::fromUtf8("mainFAQ"));
        mainIndex = new QAction(MainWindow);
        mainIndex->setObjectName(QString::fromUtf8("mainIndex"));
        mainWebsite = new QAction(MainWindow);
        mainWebsite->setObjectName(QString::fromUtf8("mainWebsite"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/data/go-home.png"), QSize(), QIcon::Normal, QIcon::Off);
        mainWebsite->setIcon(icon1);
        mainDonate = new QAction(MainWindow);
        mainDonate->setObjectName(QString::fromUtf8("mainDonate"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/data/bookmark-new.png"), QSize(), QIcon::Normal, QIcon::Off);
        mainDonate->setIcon(icon2);
        mainBugs = new QAction(MainWindow);
        mainBugs->setObjectName(QString::fromUtf8("mainBugs"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/data/tools-report-bug.png"), QSize(), QIcon::Normal, QIcon::Off);
        mainBugs->setIcon(icon3);
        mainAppDB = new QAction(MainWindow);
        mainAppDB->setObjectName(QString::fromUtf8("mainAppDB"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setSpacing(2);
        gridLayout->setContentsMargins(3, 3, 3, 3);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tbwGeneral = new QTabWidget(centralwidget);
        tbwGeneral->setObjectName(QString::fromUtf8("tbwGeneral"));
        tabPrograms = new QWidget();
        tabPrograms->setObjectName(QString::fromUtf8("tabPrograms"));
        widgetFilter = new QWidget(tabPrograms);
        widgetFilter->setObjectName(QString::fromUtf8("widgetFilter"));
        widgetFilter->setGeometry(QRect(30, 285, 461, 31));
        horizontalLayout_2 = new QHBoxLayout(widgetFilter);
        horizontalLayout_2->setSpacing(2);
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetMinimumSize);
        cmdClearFilter = new QPushButton(widgetFilter);
        cmdClearFilter->setObjectName(QString::fromUtf8("cmdClearFilter"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/data/clear-ltr.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdClearFilter->setIcon(icon4);
        cmdClearFilter->setFlat(true);

        horizontalLayout_2->addWidget(cmdClearFilter);

        txtIconFilter = new QLineEdit(widgetFilter);
        txtIconFilter->setObjectName(QString::fromUtf8("txtIconFilter"));

        horizontalLayout_2->addWidget(txtIconFilter);

        gbInfo = new QFrame(tabPrograms);
        gbInfo->setObjectName(QString::fromUtf8("gbInfo"));
        gbInfo->setGeometry(QRect(80, 170, 311, 61));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(gbInfo->sizePolicy().hasHeightForWidth());
        gbInfo->setSizePolicy(sizePolicy);
        gbInfo->setFrameShape(QFrame::StyledPanel);
        gbInfo->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(gbInfo);
        gridLayout_3->setSpacing(0);
        gridLayout_3->setContentsMargins(2, 2, 2, 2);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        lblIconInfo0 = new QLabel(gbInfo);
        lblIconInfo0->setObjectName(QString::fromUtf8("lblIconInfo0"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lblIconInfo0->sizePolicy().hasHeightForWidth());
        lblIconInfo0->setSizePolicy(sizePolicy1);
        lblIconInfo0->setTextFormat(Qt::AutoText);

        gridLayout_3->addWidget(lblIconInfo0, 0, 0, 1, 1);

        lblIconInfo1 = new QLabel(gbInfo);
        lblIconInfo1->setObjectName(QString::fromUtf8("lblIconInfo1"));
        sizePolicy1.setHeightForWidth(lblIconInfo1->sizePolicy().hasHeightForWidth());
        lblIconInfo1->setSizePolicy(sizePolicy1);
        lblIconInfo1->setTextFormat(Qt::AutoText);

        gridLayout_3->addWidget(lblIconInfo1, 0, 1, 1, 1);

        lblIconInfo2 = new QLabel(gbInfo);
        lblIconInfo2->setObjectName(QString::fromUtf8("lblIconInfo2"));
        sizePolicy1.setHeightForWidth(lblIconInfo2->sizePolicy().hasHeightForWidth());
        lblIconInfo2->setSizePolicy(sizePolicy1);

        gridLayout_3->addWidget(lblIconInfo2, 1, 0, 1, 2);

        tbwGeneral->addTab(tabPrograms, QString());
        tabProcess = new QWidget();
        tabProcess->setObjectName(QString::fromUtf8("tabProcess"));
        tabProcessLayout = new QVBoxLayout(tabProcess);
        tabProcessLayout->setSpacing(0);
        tabProcessLayout->setContentsMargins(0, 0, 0, 0);
        tabProcessLayout->setObjectName(QString::fromUtf8("tabProcessLayout"));
        tbwGeneral->addTab(tabProcess, QString());
        tabPrefixSeup = new QWidget();
        tabPrefixSeup->setObjectName(QString::fromUtf8("tabPrefixSeup"));
        frame = new QFrame(tabPrefixSeup);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(5, 50, 571, 371));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Sunken);
        gridLayout1 = new QGridLayout(frame);
        gridLayout1->setSpacing(2);
        gridLayout1->setContentsMargins(2, 2, 2, 2);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        twWineSetup = new QTabWidget(frame);
        twWineSetup->setObjectName(QString::fromUtf8("twWineSetup"));
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        _2 = new QVBoxLayout(tab_4);
        _2->setSpacing(2);
        _2->setContentsMargins(3, 3, 3, 3);
        _2->setObjectName(QString::fromUtf8("_2"));
        label = new QLabel(tab_4);
        label->setObjectName(QString::fromUtf8("label"));
        label->setWordWrap(true);

        _2->addWidget(label);

        cmdCreateFake = new QPushButton(tab_4);
        cmdCreateFake->setObjectName(QString::fromUtf8("cmdCreateFake"));
        cmdCreateFake->setEnabled(true);

        _2->addWidget(cmdCreateFake);

        cmdUpdateFake = new QPushButton(tab_4);
        cmdUpdateFake->setObjectName(QString::fromUtf8("cmdUpdateFake"));
        cmdUpdateFake->setEnabled(true);

        _2->addWidget(cmdUpdateFake);

        spacerItem = new QSpacerItem(535, 71, QSizePolicy::Minimum, QSizePolicy::Expanding);

        _2->addItem(spacerItem);

        twWineSetup->addTab(tab_4, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QString::fromUtf8("tab_5"));
        _3 = new QVBoxLayout(tab_5);
        _3->setSpacing(2);
        _3->setContentsMargins(3, 3, 3, 3);
        _3->setObjectName(QString::fromUtf8("_3"));
        label_2 = new QLabel(tab_5);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setWordWrap(true);

        _3->addWidget(label_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        _3->addItem(verticalSpacer);

        twWineSetup->addTab(tab_5, QString());

        gridLayout1->addWidget(twWineSetup, 0, 0, 1, 1);

        tbwGeneral->addTab(tabPrefixSeup, QString());
        tabPrefix = new QWidget();
        tabPrefix->setObjectName(QString::fromUtf8("tabPrefix"));
        tabPrefixLayout = new QVBoxLayout(tabPrefix);
        tabPrefixLayout->setSpacing(0);
        tabPrefixLayout->setContentsMargins(0, 0, 0, 0);
        tabPrefixLayout->setObjectName(QString::fromUtf8("tabPrefixLayout"));
        tbwGeneral->addTab(tabPrefix, QString());
        tabAppDB = new QWidget();
        tabAppDB->setObjectName(QString::fromUtf8("tabAppDB"));
        tabAppDBLayout = new QVBoxLayout(tabAppDB);
        tabAppDBLayout->setSpacing(0);
        tabAppDBLayout->setContentsMargins(0, 0, 0, 0);
        tabAppDBLayout->setObjectName(QString::fromUtf8("tabAppDBLayout"));
        tbwGeneral->addTab(tabAppDB, QString());

        gridLayout->addWidget(tbwGeneral, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 597, 21));
        menu_File = new QMenu(menubar);
        menu_File->setObjectName(QString::fromUtf8("menu_File"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuHandbook = new QMenu(menuHelp);
        menuHandbook->setObjectName(QString::fromUtf8("menuHandbook"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/data/help.png"), QSize(), QIcon::Normal, QIcon::Off);
        menuHandbook->setIcon(icon5);
        menuTools = new QMenu(menubar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menu_File->menuAction());
        menubar->addAction(menuTools->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menu_File->addAction(mainRun);
        menu_File->addAction(mainInstall);
        menu_File->addSeparator();
        menu_File->addAction(mainOptions);
        menu_File->addSeparator();
        menu_File->addAction(mainExit);
        menuHelp->addAction(mainAbout);
        menuHelp->addAction(mainAboutQt);
        menuHelp->addSeparator();
        menuHelp->addAction(menuHandbook->menuAction());
        menuHelp->addAction(mainWebsite);
        menuHelp->addAction(mainBugs);
        menuHelp->addSeparator();
        menuHelp->addAction(mainDonate);
        menuHandbook->addAction(mainFirstSteps);
        menuHandbook->addAction(mainFAQ);
        menuHandbook->addSeparator();
        menuHandbook->addAction(mainIndex);
        menuTools->addAction(mainPrograms);
        menuTools->addAction(mainProcess);
        menuTools->addAction(mainSetup);
        menuTools->addAction(mainPrefix);
        menuTools->addAction(mainAppDB);
        menuTools->addSeparator();
        menuTools->addAction(mainExportIcons);
        menuTools->addSeparator();
        menuTools->addAction(mainImageManage);

        retranslateUi(MainWindow);

        tbwGeneral->setCurrentIndex(0);
        twWineSetup->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "%1 :. Qt4 GUI for Wine v%2", 0, QApplication::UnicodeUTF8));
        mainRun->setText(QApplication::translate("MainWindow", "Run...", 0, QApplication::UnicodeUTF8));
        mainRun->setShortcut(QApplication::translate("MainWindow", "Ctrl+Z", 0, QApplication::UnicodeUTF8));
        mainExit->setText(QApplication::translate("MainWindow", "Quit", 0, QApplication::UnicodeUTF8));
        mainExit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        mainAbout->setText(QApplication::translate("MainWindow", "About", 0, QApplication::UnicodeUTF8));
        mainAbout->setShortcut(QApplication::translate("MainWindow", "Ctrl+E", 0, QApplication::UnicodeUTF8));
        mainAboutQt->setText(QApplication::translate("MainWindow", "About Qt", 0, QApplication::UnicodeUTF8));
        mainInstall->setText(QApplication::translate("MainWindow", "Instal wizard", 0, QApplication::UnicodeUTF8));
        mainInstall->setShortcut(QApplication::translate("MainWindow", "Ctrl+X", 0, QApplication::UnicodeUTF8));
        mainPrograms->setText(QApplication::translate("MainWindow", "Programs", 0, QApplication::UnicodeUTF8));
        mainPrograms->setShortcut(QApplication::translate("MainWindow", "Ctrl+A", 0, QApplication::UnicodeUTF8));
        mainProcess->setText(QApplication::translate("MainWindow", "Process", 0, QApplication::UnicodeUTF8));
        mainProcess->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        mainSetup->setText(QApplication::translate("MainWindow", "Setup", 0, QApplication::UnicodeUTF8));
        mainSetup->setShortcut(QApplication::translate("MainWindow", "Ctrl+D", 0, QApplication::UnicodeUTF8));
        mainPrefix->setText(QApplication::translate("MainWindow", "Prefixes", 0, QApplication::UnicodeUTF8));
        mainPrefix->setShortcut(QApplication::translate("MainWindow", "Ctrl+F", 0, QApplication::UnicodeUTF8));
        mainExportIcons->setText(QApplication::translate("MainWindow", "Export icons", 0, QApplication::UnicodeUTF8));
        mainExportIcons->setShortcut(QApplication::translate("MainWindow", "Ctrl+C", 0, QApplication::UnicodeUTF8));
        mainOptions->setText(QApplication::translate("MainWindow", "Options", 0, QApplication::UnicodeUTF8));
        mainOptions->setShortcut(QApplication::translate("MainWindow", "Ctrl+W", 0, QApplication::UnicodeUTF8));
        mainImageManage->setText(QApplication::translate("MainWindow", "CD image manager", 0, QApplication::UnicodeUTF8));
        mainImageManage->setShortcut(QApplication::translate("MainWindow", "Ctrl+V", 0, QApplication::UnicodeUTF8));
        mainFirstSteps->setText(QApplication::translate("MainWindow", "First steps", 0, QApplication::UnicodeUTF8));
        mainFAQ->setText(QApplication::translate("MainWindow", "Short FAQ", 0, QApplication::UnicodeUTF8));
        mainIndex->setText(QApplication::translate("MainWindow", "Index", 0, QApplication::UnicodeUTF8));
        mainWebsite->setText(QApplication::translate("MainWindow", "Website", 0, QApplication::UnicodeUTF8));
        mainDonate->setText(QApplication::translate("MainWindow", "Donate", 0, QApplication::UnicodeUTF8));
        mainBugs->setText(QApplication::translate("MainWindow", "Bugs", 0, QApplication::UnicodeUTF8));
        mainAppDB->setText(QApplication::translate("MainWindow", "Wine AppDB browser", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        cmdClearFilter->setToolTip(QApplication::translate("MainWindow", "Clear filter text", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        cmdClearFilter->setText(QString());
        lblIconInfo0->setText(QString());
        lblIconInfo1->setText(QString());
        lblIconInfo2->setText(QString());
        tbwGeneral->setTabText(tbwGeneral->indexOf(tabPrograms), QApplication::translate("MainWindow", "Programs", 0, QApplication::UnicodeUTF8));
        tbwGeneral->setTabText(tbwGeneral->indexOf(tabProcess), QApplication::translate("MainWindow", "Process", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "This is the point where you have to start when creating new empty prefix. You can also create a new or update an old fake Windows drive.", 0, QApplication::UnicodeUTF8));
        cmdCreateFake->setText(QApplication::translate("MainWindow", "Create Fake Drive", 0, QApplication::UnicodeUTF8));
        cmdUpdateFake->setText(QApplication::translate("MainWindow", "Update Fake Drive", 0, QApplication::UnicodeUTF8));
        twWineSetup->setTabText(twWineSetup->indexOf(tab_4), QApplication::translate("MainWindow", "Base setup", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "WIS scripts support is not emplemented right now. I will be iplemented in future releases. For now You can use wineriks support (see current tab toolbar actions).", 0, QApplication::UnicodeUTF8));
        twWineSetup->setTabText(twWineSetup->indexOf(tab_5), QApplication::translate("MainWindow", "System Software", 0, QApplication::UnicodeUTF8));
        tbwGeneral->setTabText(tbwGeneral->indexOf(tabPrefixSeup), QApplication::translate("MainWindow", "Setup", 0, QApplication::UnicodeUTF8));
        tbwGeneral->setTabText(tbwGeneral->indexOf(tabPrefix), QApplication::translate("MainWindow", "Prefixes", 0, QApplication::UnicodeUTF8));
        tbwGeneral->setTabText(tbwGeneral->indexOf(tabAppDB), QApplication::translate("MainWindow", "Wine AppDB", 0, QApplication::UnicodeUTF8));
        menu_File->setTitle(QApplication::translate("MainWindow", "&File", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
        menuHandbook->setTitle(QApplication::translate("MainWindow", "Handbook", 0, QApplication::UnicodeUTF8));
        menuTools->setTitle(QApplication::translate("MainWindow", "Tools", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
