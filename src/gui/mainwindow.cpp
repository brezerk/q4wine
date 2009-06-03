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

#include "mainwindow.h"

QTimer *timer = new QTimer();


MainWindow::MainWindow(QWidget * parent, Qt::WFlags f) : QMainWindow(parent, f){
	// Loading libq4wine-core.so
	libq4wine.setFileName("libq4wine-core");

	if (!libq4wine.load()){
		libq4wine.load();
	}

	// Getting corelib calss pointer
	CoreLibClassPointer = (CoreLibPrototype *) libq4wine.resolve("createCoreLib");
	CoreLib = (corelib *)CoreLibClassPointer(true);

	// Creating database classes
	db_dir = new Dir();
	db_prefix = new Prefix();

	// Base GUI setup
	setupUi(this);
	setWindowTitle(tr("%1 :. Qt4 GUI for Wine v%2").arg(APP_NAME) .arg(APP_VERS));

	// Getting settings from config file
	this->getSettings();

	// Updating database connected items
	updateDtabaseConnectedItems();
	// Slots connections

	// Timer flag to running
	_IS_TIMER_RUNNING=TRUE;

        // Connecting signals and slots
	connect(timer, SIGNAL(timeout()), this, SLOT(getWineProccessInfo()));
	connect(tbwGeneral, SIGNAL(currentChanged(int)), this, SLOT(CoreFunction_ResizeContent(int)));
	connect(cmdManagePrefixes, SIGNAL(clicked()), this, SLOT(cmdManagePrefixes_Click()));
	connect(cmdCreateFake, SIGNAL(clicked()), this, SLOT(cmdCreateFake_Click()));
	connect(cmdUpdateFake, SIGNAL(clicked()), this, SLOT(cmdUpdateFake_Click()));
        connect(cmdWinetricks, SIGNAL(clicked()), this, SLOT(cmdWinetricks_Click()));
	connect(cmdTestWis, SIGNAL(clicked()), this, SLOT(cmdTestWis_Click()));

	// Signals commection for Icons and Folders

	connect(twPrograms, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(twPrograms_ItemClick(QTreeWidgetItem *, int)));
	connect(twPrograms, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(twPrograms_ShowContextMenu(const QPoint &)));
	connect(lstIcons, SIGNAL(itemDoubleClicked (QListWidgetItem *)), this, SLOT(lstIcons_ItemDoubleClick(QListWidgetItem *)));
	connect(lstIcons, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(lstIcons_ItemClick(QListWidgetItem *)));
	connect(lstIcons, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(lstIcons_ShowContextMenu(const QPoint &)));

	// Signals for updating toolbars
	connect(tableProc, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(tableProc_ShowContextMenu(const QPoint &)));
	connect(tableProc, SIGNAL(clicked(const QModelIndex &)), this, SLOT(tablePrefix_UpdateContentList(const QModelIndex &)));

	// Init and connect SLOT & SIGNALS for context menus
	connect(tablePrefix, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(tablePrefix_ShowContextMenu(const QPoint &)));
	connect(tablePrefix, SIGNAL(clicked(const QModelIndex &)), this, SLOT(tablePrefix_UpdateContentList(const QModelIndex &)));

	//Main menu actions connection to slots
	connect(mainRun, SIGNAL(triggered()), this, SLOT(mainRun_Click()));
	connect(mainPrograms, SIGNAL(triggered()), this, SLOT(mainPrograms_Click()));
	connect(mainProcess, SIGNAL(triggered()), this, SLOT(mainProcess_Click()));
	connect(mainSetup, SIGNAL(triggered()), this, SLOT(mainSetup_Click()));
	connect(mainPrefix, SIGNAL(triggered()), this, SLOT(mainPrefix_Click()));
	connect(mainImageManage, SIGNAL(triggered()), this, SLOT(mainImageManager_Click()));
	connect(mainAbout, SIGNAL(triggered()), this, SLOT(mainAbout_Click()));
	connect(mainAboutQt, SIGNAL(triggered()), this, SLOT(mainAboutQt_Click()));
	connect(mainExportIcons, SIGNAL(triggered()), this, SLOT(mainExportIcons_Click()));

        #ifndef WITH_ICOTOOLS
            mainExportIcons->setEnabled(false);
        #endif

        #ifdef WITH_DEVELOP_STUFF
            cmdTestWis->setEnabled(true);
        #else
            cmdTestWis->setEnabled(false);
        #endif

	connect(mainOptions, SIGNAL(triggered()), this, SLOT(mainOptions_Click()));
	connect(mainInstall, SIGNAL(triggered()), this, SLOT(mainInstall_Click()));
	connect(mainExit, SIGNAL(triggered()), this, SLOT(mainExit_Click()));

	// Setting context menu policy
	tableProc->setContextMenuPolicy(Qt::CustomContextMenu);
	twPrograms->setContextMenuPolicy(Qt::CustomContextMenu);
	tablePrefix->setContextMenuPolicy(Qt::CustomContextMenu);

	// Creating actions for context menus & toolbars
	CoreFunction_CreateMenus();
	CoreFunction_CreateToolBars();

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

        // FIXME: Move this into shared libaray
	CoreFunction_WineRunAutorunItems();

	createTrayIcon();

        // FIXME: Remove this as replace for shared library
	core = new CoreMethods();

	return;
}

void MainWindow::cmdTestWis_Click(){
	// test block
	WisItem wis;

	QString path = HOME_PATH;
	path.append("/.config/q4wine/sample.xml");

	wis = core->getWisInfo(path); // test
	  qDebug() << "it is name" << wis.name;
	  qDebug () << "it is download" << wis.download;
	return;
}

void MainWindow::cmdWinetricks_Click() {
	#ifndef WITH_WINETOOLS
	QMessageBox::warning(this, tr("Warning"), tr("<p>q4wine was compiled without winetriks support.</p><p>If you wish to enable winetriks support add:</p><p> \"-DWITH_WINETOOLS=ON\" to cmake arguments.</p>"));
	#else
	QMessageBox::warning(this, tr("Warning"), tr("<p>Winetricks officaly NOT supported by q4wine.</p><p>There was some repports about bugs, slows and errors on winetriks and q4wine usage at same time.</p>"));

	if (core->getSettingValue("console", "bin").isEmpty()){
		QMessageBox::warning(this, tr("Error"), tr("<p>You do not set default console binary.</p><p>Set it into q4wine option dialog.</p>"));
		return;
	}

	winetricks *w = new winetricks (cbPrefixes->currentText());
	w->exec();
	#endif
	return;
}

void MainWindow::trayIcon_Activate(QSystemTrayIcon::ActivationReason reason){
	if (reason==QSystemTrayIcon::Trigger){
		if (!isVisible()){
			SetMeVisible(TRUE);
		} else {
			SetMeVisible(FALSE);
		}
	}
	return;
}

void MainWindow::lstIcons_ItemClick(QListWidgetItem * item){

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

void MainWindow::getSettings(){
	/*
	 * Getting application settings
	 */
	QVariant val;

	val = CoreLib->getSetting("MainWindow", "size", false, QSize(400, 450));
	    this->resize(val.toSize());
	val = CoreLib->getSetting("MainWindow", "pos", false, QPoint(200, 200));
	    this->move(val.toPoint());

	val = CoreLib->getSetting("wine", "WineBin");
	    DEFAULT_WINE_BIN=val.toString();
	val = CoreLib->getSetting("wine", "ServerBin");
	    DEFAULT_WINE_SERVER=val.toString();
	val = CoreLib->getSetting("wine", "LoaderBin");
	    DEFAULT_WINE_LOADER=val.toString();
	val = CoreLib->getSetting("wine", "WineLibs");
	    DEFAULT_WINE_LIBS=val.toString();

	val = CoreLib->getSetting("app", "showTrareyIcon", false);
	    SHOW_TRAREY_ICON=val.toBool();

	val = CoreLib->getSetting("system", "tar");
	    TAR_BIN=val.toString();
	val = CoreLib->getSetting("system", "mount");
	    MOUNT_BIN=val.toString();
	val = CoreLib->getSetting("system", "umount");
	    UMOUNT_BIN=val.toString();
	val = CoreLib->getSetting("system", "sudo");
	    SUDO_BIN=val.toString();
	val = CoreLib->getSetting("system", "gui_sudo");
	    GUI_SUDO_BIN=val.toString();
	val = CoreLib->getSetting("system", "nice");
	    NICE_BIN=val.toString();
	val = CoreLib->getSetting("system", "renice");
	    RENICE_BIN=val.toString();
	val = CoreLib->getSetting("system", "sh");
	    SH_BIN=val.toString();

	val = CoreLib->getSetting("console", "bin");
	    CONSOLE_BIN=val.toString();
	val = CoreLib->getSetting("console", "args", false);
	    CONSOLE_ARGS=val.toString();

	#ifdef WITH_ICOTOOLS
	    val = CoreLib->getSetting("icotool", "wrestool");
		WRESTOOL_BIN=val.toString();
	    val = CoreLib->getSetting("icotool", "icotool");
		ICOTOOL_BIN=val.toString();
	#endif

	switch (CoreLib->getSetting("network", "type", false).toInt()){
	    case 0:
		proxy.setType(QNetworkProxy::NoProxy);
		QNetworkProxy::setApplicationProxy(proxy);
	    break;
	    case 1:
		proxy.setType(QNetworkProxy::HttpProxy);
		proxy.setHostName(CoreLib->getSetting("network", "host", false).toString());
		proxy.setPort(CoreLib->getSetting("network", "port", false).toInt());
		proxy.setUser(CoreLib->getSetting("network", "user", false).toString());
		proxy.setPassword(CoreLib->getSetting("network", "pass", false).toString());
		QNetworkProxy::setApplicationProxy(proxy);
	    break;
	    case 2:
		proxy.setType(QNetworkProxy::Socks5Proxy);
		proxy.setHostName(CoreLib->getSetting("network", "host", false).toString());
		proxy.setPort(CoreLib->getSetting("network", "port", false).toInt());
		proxy.setUser(CoreLib->getSetting("network", "user", false).toString());
		proxy.setPassword(CoreLib->getSetting("network", "pass", false).toString());
		QNetworkProxy::setApplicationProxy(proxy);
	    break;
	}
    return;
}

void MainWindow::lstIcons_ItemDoubleClick(QListWidgetItem * item){
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

	CoreFunction_WinePrepareRunParams(execObj);

	return;
}

void MainWindow::CoreFunction_WinePrepareRunParams(ExecObject execObj){
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
				if (!cons_args.at(i).isEmpty())
					args.append(cons_args.at(i));
			}
		}

		args.append(SH_BIN);

	} else {
		exec = SH_BIN;
	}

	args.append("-c");


	if ((execObj.useconsole == "1") && (!execObj.wrkdir.isNull())){
		envargs.append(" cd \"");
		envargs.append(execObj.wrkdir);
		envargs.append("\" ; ");
	}

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

	exec_string.append(" \"");
	exec_string.append(execObj.execcmd);
	exec_string.append("\" ");
	exec_string.append(execObj.cmdargs);

	args.append(exec_string);

	query.clear();

	CoreFunction_WineRunProgram(exec, args, execObj.wrkdir);
	return;
}

void MainWindow::CoreFunction_WineRunProgram(QString exec, QStringList args, QString wrkdir){

	QProcess *proc;

	proc = new QProcess( this );
	if (!proc->startDetached( exec, args, wrkdir)){
		statusBar()->showMessage(tr("Error: can't start %1").arg(exec));
	} else {
		statusBar()->showMessage(tr("Start: Done (For more info, run in console mode)"));
	}

	return;
}

void MainWindow::twPrograms_ItemClick(QTreeWidgetItem * item, int){

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

			iconItem->setIcon(CoreFunction_IconLoad("data/exec_wine.png"));
		} else {
			if (QFile::exists (query.value(1).toString())){
			   iconItem->setIcon(QIcon(query.value(1).toString()));
			} else {
				if (query.value(1).toString()=="wineconsole"){
					iconItem->setIcon(CoreFunction_IconLoad("data/wineconsole.png"));
				} else if (query.value(1).toString()=="regedit"){
					iconItem->setIcon(CoreFunction_IconLoad("data/regedit.png"));
				} else if (query.value(1).toString()=="wordpad"){
					iconItem->setIcon(CoreFunction_IconLoad("data/notepad.png"));
				} else if (query.value(1).toString()=="winecfg"){
					iconItem->setIcon(CoreFunction_IconLoad("data/winecfg.png"));
				} else if (query.value(1).toString()=="uninstaller"){
					iconItem->setIcon(CoreFunction_IconLoad("data/uninstaller.png"));
				} else if (query.value(1).toString()=="eject"){
					iconItem->setIcon(CoreFunction_IconLoad("data/eject.png"));
				} else if (query.value(1).toString()=="explorer"){
					iconItem->setIcon(CoreFunction_IconLoad("data/explorer.png"));
				} else {
					iconItem->setIcon(CoreFunction_IconLoad("data/exec_wine.png"));
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

void MainWindow::updateDtabaseConnectedItems(int currentPrefix){
	/*
		Function for updating objects content to database values
	*/

	QList<QStringList> result, subresult;

	int curRows = 0, numRows = 0;

	QTableWidgetItem *newItem;
	QTreeWidgetItem *prefixItem;
	QTreeWidgetItem *subPrefixItem;

	// Clearing widgets
	cbPrefixes->clear();
	twPrograms->clear();
	lstIcons->clear();

	result = db_prefix->getFields();
	for (int i = 0; i < result.size(); ++i) {
	    //If first element value "-1" -- then exit;
	    if (result.at(i).at(0) == "-1")
		return;

		// Inserting root items into programs tree view
		prefixItem = new QTreeWidgetItem(twPrograms);
		prefixItem->setText(0, tr("%1").arg(result.at(i).at(1)));
		prefixItem->setIcon(0, CoreFunction_IconLoad("data/wine.png"));
		prefixItem->setExpanded (TRUE);
		twPrograms->addTopLevelItem(prefixItem);

			// Inserting subfolders items into programs tree view
			subresult = db_dir->getFieldsByPrefixId(result.at(i).at(0));
			for (int j = 0; j < subresult.size(); ++j) {
				subPrefixItem = new QTreeWidgetItem(prefixItem, 0);
				subPrefixItem->setText(0, tr("%1").arg(subresult.at(j).at(1)));
				subPrefixItem->setIcon(0, CoreFunction_IconLoad("data/folder.png"));
			}

		// Inserting items into prefixes combo list
	    cbPrefixes->addItem (result.at(i).at(1));

		// Inserting items into prefixes table widget
		curRows++;
		numRows = tablePrefix->rowCount();

	    if (curRows>numRows){
		tablePrefix->insertRow (numRows);
		numRows = tablePrefix->rowCount();
	    }
	    if (tablePrefix->item(curRows - 1, 0)){
		tablePrefix->item(curRows - 1, 0)->setText(result.at(i).at(1));
		tablePrefix->item(curRows - 1, 1)->setText(result.at(i).at(2));
	    } else {
		newItem = new QTableWidgetItem(result.at(i).at(1));
		tablePrefix->setItem(curRows - 1, 0, newItem);
		newItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
		newItem = new QTableWidgetItem(result.at(i).at(2));
		tablePrefix->setItem(curRows - 1, 1, newItem);
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
	tbwGeneral->setCurrentIndex (3);
	return;
}

void MainWindow::tableProc_UpdateContentList(const QModelIndex){
	/*
		Function for updateing tableproc content and QAction status
	*/
	getWineProccessInfo();
	return;
}

void MainWindow::tablePrefix_UpdateContentList(const QModelIndex){
	/*
		Function for updateing tablePrefix content and QAction status
	*/

	if (tablePrefix->currentRow()>=0){
		prefixDelete->setEnabled(TRUE);
		prefixImport->setEnabled(TRUE);
		prefixExport->setEnabled(TRUE);
		prefixSettings->setEnabled(TRUE);
	} else {
		prefixDelete->setEnabled(FALSE);
		prefixImport->setEnabled(FALSE);
		prefixExport->setEnabled(FALSE);
		prefixSettings->setEnabled(FALSE);
	}
	return;
}

void MainWindow::tablePrefix_ShowContextMenu(const QPoint){
	menuPrefix->exec(QCursor::pos());
	return;
}

void MainWindow::tableProc_ShowContextMenu(const QPoint){
	/*
		Function of creation context menu
	*/

	menuProc->exec(QCursor::pos());
	return;

}

void MainWindow::twPrograms_ShowContextMenu(const QPoint){
	/*
		Custom context menu for twPrograms
	*/

	QTreeWidgetItem *treeItem;
	QSqlQuery query;
	treeItem = twPrograms->currentItem();

	twPrograms_ItemClick(treeItem, 0);

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

	QString mount_drive, mount_point;
	mount_drive = query.value(0).toString();
	mount_point = query.value(1).toString();
	query.clear();

	QMenu* menuDirMountImages;
	menuDirMountImages = new QMenu(this);

	if (!treeItem->parent()){
		dirRename->setEnabled(FALSE);
		dirDelete->setEnabled(FALSE);
	} else {
		dirRename->setEnabled(TRUE);
		dirDelete->setEnabled(TRUE);
	}

		QStringList arguments;

			#ifdef _OS_LINUX_
				arguments << "-c" << tr("%1 | grep %2").arg(MOUNT_BIN).arg(mount_point);
			#endif
			#ifdef _OS_FREEBSD_
				arguments << "-c" << tr("%1 | grep %2").arg(MOUNT_BIN).arg(mount_point);
			#endif

			QProcess *myProcess = new QProcess(this);
		myProcess->start(SH_BIN, arguments);

			if (!myProcess->waitForFinished()){
				qDebug() << "Make failed:" << myProcess->errorString();
				return;
			}
			//else
				QString out = myProcess->readAll();
					if (!out.isEmpty()){
						out = out.split(" ").first();
						if (!out.isEmpty()){
							#ifdef _OS_LINUX_
							if (out.contains("loop")){
							#endif
							#ifdef _OS_FREEBSD_
							if (out.contains("md")){
							#endif
								myProcess->close ();
								arguments.clear();
								#ifdef _OS_LINUX_
								arguments << "losetup" << out;
								#endif
								#ifdef _OS_FREEBSD_
								arguments << "mdconfig" <<  "-l" << tr("-u%1").arg(out.mid(7));
								#endif

								myProcess->start(SUDO_BIN, arguments);
									if (!myProcess->waitForFinished()){
										qDebug() << "Make failed:" << myProcess->errorString();
										return;
									} else {
										out = myProcess->readAll();
											if (!out.isEmpty()){
												#ifdef _OS_LINUX_
													out = out.split("/").last().split(")").first();
												#endif
												#ifdef _OS_FREEBSD_
													out = out.split("/").last().mid(0, out.split("/").last().length()-1);
												#endif
											}
									}
							}
						} else {
							out = "none";
						}
					} else {
						out = "none";
					}

			menuDirMount->clear();
			menuDirMountImages = menuDirMount->addMenu(tr("mount [%1]").arg(out));
			if (!mount_drive.isEmpty()){
				menuDirMountImages->addAction(QIcon(":/data/drive_menu.png"), mount_drive);
				menuDirMountImages->addSeparator();
			}

				query.exec("SELECT name FROM images ORDER BY name");
				while (query.next()) {
					menuDirMountImages->addAction(QIcon(":/data/cdrom_menu.png"), query.value(0).toString());
				}

			menuDirMount->addAction(dirUnmount);
			menuDirMount->addAction(dirMountOther);

			connect (menuDirMountImages, SIGNAL(triggered(QAction*)), this, SLOT(menuMountImages_triggered(QAction*)));

		if (!mount_point.isEmpty()){
			dirUnmount->setEnabled(TRUE);
			dirMountOther->setEnabled(TRUE);
			dirUnmount->setText(tr("umount [%1]").arg(mount_point));
		} else {
			dirUnmount->setEnabled(FALSE);
			dirMountOther->setEnabled(FALSE);
			dirUnmount->setText(tr("umount [none]"));
		}

		query.clear();

		menuDir->exec(QCursor::pos());
	return;
}

void MainWindow::menuMountImages_triggered ( QAction * action ){
	/*
	 * This slot process menuDirMountImages and menuIconMountImages triggered signal
	 */

	QSqlQuery query;
	QString fileName, mountPoint;

	if (action->text().contains("/dev/")){

		fileName = action->text();
	} else {
		query.prepare("SELECT path FROM images WHERE name=:name");
		query.bindValue(":name", action->text());
		query.exec();
		query.first();
		if (query.isValid()){
			fileName = query.value(0).toString();
		}
	}

		query.clear();

		QStringList dataList = SQL_getPrefixAndDirData(twPrograms->currentItem());
		query.prepare("select cdrom_mount from prefix where id=:id");
		query.bindValue(":id", dataList.at(0));
		if (!query.exec()){
			return;
		}

		query.first();

		if (query.value(0).toString().isEmpty()){
			QMessageBox::warning(this, tr("Error"), tr("It seems no mount point was set in prefix options.<br>You might need to set it manualy."));
			return;
		}

		mountPoint = query.value(0).toString();

		CoreFunction_ImageMount(fileName, mountPoint);

	return;
}

void MainWindow::lstIcons_ShowContextMenu(const QPoint){
	/*
		Function showing context menu
	*/

	QListWidgetItem * item;
	item = lstIcons->currentItem();

	QTreeWidgetItem *treeItem;
	QSqlQuery query;
	treeItem = twPrograms->currentItem();

	QString cdrom_drive, cdrom_mount;

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

		cdrom_drive = query.value(0).toString();
		cdrom_mount = query.value(1).toString();

	query.clear();

		QMenu* menuIconMountImages;
		menuIconMountImages = new QMenu(this);

			QStringList arguments;

			#ifdef _OS_LINUX_
				arguments << "-c" << tr("%1 | grep %2").arg(MOUNT_BIN).arg(cdrom_mount);
			#endif
			#ifdef _OS_FREEBSD_
				arguments << "-c" << tr("%1 | grep %2").arg(MOUNT_BIN).arg(cdrom_mount);
			#endif

			QProcess *myProcess = new QProcess(this);
		myProcess->start(SH_BIN, arguments);
			if (!myProcess->waitForFinished()){
				qDebug() << "Make failed:" << myProcess->errorString();
				return;
			}
			//else
				QString out = myProcess->readAll();
					if (!out.isEmpty()){
						out = out.split(" ").first();
						if (!out.isEmpty()){
							#ifdef _OS_LINUX_
							if (out.contains("loop")){
							#endif
							#ifdef _OS_FREEBSD_
							if (out.contains("md")){
							#endif
								myProcess->close ();
								arguments.clear();
								#ifdef _OS_LINUX_
								arguments << "losetup" << out;
								#endif
								#ifdef _OS_FREEBSD_
								arguments << "mdconfig" <<  "-l" << tr("-u%1").arg(out.mid(7));
								#endif
								myProcess->start(SUDO_BIN, arguments);
									if (!myProcess->waitForFinished()){
										qDebug() << "Make failed:" << myProcess->errorString();
										return;
									} else {
										out = myProcess->readAll();
										#ifdef _OS_LINUX_
											out = out.split("/").last().mid(0, out.split("/").last().length()-2);
										#endif
										#ifdef _OS_FREEBSD_
											out = out.split("/").last().mid(0, out.split("/").last().length()-1);
										#endif

									}
							}
						} else {
							out = "none";
						}
					} else {
						out = "none";
					}

			menuIconMount->clear();
			menuIconMountImages = menuIconMount->addMenu(tr("mount [%1]").arg(out));

			if (!cdrom_drive.isEmpty()){
				menuIconMountImages->addAction(QIcon(":/data/drive_menu.png"), cdrom_drive);
				menuIconMountImages->addSeparator();
			}

			menuIconMount->addAction(iconUnmount);
			menuIconMount->addAction(iconMountOther);

				query.exec("SELECT name FROM images ORDER BY name");
				while (query.next()) {
					menuIconMountImages->addAction(QIcon(":/data/cdrom_menu.png") ,query.value(0).toString());
				}
				query.clear();
			connect (menuIconMountImages, SIGNAL(triggered(QAction*)), this, SLOT(menuMountImages_triggered(QAction*)));

		if (item){
			if ((!cdrom_drive.isEmpty()) && (!cdrom_mount.isEmpty())){
				iconMount->setEnabled(TRUE);
				iconUnmount->setEnabled(TRUE);
				iconMount->setText(tr("mount [%1]").arg(cdrom_drive.split("/").last()));
				iconUnmount->setText(tr("umount [%1]").arg(cdrom_mount));
			} else {
				iconMount->setEnabled(FALSE);
				iconUnmount->setEnabled(FALSE);
				iconMount->setText(tr("mount [none]"));
				iconUnmount->setText(tr("umount [none]"));
			}

			if (!cdrom_drive.isEmpty()){
				iconMountOther->setEnabled(TRUE);
			} else {
				iconMountOther->setEnabled(FALSE);
			}

		}


		query.clear();

		QListWidgetItem *iconItem;
		iconItem=lstIcons->currentItem();

		if(iconItem){
			iconRun->setEnabled(TRUE);
			iconOptions->setEnabled(TRUE);
			iconRename->setEnabled(TRUE);
			iconDelete->setEnabled(TRUE);
			iconCut->setEnabled(TRUE);
			iconCopy->setEnabled(TRUE);
			menuIconMount->setEnabled(TRUE);
		} else {
			iconRun->setEnabled(FALSE);
			iconOptions->setEnabled(FALSE);
			iconRename->setEnabled(FALSE);
			iconDelete->setEnabled(FALSE);
			iconCut->setEnabled(FALSE);
			iconCopy->setEnabled(FALSE);
			menuIconMount->setEnabled(FALSE);
		}

		if (iconBuffer.names.count()>0){
			iconPaste->setEnabled(TRUE);
		} else {
			iconPaste->setEnabled(FALSE);
		}

		menuIcon->exec(QCursor::pos());

	  return;
}

void MainWindow::createTrayIcon(){
	trayIconMenu = new QMenu(this);
	trayIconMenu->addAction(mainRun);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(mainExportIcons);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(mainImageManage);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(mainPrograms);
	trayIconMenu->addAction(mainProcess);
	trayIconMenu->addAction(mainSetup);
	trayIconMenu->addAction(mainPrefix);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(mainExit);

	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setContextMenu(trayIconMenu);

	QIcon icon = CoreFunction_IconLoad("data/q4wine.png");

	trayIcon->setIcon(icon);
	setWindowIcon(icon);

	if (SHOW_TRAREY_ICON){
		trayIcon->show();
	} else {
		trayIcon->hide();
	}

	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIcon_Activate(QSystemTrayIcon::ActivationReason)));


}

void MainWindow::closeEvent(QCloseEvent *event){
	if (trayIcon->isVisible()) {
		hide();
		event->ignore();
	}

	QSettings settings(APP_SHORT_NAME, "default");
	settings.beginGroup("MainWindow");
	settings.setValue("size", size());
	settings.setValue("pos", pos());
	settings.endGroup();
}


bool MainWindow::eventFilter(QObject *obj, QEvent *event){

	// twPrograms events

	if (obj == twPrograms) {
		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
		if (event->type() == QEvent::KeyRelease) {


			if ((keyEvent->key()==Qt::Key_Up) or (keyEvent->key()==Qt::Key_Down)){
				if (twPrograms->currentItem())
					twPrograms_ItemClick(twPrograms->currentItem(), 0);
				return true;
			}


		}

		if (event->type() == QEvent::KeyPress){
			if ((keyEvent->key()==Qt::Key_Tab)){
				lstIcons->setFocus();
				if (lstIcons->currentItem())
					lstIcons_ItemClick(lstIcons->currentItem());
				return true;
			}

			if (keyEvent->key()==Qt::Key_Delete){
				if (twPrograms->currentItem())
					dirDelete_Click();
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
					lstIcons_ItemClick(lstIcons->currentItem());
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
					lstIcons_ItemDoubleClick(lstIcons->currentItem());
				return true;
			}

			if (keyEvent->key()==Qt::Key_Delete){
				if (lstIcons->currentItem())
					iconDelete_Click();
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

	CoreFunction_ResizeContent(1);
	CoreFunction_ResizeContent(3);

	return;
}

void MainWindow::CoreFunction_ResizeContent(int tabIndex){
	/*
		Function for hendle resize event

		@screen -- tab id
	*/

	switch (tabIndex){
		case 1:
			//Initiate /proc reading
                        if (_IS_TIMER_RUNNING){
				getWineProccessInfo();
				// Starting timer for reading /proc
				timer->start(1000);
			}

			tableProc->resizeRowsToContents();
			tableProc->resizeColumnsToContents();
			tableProc->horizontalHeader()->setStretchLastSection(TRUE);
		break;
		case 3:

			tablePrefix->resizeRowsToContents();
			tablePrefix->resizeColumnsToContents();
			tablePrefix->horizontalHeader()->setStretchLastSection(TRUE);

			// Stopping timer for reading /proc
                        if (_IS_TIMER_RUNNING)
				timer->stop();
		break;
		default:
			// Stopping timer for reading /proc
                        if (_IS_TIMER_RUNNING)
				timer->stop();
		break;
	}

	return;
}

void MainWindow::processRenice_Click(void){
	/*
	 *	Getting nice level from user
	 */

	bool ok;

	int rowNum;
	rowNum = tableProc->currentRow();

	if (rowNum>=0){
		int curNice;
		curNice = tableProc->item(rowNum, 2)->text().toInt();

     int i = QInputDialog::getInteger(this, tr("Select process priority"), tr("<p>Priority value can be in<br>the range from PRIO_MIN (-20)<br>to PRIO_MAX (20).</p><p>See \"man renice\" for details.</p>"), curNice, -20, 20, 1, &ok);

     if (ok)
	   CoreFunction_SetProcNicePriority(i, tableProc->item(rowNum, 0)->text().toInt());
	}
}

void MainWindow::CoreFunction_SetProcNicePriority(int priority, int pid){
	/*
	 * Core function for executing renice command
	 */

	QStringList args;
	QString arg;
	   args << RENICE_BIN;
	   args.append(tr("%1").arg(priority));
	   args.append(tr("%1").arg(pid));

	//Fix args for kdesu\gksu\e.t.c.
	if (!GUI_SUDO_BIN.contains(QRegExp("/sudo$"))){
	   arg=args.join(" ");
	   args.clear();
	   args<<arg;
	}

	Process *exportProcess = new Process(args, GUI_SUDO_BIN, HOME_PATH, tr("reniceing..."), tr("reniceing..."));
	if (exportProcess->exec()==QDialog::Accepted){
	   getWineProccessInfo();
	} else {
	   statusBar()->showMessage(tr("Renice fail fail"));
	}

	return;
}

void MainWindow::getWineProccessInfo(void){
        // If _RUN_TIMER==FALSE then timer is stopped by user
        if (!_IS_TIMER_RUNNING)
            return;

        QList<QStringList> proclist;
        int numRows = tableProc->rowCount();
        int curRows = 0;

        // Getting QList of QStringList which describes running wine processes
        proclist = CoreLib->getWineProcessList();

        // Preccess QList items one by one
        for (int i = 0; i < proclist.size(); ++i) {
            //If first element value "-1" -- then disable timer and set _IS_TIMER_RUNNING flag
            if (proclist.at(i).at(0) == "-1"){
                _IS_TIMER_RUNNING=false;
                timer->stop();
                return;
            }

            curRows++;

            if (curRows>numRows){
                tableProc->insertRow (numRows);
                numRows = tableProc->rowCount();
            }

            if (tableProc->item(curRows - 1, 0)){
                 tableProc->item(curRows - 1, 0)->setText(proclist.at(i).at(0));
                 tableProc->item(curRows - 1, 1)->setText(proclist.at(i).at(1));
                 tableProc->item(curRows - 1, 2)->setText(proclist.at(i).at(2));
                 tableProc->item(curRows - 1, 3)->setText(proclist.at(i).at(3));
            } else {
                 QTableWidgetItem *newItem = new QTableWidgetItem(proclist.at(i).at(0));
                 tableProc->setItem(curRows - 1, 0, newItem);
                 newItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
                 newItem = new QTableWidgetItem(proclist.at(i).at(1));
                 tableProc->setItem(curRows - 1, 1, newItem);
                 newItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
                 newItem = new QTableWidgetItem(proclist.at(i).at(2));
                 tableProc->setItem(curRows - 1, 2, newItem);
                 newItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
                 newItem = new QTableWidgetItem(proclist.at(i).at(3));
                 tableProc->setItem(curRows - 1, 3, newItem);
                 newItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
            }

        }


	// Remove unneaded entyes
	numRows = tableProc->rowCount();
	if (numRows > curRows)
            for (int i=curRows; i <= numRows; i++)
                tableProc->removeRow(curRows);


	lblProcInfo->setText(tr("Total process: %1").arg(numRows));

	// Setting buttons
	if (tableProc->currentRow()>=0){
		processKillSelected->setEnabled(TRUE);
		processKillWine->setEnabled(TRUE);
		processRenice->setEnabled(TRUE);
	} else {
		processKillSelected->setEnabled(FALSE);
		processKillWine->setEnabled(FALSE);
		processRenice->setEnabled(FALSE);
        }

	return;
}

void MainWindow::cmdCreateFake_Click(){

	QString prefixPath = core->getPrefixPath(cbPrefixes->currentText(), FALSE);

	QString sysregPath;
	sysregPath.append(prefixPath);
	sysregPath.append("/system.reg");

	QFile sysreg_file (sysregPath);

	if (sysreg_file.exists()){
		if (QMessageBox::warning(this, tr("Warning"), tr("There are other Fake drive installed in %1 prefix directory.<br><br>Do you wish to REMOVE ALL FILES from prefix folder?").arg(prefixPath), QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes){

				QStringList args;
				args << "-rdf";
				args << prefixPath;

				Process *exportProcess = new Process(args, "/bin/rm", HOME_PATH, tr("Removing old fake drive.<br>This can take a while..."), tr("Removing old fake drive"));

				if (exportProcess->exec()!=QDialog::Accepted){
					return;
				}
		} else {
			return;
		}
	}

	QDir fakeDir (prefixPath);
		if (!fakeDir.exists())
			fakeDir.mkdir(prefixPath);

	Wizard *createFakeDriveWizard = new Wizard(2, cbPrefixes->currentText());
		if (createFakeDriveWizard->exec()==QDialog::Accepted){
			updateDtabaseConnectedItems(cbPrefixes->currentIndex());
		}

	return;
}


void MainWindow::cmdUpdateFake_Click(){
	//Check if FakeDive exists?
/*	QString prefix_path;
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
*/
	Wizard *createFakeDriveWizard = new Wizard(3, cbPrefixes->currentText());
	if (createFakeDriveWizard->exec()==QDialog::Accepted){
		updateDtabaseConnectedItems(cbPrefixes->currentIndex());
	}
	return;
}

void MainWindow::processKillWine_Click(){
	// Фенкция убивания процесса вина (префикс таки обрабатывается =))
	int rowNum;
	rowNum = tableProc->currentRow();

	if (rowNum>=0){
		QString prefixPath;
		prefixPath = tableProc->item(rowNum, 3)->text();

		if (QMessageBox::warning(this, tr("Warning"), tr("This action will send a TREM(-9) signal to all wine proccess with WINEPREFIX='%1'<br><br>Do you really want to proceed?").arg(tableProc->item(rowNum, 3)->text()), QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes){
			QString command;

			if (!prefixPath.isEmpty())
				command=tr("env WINEPREFIX=\"%1\" wineserver -kill").arg(tableProc->item(rowNum, 3)->text());
			else
				command="wineserver -kill";
					  if (system(command.toAscii().data())==-1)
						QMessageBox::warning(this, tr("Error"), tr("Can't run: %1").arg(command.toAscii().data()), QMessageBox::Ok);
		}
	}
	return;
}

void MainWindow::processKillSelected_Click(){
	int rowNum;
	rowNum = tableProc->currentRow();

	if (rowNum>=0){
		QString procId;
		procId = tableProc->item(rowNum, 0)->text();

		if (QMessageBox::warning(this, tr("Warning"), tr("This action will send a TREM(-9) signal to proccess '%2' pid: %1<br><br>It is HIGH risk to damage wine normal state.<br><br>Do you really want to proceed?").arg(tableProc->item(rowNum, 0)->text()) .arg(tableProc->item(rowNum, 1)->text()),                   QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes){

			QString command;
			command="kill -9 ";
			command.append(procId);
				if (system(command.toAscii().data())==-1)
				    QMessageBox::warning(this, tr("Error"), tr("Can't run: %1").arg(command.toAscii().data()), QMessageBox::Ok);
		}
	}
	return;
}

/*
 * Prefix commands
 */

void MainWindow::prefixAdd_Click(){
	// Prefix creation function

	Wizard *createPrefixWizard = new Wizard(0);
	if (createPrefixWizard->exec()==QDialog::Accepted){
		updateDtabaseConnectedItems();
	}

	return;

}

void MainWindow::prefixDelete_Click(){

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

	updateDtabaseConnectedItems();
	return;
}

void MainWindow::prefixImport_Click(){
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

	    if (tablePrefix->item(tablePrefix->currentRow(), 1)->text().isEmpty()){
		targetDir.clear();
		targetDir.append(HOME_PATH);
		targetDir.append("/.wine/");
	    } else {
		targetDir.clear();
		targetDir.append(tablePrefix->item(tablePrefix->currentRow(), 1)->text());
	    }

	    if (dir.exists(targetDir)){
		if(QMessageBox::warning(this, tr("Warning"), tr("Do you really wish to delete all old prefix files?"), QMessageBox::Ok, QMessageBox::Cancel)==QMessageBox::Ok){
		    QStringList args;
		    args << "-rdf";
		    args << targetDir;

		    Process *exportProcess = new Process(args, core->getWhichOut("rm"), HOME_PATH, tr("Removing old fake drive.<br>This can take a while..."), tr("Removing old fake drive"));
		    if (exportProcess->exec()!=QDialog::Accepted){
			return;
		    }
		} else {
		    return;
		}
	    }

	    dir.mkdir(targetDir);
	    QStringList args;
	    args << "-xjf";
	    args << fileName;
	    args << "-C" << targetDir;

		//Creating process dialog
	    Process *exportProcess = new Process(args, TAR_BIN, HOME_PATH, tr("Importing prefix.<br>This can take a while..."), tr("Importing prefix"));
	    exportProcess->show();
	}
    return;
}

void MainWindow::prefixSettings_Click(){
    /*
     *	Getting prefix name, and show settings dialog
     */
    PrefixSettings settings(tablePrefix->item(tablePrefix->currentRow(), 0)->text());
    if (settings.exec()==QDialog::Accepted){
    	updateDtabaseConnectedItems();
    }
    return;
}

void MainWindow::prefixExport_Click(){
    /*
     * Function for exporting preefix to file
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


		QString prefix_path = tablePrefix->item(tablePrefix->currentRow(), 1)->text();

		if (prefix_path.isEmpty()){
			prefix_path.clear();
			prefix_path.append(HOME_PATH);
			prefix_path.append("/.wine/");
		}

		args << "-C";
		args << prefix_path;
		args << "./";

		//Creating process dialog
		Process *exportProcess = new Process(args, TAR_BIN, prefix_path, tr("Exporting %1 prefix.<br>This can take a while...").arg(tablePrefix->item(tablePrefix->currentRow(), 0)->text()), tr("Exporting prefix"));

		exportProcess->show();
	}

	return;
}

void MainWindow::mainExit_Click(){
	/*
	 * main Menu Exit
	 */

	QSettings settings(APP_SHORT_NAME, "default");
	settings.beginGroup("MainWindow");
	settings.setValue("size", size());
	settings.setValue("pos", pos());
	settings.endGroup();

	qApp->quit();
	return;
}

void MainWindow::SetMeVisible(bool visible){
	/*
	 * Hide and show MainWindow on TrayIcon click event
	 */
	setVisible(visible);
	return;
}

void MainWindow::mainPrograms_Click(){
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

void MainWindow::mainProcess_Click(){
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

void MainWindow::mainSetup_Click(){
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

void MainWindow::mainPrefix_Click(){
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

void MainWindow::mainAbout_Click(){
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

void MainWindow::mainRun_Click(){
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
		CoreFunction_WinePrepareRunParams(run->execObj);

	return;
}


void MainWindow::mainImageManager_Click(){
	/*
	 * CD Image Manager
	 */

	ImageManager *manager = new ImageManager(0);

	manager->exec();
	return;
}

void MainWindow::mainOptions_Click(){
	/*
	 * main Menu shows About dialog
	 */

	AppSettings *options = new AppSettings();

	if (options->exec()==QDialog::Accepted){
		getSettings();

		if (SHOW_TRAREY_ICON){
			trayIcon->show();
		} else {
			trayIcon->hide();
		}

	}
	return;
}

void MainWindow::mainAboutQt_Click(){
	/*
	 * main Menu shows AboutQt dialog
	 */

	QMessageBox::aboutQt ( this );

	return;
}

void MainWindow::mainInstall_Click(){
	/*
	 * main Menu shows install Wizard dialog
	 */

	QMessageBox::warning(this, tr("WIP"), tr("Sorry, no install wizard yet. It'l  implemented at v0.110."));

	return;
}

void MainWindow::mainExportIcons_Click(){
	/*
	 * main Menu allow export icons
	 */

	if (!isVisible())
		SetMeVisible(TRUE);

	QString fileName, tmpDir;
	QStringList args;

	fileName = QFileDialog::getOpenFileName(this, tr("Open image file"), QDir::homePath(), tr("Win32 Executable and Shared libraies (*.exe *.dll);;Win32 Executable (*.exe);;Win32 Shared libraies (*.dll)") );

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

void MainWindow::CoreFunction_CreateMenus(){
	/*
	 * Context menu for process manage
	 */

	processKillSelected = new QAction(CoreFunction_IconLoad("data/kill.png"), tr("Stop current"), tableProc);
	processKillSelected->setStatusTip(tr("Send TERM signal to selected process"));
	connect(processKillSelected, SIGNAL(triggered()), this, SLOT(processKillSelected_Click()));

	processKillWine = new QAction(CoreFunction_IconLoad("data/stop.png"), tr("Stop wine"), tableProc);
	processKillWine->setStatusTip(tr("Send TERM signal to main wine process"));
	connect(processKillWine, SIGNAL(triggered()), this, SLOT(processKillWine_Click()));

	processRefresh = new QAction(CoreFunction_IconLoad("data/reload.png"), tr("Refresh list"), tableProc);
	processRefresh->setStatusTip(tr("Refresh process list"));
	connect(processRefresh, SIGNAL(triggered()), this, SLOT(getWineProccessInfo()));

	processRenice = new QAction(tr("Renice"), tableProc);
	processRenice->setStatusTip(tr("Set process priority"));
	connect(processRenice, SIGNAL(triggered()), this, SLOT(processRenice_Click()));

	/*
	 * Context menu for directory manage
	 */

	dirRun = new QAction(tr("Run..."), twPrograms);
	dirRun->setStatusTip(tr("Run application at current prefix"));
	connect(dirRun, SIGNAL(triggered()), this, SLOT(mainRun_Click()));

	dirAdd = new QAction(tr("New"), twPrograms);
	dirAdd->setStatusTip(tr("Create new directory"));
	connect(dirAdd, SIGNAL(triggered()), this, SLOT(dirAdd_Click()));

	dirRename = new QAction(tr("Rename"), twPrograms);
	dirRename->setStatusTip(tr("Rename current directory"));
	connect(dirRename, SIGNAL(triggered()), this, SLOT(dirRename_Click()));

	dirDelete = new QAction(tr("Delete"), twPrograms);
	dirDelete->setStatusTip(tr("Delete current directory"));
	connect(dirDelete, SIGNAL(triggered()), this, SLOT(dirDelete_Click()));

	dirUnmount = new QAction(tr("Unmount cd drive"), twPrograms);
	dirUnmount->setStatusTip(tr("Unmounts cdrom drive"));
	connect(dirUnmount, SIGNAL(triggered()), this, SLOT(dirUnmount_Click()));

	dirMountOther = new QAction(tr("mount ..."), lstIcons);
	dirMountOther->setStatusTip(tr("Browse for other image"));
	connect(dirMountOther, SIGNAL(triggered()), this, SLOT(dirMountOther_Click()));

	dirConfigure = new QAction(tr("Configure wine"), twPrograms);
	dirConfigure->setStatusTip(tr("Configure Wine general settings"));
	connect(dirConfigure, SIGNAL(triggered()), this, SLOT(dirConfigure_Click()));

	dirInstall = new QAction(tr("App install"), twPrograms);
	dirInstall->setStatusTip(tr("Runs application install wizard for current prefix"));
	connect(dirInstall, SIGNAL(triggered()), this, SLOT(dirInstall_Click()));

	dirUninstall = new QAction(tr("App uninstall"), twPrograms);
	dirUninstall->setStatusTip(tr("Runs application uninstall wizard for current prefix"));
	connect(dirUninstall, SIGNAL(triggered()), this, SLOT(dirUninstall_Click()));

	/*
	 * Context menus for icon manage
	 */

	iconRun = new QAction(tr("Run"), lstIcons);
	iconRun->setStatusTip(tr("Create new icon"));
	connect(iconRun, SIGNAL(triggered()), this, SLOT(iconRun_Click()));

	iconAdd = new QAction(tr("New"), lstIcons);
	iconAdd->setStatusTip(tr("Create new icon"));
	connect(iconAdd, SIGNAL(triggered()), this, SLOT(iconAdd_Click()));

	iconCut = new QAction(tr("Cut"), lstIcons);
	iconCut->setStatusTip(tr("Cut selected icons to buffer"));
	connect(iconCut, SIGNAL(triggered()), this, SLOT(iconCut_Click()));

	iconCopy = new QAction(tr("Copy"), lstIcons);
	iconCopy->setStatusTip(tr("Copy selected icons to buffer"));
	connect(iconCopy, SIGNAL(triggered()), this, SLOT(iconCopy_Click()));

	iconPaste = new QAction(tr("Paste"), lstIcons);
	iconPaste->setStatusTip(tr("Paste selected icons from buffer to selected folder"));
	connect(iconPaste, SIGNAL(triggered()), this, SLOT(iconPaste_Click()));

	iconRename = new QAction(tr("Rename"), lstIcons);
	iconRename->setStatusTip(tr("Rename current icon"));
	connect(iconRename, SIGNAL(triggered()), this, SLOT(iconRename_Click()));

	iconDelete = new QAction(tr("Delete"), lstIcons);
	iconDelete->setStatusTip(tr("Delete current icon"));
	connect(iconDelete, SIGNAL(triggered()), this, SLOT(iconDelete_Click()));

	iconOptions = new QAction(tr("Options"), lstIcons);
	iconOptions->setStatusTip(tr("Modify current icon options"));
	connect(iconOptions, SIGNAL(triggered()), this, SLOT(iconOption_Click()));

	iconMount = new QAction(tr("mount"), lstIcons);
	iconMount->setStatusTip(tr("Mount image from icon options"));
	connect(iconMount, SIGNAL(triggered()), this, SLOT(iconMount_Click()));

	iconUnmount = new QAction(tr("umount"), lstIcons);
	iconUnmount->setStatusTip(tr("Unmount image"));
	connect(iconUnmount, SIGNAL(triggered()), this, SLOT(iconUnmount_Click()));

	iconMountOther = new QAction(tr("mount ..."), lstIcons);
	iconMountOther->setStatusTip(tr("Browse for other image"));
	connect(iconMountOther, SIGNAL(triggered()), this, SLOT(iconMountOther_Click()));

	/*
	 * Context menus for prefix manage
	 */

	prefixAdd = new QAction(CoreFunction_IconLoad("data/wizard.png"), tr("Create new"), lstIcons);
	prefixAdd->setStatusTip(tr("Create new prefix"));
	connect(prefixAdd, SIGNAL(triggered()), this, SLOT(prefixAdd_Click()));

	prefixImport = new QAction(CoreFunction_IconLoad("data/down.png"), tr("Import prefix"), lstIcons);
	prefixImport->setStatusTip(tr("Import prefix"));
	connect(prefixImport, SIGNAL(triggered()), this, SLOT(prefixImport_Click()));
	prefixImport->setEnabled(FALSE);

	prefixExport = new QAction(CoreFunction_IconLoad("data/up.png"), tr("Export prefix"), lstIcons);
	prefixExport->setStatusTip(tr("Export prefix"));
	connect(prefixExport, SIGNAL(triggered()), this, SLOT(prefixExport_Click()));
	prefixExport->setEnabled(FALSE);

	prefixDelete = new QAction(CoreFunction_IconLoad("data/kill.png"), tr("Delete prefix"), lstIcons);
	prefixDelete->setStatusTip(tr("Delete prefix"));
	connect(prefixDelete, SIGNAL(triggered()), this, SLOT(prefixDelete_Click()));
	prefixDelete->setEnabled(FALSE);

	prefixSettings = new QAction(CoreFunction_IconLoad("data/configure.png"), tr("Edit prefix settings"), lstIcons);
	prefixSettings->setStatusTip(tr("Edit prefix settings"));
	connect(prefixSettings, SIGNAL(triggered()), this, SLOT(prefixSettings_Click()));
	prefixSettings->setEnabled(FALSE);

	menuProc = new QMenu(this);
		menuProc->addAction(processKillSelected);
		menuProc->addSeparator();
		menuProc->addAction(processKillWine);
		menuProc->addSeparator();
		menuProc->addAction(processRenice);
		menuProc->addSeparator();
		menuProc->addAction(processRefresh);

	menuPrefix = new QMenu(this);
		menuPrefix->addAction(prefixAdd);
		menuPrefix->addSeparator();
		menuPrefix->addAction(prefixImport);
		menuPrefix->addAction(prefixExport);
		menuPrefix->addSeparator();
		menuPrefix->addAction(prefixDelete);
		menuPrefix->addSeparator();
		menuPrefix->addAction(prefixSettings);


	menuIcon = new QMenu(this);
		menuIcon->addAction(iconRun);
		menuIcon->addSeparator();
			menuIconMount = new QMenu(this);
			menuIconMount = menuIcon->addMenu(tr("Mount iso..."));
		menuIcon->addSeparator();
		menuIcon->addAction(iconOptions);
		menuIcon->addSeparator();
		menuIcon->addAction(iconCut);
		menuIcon->addAction(iconCopy);
		menuIcon->addAction(iconPaste);
		menuIcon->addAction(iconRename);
		menuIcon->addAction(iconDelete);
		menuIcon->addSeparator();
		menuIcon->addAction(iconAdd);

	menuDir = new QMenu(this);
		menuDir->addAction(dirAdd);
		menuDir->addSeparator();
			menuDirMount = new QMenu(this);
			menuDirMount = menuDir->addMenu(tr("Mount iso..."));
		menuDir->addSeparator();
		menuDir->addAction(dirRun);
		menuDir->addSeparator();
//		menuDir->addAction(dirInstall);
//		menuDir->addAction(dirUninstall);
//		menuDir->addAction(dirConfigure);
//		menuDir->addSeparator();
		menuDir->addAction(dirRename);
		menuDir->addSeparator();
		menuDir->addAction(dirDelete);


	return;
}

QIcon MainWindow::CoreFunction_IconLoad(QString iconName){
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

void MainWindow::CoreFunction_CreateToolBars(){
	// Toolbar creation function

		procToolBar = new QToolBar(tlbProccess);

		procToolBar->addAction(processKillSelected);
		procToolBar->addAction(processKillWine);
		procToolBar->addSeparator ();
		procToolBar->addAction(processRefresh);


		prefixToolBar = new QToolBar(tlbPrefix);

		prefixToolBar->addAction(prefixAdd);
		prefixToolBar->addSeparator ();
		prefixToolBar->addAction(prefixImport);
		prefixToolBar->addAction(prefixExport);
		prefixToolBar->addSeparator ();
		prefixToolBar->addAction(prefixSettings);
		prefixToolBar->addSeparator ();
		prefixToolBar->addAction(prefixDelete);

	return;
}


/****************************\
 *    CONTEXT MENU SLOTS    *
\****************************/


void MainWindow::iconDelete_Click(void){
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
					qDebug()<<"WARNING: SQL error at MainWindow::iconDelete_Click(void)\nINFO:\n"<<query.executedQuery()<<"\n"<<query.lastError();
				#endif
				return;
			}
			query.clear();
		}
		twPrograms_ItemClick(treeItem, 0);
	}
	return;
}

void MainWindow::iconRun_Click(void){

	QListWidgetItem *iconItem;
	iconItem=lstIcons->currentItem();
	if (iconItem)
		lstIcons_ItemDoubleClick(iconItem);
	return;
}

void MainWindow::iconRename_Click(void){

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
				qDebug()<<"WARNING: SQL error at MainWindow::iconRename_Click(void)\nINFO:\n"<<query.executedQuery()<<"\n"<<query.lastError();
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
				qDebug()<<"WARNING: SQL error at MainWindow::iconRename_Click(void)\nINFO:\n"<<query.executedQuery()<<"\n"<<query.lastError();
			#endif
			return;
		}
		query.clear();

		// Updating icons view
		twPrograms_ItemClick(treeItem, 0);

	}
	return;
}

void MainWindow::iconAdd_Click(void){
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
		twPrograms_ItemClick(treeItem, 0);
	}

	return;
}

void MainWindow::iconMountOther_Click(void){
	/*
		This function request mount of selected by user image
	*/

	dirMountOther_Click();

	return;
}

void MainWindow::iconUnmount_Click(void){
	/*
		This function requests unmount by mount point described in icon settings
	*/

	dirUnmount_Click();

	/*QStringList dataList = SQL_getPrefixAndDirData(twPrograms->currentItem());
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

	CoreFunction_ImageUnmount(query.value(0).toString());
	*/
	return;
}

void MainWindow::iconMount_Click(void){
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

	CoreFunction_ImageMount(query.value(0).toString(), query.value(1).toString());

	return;
}

void MainWindow::iconCut_Click(void){
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

void MainWindow::iconCopy_Click(void){
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

void MainWindow::iconPaste_Click(void){



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
						query.prepare("SELECT name, exec, cmdargs, icon_path, desc, display, winedebug, useconsole, override, wrkdir, nice, desktop FROM icon WHERE prefix_id=:prefix_id and dir_id=:dir_id and name=:name");
						query.bindValue(":dir_id", iconBuffer.dir_id);
					} else {
						query.prepare("SELECT name, exec, cmdargs, icon_path, desc, display, winedebug, useconsole, override, wrkdir, nice, desktop FROM icon WHERE prefix_id=:prefix_id and dir_id ISNULL and name=:name");
					}
					query.bindValue(":name", iconBuffer.names.at(i));
					query.bindValue(":prefix_id", iconBuffer.prefix_id);
					if (!query.exec()){

							qDebug()<<"WARNING: SQL_getPrefixAndDirData\nINFO:\n"<<query.executedQuery()<<"\n"<<query.lastError();

					/*	return;
					}

					if (!query.isValid()){
					*/
						// If query fails, exit and
						// Clearing icon buffer
						iconBuffer.names.clear();
						iconBuffer.dir_id="";
						iconBuffer.prefix_id="";
						iconBuffer.move=false;

						qDebug()<<"Exiting...."<<query.executedQuery()<<"\n"<<query.lastError();
						return;
					}




					query.first();

					QStringList reccordBuffer;

					for (int j=0; j<=11; j++){
						reccordBuffer<<query.value(j).toString();
					}

					query.clear();

					query.prepare("INSERT INTO icon(name, exec, cmdargs, icon_path, desc, display, winedebug, useconsole, override, prefix_id, dir_id, id, wrkdir, nice, desktop) VALUES(:name, :exec, :cmdargs, :icon_path, :desc, :display, :winedebug, :useconsole, :override, :prefix_id, :dir_id, NULL, :wrkdir, :nice, :desktop)");

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
					query.bindValue(":display", reccordBuffer.at(5));
					query.bindValue(":winedebug", reccordBuffer.at(6));
					query.bindValue(":useconsole", reccordBuffer.at(7));
					query.bindValue(":override", reccordBuffer.at(8));
					query.bindValue(":wrkdir", reccordBuffer.at(9));
					query.bindValue(":nice", reccordBuffer.at(10));
					query.bindValue(":desktop", reccordBuffer.at(11));

					if (!query.exec()){
							qDebug()<<"WARNING: SQL_getPrefixAndDirData\nINFO:\n"<<query.executedQuery()<<"\n"<<query.lastError();
						return;
					}

					query.clear();
				}
			break;
		}

		twPrograms_ItemClick(twPrograms->currentItem(), 0);
	return;
}

void MainWindow::iconOption_Click(void){
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
		twPrograms_ItemClick(treeItem, 0);
	}

	return;
}

void MainWindow::dirAdd_Click(void){
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
			prefixItem->setIcon(0, CoreFunction_IconLoad("data/folder.png"));

			query.prepare("INSERT INTO dir(id, name, prefix_id) VALUES(NULL, :name, :prefix_id)");
			query.bindValue(":name", dirname);
			query.bindValue(":prefix_id", getData.at(0));
			query.exec();
			query.clear();

		}
	}
	return;
}

void MainWindow::dirUnmount_Click(void){
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
	CoreFunction_ImageUnmount(query.value(0).toString());
	query.clear();

	return;
}

void MainWindow::dirMountOther_Click(void){
	/*
	Request for unmounting cdrom drve described at wine prefix settings
	*/

	QStringList dataList = SQL_getPrefixAndDirData(twPrograms->currentItem());
	QSqlQuery query;

	query.prepare("select cdrom_mount from prefix where id=:id");
	query.bindValue(":id", dataList.at(0));
	if (!query.exec()){
		qDebug()<<"WARNING: contextDirMountDrive\nINFO:\n"<<query.executedQuery()<<"\n"<<query.lastError();
		return;
	}

	#ifdef _OS_LINUX_
	  QString fileName = QFileDialog::getOpenFileName(this, tr("Open ISO or NRG Image file"), HOME_PATH, tr("iso and nrg files (*.iso *.nrg)"));
	#endif

	#ifdef _OS_FREEBSD_
	  QString fileName = QFileDialog::getOpenFileName(this, tr("Open ISO Image file"), HOME_PATH, tr("iso files (*.iso)"));
	#endif

	if(fileName.isEmpty()){
		return;
	}


	query.first();
	CoreFunction_ImageMount(fileName, query.value(0).toString());
	query.clear();

	return;
}

void MainWindow::dirConfigure_Click(void){
	//FIXME: What is this? ;)
	//RunWineUtils("winecfg", twPrograms->currentItem());
	return;
}

void MainWindow::dirInstall_Click(void){
	//FIXME: Пока нету визарда, надо бы создать
	//RunWineUtils("winecfg", twPrograms->currentItem());

	QMessageBox::warning(this, tr("WIP"), tr("Sorry, no install wizard yet. It'l implemented at v0.110."));

	return;
}

void MainWindow::dirUninstall_Click(void){
	//FIXME: Переделать на новый класс winebinlauncher
	//RunWineUtils("uninstaller", twPrograms->currentItem());
	return;
}

void MainWindow::dirRename_Click(void){
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

void MainWindow::dirDelete_Click(void){
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

			twPrograms_ItemClick(twPrograms->currentItem(), 0);
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
			qDebug()<<"WARNING: SQL error at MainWindow::iconPaste_Click(void)\nINFO:\n"<<query.executedQuery()<<"\n"<<query.lastError();
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

void MainWindow::CoreFunction_ImageMount(QString image, QString mount){
	/*
		Mounting an image or drive to mountmount
			@image -- an image (or drive) name
			@mount -- mount point
	*/


	//mount_cd9660

	QStringList args;
	QString arg;

	#ifdef _OS_FREEBSD_


		if ((image.right(3)=="iso") or (image.right(3)=="nrg")){
			args << SH_BIN;
			args << "-c";
				arg = core->getWhichOut("mount_cd9660");
				//FIXME: not tested
				//if (image.right(3)=="nrg")
				//	arg = arg.append(" -s 307200 ");
				arg.append("  /dev/`mdconfig -f ");
				arg.append(image);
				arg.append("` ");
				arg.append(mount);
			args << arg;
		} else {
			args << MOUNT_BIN << "-t" << "cd9660" << image << mount;
		}
	#endif

	#ifdef _OS_LINUX_
		args << MOUNT_BIN;
		args << image;
		args << mount;

		if (image.right(3)=="iso"){
			args << "-o" << "loop";
		}
		if (image.right(3)=="nrg"){
			args << "-o" << "loop,offset=307200";
		}

	#endif

		//Fix args for kdesu\gksu\e.t.c.
		if (!GUI_SUDO_BIN.contains(QRegExp("/sudo$"))){
		   arg=args.join(" ");
		   args.clear();
		   args<<arg;
		}

		Process *exportProcess = new Process(args, GUI_SUDO_BIN, HOME_PATH, tr("Mounting..."), tr("Mounting..."));

		if (exportProcess->exec()==QDialog::Accepted){
			statusBar()->showMessage(tr("Image successfully mounted"));
		} else {
			statusBar()->showMessage(tr("Image mount fail"));
		}

	return;
}

void MainWindow::CoreFunction_ImageUnmount(QString mount){

	QStringList args;
	QString arg;
	Process *exportProcess;

	#ifdef _OS_FREEBSD_
		args.clear();
		args << "-c" << tr("%1 | grep %2").arg(MOUNT_BIN).arg(mount);

		QProcess *myProcess = new QProcess(this);
		myProcess->start(SH_BIN, args);
		if (!myProcess->waitForFinished()){
			qDebug() << "Make failed:" << myProcess->errorString();
			return;
		}

		QString devid = myProcess->readAll();
	#endif

	args.clear();
	args << UMOUNT_BIN;
	args << mount;

	//Fix args for kdesu\gksu\e.t.c.
	if (!GUI_SUDO_BIN.contains(QRegExp("/sudo$"))){
	   arg=args.join(" ");
	   args.clear();
	   args<<arg;
	}

	exportProcess = new Process(args, GUI_SUDO_BIN, HOME_PATH, tr("Unmounting..."), tr("Unmounting..."));

	if (exportProcess->exec()==QDialog::Accepted){
		statusBar()->showMessage(tr("image successfully unmounted"));
	} else {
		statusBar()->showMessage(tr("image unmount fail"));
	}

	#ifdef _OS_FREEBSD_
		if (!devid.isEmpty()){
			devid = devid.split(" ").first();
			if (!devid.isEmpty()){
				if (devid.contains("md")){
					args.clear();
					args << "mdconfig" <<  "-d" << tr("-u%1").arg(devid.mid(7));

					//Fix args for kdesu\gksu\e.t.c.
					if (!GUI_SUDO_BIN.contains(QRegExp("/sudo$"))){
					   arg=args.join(" ");
					   args.clear();
					   args<<arg;
					}

					exportProcess = new Process(args, GUI_SUDO_BIN, HOME_PATH, tr("Unmounting..."), tr("running mdconfig"));

					if (exportProcess->exec()==QDialog::Accepted){
						statusBar()->showMessage(tr("mdimage removed"));
					} else {
						statusBar()->showMessage(tr("mdimage remove fail"));
					}
				}
			}
		}
	#endif

	return;
}

void MainWindow::CoreFunction_WineRunAutorunItems(void){

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

			CoreFunction_WinePrepareRunParams(execObj);

		}

		icoQuery.clear();

	}

	query.clear();

	return;
}

