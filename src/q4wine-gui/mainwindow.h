/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010 by Malakhov Alexey                           *
 *   brezerk@gmail.com                                                     *
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
 * \brief q4wine-gui pakage provide general GUI functions for q4wine.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <src/q4wine-gui/ui_MainWindow.h>

//Qt includes
#include <QSystemTrayIcon>
#include <QSplitter>
#include <QLocalServer>
#include <QLocalSocket>

//Global config
#include "config.h"

//Database
#include "prefix.h"

//Widgets
#include "loggingwidget.h"
#include "iconlistwidget.h"
#include "prefixtreewidget.h"
#include "wineprocesswidget.h"
#include "prefixcontrolwidget.h"

#ifdef WITH_WINEAPPDB
#include "appdbwidget.h"
#endif

//Windows
#include "iconsview.h"
#include "wizard.h"
#include "process.h"
#include "imagemanager.h"
#include "about.h"
#include "appsettings.h"
#include "run.h"

#ifdef WITH_WINETRIKS
#include "winetricks.h"
#endif

//System
#include <stdlib.h>
#include <unistd.h>
#include <memory>

//q4wine lib
#include "q4wine-lib.h"

class MainWindow : public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT
	public:
        MainWindow(int startState, QString run_binary, QWidget * parent = 0, Qt::WFlags f = 0);

	public slots:
        void messageReceived(const QString message);
		void setcbPrefixesIndex(const QString text) const;
		void updateDtabaseConnectedItems(void);
        void setSearchFocus(void);

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

		void cmdCreateFake_Click(void);
		void cmdUpdateFake_Click(void);
		void cmdClearFilter_Click (void);

		void prefixManage_Click(void);
		void prefixRunWinetriks_Click (void);

		void updateIconDesc(QString program, QString args, QString desc, QString console, QString desktop);

		//Main menu slots
		void mainExit_Click(void);
		void mainPrograms_Click(void);
		void mainImageManager_Click(void);
		void mainProcess_Click(void);
		void mainSetup_Click(void);
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

        void newConnection();

private:
		//! Custom Widgets
		//DragListWidget* lstIcons;
#ifdef WITH_WINEAPPDB
		std::auto_ptr<AppDBWidget> appdbWidget;
#endif
		std::auto_ptr<QComboBox> cbPrefixes;
        std::auto_ptr<QLocalServer> serverSoket;

		//! This is need for libq4wine-core.so import;
		typedef void *CoreLibPrototype (bool);
			CoreLibPrototype *CoreLibClassPointer;
			std::auto_ptr<corelib> CoreLib;
		QLibrary libq4wine;

		//Classes
		Prefix db_prefix;

		// Tray icon
		std::auto_ptr<QSystemTrayIcon> trayIcon;

        bool createSocket();
        void showSocketError(QString message);

		void createTrayIcon();
		void setMeVisible(bool visible);
		void getSettings(void);
		void clearTmp();

		std::auto_ptr<QSplitter> splitter;

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

	protected:
		// Events
		void closeEvent(QCloseEvent *event);

};

#endif
