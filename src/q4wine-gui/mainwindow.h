/***************************************************************************
 *   Copyright (C) 2008-2018 by Oleksii S. Malakhov <brezerk@gmail.com>    *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 ***************************************************************************/

/*!
 * \defgroup q4wine-gui Q4Wine GUI
 * \brief q4wine-gui package provides general GUI functions for Q4Wine.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <unistd.h>

#include <src/q4wine-gui/ui_MainWindow.h>

//Qt includes
#include <QSystemTrayIcon>
#include <QSplitter>
#include <QLocalServer>
#include <QLocalSocket>
#include <QTabWidget>

//Global config
#include "config.h"

//Database
#include "prefix.h"
#include "dir.h"
#include "icon.h"
#ifndef _OS_DARWIN_
#include "sysmenu.h"
#endif

//Widgets
#include "loggingwidget.h"
#include "iconlistwidget.h"
#include "iconlisttoolbar.h"
#include "prefixtreewidget.h"
#include "prefixtreetoolbar.h"
#include "wineprocesswidget.h"
#include "prefixcontrolwidget.h"
#include "prefixconfigwidget.h"

#ifdef WITH_WINEAPPDB
#include "appdbwidget.h"
#endif

#ifdef WITH_DBUS
#include <QDBusInterface>
#endif

//Windows
#include "iconsview.h"
#include "wizard.h"
#include "process.h"
#include "progress.h"
#include "imagemanager.h"
#include "about.h"
#include "appsettings.h"
#include "run.h"
#include "fakedrivesettings.h"
#include "src/q4wine-gui/versions.h"

#include "winetricks.h"

//System
#include <stdlib.h>
#include <unistd.h>
#include <memory>

//q4wine lib
#include "q4wine-lib.h"

#include "src/core/registry.h"

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT
    public:
        MainWindow(int startState, QString run_binary, QWidget * parent = 0, Qt::WindowFlags f = 0);

    public slots:
            void messageReceived(const QString message);
            void updateDtabaseConnectedItems(void);
            void setSearchFocus(void);
            void setMeVisible(bool visible);

#ifdef WITH_WINEAPPDB
        void searchRequest(QString search);
#endif

    private slots:
        void tbwGeneral_CurrentTabChange(int tabIndex);

        void changeStatusText(QString text);

        void trayIcon_Activate(QSystemTrayIcon::ActivationReason reason);

        /*
         * Command buttons slots
         */
        void updateIconDesc(QString program, QString args, QString desc, QString console, QString desktop);

        //Main menu slots
        void mainExit_Click(void);
        void mainPrograms_Click(void);
        void mainImageManager_Click(void);
        void mainProcess_Click(void);
        void mainSetup_Click(void);
        void mainLogging_Click(void);
        void mainPrefix_Click(void);
        void mainAbout_Click(void);
        void mainAboutQt_Click(void);
        void mainExportIcons_Click(void);
        void mainRun_Click(void);
        void mainOptions_Click(void);
        void mainInstall_Click(void);
        void mainFirstSteps_Click(void);
        void mainFAQ_Click(void);
        void mainIndex_Click(void);
        void mainWebsite_Click(void);
        void mainDonate_Click(void);
        void mainBugs_Click(void);
        void mainAppDB_Click(void);
        void mainHelpThisTab_Click(void);
        void mainImportWineIcons_Click(void);
        void mainVersionManager_Click();

        void newConnection();

private:
        //! Custom Widgets
        //DragListWidget* lstIcons;
#ifdef WITH_WINEAPPDB
        std::unique_ptr<AppDBWidget> appdbWidget;
#endif
        std::unique_ptr<QLocalServer> serverSoket;

        //! This is need for libq4wine-core.so import;
        typedef void *CoreLibPrototype (bool);
            CoreLibPrototype *CoreLibClassPointer;
            std::unique_ptr<corelib> CoreLib;
        QLibrary libq4wine;

        //Classes
        Prefix db_prefix;
        Dir db_dir;
        Icon db_icon;
#ifndef _OS_DARWIN_
        system_menu sys_menu;
#endif
        // Tray icon
        std::unique_ptr<QSystemTrayIcon> trayIcon;

        bool createSocket();
        void showSocketError(QString message);

        void createTrayIcon();

        void getSettings(void);
        void clearTmp();
        void showNotifycation(const QString header, const QString message);

        std::unique_ptr<QSplitter> splitter;

//        void getWineMenuIcons(void);
//        void parseIcons(void);

    signals:
#ifdef WITH_WINEAPPDB
        void appdbWidget_startSearch(short int, QString);
        void setAppDBFocus();
#endif
        void updateDatabaseConnections(void);
        void setDefaultFocus(QString, QString);
        void stopProcTimer(void);
        void startProcTimer(void);

        void reloadLogData(void);
        void runProgramRequest(QString);

    protected:
        // Events
        void closeEvent(QCloseEvent *event);

};

#endif
