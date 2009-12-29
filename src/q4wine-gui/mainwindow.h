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

#include <QMainWindow>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QSystemTrayIcon>
#include <QWidget>
#include <QDir>
#include <QSizePolicy>
#include <QStringList>
#include <QTimer>
#include <QTableWidget>
#include <QTabWidget>
#include <QLabel>
#include <QString>
#include <QMessageBox>
#include <QToolBar>
#include <QIcon>
#include <QTreeWidgetItem>
#include <QMimeData>
#include <QDrag>
#include <QSplitter>

#include <ui_MainWindow.h>

#include "config.h"

#include "src/core/database/prefix.h"
#include "src/core/database/dir.h"
#include "src/core/database/icon.h"
#include "src/core/database/last_run_icon.h"
#include "src/core/database/image.h"

#include "xmlparser.h"

#include "draglistwidget.h"

#ifdef WITH_WINEAPPDB
#include "appdbwidget.h"
#endif

#include "wisitem.h"
#include "iconsview.h"
#include "wizard.h"
#include "process.h"
#include "winebinlauncher.h"
#include "iconsettings.h"
#include "imagemanager.h"
#include "prefixsettings.h"
#include "about.h"
#include "appsettings.h"
#include "run.h"

#include "registry.h"

#ifdef WITH_WINETRIKS
  #include "temporary/winetricks.h"
#endif

#include <stdlib.h>
#include <unistd.h>

#include <q4wine-lib/main.h>

class MainWindow : public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT
	public:
		MainWindow(int startState, QWidget * parent = 0, Qt::WFlags f = 0);
		// Icon copy\cyt structure

	public slots:
		void messageReceived(const QString message) const;
		void xdgOpenUrl(QString url);

	private slots:

		//void StartDrug(QDragEnterEvent * event);
		void getWineProccessInfo(void);
		void CoreFunction_ResizeContent(int tabIndex);
		void menuMountImages_triggered ( QAction * action );
		void menuMountRecentImages_triggered ( QAction * action );

		void changeStatusText(QString text);

		/*
		 * Icon tray slots
		 */
		void trayIcon_Activate(QSystemTrayIcon::ActivationReason reason);

		/*
		 * Command buttons slots
		 */
		void cmdManagePrefixes_Click(void);
		void cmdCreateFake_Click(void);
		void cmdUpdateFake_Click(void);
		void cmdWinetricks_Click (void);
		void cmdTestWis_Click (void);
		void cmdClearFilter_Click (void);
		void cmdAppDBSearch_Click (void);

		/*
		 * Combobox slouts
		 */

		void cbPrefixes_Change (const QString currentIndexText);
		void txtIconFilter_textChanged(QString);

		/*
		 *Prefix tool bars action
		 */
		void prefixAdd_Click(void);
		void prefixDelete_Click(void);
		void prefixExport_Click(void);
		void prefixImport_Click(void);
		void prefixSettings_Click(void);

		/*
		 *Process action slots
		 */
		void processRenice_Click(void);
		void processKillSelected_Click(void);
		void processKillWine_Click(void);

		/*! \brief This slot request icon description.
		 *
		 * \param  item		an QListWidgetItem wich trigged an a slot
		 */
		void lstIcons_ItemClick(QString program, QString args, QString desc, QString console, QString desktop);

		/*! \brief This slot runs icon.
		 *
		 * \param  item		an QListWidgetItem wich trigged an a slot
		 */
		void lstIcons_ShowContextMenu(const QPoint & iPoint);

		/*! \brief This slot request programs icons by folder and\or prefix name.
		 *
		 * When user click on directorym then q4wine displays
		 * program icons belongs to this folder
		 * \param  item		an QTreeWidgetItem wich trigged an a slot
		 */
		void twPrograms_ItemClick(QTreeWidgetItem * item, int);
		void twPrograms_ShowContextMenu(const QPoint);

		/*
		 *Prefix table slots
		 */
		void tablePrefix_ShowContextMenu(const QPoint);
		void tablePrefix_UpdateContentList(const QModelIndex);
		void tableProc_ShowContextMenu(const QPoint point);
		void tableProc_UpdateContentList(const QModelIndex);

		/*
		 * Context menu slots
		 */
		//Directory context
		void winefileOpenIconDir_Click(void);
		void winefileOpenMountDir_Click(void);
		void winefileOpenPrefixDir_Click(void);
		void dirAdd_Click(void);
		void dirRename_Click(void);
		void dirDelete_Click(void);
		void dirUnmount_Click(void);
		void dirMountOther_Click(void);
		void dirConfigure_Click(void);
		void dirInstall_Click(void);
		void dirUninstall_Click(void);

		//Icon context
		void xdgOpenIconDir_Click(void);
		void xdgOpenMountDir_Click(void);
		void xdgOpenPrefixDir_Click(void);

		void iconMountOther_Click(void);
		void iconUnmount_Click(void);

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

		void startDrag();
		void startDrop(QList<QUrl> files);

	private:
		//! Custom Widgets
		//DragListWidget* lstIcons;
		std::auto_ptr<AppDBWidget> appdbWidget;

		//! This is need for libq4wine-core.so import;
		typedef void *CoreLibPrototype (bool);
			CoreLibPrototype *CoreLibClassPointer;
			std::auto_ptr<corelib> CoreLib;
		QLibrary libq4wine;

		//Classes
		Prefix db_prefix;
		Dir db_dir;
		Icon db_icon;
		Last_Run_Icon db_last_run_icon;
		Image db_image;

		//Update timer
		std::auto_ptr<QTimer> timer;

		// Tray icon
		std::auto_ptr<QSystemTrayIcon> trayIcon;

		std::auto_ptr<QMenu> trayIconMenu;
		std::auto_ptr<QMenu> images;
		std::auto_ptr<QMenu> menuProc;
		std::auto_ptr<QMenu> menuPrefix;
		std::auto_ptr<QMenu> menuDir;
		std::auto_ptr<QMenu> menuDirMount;
		std::auto_ptr<QMenu> menuIcon;
		std::auto_ptr<QMenu> menuIconVoid;
		std::auto_ptr<QMenu> menuIconMount;
		std::auto_ptr<QMenu> menuIconMountRecent;

		QList <QAction *> recentIconsList;

		void createTrayIcon();
		void setMeVisible(bool visible);

		std::auto_ptr<QAction> processKillSelected;
		std::auto_ptr<QAction> processKillWine;
		std::auto_ptr<QAction> processRefresh;
		std::auto_ptr<QAction> processRenice;

		// Prefix actions for context menu
		std::auto_ptr<QAction> prefixAdd;
		std::auto_ptr<QAction> prefixImport;
		std::auto_ptr<QAction> prefixExport;
		std::auto_ptr<QAction> prefixDelete;
		std::auto_ptr<QAction> prefixSettings;

		// Directories control for context menu
		std::auto_ptr<QAction> dirAdd;
		std::auto_ptr<QAction> dirRun;
		std::auto_ptr<QAction> dirRename;
		std::auto_ptr<QAction> dirDelete;
		std::auto_ptr<QAction> dirMountOther;
		std::auto_ptr<QAction> dirUnmount;
		std::auto_ptr<QAction> dirConfigure;
		std::auto_ptr<QAction> dirInstall;
		std::auto_ptr<QAction> dirUninstall;

		//OpenDir
		std::auto_ptr<QAction> xdgOpenIconDir;
		std::auto_ptr<QAction> xdgOpenPrefixDir;
		std::auto_ptr<QAction> xdgOpenMountDir;
		std::auto_ptr<QAction> winefileOpenIconDir;
		std::auto_ptr<QAction> winefileOpenPrefixDir;
		std::auto_ptr<QAction> winefileOpenMountDir;

		// Icons control for context menu
		//QAction *iconOpenDirWinefile;
		std::auto_ptr<QAction> iconMount;
		std::auto_ptr<QAction> iconUnmount;
		std::auto_ptr<QAction> iconMountOther;

		// Toolbars
		std::auto_ptr<QToolBar> procToolBar;
		std::auto_ptr<QToolBar> prefixToolBar;

		// Settings functions
		void createMenuActions(void);
		void createToolBarActions(void);
		void getSettings(void);

		void runAutostart(void);
		void CoreFunction_SetProcNicePriority(int priority, int pid);

	   /*! \brief This function updates all database connectd widgets to current state.
		*
		* \param  currentPrefix  Current user selected prefix id.
		*/
		void updateDtabaseConnectedItems(int currentPrefix = -1);

		//Events definition
		void resizeEvent (QResizeEvent);
		void clearTmp();

		std::auto_ptr<QSplitter> splitter;

	signals:
		void appdbWidget_startSearch(short int, QString);
		void showFolderContents(QString, QString, QString);

	protected:
		// Event filter
		bool eventFilter(QObject *obj, QEvent *event);
		void closeEvent(QCloseEvent *event);

		//Resource\theme loader
		QIcon loadIcon(QString iconName);

		QString HOME_PATH;
		QString ROOT_PATH;
		QString TEMP_PATH;
		QString PREFIX_EI_PATH;

		QString WINE_DEFAULT_PREFIX;
		QString DEFAULT_WINE_BIN, DEFAULT_WINE_SERVER, DEFAULT_WINE_LOADER, DEFAULT_WINE_LIBS;
		QString WRESTOOL_BIN, ICOTOOL_BIN;
		QString TAR_BIN, MOUNT_BIN, UMOUNT_BIN, SUDO_BIN, GUI_SUDO_BIN, NICE_BIN, RENICE_BIN, SH_BIN;
		QString CONSOLE_BIN, CONSOLE_ARGS;
		QString THEME_NAME;

		bool SHOW_TRAREY_ICON;
		bool _IS_TIMER_RUNNING;

};

#endif


