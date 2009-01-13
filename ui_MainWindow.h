/********************************************************************************
** Form generated from reading ui file 'MainWindow.ui'
**
** Created: Tue Jan 13 11:38:31 2009
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_Import;
    QAction *action_Export;
    QAction *mainRun;
    QAction *mainExit;
    QAction *mainAbout;
    QAction *mainHandbook;
    QAction *mainAboutQt;
    QAction *mainInstall;
    QAction *mainPrograms;
    QAction *mainProcess;
    QAction *mainSetup;
    QAction *mainPrefix;
    QAction *mainExportIcons;
    QAction *mainOptions;
    QAction *mainImageManage;
    QAction *mainWinetricks;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QTabWidget *tbwGeneral;
    QWidget *tab;
    QGridLayout *gridLayout_2;
    QGroupBox *gbCategoryes;
    QGridLayout *gridLayout_3;
    QTreeWidget *twPrograms;
    QGroupBox *gbIcons;
    QGridLayout *gridLayout1;
    QListWidget *lstIcons;
    QGroupBox *gbInfo;
    QGridLayout *_5;
    QLabel *lblInfo;
    QWidget *tab_2;
    QVBoxLayout *vboxLayout;
    QFrame *tlbProccess;
    QTableWidget *tableProc;
    QLabel *lblProcInfo;
    QWidget *tab_3;
    QGridLayout *gridLayout2;
    QGroupBox *gbSelPrefix;
    QHBoxLayout *horizontalLayout;
    QComboBox *cbPrefixes;
    QPushButton *cmdManagePrefixes;
    QGroupBox *gbWineSetup;
    QGridLayout *gridLayout3;
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
    QPushButton *cmdWinetricks;
    QSpacerItem *spacerItem1;
    QWidget *tab_6;
    QVBoxLayout *_4;
    QLabel *label_3;
    QSpacerItem *spacerItem2;
    QWidget *tab_8;
    QVBoxLayout *vboxLayout1;
    QFrame *tlbPrefix;
    QTableWidget *tablePrefix;
    QMenuBar *menubar;
    QMenu *menu_File;
    QMenu *menuHelp;
    QMenu *menuTools;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
    if (MainWindow->objectName().isEmpty())
        MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
    MainWindow->resize(620, 508);
    action_Import = new QAction(MainWindow);
    action_Import->setObjectName(QString::fromUtf8("action_Import"));
    action_Export = new QAction(MainWindow);
    action_Export->setObjectName(QString::fromUtf8("action_Export"));
    mainRun = new QAction(MainWindow);
    mainRun->setObjectName(QString::fromUtf8("mainRun"));
    mainExit = new QAction(MainWindow);
    mainExit->setObjectName(QString::fromUtf8("mainExit"));
    mainAbout = new QAction(MainWindow);
    mainAbout->setObjectName(QString::fromUtf8("mainAbout"));
    mainHandbook = new QAction(MainWindow);
    mainHandbook->setObjectName(QString::fromUtf8("mainHandbook"));
    mainAboutQt = new QAction(MainWindow);
    mainAboutQt->setObjectName(QString::fromUtf8("mainAboutQt"));
    mainInstall = new QAction(MainWindow);
    mainInstall->setObjectName(QString::fromUtf8("mainInstall"));
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
    mainWinetricks = new QAction(MainWindow);
    mainWinetricks->setObjectName(QString::fromUtf8("mainWinetricks"));
    centralwidget = new QWidget(MainWindow);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    gridLayout = new QGridLayout(centralwidget);
    gridLayout->setSpacing(2);
    gridLayout->setMargin(3);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    tbwGeneral = new QTabWidget(centralwidget);
    tbwGeneral->setObjectName(QString::fromUtf8("tbwGeneral"));
    tab = new QWidget();
    tab->setObjectName(QString::fromUtf8("tab"));
    gridLayout_2 = new QGridLayout(tab);
    gridLayout_2->setSpacing(2);
    gridLayout_2->setMargin(3);
    gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
    gbCategoryes = new QGroupBox(tab);
    gbCategoryes->setObjectName(QString::fromUtf8("gbCategoryes"));
    gbCategoryes->setMinimumSize(QSize(250, 0));
    gbCategoryes->setMaximumSize(QSize(250, 16777215));
    gridLayout_3 = new QGridLayout(gbCategoryes);
    gridLayout_3->setSpacing(2);
    gridLayout_3->setMargin(3);
    gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
    twPrograms = new QTreeWidget(gbCategoryes);
    new QTreeWidgetItem(twPrograms);

    twPrograms->setObjectName(QString::fromUtf8("twPrograms"));
    twPrograms->setContextMenuPolicy(Qt::CustomContextMenu);
    twPrograms->setAcceptDrops(true);
    twPrograms->setAutoExpandDelay(0);
    twPrograms->setItemsExpandable(true);

    gridLayout_3->addWidget(twPrograms, 0, 0, 1, 1);


    gridLayout_2->addWidget(gbCategoryes, 0, 0, 1, 1);

    gbIcons = new QGroupBox(tab);
    gbIcons->setObjectName(QString::fromUtf8("gbIcons"));
    gridLayout1 = new QGridLayout(gbIcons);
    gridLayout1->setSpacing(2);
    gridLayout1->setMargin(3);
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    lstIcons = new QListWidget(gbIcons);
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

    gridLayout1->addWidget(lstIcons, 0, 0, 1, 1);


    gridLayout_2->addWidget(gbIcons, 0, 1, 1, 1);

    gbInfo = new QGroupBox(tab);
    gbInfo->setObjectName(QString::fromUtf8("gbInfo"));
    _5 = new QGridLayout(gbInfo);
    _5->setSpacing(2);
    _5->setMargin(3);
    _5->setObjectName(QString::fromUtf8("_5"));
    lblInfo = new QLabel(gbInfo);
    lblInfo->setObjectName(QString::fromUtf8("lblInfo"));

    _5->addWidget(lblInfo, 0, 0, 1, 1);


    gridLayout_2->addWidget(gbInfo, 1, 0, 1, 2);

    tbwGeneral->addTab(tab, QString());
    tab_2 = new QWidget();
    tab_2->setObjectName(QString::fromUtf8("tab_2"));
    vboxLayout = new QVBoxLayout(tab_2);
    vboxLayout->setSpacing(2);
    vboxLayout->setMargin(3);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    tlbProccess = new QFrame(tab_2);
    tlbProccess->setObjectName(QString::fromUtf8("tlbProccess"));
    tlbProccess->setMinimumSize(QSize(16, 40));
    tlbProccess->setMaximumSize(QSize(16777215, 40));
    tlbProccess->setFrameShape(QFrame::NoFrame);
    tlbProccess->setFrameShadow(QFrame::Raised);

    vboxLayout->addWidget(tlbProccess);

    tableProc = new QTableWidget(tab_2);
    if (tableProc->columnCount() < 4)
        tableProc->setColumnCount(4);
    QTableWidgetItem *__colItem = new QTableWidgetItem();
    tableProc->setHorizontalHeaderItem(0, __colItem);
    QTableWidgetItem *__colItem1 = new QTableWidgetItem();
    tableProc->setHorizontalHeaderItem(1, __colItem1);
    QTableWidgetItem *__colItem2 = new QTableWidgetItem();
    tableProc->setHorizontalHeaderItem(2, __colItem2);
    QTableWidgetItem *__colItem3 = new QTableWidgetItem();
    tableProc->setHorizontalHeaderItem(3, __colItem3);
    tableProc->setObjectName(QString::fromUtf8("tableProc"));
    tableProc->setSelectionMode(QAbstractItemView::SingleSelection);
    tableProc->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableProc->setSortingEnabled(false);

    vboxLayout->addWidget(tableProc);

    lblProcInfo = new QLabel(tab_2);
    lblProcInfo->setObjectName(QString::fromUtf8("lblProcInfo"));
    lblProcInfo->setMargin(3);

    vboxLayout->addWidget(lblProcInfo);

    tbwGeneral->addTab(tab_2, QString());
    tab_3 = new QWidget();
    tab_3->setObjectName(QString::fromUtf8("tab_3"));
    gridLayout2 = new QGridLayout(tab_3);
    gridLayout2->setSpacing(2);
    gridLayout2->setMargin(3);
    gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
    gbSelPrefix = new QGroupBox(tab_3);
    gbSelPrefix->setObjectName(QString::fromUtf8("gbSelPrefix"));
    horizontalLayout = new QHBoxLayout(gbSelPrefix);
    horizontalLayout->setSpacing(2);
    horizontalLayout->setMargin(3);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    cbPrefixes = new QComboBox(gbSelPrefix);
    cbPrefixes->setObjectName(QString::fromUtf8("cbPrefixes"));

    horizontalLayout->addWidget(cbPrefixes);

    cmdManagePrefixes = new QPushButton(gbSelPrefix);
    cmdManagePrefixes->setObjectName(QString::fromUtf8("cmdManagePrefixes"));
    cmdManagePrefixes->setMaximumSize(QSize(160, 16777215));

    horizontalLayout->addWidget(cmdManagePrefixes);


    gridLayout2->addWidget(gbSelPrefix, 0, 0, 1, 1);

    gbWineSetup = new QGroupBox(tab_3);
    gbWineSetup->setObjectName(QString::fromUtf8("gbWineSetup"));
    gridLayout3 = new QGridLayout(gbWineSetup);
    gridLayout3->setSpacing(2);
    gridLayout3->setMargin(3);
    gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
    twWineSetup = new QTabWidget(gbWineSetup);
    twWineSetup->setObjectName(QString::fromUtf8("twWineSetup"));
    tab_4 = new QWidget();
    tab_4->setObjectName(QString::fromUtf8("tab_4"));
    _2 = new QVBoxLayout(tab_4);
    _2->setSpacing(2);
    _2->setMargin(3);
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
    _3->setMargin(3);
    _3->setObjectName(QString::fromUtf8("_3"));
    label_2 = new QLabel(tab_5);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    _3->addWidget(label_2);

    cmdWinetricks = new QPushButton(tab_5);
    cmdWinetricks->setObjectName(QString::fromUtf8("cmdWinetricks"));

    _3->addWidget(cmdWinetricks);

    spacerItem1 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    _3->addItem(spacerItem1);

    twWineSetup->addTab(tab_5, QString());
    tab_6 = new QWidget();
    tab_6->setObjectName(QString::fromUtf8("tab_6"));
    _4 = new QVBoxLayout(tab_6);
    _4->setSpacing(2);
    _4->setMargin(3);
    _4->setObjectName(QString::fromUtf8("_4"));
    label_3 = new QLabel(tab_6);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    _4->addWidget(label_3);

    spacerItem2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    _4->addItem(spacerItem2);

    twWineSetup->addTab(tab_6, QString());

    gridLayout3->addWidget(twWineSetup, 0, 0, 1, 1);


    gridLayout2->addWidget(gbWineSetup, 1, 0, 1, 1);

    tbwGeneral->addTab(tab_3, QString());
    tab_8 = new QWidget();
    tab_8->setObjectName(QString::fromUtf8("tab_8"));
    vboxLayout1 = new QVBoxLayout(tab_8);
    vboxLayout1->setSpacing(2);
    vboxLayout1->setMargin(3);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    tlbPrefix = new QFrame(tab_8);
    tlbPrefix->setObjectName(QString::fromUtf8("tlbPrefix"));
    tlbPrefix->setMinimumSize(QSize(16, 40));
    tlbPrefix->setMaximumSize(QSize(16777215, 40));
    tlbPrefix->setFrameShape(QFrame::NoFrame);
    tlbPrefix->setFrameShadow(QFrame::Raised);

    vboxLayout1->addWidget(tlbPrefix);

    tablePrefix = new QTableWidget(tab_8);
    if (tablePrefix->columnCount() < 3)
        tablePrefix->setColumnCount(3);
    QTableWidgetItem *__colItem4 = new QTableWidgetItem();
    tablePrefix->setHorizontalHeaderItem(0, __colItem4);
    QTableWidgetItem *__colItem5 = new QTableWidgetItem();
    tablePrefix->setHorizontalHeaderItem(1, __colItem5);
    QTableWidgetItem *__colItem6 = new QTableWidgetItem();
    tablePrefix->setHorizontalHeaderItem(2, __colItem6);
    tablePrefix->setObjectName(QString::fromUtf8("tablePrefix"));
    tablePrefix->setSelectionMode(QAbstractItemView::SingleSelection);
    tablePrefix->setSelectionBehavior(QAbstractItemView::SelectRows);

    vboxLayout1->addWidget(tablePrefix);

    tbwGeneral->addTab(tab_8, QString());

    gridLayout->addWidget(tbwGeneral, 0, 0, 1, 1);

    MainWindow->setCentralWidget(centralwidget);
    menubar = new QMenuBar(MainWindow);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 620, 25));
    menu_File = new QMenu(menubar);
    menu_File->setObjectName(QString::fromUtf8("menu_File"));
    menuHelp = new QMenu(menubar);
    menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
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
    menuHelp->addAction(mainHandbook);
    menuTools->addAction(mainPrograms);
    menuTools->addAction(mainProcess);
    menuTools->addAction(mainSetup);
    menuTools->addAction(mainPrefix);
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
    action_Import->setText(QApplication::translate("MainWindow", "&Import", 0, QApplication::UnicodeUTF8));
    action_Export->setText(QApplication::translate("MainWindow", "&Export", 0, QApplication::UnicodeUTF8));
    mainRun->setText(QApplication::translate("MainWindow", "&Run...", 0, QApplication::UnicodeUTF8));
    mainExit->setText(QApplication::translate("MainWindow", "E&xit", 0, QApplication::UnicodeUTF8));
    mainAbout->setText(QApplication::translate("MainWindow", "&About", 0, QApplication::UnicodeUTF8));
    mainHandbook->setText(QApplication::translate("MainWindow", "Handbook", 0, QApplication::UnicodeUTF8));
    mainAboutQt->setText(QApplication::translate("MainWindow", "About Qt", 0, QApplication::UnicodeUTF8));
    mainInstall->setText(QApplication::translate("MainWindow", "Instal wizard", 0, QApplication::UnicodeUTF8));
    mainPrograms->setText(QApplication::translate("MainWindow", "Programs", 0, QApplication::UnicodeUTF8));
    mainProcess->setText(QApplication::translate("MainWindow", "Process", 0, QApplication::UnicodeUTF8));
    mainSetup->setText(QApplication::translate("MainWindow", "Setup", 0, QApplication::UnicodeUTF8));
    mainPrefix->setText(QApplication::translate("MainWindow", "Prefixes", 0, QApplication::UnicodeUTF8));
    mainExportIcons->setText(QApplication::translate("MainWindow", "Export icons", 0, QApplication::UnicodeUTF8));
    mainOptions->setText(QApplication::translate("MainWindow", "Options", 0, QApplication::UnicodeUTF8));
    mainImageManage->setText(QApplication::translate("MainWindow", "CD image manager", 0, QApplication::UnicodeUTF8));
    mainWinetricks->setText(QApplication::translate("MainWindow", "Winetricks", 0, QApplication::UnicodeUTF8));
    gbCategoryes->setTitle(QApplication::translate("MainWindow", "\320\241ategories", 0, QApplication::UnicodeUTF8));
    twPrograms->headerItem()->setText(0, QApplication::translate("MainWindow", "Prefixes", 0, QApplication::UnicodeUTF8));

    const bool __sortingEnabled = twPrograms->isSortingEnabled();
    twPrograms->setSortingEnabled(false);
    twPrograms->topLevelItem(0)->setText(0, QApplication::translate("MainWindow", "Default", 0, QApplication::UnicodeUTF8));

    twPrograms->setSortingEnabled(__sortingEnabled);
    gbIcons->setTitle(QApplication::translate("MainWindow", "Icons", 0, QApplication::UnicodeUTF8));
    gbInfo->setTitle(QApplication::translate("MainWindow", "Info", 0, QApplication::UnicodeUTF8));
    lblInfo->setText(QApplication::translate("MainWindow", "Name:<br>Desctiption:", 0, QApplication::UnicodeUTF8));
    tbwGeneral->setTabText(tbwGeneral->indexOf(tab), QApplication::translate("MainWindow", "Programs", 0, QApplication::UnicodeUTF8));
    tableProc->horizontalHeaderItem(0)->setText(QApplication::translate("MainWindow", "PID", 0, QApplication::UnicodeUTF8));
    tableProc->horizontalHeaderItem(1)->setText(QApplication::translate("MainWindow", "Name", 0, QApplication::UnicodeUTF8));
    tableProc->horizontalHeaderItem(2)->setText(QApplication::translate("MainWindow", "Ni", 0, QApplication::UnicodeUTF8));
    tableProc->horizontalHeaderItem(3)->setText(QApplication::translate("MainWindow", "Prefix", 0, QApplication::UnicodeUTF8));
    lblProcInfo->setText(QApplication::translate("MainWindow", "Total procces:", 0, QApplication::UnicodeUTF8));
    tbwGeneral->setTabText(tbwGeneral->indexOf(tab_2), QApplication::translate("MainWindow", "Process", 0, QApplication::UnicodeUTF8));
    gbSelPrefix->setTitle(QApplication::translate("MainWindow", "Current Prefix", 0, QApplication::UnicodeUTF8));
    cbPrefixes->clear();
    cbPrefixes->insertItems(0, QStringList()
     << QApplication::translate("MainWindow", "Default (Windows XP)", 0, QApplication::UnicodeUTF8)
    );
    cmdManagePrefixes->setText(QApplication::translate("MainWindow", "Manage", 0, QApplication::UnicodeUTF8));
    gbWineSetup->setTitle(QApplication::translate("MainWindow", "Options", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Fixed [Misc]'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"justify\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">This is the point where you have to start when creating new empty prefix. You can also create a new or update an old fake Windows drive.</p></body></html>", 0, QApplication::UnicodeUTF8));
    cmdCreateFake->setText(QApplication::translate("MainWindow", "Create Fake Drive", 0, QApplication::UnicodeUTF8));
    cmdUpdateFake->setText(QApplication::translate("MainWindow", "Update Fake Drive", 0, QApplication::UnicodeUTF8));
    twWineSetup->setTabText(twWineSetup->indexOf(tab_4), QApplication::translate("MainWindow", "Base setup", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("MainWindow", "It (temporary) implemented by winetricks", 0, QApplication::UnicodeUTF8));
    cmdWinetricks->setText(QApplication::translate("MainWindow", "Run Winetricks", 0, QApplication::UnicodeUTF8));
    twWineSetup->setTabText(twWineSetup->indexOf(tab_5), QApplication::translate("MainWindow", "System Software", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("MainWindow", "Hi. No functions here. It'l be implemented at v.0.120.", 0, QApplication::UnicodeUTF8));
    twWineSetup->setTabText(twWineSetup->indexOf(tab_6), QApplication::translate("MainWindow", "Core fonts", 0, QApplication::UnicodeUTF8));
    tbwGeneral->setTabText(tbwGeneral->indexOf(tab_3), QApplication::translate("MainWindow", "Setup", 0, QApplication::UnicodeUTF8));
    tablePrefix->horizontalHeaderItem(0)->setText(QApplication::translate("MainWindow", "Name", 0, QApplication::UnicodeUTF8));
    tablePrefix->horizontalHeaderItem(1)->setText(QApplication::translate("MainWindow", "Version", 0, QApplication::UnicodeUTF8));
    tablePrefix->horizontalHeaderItem(2)->setText(QApplication::translate("MainWindow", "Path", 0, QApplication::UnicodeUTF8));
    tbwGeneral->setTabText(tbwGeneral->indexOf(tab_8), QApplication::translate("MainWindow", "Prefixes", 0, QApplication::UnicodeUTF8));
    menu_File->setTitle(QApplication::translate("MainWindow", "&File", 0, QApplication::UnicodeUTF8));
    menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
    menuTools->setTitle(QApplication::translate("MainWindow", "Tools", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
