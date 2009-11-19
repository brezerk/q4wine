/***************************************************************************
 *   Copyright (C) 2008, 2009 by Malakhov Alexey                           *
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
 *   In addition, as a special exception, the copyright holders give       *
 *   permission to link the code of this program with any edition of       *
 *   the Qt library by Trolltech AS, Norway (or with modified versions     *
 *   of Qt that use the same license as Qt), and distribute linked         *
 *   combinations including the two.  You must obey the GNU General        *
 *   Public License in all respects for all of the code used other than    *
 *   Qt.  If you modify this file, you may extend this exception to        *
 *   your version of the file, but you are not obligated to do so.  If     *
 *   you do not wish to do so, delete this exception statement from        *
 *   your version.                                                         *
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
#include <QObject>
#include <QSystemTrayIcon>
#include <QNetworkProxy>
#include <QWidget>
#include <QDir>
#include <QSizePolicy>
#include <QtGui>
#include <QSqlError>
#include <QStringList>

#include <QTimer>
#include <QTableWidget>
#include <QTabWidget>
#include <QLabel>
#include <QString>
#include <QMessageBox>
#include <QChar>
#include <QSize>
#include <QToolBar>
#include <QIcon>
#include <QTreeWidgetItem>
#include <QGroupBox>
#include <QVariant>

#include <QMimeData>
#include <QDrag>

#include <ui_MainWindow.h>

#include "config.h"

#include "src/core/database/prefix.h"
#include "src/core/database/dir.h"
#include "src/core/database/icon.h"
#include "src/core/database/last_run_icon.h"
#include "src/core/database/image.h"

#include "draglistwidget.h"
#include "appdbsearchwidget.h"

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

class QAction;
class QMenu;
class QTextEdit;

struct iconCopyBuffer {
	QString dir_name;
	QString prefix_name;
	bool move;
	QStringList names;
};


class MainWindow : public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT
	public:
		MainWindow(int startState, QWidget * parent = 0, Qt::WFlags f = 0);
		// Icon copy\cyt structure
		iconCopyBuffer iconBuffer;
	public slots:
		void messageReceived(const QString message) const;

	private slots:

		//void StartDrug(QDragEnterEvent * event);

		void getWineProccessInfo(void);
		void CoreFunction_ResizeContent(int tabIndex);
		void menuMountImages_triggered ( QAction * action );
		void menuMountRecentImages_triggered ( QAction * action );
		void menuRun_triggered ( QAction * action );

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
		void lstIcons_ItemClick(QListWidgetItem * item);

		/*! \brief This slot runs icon.
		 *
		 * \param  item		an QListWidgetItem wich trigged an a slot
		 */
		void lstIcons_ItemDoubleClick(QListWidgetItem * item);
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
		void iconAdd_Click(void);
		void iconRun_Click(void);
		void iconCut_Click(void);
		void iconCopy_Click(void);
		void iconPaste_Click(void);
		void iconRename_Click(void);
		void iconOption_Click(void);
		void iconDelete_Click(void);
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

		DragListWidget *lstIcons;

		//! This is need for libq4wine-core.so import;
		typedef void *CoreLibPrototype (bool);
			CoreLibPrototype *CoreLibClassPointer;
			corelib *CoreLib;
		QLibrary libq4wine;

		//Classes
		Prefix *db_prefix;
		Dir *db_dir;
		Icon *db_icon;
		Last_Run_Icon *db_last_run_icon;
		Image *db_image;



		// Proxy
		QNetworkProxy proxy;
		// Tray icon
		QSystemTrayIcon *trayIcon;

		QMenu *trayIconMenu;
		QMenu *images;
		QMenu *menuProc;
		QMenu *menuPrefix;
		QMenu *menuDir;
		QMenu *menuDirMount;
		QMenu *menuIcon;
		QMenu *menuIconVoid;
		QMenu *menuIconMount;
		QMenu *menuIconMountRecent;
		QMenu *menuIconXdgOpendir;
		QMenu *menuIconWineOpendir;

		QMenu *menuRun;
		QList <QAction *> recentIconsList;

		void createTrayIcon();
		void setMeVisible(bool visible);

		QAction *processKillSelected;
		QAction *processKillWine;
		QAction *processRefresh;
		QAction *processRenice;

		// Prefix actions for context menu
		QAction *prefixAdd;
		QAction *prefixImport;
		QAction *prefixExport;
		QAction *prefixDelete;
		QAction *prefixSettings;

		// Directories control for context menu
		QAction *dirAdd;
		QAction *dirRun;
		QAction *dirRename;
		QAction *dirDelete;
		QAction *dirMountOther;
		QAction *dirUnmount;
		QAction *dirConfigure;
		QAction *dirInstall;
		QAction *dirUninstall;

		//OpenDir
		QAction *xdgOpenIconDir;
		QAction *xdgOpenPrefixDir;
		QAction *xdgOpenMountDir;
		QAction *winefileOpenIconDir;
		QAction *winefileOpenPrefixDir;
		QAction *winefileOpenMountDir;

		// Icons control for context menu
		//QAction *iconOpenDirWinefile;
		QAction *iconRun;
		QAction *iconAdd;
		QAction *iconRename;
		QAction *iconDelete;
		QAction *iconOptions;
		QAction *iconCopy;
		QAction *iconCut;
		QAction *iconPaste;
		QAction *iconMount;
		QAction *iconUnmount;
		QAction *iconMountOther;

		// Toolbars
		QToolBar *procToolBar;
		QToolBar *prefixToolBar;

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

		QSplitter *splitter;
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


