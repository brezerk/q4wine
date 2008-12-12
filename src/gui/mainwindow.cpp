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


#include <QtGui>
#include <QSqlDatabase>
#include <QSqlError>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlRelation>
#include <QSqlRelationalTableModel>

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

#include "mainwindow.h"

QTimer *timer = new QTimer();

MainWindow::MainWindow(QWidget * parent, Qt::WFlags f) : QMainWindow(parent, f){
	
	// GUI setup
	setupUi(this);
	
	setWindowTitle(tr("%1 :. Qt4 GUI for Wine v%2").arg(APP_NAME) .arg(APP_VERS));

	connect(mainExit, SIGNAL(triggered()), this, SLOT(menuMainExit()));
	
	// Getting settings from config file
	SettingGet();
	
	// Database check
	if (checkDb()){
		UpdateDatabaseConnectedItems();
	} else {
		QMessageBox::warning(this, tr("Database is corrupted..."), tr("Sorry, the database has a wrong structure."));
		statusBar()->showMessage(tr("Error: Database is corrupted"));
		return;
	}
	// Slots connections

	// Timer signal for getting proc info
	connect(timer, SIGNAL(timeout()), this, SLOT(GetProcProccessInfo()));

	connect(tbwGeneral, SIGNAL(currentChanged(int)), this, SLOT(ResizeContent(int)));
	connect(cmdManagePrefixes, SIGNAL(clicked()), this, SLOT(cmdManagePrefixes_Click()));
	connect(cmdCreateFake, SIGNAL(clicked()), this, SLOT(cmdCreateFake_Click()));
	connect(cmdUpdateFake, SIGNAL(clicked()), this, SLOT(cmdUpdateFake_Click()));
	
	
	// Signals commection for Icons and Folders 
	
	connect(twPrograms, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(twPrograms_ItemClicked(QTreeWidgetItem *, int)));
	
	connect(lstIcons, SIGNAL(itemDoubleClicked (QListWidgetItem *)), this, SLOT(lstIcons_ItemDoubleClicked(QListWidgetItem *)));
	connect(lstIcons, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(lstIcons_ItemClicked(QListWidgetItem *)));
	
	// Signals for updating toolbars
	connect(tableProc, SIGNAL(clicked(const QModelIndex &)), this, SLOT(UpdateProcListContent(const QModelIndex &)));
	connect(tablePrefix, SIGNAL(clicked(const QModelIndex &)), this, SLOT(UpdatePrefixListContent(const QModelIndex &)));

	// Init and connect SLOT & SIGNALS for context menus
	connect(tableProc, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(showProcContextMenu(const QPoint &)));
	connect(tablePrefix, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(tablePrefix_showContextMenu(const QPoint &)));
	
	connect(twPrograms, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(twPrograms_ShowContextMenu(const QPoint &)));
	
	connect(lstIcons, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(lstIcons_ShowContextMenu(const QPoint &)));
	
	
	//Main menu actions connection to slots
	connect(mainRun, SIGNAL(triggered()), this, SLOT(menuMainRun()));
	connect(mainPrograms, SIGNAL(triggered()), this, SLOT(menuMainPrograms()));
	connect(mainProcess, SIGNAL(triggered()), this, SLOT(menuMainProcess()));
	connect(mainSetup, SIGNAL(triggered()), this, SLOT(menuMainSetup()));	
	connect(mainPrefix, SIGNAL(triggered()), this, SLOT(menuMainPrefix()));
	connect(mainAbout, SIGNAL(triggered()), this, SLOT(menuMainAbout()));
	connect(mainAboutQt, SIGNAL(triggered()), this, SLOT(menuMainAboutQt()));
	connect(mainExportIcons, SIGNAL(triggered()), this, SLOT(menuMainExportIcons()));
	connect(mainOptions, SIGNAL(triggered()), this, SLOT(menuMainOptions()));
	connect(mainInstall, SIGNAL(triggered()), this, SLOT(menuMainInstall()));
	
	// Setting context menu policy
	tableProc->setContextMenuPolicy(Qt::CustomContextMenu);
	twPrograms->setContextMenuPolicy(Qt::CustomContextMenu);
	tablePrefix->setContextMenuPolicy(Qt::CustomContextMenu);

	// Creating actions for context menus & toolbars
	createMenus();
	createToolBars();
	
	// Enveropment path initialization
	HOME_PATH = QDir::homePath();
	ROOT_PATH = QDir::rootPath();
	TEMP_PATH = QDir::tempPath();
	
	WINE_DEFAULT_PREFIX.append(HOME_PATH);
	WINE_DEFAULT_PREFIX.append("/.wine");
	

	// Setting default IconsSize for lstIcons (Wine-Programm-Menu) nead for user settings
	lstIcons->setIconSize(QSize(32, 32));


	
	twPrograms->installEventFilter(this);
	lstIcons->installEventFilter(this);

	RunAutorunItems();
	
	
	createTrayIcon();
	
	

	return;
}

bool MainWindow::checkDb(){
	QSqlDatabase db = QSqlDatabase::database();
	
	QStringList tables = db.tables();
	if (!tables.contains("prefix", Qt::CaseInsensitive)){
		return FALSE;
	}
	if (!tables.contains("icon", Qt::CaseInsensitive)){
		return FALSE;
	}
	if (!tables.contains("dir", Qt::CaseInsensitive)){
		return FALSE;
	}
	
	return TRUE;
}

void MainWindow::lstIcons_ItemClicked(QListWidgetItem * item){

	/*
	 * This is function for selection icons, and displaying
	 * icon informationm like path and description
	 */
	
	if (!item)
		return;
	
	QTreeWidgetItem *treeItem;

	if (!twPrograms->currentItem()){
		lstIcons->clear();
		return;
	}

	treeItem = twPrograms->currentItem();
				
	QSqlQuery query;
	
	if (treeItem->parent()){
		query.exec(tr("SELECT name, desc FROM icon WHERE dir_id=(SELECT id FROM dir WHERE prefix_id=(SELECT id FROM prefix WHERE name=\"%1\") AND name=\"%2\") and name=\"%3\"").arg(treeItem->parent()->text(0)) .arg(treeItem->text(0)) .arg(item->text()));
	} else {
		query.exec(tr("SELECT name, desc FROM icon WHERE prefix_id=(SELECT id FROM prefix WHERE name=\"%1\") AND  dir_id ISNULL AND name=\"%2\"").arg(treeItem->text(0)) .arg(item->text()));
	}
	
	query.next();
	lblInfo->setText(tr("Program: %1<br> Description: %2").arg(query.value(0).toString()) .arg(query.value(1).toString()));
	
 	return;
}

void MainWindow::SettingGet(){
	/*
	 * Getting application settings 
	 */
	
	QSettings settings(APP_SHORT_NAME, "default");
	
	//qDebug()<<"Sett: "<<settings.fileName();
	
	settings.beginGroup("wine");
		DEFAULT_WINE_BIN=settings.value("WineBin").toString();
			SettingCheck(DEFAULT_WINE_BIN, tr("Can't find wine binary."));
		DEFAULT_WINE_SERVER=settings.value("ServerBin").toString();
			SettingCheck(DEFAULT_WINE_SERVER, tr("Can't find wine server binary."));
		DEFAULT_WINE_LOADER=settings.value("LoaderBin").toString();
			SettingCheck(DEFAULT_WINE_LOADER, tr("Can't find wine loader."));
		DEFAULT_WINE_LIBS=settings.value("WineLibs").toString();
			SettingCheck(DEFAULT_WINE_LIBS, tr("Can't find wine library directory."));
	settings.endGroup();
	
	settings.beginGroup("app");
	if (settings.value("showTrareyIcon").toInt()==1){
		SHOW_TRAREY_ICON=TRUE;
	} else {
		SHOW_TRAREY_ICON=FALSE;
	}
		
	THEME_NAME=settings.value("theme").toString();
	if (!THEME_NAME.isEmpty() and THEME_NAME!="Default")
		SettingCheck(THEME_NAME, tr("Can't find theme."));
	
	settings.endGroup();	
	
	
	settings.beginGroup("system");
		TAR_BIN=settings.value("tar").toString();
			SettingCheck(TAR_BIN, tr("Can't find tar binary."));
		MOUNT_BIN=settings.value("mount").toString();
			SettingCheck(MOUNT_BIN, tr("Can't find mount binary."));
		UMOUNT_BIN=settings.value("umount").toString();
			SettingCheck(UMOUNT_BIN, tr("Can't find umount binary."));
		SUDO_BIN=settings.value("sudo").toString();
			SettingCheck(SUDO_BIN, tr("Can't find sudo binary."));
		NICE_BIN=settings.value("nice").toString();
			SettingCheck(NICE_BIN, tr("Can't find nice binary."));
		RENICE_BIN=settings.value("renice").toString();
			SettingCheck(RENICE_BIN, tr("Can't find renice binary."));
		SH_BIN=settings.value("sh").toString();
			SettingCheck(RENICE_BIN, tr("Can't find sh binary."));
	settings.endGroup();
	
	settings.beginGroup("console");
		CONSOLE_BIN=settings.value("bin").toString();
		SettingCheck(CONSOLE_BIN, tr("Can't find console binary."));

		CONSOLE_ARGS=settings.value("args").toString();
	settings.endGroup();
	
	settings.beginGroup("icotool");
		WRESTOOL_BIN=settings.value("wrestool").toString();
			SettingCheck(WRESTOOL_BIN, tr("Can't find wrestool binary."));
		ICOTOOL_BIN=settings.value("icotool").toString();
			SettingCheck(ICOTOOL_BIN, tr("Can't find icotool binary."));
	settings.endGroup();
	
	
	settings.beginGroup("network");
	switch (settings.value("host").toInt()){
		case 0:
			proxy.setType(QNetworkProxy::NoProxy);
			QNetworkProxy::setApplicationProxy(proxy);
		break;
		case 1:
			proxy.setType(QNetworkProxy::HttpProxy);
			proxy.setHostName(settings.value("host").toString());
			proxy.setPort(settings.value("port").toInt());
			proxy.setUser(settings.value("user").toString());
			proxy.setPassword(settings.value("pass").toString());
			QNetworkProxy::setApplicationProxy(proxy);
		break;
		case 2:
			proxy.setType(QNetworkProxy::Socks5Proxy);
			proxy.setHostName(settings.value("host").toString());
			proxy.setPort(settings.value("port").toInt());
			proxy.setUser(settings.value("user").toString());
			proxy.setPassword(settings.value("pass").toString());
			QNetworkProxy::setApplicationProxy(proxy);
		break;
	}
	
	settings.endGroup();
	
	
	
	
	return;	
}

void MainWindow::SettingCheck(QString filePath, QString message){
	/*
	 * Cheking for correct file\directory path
	 */
	
	QFileInfo *file = new QFileInfo(filePath);
	
	if (!file->exists ()){
		QMessageBox::warning(this, tr("Warning"), tr("<p>%1</p><p>File or path not exists: \"%2\"</p>").arg(message).arg(filePath));
		statusBar()->showMessage(tr("Warning: \"%1\" use options dialog for fix").arg(message));
	}
	
	return;
}

void MainWindow::lstIcons_ItemDoubleClicked(QListWidgetItem * item){
	/*
	* This is function for getting icon settings and run associated program
	*/
	
	statusBar()->showMessage(tr("Starting programm..."));

	if (!twPrograms->currentItem()){
		lstIcons->clear();
		return;
	}

	QTreeWidgetItem *treeItem;
	treeItem = twPrograms->currentItem();
	QSqlQuery query;
	
	//QString execcmd, prefixid, runcmd, useconsole, cmdargs, override, winedebug, display, wrkdir, envargs;
	
	//Getting icon info
	if (treeItem->parent()){
		query.prepare("SELECT exec, prefix_id, useconsole, cmdargs, override, winedebug, display, wrkdir, desktop, nice FROM icon WHERE dir_id=(SELECT id FROM dir WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name) AND name=:dir_name) and name=:icon_name");
		query.bindValue(":prefix_name", treeItem->parent()->text(0));
		query.bindValue(":dir_name", treeItem->text(0));
		query.bindValue(":icon_name", item->text());
	} else {
		query.prepare("SELECT exec, prefix_id, useconsole, cmdargs, override, winedebug, display, wrkdir, desktop, nice FROM icon WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name) AND  dir_id ISNULL AND name=:icon_name");
		query.bindValue(":prefix_name", treeItem->text(0));
		query.bindValue(":icon_name", item->text());
	}
		
	query.exec();
	query.first();
	
	//Saving icon info
	
	ExecObject execObj;
	
	execObj.execcmd = query.value(0).toString();
	execObj.prefixid = query.value(1).toString();
	execObj.useconsole = query.value(2).toString();
	execObj.cmdargs = query.value(3).toString();
	execObj.override = query.value(4).toString();
	execObj.winedebug = query.value(5).toString();
	execObj.display = query.value(6).toString();
	execObj.wrkdir = query.value(7).toString();
	execObj.desktop = query.value(8).toString();
	execObj.nice = query.value(9).toString();

	query.clear();
	
	PrepareWineRunParams(execObj);
				
	return;
}

void MainWindow::PrepareWineRunParams(ExecObject execObj){
	/*
	 * Function prepare params for running with WineRun
	 */
	
	QSqlQuery query;
	
	query.prepare("SELECT path, wine_dllpath, wine_loader, wine_exec, wine_server FROM prefix WHERE id=:prefix_id");
	query.bindValue(":prefix_id", execObj.prefixid);
	query.exec();
	query.first();
	
	QString exec;
	QStringList args;
	QString envargs;
			
	if (execObj.useconsole == "1"){
		// If we gona use console output, so exec program is program specificed at CONSOLE global variable
		exec = CONSOLE_BIN;
			
		if (!CONSOLE_ARGS.isEmpty()){
			// If we have any conslope parametres, we gona preccess them one by one
			QStringList cons_args = CONSOLE_ARGS.split(" ");
			for (int i=0; i<cons_args.count(); i++){
				args.append(cons_args.at(i));
			}
		}
		
		args.append(SH_BIN);
	} else {
		exec = SH_BIN;
	}
		
	args.append("-c");
		
	if (!query.value(0).toString().isEmpty()){
		//If icon has prefix -- add to args
		envargs.append(tr(" WINEPREFIX=%1 ").arg(query.value(0).toString()));
	} else {
		//Else use default prefix
		envargs.append(tr(" WINEPREFIX=%1/.wine ").arg(HOME_PATH));	
	}
		
	if (!query.value(1).toString().isEmpty()){
		envargs.append(tr(" WINEDLLPATH=%1 ").arg(query.value(1).toString()));
	} else {
		envargs.append(tr(" WINEDLLPATH=%1 ").arg(DEFAULT_WINE_LIBS));
	}
		
	if (!query.value(2).toString().isEmpty()){
		envargs.append(tr(" WINELOADER=%1 ").arg(query.value(2).toString()));
	} else {
		envargs.append(tr(" WINELOADER=%1 ").arg(DEFAULT_WINE_LOADER));
	}

	if (!query.value(4).toString().isEmpty()){
		envargs.append(tr(" WINESERVER=%1 ").arg(query.value(4).toString()));
	} else {
		envargs.append(tr(" WINESERVER=%1 ").arg(DEFAULT_WINE_SERVER));
	}

	if (!execObj.override.isEmpty()){
		envargs.append(tr(" WINEDLLOVERRIDES=\"%1\" ").arg(execObj.override));
	}
		
	if (!execObj.winedebug.isEmpty() && execObj.useconsole == "1"){
		envargs.append(tr(" WINEDEBUG=%1 ").arg(execObj.winedebug));
	}
	
	if (!execObj.display.isEmpty()){
		envargs.append(tr(" DISPLAY=%1 ").arg(execObj.display));
	}

	QString exec_string = "";

	exec_string.append(envargs);

	if(!execObj.nice.isEmpty()){
		exec_string.append(NICE_BIN);
		exec_string.append(" -n ");
		exec_string.append(execObj.nice);
	}
	
	exec_string.append(" ");

	if (!query.value(3).toString().isEmpty()){
		exec_string.append(query.value(3).toString());
	} else {
		exec_string.append(DEFAULT_WINE_BIN);
	}

	exec_string.append(" ");

	if (!execObj.desktop.isEmpty()){
		exec_string.append(" explorer.exe /desktop=");
		exec_string.append(execObj.desktop);
	}

	exec_string.append(" ");
	exec_string.append(execObj.execcmd);
	exec_string.append(" ");
	exec_string.append(execObj.cmdargs);

	args.append(exec_string);
	
	query.clear();
	
	RunWineProgram(exec, args, execObj.wrkdir);
	return;
}

void MainWindow::RunWineProgram(QString exec, QStringList args, QString wrkdir){
	
	QProcess *proc;
	
	proc = new QProcess( this );
	if (!proc->startDetached( exec, args, wrkdir)){
		statusBar()->showMessage(tr("Error: can't start %1").arg(exec));
	} else {
		proc->waitForStarted ();
		statusBar()->showMessage(tr("Start: Done"));
	}

	return;
}

void MainWindow::twPrograms_ItemClicked(QTreeWidgetItem * item, int){

	/*
	 * This is check for root element, or not.
	 * If yes -- show root-level Icons.
	 * otherwise -- show children-level icons
	 */
	
	QListWidgetItem *iconItem;
	QSqlQuery query;
	
	lstIcons->clear();
	lblInfo->setText(tr("Program: <br> Description:"));
	
	if (!item)
		return;
	
	QStringList getData = SQL_getPrefixAndDirData(item);
		
	if (item->parent()){
		query.prepare("SELECT name, icon_path FROM icon WHERE prefix_id=:prefix_id and dir_id=:dir_id");
		query.bindValue(":prefix_id", getData.at(0));
		query.bindValue(":dir_id", getData.at(2));
		
	} else {	
		query.prepare("SELECT name, icon_path FROM icon WHERE prefix_id=:prefix_id and dir_id ISNULL");
		query.bindValue(":prefix_id", getData.at(0));
	}
	
	query.exec();
		
	while (query.next()) {
		iconItem = new QListWidgetItem(lstIcons, 0);
		iconItem->setText(query.value(0).toString());

		//Seting icon. If no icon or icon file not exists -- setting default
		if (query.value(1).toString().isEmpty()){
			
			iconItem->setIcon(loadIcon("data/exec_wine.png"));
		} else {
			if (QFile::exists (query.value(1).toString())){
			   iconItem->setIcon(QIcon(query.value(1).toString()));
			} else {

				if (query.value(1).toString()=="wineconsole"){
					iconItem->setIcon(loadIcon("data/wineconsole.png"));
				} else if (query.value(1).toString()=="regedit"){
					iconItem->setIcon(loadIcon("data/regedit.png"));
				} else if (query.value(1).toString()=="wordpad"){
					iconItem->setIcon(loadIcon("data/notepad.png"));
				} else if (query.value(1).toString()=="winecfg"){
					iconItem->setIcon(loadIcon("data/winecfg.png"));
				} else if (query.value(1).toString()=="uninstaller"){
					iconItem->setIcon(loadIcon("data/uninstaller.png"));
				} else if (query.value(1).toString()=="eject"){
					iconItem->setIcon(loadIcon("data/eject.png"));
				} else if (query.value(1).toString()=="explorer"){
					iconItem->setIcon(loadIcon("data/explorer.png"));
				} else {
					iconItem->setIcon(loadIcon("data/exec_wine.png"));
				}
			}
			
			// If icon cutted -- set icon disabled style
			
			// FIXME: It pice works fine, but we nead to usse pixmaps fo grayscale, not Qt::ItemIsEnabled flag....
			// FIXME: It is optional, i don't work on it until release... ;)
			
			//if ((getData.at(0)==iconBuffer.prefix_id) and (getData.at(2)==iconBuffer.dir_id)){
			//	if (iconBuffer.names.indexOf (query.value(0).toString())!=-1)
			//		iconItem->setFlags(!Qt::ItemIsEnabled);
			//}
		}
	}
	
	query.clear();	

	
	return;
}

void MainWindow::UpdateDatabaseConnectedItems(int currentPrefix){
	/*
		Function for updating objects content to database values
	*/
	
	QSqlQuery query;
	QSqlQuery subQuery;
	QTreeWidgetItem *prefixItem;
	QTreeWidgetItem *subPrefixItem;
	
	// Updating "Program Files" TreeView ptrefix root items
	twPrograms->clear();
	lstIcons->clear();
	query.exec("SELECT id,name FROM prefix ORDER BY id;");
	while (query.next()) {
		prefixItem = new QTreeWidgetItem(twPrograms);
		prefixItem->setText(0, tr("%1").arg(query.value(1).toString()));
		prefixItem->setIcon(0, loadIcon("data/wine.png"));
		prefixItem->setExpanded (TRUE);
		twPrograms->addTopLevelItem(&QTreeWidgetItem(prefixItem));
		subQuery.exec(tr("SELECT id,name FROM dir WHERE prefix_id=%1").arg(query.value(0).toString()));
		while (subQuery.next()){
			subPrefixItem = new QTreeWidgetItem(prefixItem, 0);
			subPrefixItem->setText(0, tr("%1").arg(subQuery.value(1).toString()));	
			subPrefixItem->setIcon(0, loadIcon("data/folder.png"));
		}
	}
	
	// Updating ComboBox prefix items
	cbPrefixes->clear();
			
	query.exec("SELECT id,name,version FROM prefix ORDER BY id;");
	while (query.next()) {
		cbPrefixes->addItem ( tr("%1").arg(query.value(1).toString()));
	}

	int curRows = 0, numRows = 0;
	numRows = tablePrefix->rowCount();
				
	query.exec("SELECT name,version,path FROM prefix ORDER BY id;");
	while (query.next()) {
		curRows++;
				
		if (curRows>numRows){
			tablePrefix->insertRow (numRows);
			numRows = tablePrefix->rowCount();
		}
							
		
			if (tablePrefix->item(curRows - 1, 0)){
			tablePrefix->item(curRows - 1, 0)->setText(query.value(0).toString());
			tablePrefix->item(curRows - 1, 1)->setText(query.value(1).toString());
			tablePrefix->item(curRows - 1, 2)->setText(query.value(2).toString());
		} else {
		
			QTableWidgetItem *newItem = new QTableWidgetItem(query.value(0).toString());
			tablePrefix->setItem(curRows - 1, 0, newItem);
			newItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
			newItem = new QTableWidgetItem(query.value(1).toString());
			tablePrefix->setItem(curRows - 1, 1, newItem);
			newItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
			newItem = new QTableWidgetItem(query.value(2).toString());
			tablePrefix->setItem(curRows - 1, 2, newItem);
			newItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
		}
	}


	numRows = tablePrefix->rowCount();
	if (numRows > curRows){
		for (int i=curRows; i <= numRows; i++)
			tablePrefix->removeRow(curRows);
	}

		
	if (currentPrefix > 0)
		cbPrefixes->setCurrentIndex (currentPrefix);

	return;	
}

void MainWindow::cmdManagePrefixes_Click(){
	/*
		Функция обработки соб╕тия нажатия кнопки сmdManagePrefixes
	*/
	
	tbwGeneral->setCurrentIndex (3);

	
	return;
}

void MainWindow::UpdateProcListContent(const QModelIndex){
	/*
		Функция обновления содержимого таблицы и состояния кнопок тулбаров при клике на таблицу tableProc
	*/
	GetProcProccessInfo();

	return;
}

void MainWindow::UpdatePrefixListContent(const QModelIndex){
	/*
		Функция обновления содержимого таблицы и состояния кнопок тулбаров при клике на таблицу tablePrefix
	*/

	// Устанавливаем доступность кнопок
	if (tablePrefix->currentRow()>=0){
		toolPrefixDelete->setEnabled(TRUE);
		toolPrefixImport->setEnabled(TRUE);
		toolPrefixExport->setEnabled(TRUE);
		toolPrefixSettings->setEnabled(TRUE);
		
		prefixDelete->setEnabled(TRUE);
		prefixImport->setEnabled(TRUE);
		prefixExport->setEnabled(TRUE);
		prefixSettings->setEnabled(TRUE);
		
	} else {
		toolPrefixDelete->setEnabled(FALSE);
		toolPrefixImport->setEnabled(FALSE);
		toolPrefixExport->setEnabled(FALSE);
		toolPrefixSettings->setEnabled(FALSE);
		
		prefixDelete->setEnabled(FALSE);
		prefixImport->setEnabled(FALSE);
		prefixExport->setEnabled(FALSE);
		prefixSettings->setEnabled(FALSE);
	}


	return;
}

void MainWindow::tablePrefix_showContextMenu(const QPoint){
	QMenu menu(this);
		menu.addAction(prefixAdd);
		menu.addSeparator();
		menu.addAction(prefixImport);
		menu.addAction(prefixExport);
		menu.addSeparator();
		menu.addAction(prefixDelete);
		menu.addSeparator();
		menu.addAction(prefixSettings);
		menu.exec(QCursor::pos());
	return;
}

void MainWindow::showProcContextMenu(const QPoint){
	/*
		Function of creation context menu
	*/
	
	QMenu menu(this);
		menu.addAction(killAct);
		menu.addSeparator();
		menu.addAction(killWineAct);
		menu.addSeparator();
		menu.addAction(reniceProc);
		menu.addSeparator();
		menu.addAction(refreshAct);
		menu.exec(QCursor::pos());
	return;

}

void MainWindow::twPrograms_ShowContextMenu(const QPoint){
	/*
		Custom context menu for twPrograms
	*/
	
	QTreeWidgetItem *treeItem;
	QSqlQuery query;
	treeItem = twPrograms->currentItem();
	
	if (!treeItem)
		return;
	
	if (treeItem->parent()){
		query.prepare("select cdrom_drive, cdrom_mount from prefix where name=:name");
		query.bindValue(":name", treeItem->parent()->text(0));
		query.exec();
		query.first();
	} else {
		query.prepare("select cdrom_drive, cdrom_mount from prefix where name=:name");
		query.bindValue(":name", treeItem->text(0));
		query.exec();
		query.first();
	}
	
	
	if (!treeItem->parent()){
		dirRename->setEnabled(FALSE);
		dirDelete->setEnabled(FALSE);
	} else {	
		dirRename->setEnabled(TRUE);
		dirDelete->setEnabled(TRUE);
	}
		
	QMenu menu(this);
		menu.addAction(dirAdd);
		menu.addSeparator();
		QMenu* mount;
		mount = menu.addMenu(tr("Mount iso..."));
		mount->addAction(dirMountDrive);
		mount->addAction(dirUnMountDrive);
		mount->addAction(dirMountOther);
		
		if ((!query.value(0).toString().isNull()) and (!query.value(1).toString().isNull())){
			dirMountDrive->setEnabled(TRUE);
			dirUnMountDrive->setEnabled(TRUE);
			dirMountDrive->setText(tr("mount [%1]").arg(query.value(0).toString()));
			dirUnMountDrive->setText(tr("umount [%1]").arg(query.value(1).toString()));
		} else {
			dirMountDrive->setEnabled(FALSE);
			dirUnMountDrive->setEnabled(FALSE);
			dirMountDrive->setText(tr("mount [none]"));
			dirUnMountDrive->setText(tr("umount [none]"));
		}
		
		if (!query.value(1).toString().isNull()){
			dirMountOther->setEnabled(TRUE);
		} else {
			dirMountOther->setEnabled(FALSE);
		}
		
		query.clear();
		
		menu.addAction(dirInstall);
		menu.addAction(dirUninstall);
		menu.addAction(dirConfigure);
		menu.addSeparator();
		menu.addAction(dirRename);
		menu.addSeparator();
		menu.addAction(dirDelete);
		menu.exec(QCursor::pos());
	return;
}

void MainWindow::lstIcons_ShowContextMenu(const QPoint){
	/*
		Function showing context menu
	*/
	
	QListWidgetItem * item;
	item = lstIcons->currentItem();

	
	QTreeWidgetItem *treeItem;
	treeItem = twPrograms->currentItem();
		if (!treeItem)
			return;
	
	QSqlQuery query;
	
		if (item){
			if (treeItem->parent()){
				query.prepare("SELECT image, mount FROM icon WHERE dir_id=(SELECT id FROM dir WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name) AND name=:dir_name) and name=:icon_name");
				query.bindValue(":prefix_name", treeItem->parent()->text(0));
				query.bindValue(":dir_name", treeItem->text(0));
				query.bindValue(":icon_name", item->text());
			} else {
				query.prepare("SELECT image, mount FROM icon WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name) AND  dir_id ISNULL AND name=:icon_name");
				query.bindValue(":prefix_name", treeItem->text(0));
				query.bindValue(":icon_name", item->text());
			}
			query.exec();
			query.first();
		}
	
		QMenu menu(this);
		menu.addAction(iconRun);
		menu.addSeparator();
		
		QMenu* mount;
		mount = menu.addMenu(tr("Mount iso..."));
		mount->addAction(iconMount);
		mount->addAction(iconUnmount);
		mount->addAction(iconMountOther);
		
		if (item){
			if ((!query.value(0).toString().isEmpty()) && (!query.value(1).toString().isEmpty())){
				iconMount->setEnabled(TRUE);
				iconUnmount->setEnabled(TRUE);
				iconMount->setText(tr("mount [%1]").arg(query.value(0).toString().split("/").last()));
				iconUnmount->setText(tr("umount [%1]").arg(query.value(1).toString()));
			} else {
				iconMount->setEnabled(FALSE);
				iconUnmount->setEnabled(FALSE);
				iconMount->setText(tr("mount [none]"));
				iconUnmount->setText(tr("umount [none]"));
			}
		
		if (!query.value(1).toString().isEmpty()){
			iconMountOther->setEnabled(TRUE);
		} else {
			iconMountOther->setEnabled(FALSE);
		}
		
		}
		
			
		query.clear();
		
		menu.addSeparator();
		menu.addAction(iconOptions);
		menu.addSeparator();
		menu.addAction(iconCut);
		menu.addAction(iconCopy);
		menu.addAction(iconPaste);
		menu.addAction(iconRename);
		menu.addAction(iconDelete);
		menu.addSeparator();
		menu.addAction(iconAdd);
		
		
		QListWidgetItem *iconItem;
		iconItem=lstIcons->currentItem();
	
		if(iconItem){
			iconRun->setEnabled(TRUE);
			iconOptions->setEnabled(TRUE);
			iconRename->setEnabled(TRUE);
			iconDelete->setEnabled(TRUE);
			iconCut->setEnabled(TRUE);
			iconCopy->setEnabled(TRUE);
			mount->setEnabled(TRUE);
		} else {
			iconRun->setEnabled(FALSE);
			iconOptions->setEnabled(FALSE);
			iconRename->setEnabled(FALSE);
			iconDelete->setEnabled(FALSE);	
			iconCut->setEnabled(FALSE);
			iconCopy->setEnabled(FALSE);
			mount->setEnabled(FALSE);
		}
		
		if (iconBuffer.names.count()>0){
			iconPaste->setEnabled(TRUE);
		} else {
			iconPaste->setEnabled(FALSE);
		}
					
		menu.exec(QCursor::pos());
	
	
	
	return;
}

void MainWindow::createTrayIcon(){
	trayIconMenu = new QMenu(this);
	trayIconMenu->addAction(mainRun);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(mainExportIcons);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(mainPrograms);
	trayIconMenu->addAction(mainProcess);
	trayIconMenu->addAction(mainSetup);
	trayIconMenu->addAction(mainPrefix);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(mainExit);
	
	//trayIconMenu->addAction(restoreAction);
	//trayIconMenu->addSeparator();
	//trayIconMenu->addAction(quitAction);

	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setContextMenu(trayIconMenu);
	
	QIcon icon = loadIcon("data/wine.png");
	
	trayIcon->setIcon(icon);
	setWindowIcon(icon);
	
	if (SHOW_TRAREY_ICON){
		trayIcon->show();
	} else {
		trayIcon->hide();
	}
	
}

void MainWindow::closeEvent(QCloseEvent *event){
	if (trayIcon->isVisible()) {
		hide();
		event->ignore();
	} else {
		QSqlDatabase db = QSqlDatabase::database();
		db.close();
	}
}


bool MainWindow::eventFilter(QObject *obj, QEvent *event){
	
	// twPrograms events
	
	if (obj == twPrograms) {
		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
		if (event->type() == QEvent::KeyRelease) {
			
			
			if ((keyEvent->key()==Qt::Key_Up) or (keyEvent->key()==Qt::Key_Down)){
				if (twPrograms->currentItem())
					twPrograms_ItemClicked(twPrograms->currentItem(), 0);
				return true;
			}
				

		}
		
		if (event->type() == QEvent::KeyPress){
			if ((keyEvent->key()==Qt::Key_Tab)){
				lstIcons->setFocus();
				if (lstIcons->currentItem())
					lstIcons_ItemClicked(lstIcons->currentItem());
				return true;
			}
			
			if (keyEvent->key()==Qt::Key_Delete){
				if (twPrograms->currentItem())
					contextDirDelete();
				return true;
			}
		}
		
		return false;
		
	} 
	
	// lstIcons events
	
	if (obj == lstIcons){
		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
		
		if (event->type() == QEvent::KeyRelease) {
			
			
			if ((keyEvent->key()==Qt::Key_Up) or (keyEvent->key()==Qt::Key_Down) or (keyEvent->key()==Qt::Key_Left) or (keyEvent->key()==Qt::Key_Right)){
				if (lstIcons->currentItem())
					lstIcons_ItemClicked(lstIcons->currentItem());
				return true;
			}
		}
		
		if (event->type() == QEvent::KeyPress){
			if ((keyEvent->key()==Qt::Key_Tab)){
				twPrograms->setFocus();
				return true;
			}
			
			if (keyEvent->key()==Qt::Key_Return){
				if (lstIcons->currentItem())
					lstIcons_ItemDoubleClicked(lstIcons->currentItem());
				return true;
			}
			
			if (keyEvent->key()==Qt::Key_Delete){
				if (lstIcons->currentItem())
					contextIconDelete();
				return true;
			}
		}
		
			return false;
	}
	
	return QMainWindow::eventFilter(obj, event);
}


void MainWindow::resizeEvent (QResizeEvent){
	/* 
		Function for hendle resize event: tableProc (см код)
	*/

	ResizeContent(1);
	ResizeContent(3);

	return;
}

void MainWindow::ResizeContent(int tabIndex){
	/*
		Function for hendle resize event

		@screen -- tab id
	*/

	switch (tabIndex){
		case 0:
			// Stopping timer for reading /proc
			timer->stop();
		break;
		case 1:
			//Initiate /proc reading
			GetProcProccessInfo();
			// Starting timer for reading /proc
			timer->start(1000);

		/*	gbProcessList->setGeometry(3, 3, tbwGeneral->width()-gbControl->width()-12, tbwGeneral->height()-(gbProccesInfo->height()+33));

			gbControl->setGeometry(tbwGeneral->width()-gbControl->width()-7, 3, 142, gbProcessList->height());

			gbProccesInfo->setGeometry(3, tbwGeneral->height()-(gbProccesInfo->height()+30),  tbwGeneral->width()-10, gbProccesInfo->height());
		*/

			procToolBar->setFixedWidth (tbwGeneral->width()-10);

			tableProc->setColumnWidth (0, 60);
			tableProc->setColumnWidth (1, 120);
			tableProc->setColumnWidth (2, 40);
			tableProc->setColumnWidth (3, tableProc->width()-241);
		break;
		case 3:
			prefixToolBar->setFixedWidth (tbwGeneral->width()-10);

			tablePrefix->setColumnWidth (0, 120);
			tablePrefix->setColumnWidth (1, 120);
			tablePrefix->setColumnWidth (2, tablePrefix->width()-261);

			timer->stop();
		break;
	}
	
	return;
}

void MainWindow::SelectProcNicePriority(void){
	 bool ok;

	int rowNum;
	rowNum = tableProc->currentRow();

	if (rowNum>=0){
		int curNice;
		curNice = tableProc->item(rowNum, 2)->text().toInt();

		qDebug()<<tableProc->item(rowNum, 2)->text();

     int i = QInputDialog::getInteger(this, tr("Select process priority"),
                                      tr("<p>Priority value can be in<br>the range from PRIO_MIN (-20)<br>to PRIO_MAX (20).</p><p>See \"man renice\" for details.</p>"), curNice, -20, 20, 1, &ok);




     if (ok)
         SetProcNicePriority(i, tableProc->item(rowNum, 0)->text().toInt());
	}
}

void MainWindow::SetProcNicePriority(int priority, int pid){
	QStringList args;
	
	args << RENICE_BIN;
	args.append(tr("%1").arg(priority));
	args.append(tr("%1").arg(pid));
		
	Process *exportProcess = new Process(args, SUDO_BIN, HOME_PATH, tr("reniceing..."), tr("reniceing..."));
				
	if (exportProcess->exec()==QDialog::Accepted){
		GetProcProccessInfo();
	} else {
		statusBar()->showMessage(tr("Renice fail fail"));
	}

	return;
}

void MainWindow::GetProcProccessInfo(void){
	/*
		This net engine for getting proccess info from /proc directory
		its fully wrighter with QT, might work more stable =)
	*/

	QString name, procstat, path, prefix;;

	QDir dir("/proc");

	int curRows = 0, numRows = 0;
	double memSum;
	
	// Check for /proc..
	if (!dir.exists()){
		int ret = QMessageBox::warning(this, tr("Error"),
                   tr("Procces is unable access to /proc directory.\n"
							 "Access is necessary for displaying wine proccess information."
                      ),
                   QMessageBox::Retry ,
                   QMessageBox::Ignore);

		if (ret == QMessageBox::Ignore)
			timer->stop();
		return;
	}

	memSum = 0;
	numRows = tableProc->rowCount();

	dir.setFilter(QDir::Dirs | QDir::NoSymLinks);
	dir.setSorting(QDir::Name);

	QFileInfoList list = dir.entryInfoList();
	
	// Getting directoryes one by one
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		path = "/proc/";
		path.append(fileInfo.fileName());
		path.append("/stat");
		
		QFile file(path);
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
			QTextStream in(&file);
				QString line = in.readLine();
				if (!line.isNull()){
					QString nice;
					nice = line.section(' ', 18, 18);
					QString name;
					name = line.section(' ', 1, 1);
					name.remove(QChar('('));
					name.remove(QChar(')'));
					name = name.toLower();

					if ((name.contains("wine") || name.contains(".exe")) && !name.contains(APP_SHORT_NAME)){

						path = "/proc/";
						path.append(fileInfo.fileName());
						path.append("/environ");
						QFile e_file(path);
						
						if (e_file.open(QIODevice::ReadOnly | QIODevice::Text)){
							QTextStream e_in(&e_file);

							QString e_line = e_in.readLine();

							int index = e_line.indexOf("WINEPREFIX=");
							prefix="";
							if (index!=-1)
								for (int i=index+11; i<=e_line.length(); i++){
									if (e_line.mid(i, 1).data()->isPrint()){
										prefix.append(e_line.mid(i, 1));
									} else {
										break;
									}
								}
						}

						curRows++;
	
						if (curRows>numRows){
							tableProc->insertRow (numRows);
							numRows = tableProc->rowCount();
						}
	
						if (tableProc->item(curRows - 1, 0)){
							tableProc->item(curRows - 1, 0)->setText(fileInfo.fileName());
							tableProc->item(curRows - 1, 1)->setText(name);
							tableProc->item(curRows - 1, 2)->setText(nice);
							tableProc->item(curRows - 1, 3)->setText(prefix);
						} else {
							QTableWidgetItem *newItem = new QTableWidgetItem(fileInfo.fileName());
							tableProc->setItem(curRows - 1, 0, newItem);
							newItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
							newItem = new QTableWidgetItem(name);
							tableProc->setItem(curRows - 1, 1, newItem);
							newItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
							newItem = new QTableWidgetItem(nice);
							tableProc->setItem(curRows - 1, 2, newItem);
							newItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
							newItem = new QTableWidgetItem(prefix);
							tableProc->setItem(curRows - 1, 3, newItem);
							newItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
						}
					}
				}
			file.close();
		}
		
	}

	// Remove unneaded entyes
	numRows = tableProc->rowCount();
	if (numRows > curRows)
		for (int i=curRows; i <= numRows; i++)
			tableProc->removeRow(curRows);

	
	lblProcInfo->setText(tr("Total procces: %1").arg(numRows));

	// Setting buttons
	if (numRows>0){
		killWineAct->setEnabled(TRUE);
		killWineTool->setEnabled(TRUE);
		if (tableProc->currentRow()>=0){
			killWineProcTool->setEnabled(TRUE);
			killAct->setEnabled(TRUE);
			reniceProc->setEnabled(TRUE);
		} else {
			killWineProcTool->setEnabled(FALSE);
			killAct->setEnabled(FALSE);
			reniceProc->setEnabled(FALSE);
		}
	} else {
		reniceProc->setEnabled(FALSE);
		killWineAct->setEnabled(FALSE);
		killWineProcTool->setEnabled(FALSE);
		killWineTool->setEnabled(FALSE);
		killAct->setEnabled(FALSE);
	}
	
	return;
}

void MainWindow::cmdCreateFake_Click(){

	//Check if FakeDive exists?
	QString prefix_path;
	QSqlQuery query;
	query.prepare("SELECT path FROM prefix WHERE name=:name");
	query.bindValue(":name", cbPrefixes->currentText());
	query.exec();
	query.first();
	prefix_path = query.value(0).toString();
	query.clear();

	if (prefix_path.isEmpty())
		prefix_path = WINE_DEFAULT_PREFIX;

	QDir fake_dir (prefix_path);

	if (!fake_dir.exists()){
		QMessageBox::warning(this, tr("Error"), tr("Sorry, specified prefix %1 directory not exists.").arg(prefix_path), QMessageBox::Ok);
			return;
	}

	QString sysreg_path;
	sysreg_path.append(prefix_path);
	sysreg_path.append("/system.reg");

	QFile sysreg_file (sysreg_path);

	if (sysreg_file.exists()){
		if (QMessageBox::warning(this, tr("Warning"), tr("There are other Fake drive installed in %1 prefix directory.<br><br>Do you wish to REMOVE ALL FILES from prefix folder?").arg(prefix_path), QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes){
	
				QStringList args;
				args << "-rdf";
				args << prefix_path;

				Process *exportProcess = new Process(args, "/bin/rm", HOME_PATH, tr("Removing old fake drive.<br>This can take a while..."), tr("Removing old fake drive"));
			
				if (exportProcess->exec()!=QDialog::Accepted){
					return;
				}

				fake_dir.mkdir(prefix_path);

				Wizard *createFakeDriveWizard = new Wizard(2, cbPrefixes->currentText());
					if (createFakeDriveWizard->exec()==QDialog::Accepted){
						UpdateDatabaseConnectedItems(cbPrefixes->currentIndex());
					}
		}
	}



	return;
}


void MainWindow::cmdUpdateFake_Click(){

	//Check if FakeDive exists?
	QString prefix_path;
	QSqlQuery query;
	query.prepare("SELECT path FROM prefix WHERE name=:name");
	query.bindValue(":name", cbPrefixes->currentText());
	query.exec();
	query.first();
	prefix_path = query.value(0).toString();
	query.clear();

	if (prefix_path.isEmpty())
		prefix_path = WINE_DEFAULT_PREFIX;

	QDir fake_dir (prefix_path);

	if (!fake_dir.exists()){
		QMessageBox::warning(this, tr("Error"), tr("Sorry, specified prefix %1 directory not exists.").arg(prefix_path), QMessageBox::Ok);
			return;
	}

	Wizard *createFakeDriveWizard = new Wizard(3, cbPrefixes->currentText());
	if (createFakeDriveWizard->exec()==QDialog::Accepted){
		UpdateDatabaseConnectedItems(cbPrefixes->currentIndex());
	}

	return;
}

void MainWindow::cmdKillWineProccess(){
	// Фенкция убивания процесса вина (префикс таки обрабатывается =))
	int rowNum;
	rowNum = tableProc->currentRow();

	if (rowNum>=0){
		QString prefixPath;
		prefixPath = tableProc->item(rowNum, 3)->text();

		if (QMessageBox::warning(this, tr("Warning"), tr("This action will send a TREM(-9) signal to all wine proccess with WINEPREFIX='%1'<br><br>Do you realy whant it? ").arg(tableProc->item(rowNum, 3)->text()), QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes){
			QString command;

			if (!prefixPath.isEmpty())
				command=tr("env WINEPREFIX=\"%1\" wineserver -kill").arg(tableProc->item(rowNum, 3)->text());
			else
				command="wineserver -kill";
			system(command.toAscii().data());
		}
	}
	return;
}

void MainWindow::cmdKillSelectedProccess(){
	int rowNum;
	rowNum = tableProc->currentRow();

	if (rowNum>=0){
		QString procId;
		procId = tableProc->item(rowNum, 0)->text();

		if (QMessageBox::warning(this, tr("Warning"), tr("This action will send a TREM(-9) signal to proccess '%2' pid: %1<br><br>It is HIGH risk to damage wine normal state.<br><br>Do you realy whant it? ").arg(tableProc->item(rowNum, 0)->text()) .arg(tableProc->item(rowNum, 1)->text()),                   QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes){

			QString command;
			command="kill -9 ";
			command.append(procId);
			system(command.toAscii().data());
		}
	}
	return;
}

/*
 * Prefix commands
 */

void MainWindow::toolPrefixCreate_Clicked(){
	// Prefix creation function	

	Wizard *createPrefixWizard = new Wizard(0);
	if (createPrefixWizard->exec()==QDialog::Accepted){
		UpdateDatabaseConnectedItems();
	}
	
	return;
	
}

void MainWindow::toolPrefixDelete_Clicked(){

	if (tablePrefix->currentRow()>=0){
		if (tablePrefix->item(tablePrefix->currentRow(), 0)->text()=="Default"){
			QMessageBox::warning(this, tr("Error"),
         tr("Sorry, you can't delete Default prefix."), QMessageBox::Ok);
			return;
		}

		if(QMessageBox::warning(this, tr("Warning"),
         tr("Do you really wish to delete prefix named \"%1\" and all associated icons?").arg(tablePrefix->item(tablePrefix->currentRow(), 0)->text()), QMessageBox::Ok, QMessageBox::Cancel)==QMessageBox::Ok){
				
				QSqlQuery q;
					q.prepare("SELECT id FROM prefix WHERE name=:name;");
					q.bindValue(":name", tablePrefix->item(tablePrefix->currentRow(), 0)->text());
				q.exec();
				q.first();
								
				QString prefix_id=q.value(0).toString();
				
				q.clear();
				
				if (prefix_id.isEmpty())
					return;
				
				q.prepare("DELETE FROM prefix WHERE id=:prefix_id;");
				q.bindValue(":prefix_id", prefix_id);
				q.exec();
				q.clear();
				
				q.prepare("DELETE FROM dir WHERE prefix_id=:prefix_id;");
				q.bindValue(":prefix_id", prefix_id);
				q.exec();
				q.clear();
				
				q.prepare("DELETE FROM icon WHERE prefix_id=:prefix_id;");
				q.bindValue(":prefix_id", prefix_id);
				q.exec();
				q.clear();
				
		}
	}

	UpdateDatabaseConnectedItems();
	return;
}

void MainWindow::toolPrefixImport_Clicked(){
	/*
		Function for importing preefix from file
	*/
	
	QString openpath;
	
		//Getting user selected path for prefix store, if not -- use default
	if (!PREFIX_EI_PATH.isEmpty()){
		openpath.append(PREFIX_EI_PATH);
	} else {

			openpath.append(QDir::homePath());
			openpath.append("/.config/");
			openpath.append(APP_SHORT_NAME);
			openpath.append("/prefixes/");

	}
	
	QString fileName = QFileDialog::getOpenFileName(this, tr("Select file to import"), openpath , tr("Images (*.tbz)"));
	
	if (!fileName.isEmpty()){
		QDir dir;
		QString targetDir;
		
		if (tablePrefix->item(tablePrefix->currentRow(), 2)->text().isNull()){
			targetDir.clear();
			targetDir.append(HOME_PATH);
			targetDir.append("/.wine/");
		} else {
			targetDir.clear();
			targetDir.append(tablePrefix->item(tablePrefix->currentRow(), 2)->text());
		}
		
		if (dir.exists(targetDir)){
			if(QMessageBox::warning(this, tr("Warning"), tr("Do you really wish to delete all old prefix files?"), QMessageBox::Ok, QMessageBox::Cancel)==QMessageBox::Ok){


				QStringList args;
				args << "-rdf";
				args << targetDir;

				Process *exportProcess = new Process(args, "/bin/rm", HOME_PATH, tr("Removing old fake drive.<br>This can take a while..."), tr("Removing old fake drive"));
			
				if (exportProcess->exec()!=QDialog::Accepted){
					return;
				}

				/*
					QProgressDialog progress("Deleting files...", "Abort", -1, -1, this);
					progress.setWindowModality(Qt::WindowModal);
					progress.setValue(-1);
					DeleteDirectory(targetDir);
				*/
			} else { 
				return; 
			}
		}
		
			QStringList args;
			dir.mkdir(targetDir);
			
			args << "-xjf";
			args << fileName;
			args << "-C" << targetDir;
			
			//Creating process dialog
			Process *exportProcess = new Process(args, "/bin/tar", HOME_PATH, tr("Importing prefix.<br>This can take a while..."), tr("Importing prefix"));
	
			exportProcess->show();
			
		
	}
	
	return;
}

void MainWindow::DeleteDirectory(QString path){
	QDir dir(path);
	QFile file;
	dir.setFilter(QDir::Dirs | QDir::Files | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot );
	
	QFileInfoList list = dir.entryInfoList();
	
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		if ((fileInfo.isDir()) and (!fileInfo.isSymLink())){
				DeleteDirectory(fileInfo.filePath());
		} else {
			if (!file.remove(fileInfo.filePath()))
				qDebug()<<"cant kill:"<<fileInfo.filePath();
		}
	}
	
	if (!dir.rmdir(path))
		qDebug()<<"cant kill dir:"<<path;
	
	return;	
}

void MainWindow::toolPrefixSettings_Clicked(){
	
	// We gona get prefix_id and dir_id by calling SQL_getPrefixAndDirData
	
	PrefixSettings settings(tablePrefix->item(tablePrefix->currentRow(), 0)->text());
	if (settings.exec()==QDialog::Accepted){
		UpdateDatabaseConnectedItems();
	}
}

void MainWindow::toolPrefixExport_Clicked(){
	/*
		Function for exporting preefix to file
	*/
	
	QString savepath;
	
	//Getting user selected path for prefix store, if not -- use default
	if (!PREFIX_EI_PATH.isEmpty()){
		savepath.append(PREFIX_EI_PATH);
	} else {
		savepath.append(QDir::homePath());
		savepath.append("/.config/");
		savepath.append(APP_SHORT_NAME);
		savepath.append("/prefixes/");
	}
	
	//Creating save path
	savepath.append(tablePrefix->item(tablePrefix->currentRow(), 0)->text());
	savepath.append("-");
	savepath.append(QDate::currentDate().toString(Qt::ISODate));
	savepath.append(".tbz");
	
	//Request user for valid save path
	QString fileName = QFileDialog::getSaveFileName(this, tr("Select file to export"), savepath , tr("Images (*.tbz)"));
	
	if (!fileName.isEmpty()){
		QStringList args;
		
		args << "-cjf";
		args << fileName;
		args << "./";

		QString prefix_path = tablePrefix->item(tablePrefix->currentRow(), 2)->text();
		
		if (prefix_path.isEmpty()){
			prefix_path.clear();
			prefix_path.append(HOME_PATH);
			prefix_path.append("/.wine/");
		}
		
		//Creating process dialog
		Process *exportProcess = new Process(args, "/bin/tar", prefix_path, tr("Exporting %1 prefix.<br>This can take a while...").arg(tablePrefix->item(tablePrefix->currentRow(), 0)->text()), tr("Exporting prefix"));
	
		exportProcess->show();
	}
	
	return;
}

void MainWindow::menuMainExit(){
	/*
	 * main Menu Exit
	 */

	
	QSqlDatabase db = QSqlDatabase::database();
	db.close();
	
	qApp->quit();
	return;
}

void MainWindow::SetMeVisible(bool visible){
	setVisible(visible);
}

void MainWindow::menuMainPrograms(){
	/*
	* main Menu go Programs manage tool
	*/
	
	if (!isVisible())
		SetMeVisible(TRUE);
	
	if (isMinimized ())
		showNormal ();
	
	
	tbwGeneral->setCurrentIndex (0);
	return;
}

void MainWindow::menuMainProcess(){
	/*
	* main Menu go Process manage tool
	*/
	
	if (!isVisible())
		SetMeVisible(TRUE);
	
	if (isMinimized ())
		showNormal ();
	
	tbwGeneral->setCurrentIndex ( 1 );
	return;
}

void MainWindow::menuMainSetup(){
	/*
	* main Menu go Prefix Setup tool
	*/
	
	if (!isVisible())
		SetMeVisible(TRUE);
	
	if (isMinimized ())
		showNormal ();
	
	tbwGeneral->setCurrentIndex ( 2 );
	return;
}

void MainWindow::menuMainPrefix(){
	/*
	 * main Menu go Prefix manage tool
	 */
	
	if (!isVisible())
		SetMeVisible(TRUE);
	
	if (isMinimized ())
		showNormal ();
	
	tbwGeneral->setCurrentIndex ( 3 );
	return;
}

void MainWindow::menuMainAbout(){
	/*
	 * main Menu shows About dialog
	 */
	
	if (!isVisible())
		SetMeVisible(TRUE);
	
	if (isMinimized ())
		showNormal ();
	
	About *about = new About();
	about->exec();
	return;
}

void MainWindow::menuMainRun(){
	/*
	 * main Menu shows Run dialog
	 */
	
	
	QTreeWidgetItem *treeItem;
	treeItem = twPrograms->currentItem();
	QStringList dataList;
		
	
	if (treeItem){
		
		// We gona get prefix_id and dir_id by calling SQL_getPrefixAndDirData
		dataList = SQL_getPrefixAndDirInfo(treeItem);
		if (dataList.at(0)=="-1")
			return;
	} else {
		dataList << "" << "" << "";	
	}
	
	
	if (!isVisible())
		SetMeVisible(TRUE);
	
	if (isMinimized ())
		showNormal ();
	
	
	Run *run = new Run(dataList.at(0), dataList.at(1), dataList.at(2));
	
	if (run->exec()==QDialog::Accepted)
		PrepareWineRunParams(run->execObj);
	
	return;
}

void MainWindow::menuMainOptions(){
	/*
	 * main Menu shows About dialog
	 */
	
	AppSettings *options = new AppSettings();
	
	if (options->exec()==QDialog::Accepted){
		SettingGet();
	
		if (SHOW_TRAREY_ICON){
			trayIcon->show();
		} else {
			trayIcon->hide();
		}
		
	}
	return;
}

void MainWindow::menuMainAboutQt(){
	/*
	 * main Menu shows AboutQt dialog
	 */
	
	QMessageBox::aboutQt ( this );
	
	return;
}

void MainWindow::menuMainInstall(){
	/*
	* main Menu shows install Wizard dialog
	*/

	QMessageBox::warning(this, tr("WIP"), tr("Sorry, no install wizard yet. It'l  implemented at v0.110."));

	return;
}
	
void MainWindow::menuMainExportIcons(){
	/*
	* main Menu allow export icons
	*/
	
	if (!isVisible())
		SetMeVisible(TRUE);
	
	QString fileName, tmpDir;
	QStringList args;
	
	fileName = QFileDialog::getOpenFileName(this, tr("Open image file"), QDir::homePath(), tr("Win32 Executable (*.exe *.dll)") );

	if(fileName.isEmpty())
		return;

	args << "-x";
	args << "-t" << "14";
			
	QStringList list1 = fileName.split("/");
			
	tmpDir.clear();

	tmpDir.append(QDir::homePath());
	tmpDir.append("/.config/");
	tmpDir.append(APP_SHORT_NAME);
	tmpDir.append("/tmp/");
	tmpDir.append(list1.last());
	
	QDir tmp(tmpDir);
	tmp.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	QFileInfoList list = tmp.entryInfoList();
			
	if (tmp.exists(tmpDir)){
		for (int i = 0; i < list.size(); ++i) {
			QFileInfo fileInfo = list.at(i);
			if (!tmp.remove(fileInfo.filePath()))
				qDebug()<<"[EE] - Can't delete files at: "<<fileInfo.filePath();
		}
				
	} else {
		if (!tmp.mkdir(tmpDir)){
			qDebug()<<"[EE] - Can't create temp directory at: "<<tmpDir;
		}
	}
			
			
	args << "-o" << tmpDir;
	args << fileName;
					
	Process *exportProcess = new Process(args, WRESTOOL_BIN, HOME_PATH, tr("Exporting icon from binary file.<br>This can take a while..."), tr("Exporting icon"));
			
	if (exportProcess->exec()==QDialog::Accepted){
			//icotool -x -o ./regedit.png --width=32 --height=32 ./regedit.exe_14_100_0.ico
		

		
		args.clear();
		args << "-x";
		args << "-o" << tr("%1/").arg(tmpDir);
				
				// Updating file index
		list = tmp.entryInfoList();
				
		//Creating file list for converting
		for (int i = 0; i < list.size(); ++i) {
			QFileInfo fileInfo = list.at(i);
			if (fileInfo.fileName().right(3)=="ico")
				args << fileInfo.filePath();
		}
				

		//Look here, this function checks is some icons found, or not. 5 -- is default number of arguments,
		//if more -- then we have some ico file to convert
		if (args.size()>=4){
				
			exportProcess = new Process(args, ICOTOOL_BIN, HOME_PATH, tr("Convering icon from binary file.<br>This can take a while..."), tr("Converting icon"));
				
			if (exportProcess->exec()==QDialog::Accepted){
				IconsView *iconsView = new IconsView(tmpDir);
				iconsView->exec();
			}
				
		} else {
			IconsView *iconsView = new IconsView(tmpDir);
			iconsView->exec();
		}
	}
			
			//Clearing temp files
	list = tmp.entryInfoList();
				
				//Creating file list for converting
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		if (!QFile::remove(fileInfo.filePath()))
			qDebug()<<"[EE] - Can't delete files at: "<<fileInfo.filePath();
	}
			
	if (!tmp.rmdir(tmpDir))
		qDebug()<<"[EE] - Can't delete tmp dir: "<<tmpDir;

	return;
}

void MainWindow::createMenus(){
	// Собственно создаем и соединяем менюшки :)

	/*
	 * Context menu for process manage
	 */
	
	killAct = new QAction(tr("Stop current"), tableProc);
	killAct->setStatusTip(tr("Send TERM signal to selected procces"));
	connect(killAct, SIGNAL(triggered()), this, SLOT(cmdKillSelectedProccess()));

	killWineAct = new QAction(tr("Stop wine"), tableProc);
	killWineAct->setStatusTip(tr("Send TERM signal to main wine procces"));
	connect(killWineAct, SIGNAL(triggered()), this, SLOT(cmdKillWineProccess()));

	refreshAct = new QAction(tr("Refresh list"), tableProc);
	refreshAct->setStatusTip(tr("Refresh procces list"));
	connect(refreshAct, SIGNAL(triggered()), this, SLOT(GetProcProccessInfo()));

	reniceProc = new QAction(tr("Renice"), tableProc);
	reniceProc->setStatusTip(tr("Set procces priority"));
	connect(reniceProc, SIGNAL(triggered()), this, SLOT(SelectProcNicePriority()));

	/*
	 * Context menu for directory manage
	 */
		
	dirAdd = new QAction(tr("New"), twPrograms);
	dirAdd->setStatusTip(tr("Create new directory"));
	connect(dirAdd, SIGNAL(triggered()), this, SLOT(contextDirAdd()));

	dirRename = new QAction(tr("Rename"), twPrograms);
	dirRename->setStatusTip(tr("Rename current directory"));
	connect(dirRename, SIGNAL(triggered()), this, SLOT(contextDirRename()));
	
	dirDelete = new QAction(tr("Delete"), twPrograms);
	dirDelete->setStatusTip(tr("Delete current directory"));
	connect(dirDelete, SIGNAL(triggered()), this, SLOT(contextDirDelete()));
	
	dirMountDrive = new QAction(tr("Mount cd drive"), twPrograms);
	dirMountDrive->setStatusTip(tr("Mounts cdrom drive"));
	connect(dirMountDrive, SIGNAL(triggered()), this, SLOT(contextDirMountDrive()));
	
	dirUnMountDrive = new QAction(tr("Unmount cd drive"), twPrograms);
	dirUnMountDrive->setStatusTip(tr("Unmounts cdrom drive"));
	connect(dirUnMountDrive, SIGNAL(triggered()), this, SLOT(contextDirUnMountDrive()));
	
	dirMountOther = new QAction(tr("mount ..."), lstIcons);
	dirMountOther->setStatusTip(tr("Browse for other image"));
	connect(dirMountOther, SIGNAL(triggered()), this, SLOT(contextDirMountOther()));
	
	dirConfigure = new QAction(tr("Configure wine"), twPrograms);
	dirConfigure->setStatusTip(tr("Runs winecfg for current prefix"));
	connect(dirConfigure, SIGNAL(triggered()), this, SLOT(contextDirConfigure()));
	
	dirInstall = new QAction(tr("App install"), twPrograms);
	dirInstall->setStatusTip(tr("Runs application install wizard for current prefix"));
	connect(dirInstall, SIGNAL(triggered()), this, SLOT(contextDirInstall()));
	
	dirUninstall = new QAction(tr("App uninstall"), twPrograms);
	dirUninstall->setStatusTip(tr("Runs application uninstall wizard for current prefix"));
	connect(dirUninstall, SIGNAL(triggered()), this, SLOT(contextDirUnInstall()));
	
	/*
	 * Context menus for icon manage
	 */
	
	iconRun = new QAction(tr("Run"), lstIcons);
	iconRun->setStatusTip(tr("Create new icon"));
	connect(iconRun, SIGNAL(triggered()), this, SLOT(contextIconRun()));
	
	iconAdd = new QAction(tr("New"), lstIcons);
	iconAdd->setStatusTip(tr("Create new icon"));
	connect(iconAdd, SIGNAL(triggered()), this, SLOT(contextIconAdd()));
	
	iconCut = new QAction(tr("Cut"), lstIcons);
	iconCut->setStatusTip(tr("Cut selected icons to buffer"));
	connect(iconCut, SIGNAL(triggered()), this, SLOT(contextIconCut()));
	
	iconCopy = new QAction(tr("Copy"), lstIcons);
	iconCopy->setStatusTip(tr("Copy selected icons to buffer"));
	connect(iconCopy, SIGNAL(triggered()), this, SLOT(contextIconCopy()));

	iconPaste = new QAction(tr("Paste"), lstIcons);
	iconPaste->setStatusTip(tr("Paste selected icons from buffer to selected folder"));
	connect(iconPaste, SIGNAL(triggered()), this, SLOT(contextIconPaste()));

	
	
	iconRename = new QAction(tr("Rename"), lstIcons);
	iconRename->setStatusTip(tr("Rename current icon"));
	connect(iconRename, SIGNAL(triggered()), this, SLOT(contextIconRename()));
	
	iconDelete = new QAction(tr("Delete"), lstIcons);
	iconDelete->setStatusTip(tr("Delete current icon"));
	connect(iconDelete, SIGNAL(triggered()), this, SLOT(contextIconDelete()));
	
	iconOptions = new QAction(tr("Options"), lstIcons);
	iconOptions->setStatusTip(tr("Modify current icon options"));
	connect(iconOptions, SIGNAL(triggered()), this, SLOT(contextIconOption()));
	
	iconMount = new QAction(tr("mount"), lstIcons);
	iconMount->setStatusTip(tr("Mount image from icon options"));
	connect(iconMount, SIGNAL(triggered()), this, SLOT(contextIconMount()));
	
	iconUnmount = new QAction(tr("umount"), lstIcons);
	iconUnmount->setStatusTip(tr("Unmount image"));
	connect(iconUnmount, SIGNAL(triggered()), this, SLOT(contextIconUnmount()));
	
	iconMountOther = new QAction(tr("mount ..."), lstIcons);
	iconMountOther->setStatusTip(tr("Browse for other image"));
	connect(iconMountOther, SIGNAL(triggered()), this, SLOT(contextIconMountOther()));
	
	/*
	 * Context menus for prefix manage
	 */
	
	prefixAdd = new QAction(tr("New"), lstIcons);
	prefixAdd->setStatusTip(tr("Create new prefix"));
	connect(prefixAdd, SIGNAL(triggered()), this, SLOT(toolPrefixCreate_Clicked()));
	
	prefixImport = new QAction(tr("Import"), lstIcons);
	prefixImport->setStatusTip(tr("Import prefix"));
	connect(prefixImport, SIGNAL(triggered()), this, SLOT(toolPrefixImport_Clicked()));
	
	prefixExport = new QAction(tr("Export"), lstIcons);
	prefixExport->setStatusTip(tr("Export prefix"));
	connect(prefixExport, SIGNAL(triggered()), this, SLOT(toolPrefixExport_Clicked()));
	
	prefixDelete = new QAction(tr("Delete"), lstIcons);
	prefixDelete->setStatusTip(tr("Delete prefix"));
	connect(prefixDelete, SIGNAL(triggered()), this, SLOT(toolPrefixDelete_Clicked()));
	
	prefixSettings = new QAction(tr("Settings"), lstIcons);
	prefixSettings->setStatusTip(tr("Edit prefix settings"));
 	connect(prefixSettings, SIGNAL(triggered()), this, SLOT(toolPrefixSettings_Clicked()));
	
	return;
}

QIcon MainWindow::loadIcon(QString iconName){
	// Function tryes to load icon image from theme dir
	// If it fails -> load default from rsource file
	
	QIcon icon;
	
	if ((!THEME_NAME.isEmpty()) and (THEME_NAME!="Default")){
		icon.addFile(tr("%1/%2").arg(THEME_NAME).arg(iconName));
		if (icon.isNull()){
			icon.addFile(tr(":/%1").arg(iconName));
		}
	} else {
		icon.addFile(tr(":/%1").arg(iconName));
	}

	return icon;	
}

void MainWindow::createToolBars(){
	// Toolbar creation function
	
		procToolBar = new QToolBar(tlbProccess);

		killWineTool = procToolBar->addAction (loadIcon("data/stop.png"), tr("Send TERM signal to main wine procces"));
		killWineTool->setStatusTip(tr("Send TERM signal to main wine procces"));
		connect(killWineTool, SIGNAL(triggered()), this, SLOT(cmdKillWineProccess()));

		// По умолчанию кнопка не активна
		killWineTool->setEnabled(FALSE);

		killWineProcTool = procToolBar->addAction (loadIcon("data/kill.png"), tr("Send TERM signal to selected procces"));
		killWineProcTool->setStatusTip(tr("Send TERM signal to selected procces"));
		connect(killWineProcTool, SIGNAL(triggered()), this, SLOT(cmdKillSelectedProccess()));

		// По умолчанию кнопка не активна
		killWineProcTool->setEnabled(FALSE);

		procToolBar->addSeparator ();
	
		//FIXME: создать функцию симуляции перезагрузки вина
		rebootWineTool = procToolBar->addAction (loadIcon("data/reload.png"), tr("Initiate wineboot for selected prefix"));
		rebootWineTool->setStatusTip(tr("Initiate wineboot for selected prefix"));
		//connect(rebootWineTool, SIGNAL(triggered()), this, SLOT(cmdKillSelectedProccess()));
	
		procToolBar->addSeparator ();
	
		refreshWineProcTool = procToolBar->addAction (loadIcon("data/rebuild.png"), tr("Refresh procces list"));
		refreshWineProcTool->setStatusTip(tr("Refresh procces list"));
		connect(refreshWineProcTool, SIGNAL(triggered()), this, SLOT(GetProcProccessInfo()));

	// Тулбар на вкладке процессов
		prefixToolBar = new QToolBar(tlbPrefix);

		toolPrefixCreate = prefixToolBar->addAction (loadIcon("data/wizard.png"), tr("Create new prefix"));
		toolPrefixCreate->setStatusTip(tr("Create new prefix"));
		connect(toolPrefixCreate, SIGNAL(triggered()), this, SLOT(toolPrefixCreate_Clicked()));

		prefixToolBar->addSeparator ();

		toolPrefixImport = prefixToolBar->addAction (loadIcon("data/down.png"), tr("Import prefix"));
		toolPrefixImport->setStatusTip(tr("Import prefix"));
		connect(toolPrefixImport, SIGNAL(triggered()), this, SLOT(toolPrefixImport_Clicked()));

		toolPrefixImport->setEnabled(FALSE);

		toolPrefixExport = prefixToolBar->addAction (loadIcon("data/up.png"), tr("Export prefix"));
		toolPrefixExport->setStatusTip(tr("Export prefix"));
		connect(toolPrefixExport, SIGNAL(triggered()), this, SLOT(toolPrefixExport_Clicked()));

		toolPrefixExport->setEnabled(FALSE);

		prefixToolBar->addSeparator ();

		toolPrefixSettings = prefixToolBar->addAction (loadIcon("data/configure.png"), tr("Edit prefix settings"));
		toolPrefixSettings->setStatusTip(tr("Edit prefix settings"));
		connect(toolPrefixSettings, SIGNAL(triggered()), this, SLOT(toolPrefixSettings_Clicked()));

		toolPrefixSettings->setEnabled(FALSE);

		prefixToolBar->addSeparator ();

		toolPrefixDelete = prefixToolBar->addAction (loadIcon("data/kill.png"), tr("Delete prefix"));
		toolPrefixDelete->setStatusTip(tr("Delete prefix"));
		connect(toolPrefixDelete, SIGNAL(triggered()), this, SLOT(toolPrefixDelete_Clicked()));

		toolPrefixDelete->setEnabled(FALSE);

		
		
	return;
}


/****************************\
 *    CONTEXT MENU SLOTS    *
\****************************/


void MainWindow::contextIconDelete(void){
	/* 
		This function delete add selected icons
	*/
	
	QList<QListWidgetItem *> icoList;
	QTreeWidgetItem *treeItem;
	QSqlQuery query;
	
	QString prefix_id, dir_id, icon_name;
	
	// Getting items
	treeItem = twPrograms->currentItem();
	icoList = lstIcons->selectedItems();
	
//	qDebug()<<
	
	if (!treeItem)
		return;

	if (icoList.count()<0)
		return;

	if (QMessageBox::warning(this, tr("Delete Icon"), tr("Do you want to delete all selected icons?"),  QMessageBox::Yes, QMessageBox::No	)==QMessageBox::Yes){
		
		// We gona get prefix_id and dir_id by calling SQL_getPrefixAndDirData
		QStringList dataList;
		dataList = SQL_getPrefixAndDirData(treeItem);
		
		if (dataList.at(0)=="-1"){
			return;
		} else {
			prefix_id = dataList.at(0);
			dir_id = dataList.at(2);
		}
		
		
		
		for (int i=0; i<icoList.count(); i++){
			if(dir_id.isEmpty()){
				query.prepare("DELETE FROM icon WHERE name=:name and prefix_id=:prefix_id and dir_id ISNULL");
			}else{
				query.prepare("DELETE FROM icon WHERE name=:name and prefix_id=:prefix_id and dir_id=:dir_id");
				query.bindValue(":dir_id", dir_id);
			}
				
			query.bindValue(":prefix_id", prefix_id);
			query.bindValue(":name", icoList.at(i)->text());
			
			if (!query.exec()){
				#ifdef DEBUG
					qDebug()<<"WARNING: SQL error at MainWindow::contextIconDelete(void)\nINFO:\n"<<query.executedQuery()<<"\n"<<query.lastError();
				#endif
				return;
			}
			query.clear();
		}
		twPrograms_ItemClicked(treeItem, 0);
	}
	return;
}

void MainWindow::contextIconRun(void){
	
	QListWidgetItem *iconItem;
	iconItem=lstIcons->currentItem();
	if (iconItem)
		lstIcons_ItemDoubleClicked(iconItem);
	return;
}

void MainWindow::contextIconRename(void){
		
	QTreeWidgetItem *treeItem;
	QListWidgetItem *iconItem;
	QSqlQuery query;
	
	QString prefix_id, dir_id, icon_name;
	bool ok;
	
	treeItem = twPrograms->currentItem();
	iconItem = lstIcons->currentItem();
	
	if (!treeItem)
		return;

	if (!iconItem)
		return;
	
	// We gona get prefix_id and dir_id by calling SQL_getPrefixAndDirData
	QStringList dataList;
	dataList = SQL_getPrefixAndDirData(treeItem);
	if (dataList.at(0)=="-1"){
		return;
	} else {
		prefix_id = dataList.at(0);
		dir_id = dataList.at(2);
	}
	
	
	QString text = QInputDialog::getText(this, tr("Enter new icon name"), tr("Icon name:"), QLineEdit::Normal, iconItem->text(), &ok);
	
	if (ok && !text.isEmpty()){
			
		
		if(dir_id.isEmpty()){
			query.prepare("select id from icon where name=:name and prefix_id=:prefix_id and dir_id ISNULL");
		}else{
			query.prepare("select id from icon where name=:name and prefix_id=:prefix_id and dir_id=:dir_id");
			query.bindValue(":dir_id", dir_id);
		}
		
		query.bindValue(":name", text);
		query.bindValue(":prefix_id", prefix_id);
		
		if (!query.exec()){
			#ifdef DEBUG
				qDebug()<<"WARNING: SQL error at MainWindow::contextIconRename(void)\nINFO:\n"<<query.executedQuery()<<"\n"<<query.lastError();
			#endif
			return;
		}
		query.first();
		
		if (query.isValid ()){
			QMessageBox::warning(this, tr("Error"), tr("Sorry, but icon named %1 already exists.").arg(text));
			query.clear();
			return;
		}
		query.clear();
		
		if(dir_id.isEmpty()){
			query.prepare("UPDATE icon SET name=:new_name WHERE name=:name and prefix_id=:prefix_id and dir_id ISNULL");
		}else{
			query.prepare("UPDATE icon SET name=:new_name WHERE name=:name and prefix_id=:prefix_id and dir_id=:dir_id");
			query.bindValue(":dir_id", dir_id);
		}
			
		query.bindValue(":prefix_id", prefix_id);
		query.bindValue(":new_name", text);
		query.bindValue(":name", iconItem->text());
		
		if (!query.exec()){
			#ifdef DEBUG
				qDebug()<<"WARNING: SQL error at MainWindow::contextIconRename(void)\nINFO:\n"<<query.executedQuery()<<"\n"<<query.lastError();
			#endif
			return;
		}
		query.clear();
		
		// Updating icons view
		twPrograms_ItemClicked(treeItem, 0);
		
	}
	return;
}

void MainWindow::contextIconAdd(void){
	QTreeWidgetItem *treeItem;
	QSqlQuery query;
	
	//QString prefix_id, dir_id, prefix_dir;
	
	treeItem = twPrograms->currentItem();
	
	if (!treeItem)
		return;
	
	// We gona get prefix_id and dir_id by calling SQL_getPrefixAndDirData
	QStringList dataList;
	dataList = SQL_getPrefixAndDirInfo(treeItem);
	if (dataList.at(0)=="-1")
		return;
		
		IconSettings *iconAddWizard = new IconSettings(TRUE, dataList.at(0), dataList.at(1), dataList.at(6), "", dataList.at(2));
	if (iconAddWizard->exec() == QDialog::Accepted){
		// Updating icons view
		twPrograms_ItemClicked(treeItem, 0);
	}
	
	return;
}

void MainWindow::contextIconMountOther(void){
	/*
		This function request mount of selected by user image
	*/
	
	QStringList dataList = SQL_getPrefixAndDirData(twPrograms->currentItem());
	QSqlQuery query;
	
	if (!dataList.at(2).isEmpty()){
		query.prepare("SELECT mount FROM icon WHERE prefix_id=:prefix_id and dir_id=:dir_id and name=:name");
		query.bindValue(":dir_id", dataList.at(2));
	} else {
		query.prepare("SELECT mount FROM icon WHERE prefix_id=:prefix_id and dir_id ISNULL and name=:name");
	}
	
	query.bindValue(":prefix_id", dataList.at(0));
	query.bindValue(":name", lstIcons->currentItem()->text());
	query.exec();
	query.first();
	
	
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open ISO Image file"), HOME_PATH, tr("Iso files (*.iso)"));
	
	if(fileName.isEmpty()){
		return;
	}
	
	if (query.value(0).toString().isEmpty()){
		QMessageBox::warning(this, tr("Error"), tr("It seems no mount point was set in icon options.<br>You might need to set it manualy."));
		return;
	}
	
	MountImage(fileName, query.value(0).toString());
	
	
	return;
}

void MainWindow::contextIconUnmount(void){
	/*
		This function requests unmount by mount point described in icon settings
	*/
	
	QStringList dataList = SQL_getPrefixAndDirData(twPrograms->currentItem());
	QSqlQuery query;
	
	if (!dataList.at(2).isEmpty()){
		query.prepare("SELECT mount FROM icon WHERE prefix_id=:prefix_id and dir_id=:dir_id and name=:name");
		query.bindValue(":dir_id", dataList.at(2));
	} else {
		query.prepare("SELECT mount FROM icon WHERE prefix_id=:prefix_id and dir_id ISNULL and name=:name");
	}
	
	query.bindValue(":prefix_id", dataList.at(0));
	query.bindValue(":name", lstIcons->currentItem()->text());
	query.exec();
	query.first();
	
	if (query.value(0).toString().isEmpty()){
		QMessageBox::warning(this, tr("Error"), tr("It seems no mount point was set in icon options.<br>You might need to set it manualy."));
		return;
	}	
	
	UnMountImage(query.value(0).toString());
	
	return;
}

void MainWindow::contextIconMount(void){
	/*
		This function request mount of image from icon settings
	*/
	
	QStringList dataList = SQL_getPrefixAndDirData(twPrograms->currentItem());
	QSqlQuery query;
	
	if (!dataList.at(2).isEmpty()){
		query.prepare("SELECT image, mount FROM icon WHERE prefix_id=:prefix_id and dir_id=:dir_id and name=:name");
		query.bindValue(":dir_id", dataList.at(2));
	} else {
		query.prepare("SELECT image, mount FROM icon WHERE prefix_id=:prefix_id and dir_id ISNULL and name=:name");
	}
	
	query.bindValue(":prefix_id", dataList.at(0));
	query.bindValue(":name", lstIcons->currentItem()->text());
	query.exec();
	query.first();
	
	if (query.value(0).toString().isEmpty()){
		QMessageBox::warning(this, tr("Error"), tr("It seems no image file was set in icon options.<br>You might need to set it manualy."));
		return;
	}
	
	if (query.value(1).toString().isEmpty()){
		QMessageBox::warning(this, tr("Error"), tr("It seems no mount point was set in icon options.<br>You might need to set it manualy."));
		return;
	}
	
	MountImage(query.value(0).toString(), query.value(1).toString());
	
	return;
}

void MainWindow::contextIconCut(void){
	/*
		This function fill iconBuffer with selected icons names
		and sets other informations required for copy\cut
	
		see struct iconCopyBuffer definition for details
	*/
	
	QList<QListWidgetItem *> icoList;
	
	// If icon cutted -- set icon disabled style		
	// FIXME: It pice works fine, but we nead to usse pixmaps for grayscale, not Qt::ItemIsEnabled flag....
	// FIXME: It is optional, i don't work on it until release... ;)
	//icoList2 = lstIcons->findItems("*", Qt::MatchWrap | Qt::MatchWildcard);
	
	//	for (int i=0; i<icoList2.count(); i++){
	//		icoList2.at(i)->icon()->addPixmap(QPixmap.alphaChannel (), 0, 0);
	//	}
	
	icoList = lstIcons->selectedItems();
	
	// Clearing icon buffer
	iconBuffer.names.clear();
	iconBuffer.dir_id="";
	iconBuffer.prefix_id="";
	iconBuffer.move=false;
	
	// Fiffing buffer with new items
	for (int i=0; i<icoList.count(); i++){
		iconBuffer.names.append(icoList.at(i)->text());
		//icoList.at(i)->setFlags(Qt::ItemIsEnabled);
	}
	
	// Getting dir_id and prefix_id
	QStringList dataList = SQL_getPrefixAndDirData(twPrograms->currentItem());

	iconBuffer.prefix_id=dataList.at(0);
	iconBuffer.dir_id=dataList.at(2);
	iconBuffer.move=true;

	return;
}

void MainWindow::contextIconCopy(void){
	/*
	This function fill iconBuffer with selected icons names
	and sets other informations required for copy\cut
	
	see struct iconCopyBuffer definition for details
	*/
	
	QList<QListWidgetItem *> icoList = lstIcons->selectedItems();
	
	// Clearing icon buffer
	iconBuffer.names.clear();
	iconBuffer.dir_id="";
	iconBuffer.prefix_id="";
	iconBuffer.move=false;
	
	// Fiffing buffer with new items
	for (int i=0; i<icoList.count(); i++){
		iconBuffer.names.append(icoList.at(i)->text());
	}
	
	// Getting dir_id and prefix_id
	QStringList dataList = SQL_getPrefixAndDirData(twPrograms->currentItem());
	
	iconBuffer.prefix_id=dataList.at(0);
	iconBuffer.dir_id=dataList.at(2);
	iconBuffer.move=false;
		
	return;	
}

void MainWindow::contextIconPaste(void){
	QSqlQuery query;
	QStringList dataList = SQL_getPrefixAndDirData(twPrograms->currentItem());
	
	if (dataList.at(0)==iconBuffer.prefix_id and dataList.at(2)==iconBuffer.dir_id){
		QMessageBox::warning(this, tr("Error"), tr("Sorry. Cannot copy or move in to the same file"));
		return;
	}
	
	bool fexists=FALSE, ok;
	QString newName;
	
	if (iconBuffer.names.count()>0)
		switch (iconBuffer.move){
			case true:
				//Cut & Paste section
				for (int i=0; i<iconBuffer.names.count(); i++){
										
					
					// Checking for not unic names
					newName = iconBuffer.names.at(i);
					fexists=FALSE;
					ok=FALSE;
					fexists = SQL_isIconExistsByName(dataList.at(0), dataList.at(2), iconBuffer.names.at(i));
					
					while (fexists){
						newName = QInputDialog::getText(this, tr("Sorry. It seems file already exists."),
								tr("Sorry. It seems file already exists.<br>Please rename it, or cancel paste operation."), QLineEdit::Normal,
									iconBuffer.names.at(i) , &ok);
						
						if (!ok){
							return;
						}
						
							fexists = SQL_isIconExistsByName(dataList.at(0), dataList.at(2), newName);
					}
					
					// uPDATING RECCORD
					if (!iconBuffer.dir_id.isEmpty()){
						query.prepare("UPDATE icon SET name=:newName, prefix_id=:prefix_id, dir_id=:dir_id WHERE name=:name and prefix_id=:old_prefix_id and dir_id=:old_dir_id");
						query.bindValue(":old_dir_id", iconBuffer.dir_id);
					} else {
						query.prepare("UPDATE icon SET name=:newName, prefix_id=:prefix_id, dir_id=:dir_id WHERE name=:name and prefix_id=:old_prefix_id and dir_id ISNULL");
					}
					
					query.bindValue(":newName", newName);
					query.bindValue(":name", iconBuffer.names.at(i));
					query.bindValue(":prefix_id", dataList.at(0));
					
					if(dataList.at(2).isEmpty())
						query.bindValue(":dir_id", QVariant(QVariant::String));
					else
						query.bindValue(":dir_id", dataList.at(2));
					
					query.bindValue(":old_prefix_id", iconBuffer.prefix_id);
					
					if (!query.exec()){
						#ifdef DEBUG
							qDebug()<<"WARNING: SQL_getPrefixAndDirData\nINFO:\n"<<query.executedQuery()<<"\n"<<query.lastError();
						#endif
						return;
					}
					query.clear();
				}
				
				// Clearing icon buffer
				iconBuffer.names.clear();
				iconBuffer.dir_id="";
				iconBuffer.prefix_id="";
				iconBuffer.move=false;
			break;
			case false:
				//Copy & Paste section
				for (int i=0; i<iconBuffer.names.count(); i++){
					
										// Checking for not unic names
					newName = iconBuffer.names.at(i);
					fexists=FALSE;
					ok=FALSE;
					fexists = SQL_isIconExistsByName(dataList.at(0), dataList.at(2), iconBuffer.names.at(i));
					
					while (fexists){
						newName = QInputDialog::getText(this, tr("Sorry. It seems file already exists."),
								tr("Sorry. It seems file already exists.<br>Please rename it, or cancel paste operation."), QLineEdit::Normal,
									iconBuffer.names.at(i) , &ok);
						
						if (!ok){
							return;
						}
						
						fexists = SQL_isIconExistsByName(dataList.at(0), dataList.at(2), newName);
					}

					
					
					if (!iconBuffer.dir_id.isEmpty()){
						query.prepare("SELECT name, exec, cmdargs, icon_path, desc, image, mount, display, winedebug, useconsole, override, wrkdir FROM icon WHERE prefix_id=:prefix_id and dir_id=:dir_id and name=:name");
						query.bindValue(":dir_id", iconBuffer.dir_id);
					} else {
						query.prepare("SELECT name, exec, cmdargs, icon_path, desc, image, mount, display, winedebug, useconsole, override, wrkdir FROM icon WHERE prefix_id=:prefix_id and dir_id ISNULL and name=:name");
					}
					query.bindValue(":name", iconBuffer.names.at(i));
					query.bindValue(":prefix_id", iconBuffer.prefix_id);
					if (!query.exec()){
						#ifdef DEBUG
							qDebug()<<"WARNING: SQL_getPrefixAndDirData\nINFO:\n"<<query.executedQuery()<<"\n"<<query.lastError();
						#endif
						
						return;
					}
					
					if (!query.isValid()){
						// If query fails, exit and
						// Clearing icon buffer
						iconBuffer.names.clear();
						iconBuffer.dir_id="";
						iconBuffer.prefix_id="";
						iconBuffer.move=false;
						return;	
					}
						
					
					query.first();
					
					QStringList reccordBuffer;
					
					for (int j=0; j<=11; j++){
						reccordBuffer<<query.value(j).toString();
					}
					
					query.clear();
					
					
					query.prepare("INSERT INTO icon(name, exec, cmdargs, icon_path, desc, image, mount, display, winedebug, useconsole, override, prefix_id, dir_id, id, wrkdir) VALUES(:name, :exec, :cmdargs, :icon_path, :desc, :image, :mount, :display, :winedebug, :useconsole, :override, :prefix_id, :dir_id, NULL, :wrkdir)");
			
					if(dataList.at(2).isEmpty())
						query.bindValue(":dir_id", QVariant(QVariant::String));
					else
						query.bindValue(":dir_id", dataList.at(2));
		
					query.bindValue(":prefix_id", dataList.at(0));
					query.bindValue(":name", newName);
					query.bindValue(":exec", reccordBuffer.at(1));
					query.bindValue(":cmdargs", reccordBuffer.at(2));
					query.bindValue(":icon_path", reccordBuffer.at(3));
					query.bindValue(":desc", reccordBuffer.at(4));
					query.bindValue(":image", reccordBuffer.at(5));
					query.bindValue(":mount", reccordBuffer.at(6));
					query.bindValue(":display", reccordBuffer.at(7));
					query.bindValue(":winedebug", reccordBuffer.at(8));
					query.bindValue(":useconsole", reccordBuffer.at(9));
					query.bindValue(":override", reccordBuffer.at(10));
					query.bindValue(":wrkdir", reccordBuffer.at(11));

					if (!query.exec()){
						#ifdef DEBUG
							qDebug()<<"WARNING: SQL_getPrefixAndDirData\nINFO:\n"<<query.executedQuery()<<"\n"<<query.lastError();
						#endif
						return;
					}
					
					query.clear();
				}
			break;
		}
		
		twPrograms_ItemClicked(twPrograms->currentItem(), 0);
	return;
}

void MainWindow::contextIconOption(void){
	QTreeWidgetItem *treeItem;
	QListWidgetItem *iconItem;
	QSqlQuery query;
	
	QString prefix_id, dir_id, prefix_dir;
	
	treeItem = twPrograms->currentItem();
	iconItem = lstIcons->currentItem();
	
	if (!treeItem)
		return;

	if (!iconItem)
		return;
	
	// We gona get prefix_id and dir_id by calling SQL_getPrefixAndDirData
	QStringList dataList;
	dataList = SQL_getPrefixAndDirData(treeItem);
	if (dataList.at(0)=="-1"){
		return;
	} else {
		prefix_id = dataList.at(0);
		prefix_dir = dataList.at(1);
		dir_id = dataList.at(2);
	}
		
	IconSettings *iconAddWizard = new IconSettings(FALSE, prefix_id, prefix_dir, dir_id, iconItem->text());
	if (iconAddWizard->exec() == QDialog::Accepted){
		// Updating icons view
		twPrograms_ItemClicked(treeItem, 0);
	}
	
	return;
}

void MainWindow::contextDirAdd(void){
	QSqlQuery query;
	QTreeWidgetItem *treeItem;
	QTreeWidgetItem *prefixItem;
	treeItem = twPrograms->currentItem();
	QString prefix_id;
	
	if (!treeItem->text(0).isEmpty()){
		bool ok;
		QString dirname = QInputDialog::getText(this, tr("Enter new directory name"), tr("Directory name:"), QLineEdit::Normal, "" , &ok);
		
		if (ok && !dirname.isEmpty()){
			
			QStringList getData;
			if (treeItem->parent()){
				getData = SQL_getDirctoryInfo(treeItem->parent()->text(0), dirname);
				if (getData.at(0)=="-1")
					return;
				prefixItem = new QTreeWidgetItem(treeItem->parent());
			} else {
				getData = SQL_getDirctoryInfo(treeItem->text(0), dirname);
				if (getData.at(0)=="-1")
					return;
				prefixItem = new QTreeWidgetItem(treeItem);
			}
			
			prefixItem->setText(0, dirname);
			prefixItem->setIcon(0, loadIcon("data/folder.png"));
			
			query.prepare("INSERT INTO dir(id, name, prefix_id) VALUES(NULL, :name, :prefix_id)");
			query.bindValue(":name", dirname);
			query.bindValue(":prefix_id", getData.at(0));
			query.exec();
			query.clear();
				
		}
	}
	return;
}

void MainWindow::contextDirUnMountDrive(void){
	/*
		Request for unmounting cdrom drve described at wine prefix settings
	*/
	
	QStringList dataList = SQL_getPrefixAndDirData(twPrograms->currentItem());
	QSqlQuery query;
	
	query.prepare("select cdrom_mount from prefix where id=:id");
	query.bindValue(":id", dataList.at(0));
	if (!query.exec()){
		#ifdef DEBUG
			qDebug()<<"WARNING: contextDirMountDrive\nINFO:\n"<<query.executedQuery()<<"\n"<<query.lastError();
		#endif
		return;
	}
	
	query.first();
	UnMountImage(query.value(0).toString());
	query.clear();

	return;	
}

void MainWindow::contextDirMountDrive(void){
	/*
	Request for mounting cdrom drve described at wine prefix settings
	*/
		
	QStringList dataList = SQL_getPrefixAndDirData(twPrograms->currentItem());
	QSqlQuery query;
	
	query.prepare("select cdrom_drive, cdrom_mount from prefix where id=:id");
	query.bindValue(":id", dataList.at(0));
	if (!query.exec()){
#ifdef DEBUG
		qDebug()<<"WARNING: contextDirMountDrive\nINFO:\n"<<query.executedQuery()<<"\n"<<query.lastError();
#endif
		return;
	}
	
	query.first();
	MountImage(query.value(0).toString(), query.value(1).toString());
	query.clear();
	
	return;	
}

void MainWindow::contextDirMountOther(void){
	/*
	Request for unmounting cdrom drve described at wine prefix settings
	*/
	
	QStringList dataList = SQL_getPrefixAndDirData(twPrograms->currentItem());
	QSqlQuery query;
	
	query.prepare("select cdrom_mount from prefix where id=:id");
	query.bindValue(":id", dataList.at(0));
	if (!query.exec()){
#ifdef DEBUG
		qDebug()<<"WARNING: contextDirMountDrive\nINFO:\n"<<query.executedQuery()<<"\n"<<query.lastError();
#endif
		return;
	}
	
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open ISO Image file"), HOME_PATH, tr("Iso files (*.iso)"));
	
	if(fileName.isEmpty()){
		return;
	}

	
	query.first();
	MountImage(fileName, query.value(0).toString());
	query.clear();
	
	return;	
}

void MainWindow::contextDirConfigure(void){
	//FIXME: What is this? ;)
	//RunWineUtils("winecfg", twPrograms->currentItem());
	return;	
}

void MainWindow::contextDirInstall(void){
	//FIXME: Пока нету визарда, надо бы создать
	//RunWineUtils("winecfg", twPrograms->currentItem());
	
	QMessageBox::warning(this, tr("WIP"), tr("Sorry, no install wizard yet. It'l implemented at v0.110."));
	
	return;	
}

void MainWindow::contextDirUnInstall(void){
	//FIXME: Переделать на новый класс winebinlauncher
	//RunWineUtils("uninstaller", twPrograms->currentItem());
	return;	
}



void MainWindow::RunWineUtils(QString util_name, QTreeWidgetItem *item){

/*	
	/
		Runs wine util for current prefix
	/

	qDebug()<<"WTF?!!";
	
	//FIXME: !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	QStringList dataList = SQL_getPrefixAndDirData(item);
	
	QStringList args;
	QString envargs;
	QProcess * proc;

	args.append("-c");
		//FIXME: Нафиг енв? убрать и перевести на Sh
	envargs.append("env ");
		
	if (!dataList.at(1).isEmpty()){
		//If icon has prefix -- add to args
		envargs.append(tr(" WINEPREFIX=%1 ").arg(dataList.at(1)));
	} else {
		//Else use default prefix
		envargs.append(tr(" WINEPREFIX=%1 ").arg(WINE_DEFAULT_PREFIX));	
	}
	
	args.append(tr("%1 %2").arg(envargs) .arg(util_name));
	
	proc = new QProcess( this );
	//FIXME: заменить на sh
	proc->startDetached( "/bin/bash" , args );
	
	return;
*/

}



void MainWindow::contextDirRename(void){
	QSqlQuery query;
	QTreeWidgetItem *treeItem;
	treeItem = twPrograms->currentItem();
	QString prefix_id;
	
	if (treeItem->parent()){
		bool ok;
		QString dirname = QInputDialog::getText(this, tr("Enter new name for directory"), tr("Directory name:"), QLineEdit::Normal, treeItem->text(0) , &ok);
		if (ok && !dirname.isEmpty() && dirname!=treeItem->text(0)){
			query.prepare("select id from prefix where name=:name");
			query.bindValue(":name", treeItem->parent()->text(0));
			query.exec();
			query.first();
			prefix_id=query.value(0).toString();
			query.clear();
			
			query.prepare("UPDATE dir SET name=:name WHERE prefix_id=:prefix_id and name=:oldname");
			query.bindValue(":name", dirname);
			query.bindValue(":prefix_id", prefix_id);
			query.bindValue(":oldname", treeItem->text(0));
			query.exec();
			query.clear();
			
			treeItem->setText(0, dirname);
		}
	}
	return;
}

void MainWindow::contextDirDelete(void){
	QSqlQuery query;
	QTreeWidgetItem *treeItem;
	treeItem = twPrograms->currentItem();
	QString prefix_id, dir_id;
	
	if (treeItem->parent()){
			
		if (QMessageBox::warning(this, tr("Q4Wine"), tr("Do you really wish delete folder named \"%1\" and all associated icons?\n").arg(twPrograms->currentItem()->text(0)),
			 QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes){
			
			query.prepare("select id from prefix where name=:name");
			query.bindValue(":name", treeItem->parent()->text(0));
			query.exec();
			query.first();
			prefix_id=query.value(0).toString();
			query.clear();
					
			query.prepare("select id from dir where name=:name and prefix_id=:prefix_id");
			query.bindValue(":prefix_id", prefix_id);
			query.bindValue(":name", treeItem->text(0));
			query.exec();
			query.first();
			dir_id=query.value(0).toString();
			query.clear();

			query.prepare("DELETE FROM icon WHERE prefix_id=:prefix_id and dir_id=:dir_id");
			query.bindValue(":prefix_id", prefix_id);
			query.bindValue(":dir_id", dir_id);
			query.exec();
			query.clear();
			
			query.prepare("DELETE FROM dir WHERE prefix_id=:prefix_id and name=:name");
			query.bindValue(":prefix_id", prefix_id);
			query.bindValue(":name", treeItem->text(0));
			query.exec();
			query.clear();
						
			treeItem->parent()->removeChild (treeItem);
			
			twPrograms_ItemClicked(twPrograms->currentItem(), 0);
		}
	}
	
	return;	
}

/**************************************\
|			Embeded SQL functions	    |
\**************************************/

QStringList MainWindow::SQL_getDirctoryInfo(QString prefix_name, QString dirname){
	
	QSqlQuery query;
	QStringList dataList;
	query.prepare("select id from prefix where name=:name");
	query.bindValue(":name", prefix_name);
	query.exec();
	query.first();
	dataList << query.value(0).toString();
	query.clear();
				
	query.prepare("select id from dir where name=:name and prefix_id=:prefix_id");
	query.bindValue(":name", dirname);
	query.bindValue(":prefix_id", dataList.at(0));
	query.exec();
	query.first();
				
	if (query.isValid ()){
		QMessageBox::warning(this, tr("Error"), tr("Sorry, but prefix already have directory named %1.").arg(dirname));
		query.clear();
		return QStringList ("-1");
	}
	query.clear();	
				
	return dataList;
	
}

QStringList MainWindow::SQL_getPrefixAndDirInfo(QTreeWidgetItem *treeItem){
	/*
	This function, gets prefix_id and dir_id info
	This pice of code repeats some times, so i decided to place it in embeded function
	
	@treeItem -- this is start item for discovery...
	
	It returns QStringList size of some elements; 
	
	0 : prefix_id
	1 : prefix_path
	2 : wine_dllpath
	3 : wine_loader
	4 : wine_server
	5 : wine_exec
	6 : dir_id
	
	Or 0 : '-1' at error;
	*/
		
	QSqlQuery query;
	QStringList dataList;
	
	if (treeItem->parent()){
		query.prepare("select id, path, wine_dllpath, wine_loader, wine_server, wine_exec from prefix where name=:name");
		query.bindValue(":name", treeItem->parent()->text(0));
		if (!query.exec()){
#ifdef DEBUG
			qDebug()<<"WARNING: SQL_getPrefixAndDirData\nINFO:\n"<<query.executedQuery()<<"\n"<<query.lastError();
#endif
			dataList	<< "-1";
			return dataList;
		}
		query.first();
		dataList << query.value(0).toString();
		dataList << query.value(1).toString();
		dataList << query.value(2).toString();
		dataList << query.value(3).toString();
		dataList << query.value(4).toString();
		dataList << query.value(5).toString();
		query.clear();
				
		query.prepare("select id from dir where name=:name and prefix_id=:prefix_id");
		query.bindValue(":name", treeItem->text(0));
		query.bindValue(":prefix_id", dataList.at(0));
		if (!query.exec()){
#ifdef DEBUG
			qDebug()<<"WARNING: SQL_getPrefixAndDirData\nINFO:\n"<<query.executedQuery()<<"\n"<<query.lastError();
#endif
			return dataList;
		}
		query.first();	
		dataList << query.value(0).toString();
		query.clear();
	} else {
		query.prepare("select id, path, wine_dllpath, wine_loader, wine_server, wine_exec from prefix where name=:name");
		query.bindValue(":name", treeItem->text(0));
		if (!query.exec()){
#ifdef DEBUG
			qDebug()<<"WARNING: SQL_getPrefixAndDirData\nINFO:\n"<<query.executedQuery()<<"\n"<<query.lastError();
#endif
			dataList	<< "-1";
			return dataList;
		}
		query.first();
		dataList << query.value(0).toString();
		dataList << query.value(1).toString();
		dataList << query.value(2).toString();
		dataList << query.value(3).toString();
		dataList << query.value(4).toString();
		dataList << query.value(5).toString();
		dataList << "";
		query.clear();
	}
	
	return dataList;
}

QStringList MainWindow::SQL_getPrefixAndDirData(QTreeWidgetItem *treeItem){
	/*
		This function, gets prefix_id and dir_id
		This pice of code repeats some times, so i decided to place it in embeded function
	
		@treeItem -- this is start item for discovery...
	
		It returns QStringList size of 3 elements; 
	
			first : prefix_id
			second: prefix_path
			third : dir_id
	
		Or '-1','','-1' at error;
	*/
		
	QSqlQuery query;
	QStringList dataList;
	
	if (treeItem->parent()){
		query.prepare("select id, path from prefix where name=:name");
		query.bindValue(":name", treeItem->parent()->text(0));
		if (!query.exec()){
			#ifdef DEBUG
				qDebug()<<"WARNING: SQL_getPrefixAndDirData\nINFO:\n"<<query.executedQuery()<<"\n"<<query.lastError();
			#endif
			dataList	<< "-1" << "" << "-1";
			return dataList;
		}
		query.first();
		dataList << query.value(0).toString();
		dataList << query.value(1).toString();
		query.clear();
				
		query.prepare("select id from dir where name=:name and prefix_id=:prefix_id");
		query.bindValue(":name", treeItem->text(0));
		query.bindValue(":prefix_id", dataList.at(0));
		if (!query.exec()){
			#ifdef DEBUG
				qDebug()<<"WARNING: SQL_getPrefixAndDirData\nINFO:\n"<<query.executedQuery()<<"\n"<<query.lastError();
			#endif
			dataList	<< "-1" << "" << "-1";
			return dataList;
		}
		query.first();	
		dataList << query.value(0).toString();
		query.clear();
	} else {
		query.prepare("select id, path from prefix where name=:name");
		query.bindValue(":name", treeItem->text(0));
		if (!query.exec()){
			#ifdef DEBUG
				qDebug()<<"WARNING: SQL_getPrefixAndDirData\nINFO:\n"<<query.executedQuery()<<"\n"<<query.lastError();
			#endif
			dataList	<< "-1" << "" << "-1";
			return dataList;
		}
		query.first();
		dataList << query.value(0).toString();
		dataList << query.value(1).toString();
		dataList << "";
		query.clear();
	}
	
	return dataList;
}

bool MainWindow::SQL_isIconExistsByName(QString prefix_id, QString dir_id, QString name){
	QSqlQuery query;
	
	if(dir_id.isEmpty()){
		query.prepare("select id from icon where name=:name and prefix_id=:prefix_id and dir_id ISNULL");
	}else{
		query.prepare("select id from icon where name=:name and prefix_id=:prefix_id and dir_id=:dir_id");
		query.bindValue(":dir_id", dir_id);
	}
	query.bindValue(":name", name);
	query.bindValue(":prefix_id", prefix_id);
		
	if (!query.exec()){
		#ifdef DEBUG
			qDebug()<<"WARNING: SQL error at MainWindow::contextIconPaste(void)\nINFO:\n"<<query.executedQuery()<<"\n"<<query.lastError();
		#endif
		return FALSE;
	}
	query.first();
		
	if (query.isValid ()){
		query.clear();
		return TRUE;
	}
	query.clear();
	
	return FALSE;
}

void MainWindow::MountImage(QString image, QString mount){
	QStringList args;
	
	args << MOUNT_BIN;
	args << image;
	args << mount;
		
	if (image.right(3)=="iso")
		args << "-o" << "loop";
	
	Process *exportProcess = new Process(args, SUDO_BIN, HOME_PATH, tr("Mounting..."), tr("Mounting..."));
				
	if (exportProcess->exec()==QDialog::Accepted){
		statusBar()->showMessage(tr("Image successfully mounted"));
	} else {
		statusBar()->showMessage(tr("Image mount fail"));
	}
	return;
}

void MainWindow::UnMountImage(QString mount){
	
	QStringList args;
	
	args << UMOUNT_BIN;
	args << mount;
		

	Process *exportProcess = new Process(args, SUDO_BIN, HOME_PATH, tr("Mounting..."), tr("Mounting..."));
				
	if (exportProcess->exec()==QDialog::Accepted){
		statusBar()->showMessage(tr("Image successfully unmounted"));
	} else {
		statusBar()->showMessage(tr("Image unmount fail"));
	}
	
	return;
}

void MainWindow::RunAutorunItems(void){
	
	QSqlQuery query, icoQuery;
	QString execcmd, prefixid, runcmd, useconsole, cmdargs, override, winedebug, display, wrkdir, envargs;

	query.exec("select id, path, wine_dllpath, wine_loader, wine_exec, wine_server from prefix");
	
	while(query.next()){
		icoQuery.prepare("SELECT exec, prefix_id, useconsole, cmdargs, override, winedebug, display, wrkdir FROM icon WHERE dir_id=(SELECT id FROM dir WHERE prefix_id=:prefix_id and name=:name)");
		
		icoQuery.bindValue(":prefix_id", query.value(0).toString());
		icoQuery.bindValue(":name", "autostart");
		
		if (!icoQuery.exec()){
			#ifdef DEBUG
				qDebug()<<"WARNING: SQL_getPrefixAndDirData\nINFO:\n"<<icoQuery.executedQuery()<<"\n"<<icoQuery.lastError();
			#endif
			return;
		}
		
		while (icoQuery.next()){
			ExecObject execObj;
	
			execObj.execcmd = icoQuery.value(0).toString();
			execObj.prefixid = icoQuery.value(1).toString();
			execObj.useconsole = icoQuery.value(2).toString();
			execObj.cmdargs = icoQuery.value(3).toString();
			execObj.override = icoQuery.value(4).toString();
			execObj.winedebug = icoQuery.value(5).toString();
			execObj.display = icoQuery.value(6).toString();
			execObj.wrkdir = icoQuery.value(7).toString();
	
			PrepareWineRunParams(execObj);
			
		}
	
		icoQuery.clear();
		
	}
	
	query.clear();
	
	return;	
}
