/***************************************************************************
 *   Copyright (C) 2008 by Malakhov Alexey                                 *
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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H



#include <QMainWindow>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QTimer>
#include <QObject>
#include <QSystemTrayIcon>
#include <QNetworkProxy>
#include <QWidget>
#include <QMessageBox>
#include <QDir>

#include <ui_MainWindow.h>

#include "config.h"

#include "iconsview.h"
#include "wizard.h"
#include "process.h"
#include "winebinlauncher.h"
#include "iconsettings.h"
#include "prefixsettings.h"
#include "about.h"
#include "appsettings.h"
#include "run.h"

#include <stdlib.h>
#include <unistd.h>

class QAction;
class QMenu;
class QTextEdit;

struct iconCopyBuffer {
	QString dir_id;	
	QString prefix_id;
	bool move;
	QStringList names;
};


class MainWindow : public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT
	public:
		MainWindow(QWidget * parent = 0, Qt::WFlags f = 0);
			
		// Icon copy\cyt structure
		iconCopyBuffer iconBuffer;
		
		
		
	private slots:

		//FIXME: plz. normalize function names..
		void GetProcProccessInfo(void);
		void SelectProcNicePriority(void);
		void SetProcNicePriority(int priority, int pid);
		void cmdKillSelectedProccess(void);
		void cmdKillWineProccess(void);
		void cmdManagePrefixes_Click(void);

		void cmdCreateFake_Click(void);
		void cmdUpdateFake_Click(void);
		
		//FIXME: here to...
		void showProcContextMenu( const QPoint);
		
		void ResizeContent(int tabIndex);

		/*
		 *Prefix tool bars action
		 */
		void toolPrefixCreate_Clicked(void);
		void toolPrefixDelete_Clicked(void);
		void toolPrefixExport_Clicked(void);
		void toolPrefixImport_Clicked(void);
		void toolPrefixSettings_Clicked(void);

		//FIXME: PUT IN TO EVENT FILTER
		void UpdateProcListContent(const QModelIndex);
		void UpdatePrefixListContent(const QModelIndex);
		
		void lstIcons_ItemClicked(QListWidgetItem * item);
		void lstIcons_ItemDoubleClicked(QListWidgetItem * item);
		void lstIcons_ShowContextMenu(const QPoint);

		void twPrograms_ItemClicked(QTreeWidgetItem * item, int);
		void twPrograms_ShowContextMenu(const QPoint);
	
		void tablePrefix_showContextMenu(const QPoint);
		
		/*
		 * Context menu slots
		 */
		
			//Directory context
			void contextDirAdd(void);
			void contextDirRename(void);
			void contextDirDelete(void);
			void contextDirMountDrive(void);
			void contextDirUnMountDrive(void);
			void contextDirMountOther(void);
			void contextDirConfigure(void);
			void contextDirInstall(void);
			void contextDirUnInstall(void);
			
			//Icon context
			void contextIconAdd(void);
			void contextIconRun(void);
			void contextIconCut(void);
			void contextIconCopy(void);
			void contextIconPaste(void);
			void contextIconRename(void);
			void contextIconOption(void);
			void contextIconDelete(void);
			void contextIconMount(void);
			void contextIconMountOther(void);
			void contextIconUnmount(void);
		
			//Main menu slots
			void menuMainExit(void);
			void menuMainPrograms(void);
			void menuMainProcess(void);
			void menuMainSetup(void);
			void menuMainPrefix(void);
			void menuMainAbout(void);
			void menuMainAboutQt(void);
			void menuMainExportIcons(void);
			void menuMainRun(void);
			void menuMainOptions(void);
			void menuMainInstall(void);
			
	private:
		// Proxy
		QNetworkProxy proxy;
		// Tray icon
		QSystemTrayIcon *trayIcon;
		QMenu *trayIconMenu;
		
		void createTrayIcon();
		void SetMeVisible(bool visible);
		
		// Process List control context menu
		QAction *killAct;
		QAction *killWineAct;
		QAction *refreshAct;

		// Procces control option for toolbar
		QAction *killWineProcTool;
		QAction *killWineTool;
		QAction *reniceProc;
		QAction *rebootWineTool;
		QAction *refreshWineProcTool;

		// Prefix control option for toolbar
		QAction *toolPrefixCreate;
		QAction *toolPrefixExport;
		QAction *toolPrefixImport;
		QAction *toolPrefixDelete;
		QAction *toolPrefixSettings;

		// Directories control for context menu
		QAction *dirAdd;
		QAction *dirRename;
		QAction *dirDelete; 
		QAction *dirMountDrive;
		QAction *dirMountOther;
		QAction *dirUnMountDrive;
		QAction *dirConfigure;
		QAction *dirInstall;
		QAction *dirUninstall;
		
		// Icons control for context menu
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
		
		// Prefix actions for context menu
		QAction *prefixAdd;
		QAction *prefixImport;
		QAction *prefixExport;
		QAction *prefixDelete;
		QAction *prefixSettings;
		
		// Icons control for context menu
		QAction *icoAdd;
		QAction *icoRename;
		QAction *icoDelete;
		QAction *icoOption;
		
		// Toolbars
		QToolBar *procToolBar;
		QToolBar *prefixToolBar;

		// Settings functions
		
		void SettingGet(void);
		void SettingCheck(QString filePath, QString message);
		
	
		//Events definition
		void resizeEvent (QResizeEvent);
		void createMenus(void);
		void createToolBars(void);
		void UpdateDatabaseConnectedItems(int currentPrefix = -1);
		
		void MountImage(QString image, QString mount);
		void UnMountImage(QString mount);
		void DeleteDirectory(QString path);
		
		QStringList SQL_getPrefixAndDirData(QTreeWidgetItem *treeItem);
		QStringList SQL_getPrefixAndDirInfo(QTreeWidgetItem *treeItem);
		QStringList SQL_getDirctoryInfo(QString prefix_name, QString dirname);
		bool SQL_isIconExistsByName(QString prefix_id, QString dir_id, QString name);
		
		void RunWineUtils(QString util_name, QTreeWidgetItem *item);
		void RunAutorunItems(void);
		void RunWineProgram(QString exec, QStringList args, QString wrkdir);
		void PrepareWineRunParams(ExecObject execObj);
		
		bool checkDb();
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
		QString TAR_BIN, MOUNT_BIN, UMOUNT_BIN, SUDO_BIN, NICE_BIN, RENICE_BIN, SH_BIN;
		QString CONSOLE_BIN, CONSOLE_ARGS;
		QString THEME_NAME;
		bool SHOW_TRAREY_ICON;

};

#endif
