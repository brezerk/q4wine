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

#include "mainwindow.h"

QTimer *timer = new QTimer();


MainWindow::MainWindow(int startState, QWidget * parent, Qt::WFlags f) : QMainWindow(parent, f){

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
  db_icon = new Icon();
  db_last_run_icon = new Last_Run_Icon();
  db_image = new Image();
  db_prefix = new Prefix();


  clearTmp();
  // Base GUI setup
  setupUi(this);

  if (startState == 1)
	  this->showMinimized();

  setWindowTitle(tr("%1 :. Qt4 GUI for Wine v%2").arg(APP_NAME) .arg(APP_VERS));

  lstIcons = new DragListWidget(tab);
  lstIcons->setViewMode(QListView::IconMode);
  lstIcons->setGridSize(QSize(86, 86));
  lstIcons->setResizeMode(QListView::Adjust);
  lstIcons->setWrapping(TRUE);
  lstIcons->setWordWrap(TRUE);
  lstIcons->setAcceptDrops(TRUE);
  lstIcons->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
  lstIcons->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
  lstIcons->setMovement(QListView::Snap);
  lstIcons->setDragDropMode(QAbstractItemView::InternalMove);
  lstIcons->setSelectionMode(QAbstractItemView::ContiguousSelection);

  /*QHBoxLayout *layout = new QHBoxLayout;
  layout->addWidget(lstIcons);
  layout->setMargin(3);
  gbIcons->setLayout(layout);
*/

  splitter = new QSplitter(tab);
  splitter->addWidget(twPrograms);
  splitter->addWidget(lstIcons);

  QVBoxLayout *vlayout = new QVBoxLayout;
  vlayout->addWidget(splitter);
  vlayout->addWidget(gbInfo);
  vlayout->setMargin(3);
  tab->setLayout(vlayout);


  // Updating database connected items
  updateDtabaseConnectedItems();

  // Getting settings from config file
  this->getSettings();

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
  connect(mainFirstSteps, SIGNAL(triggered()), this, SLOT(mainFirstSteps_Click()));
  connect(mainFAQ, SIGNAL(triggered()), this, SLOT(mainFAQ_Click()));
  connect(mainIndex, SIGNAL(triggered()), this, SLOT(mainIndex_Click()));
  connect(mainWebsite, SIGNAL(triggered()), this, SLOT(mainWebsite_Click()));
  connect(mainDonate, SIGNAL(triggered()), this, SLOT(mainDonate_Click()));
  connect(mainBugs, SIGNAL(triggered()), this, SLOT(mainBugs_Click()));

  connect(lstIcons, SIGNAL(startDrag ()), this, SLOT(startDrag()));
  connect(lstIcons, SIGNAL(startDrop(QList<QUrl>)), this, SLOT(startDrop(QList<QUrl>)));

#ifndef WITH_ICOUTILS
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
  lstIcons->setContextMenuPolicy(Qt::CustomContextMenu);

  // Creating actions for context menus & toolbars
  createMenuActions();
  createToolBarActions();

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
  installEventFilter(this);

  // FIXME: Move this into shared libaray
  runAutostart();

  createTrayIcon();

  // FIXME: Remove this as replace for shared library
  //core = new CoreMethods();
  connect (menuRun, SIGNAL(triggered(QAction*)), this, SLOT(menuRun_triggered(QAction*)));
  return;
}


void MainWindow::clearTmp(){
  QString fileName = QDir::homePath();
  fileName.append("/.config/");
  fileName.append(APP_SHORT_NAME);
  fileName.append("/tmp/");

  QDir dir(fileName);
  dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);

  QFileInfoList list = dir.entryInfoList();
  for (int i = 0; i < list.size(); ++i) {
	QFile(list.at(i).absoluteFilePath()).remove();
  }
  return;
}

void MainWindow::startDrop(QList<QUrl> files){
  //QList<QUrl> list = event->mimeData()->urls();
  QTreeWidgetItem *treeItem = twPrograms->currentItem();

  if (!treeItem)
	return;

  bool ok;
  QString file, fileName;
  QStringList list1;

  QString dir_name, prefix_name;

  if (treeItem->parent()){
	prefix_name = treeItem->parent()->text(0);
	dir_name = treeItem->text(0);
  } else {
	prefix_name = treeItem->text(0);
	dir_name = "";
  }

  for (int i=0; i < files.count(); i++){
	if (files.at(i).toLocalFile().contains(".exe", Qt::CaseInsensitive) || files.at(i).toLocalFile().contains(".bat", Qt::CaseInsensitive) || files.at(i).toLocalFile().contains(".com", Qt::CaseInsensitive)){
	  file = files.at(i).toLocalFile();

	  list1 = file.split("/");
	  fileName=list1.last().left(list1.last().length() - list1.last().split(".").last().length() - 1);

	  while (db_icon->isExistsByName(prefix_name, dir_name, fileName)){
		fileName = QInputDialog::getText(this, tr("Sorry. It seems icon already exists."), tr("Sorry. It seems icon already exists.<br>Please choose another name, or cancel operation."), QLineEdit::Normal, iconBuffer.names.at(i) , &ok);
		if (!ok){
		  return;
		}
	  }

	  if (files.at(i).toLocalFile().contains(".bat", Qt::CaseInsensitive)){
		file = "--backend=user ";
		file.append(CoreLib->getWinePath(files.at(i).toLocalFile(), "-w"));
		db_icon->addIcon(file, "wineconsole", "", "", prefix_name, dir_name, fileName, "", "", "", "", file.left(file.length() - file.split("/").last().length()), "", 0);
	  } else {
		db_icon->addIcon("", file, "", "", prefix_name, dir_name, fileName, "", "", "", "", file.left(file.length() - file.split("/").last().length()), "", 0);
	  }
	}
  }
  twPrograms_ItemClick(treeItem, 0);
}

void MainWindow::startDrag (){
  QString fileName;
  QList<QListWidgetItem *> items = lstIcons->selectedItems ();

  if (items.count()>0){
	QTreeWidgetItem *treeItem = twPrograms->currentItem();

	if (!treeItem)
	  return;

	QMimeData *mimeData = new QMimeData;
	QList<QUrl> urls;

	for (int i=0; i<items.count(); i++){
	  if (treeItem->parent()){
		fileName = CoreLib->createDesktopFile(treeItem->parent()->text(0), treeItem->text(0), items.at(i)->text());
	  } else {
		fileName = CoreLib->createDesktopFile(treeItem->text(0), "", items.at(i)->text());
	  }
	  urls<<QUrl::fromLocalFile(fileName);
	}
	mimeData->setUrls(urls);
	QDrag *drag = new QDrag(this);
	drag->setMimeData(mimeData);
	drag->setPixmap(items.at(0)->icon().pixmap(32));
	drag->start(Qt::MoveAction);
  }

  return;
}

void MainWindow::cmdTestWis_Click(){
  // test block
  /*
	WisItem wis;

	QString path = HOME_PATH;
	path.append("/.config/q4wine/sample.xml");

	wis = core->getWisInfo(path); // test
	  qDebug() << "it is name" << wis.name;
	  qDebug () << "it is download" << wis.download;
	*/
  return;
}

void MainWindow::cmdWinetricks_Click() {
#ifndef WITH_WINETRIKS
  QMessageBox::warning(this, tr("Warning"), tr("<p>q4wine was compiled without winetriks support.</p><p>If you wish to enable winetriks support add:</p><p> \"-DWITH_WINETRIKS=ON\" to cmake arguments.</p>"));
#else
  QMessageBox::warning(this, tr("Warning"), tr("<p>Winetricks officaly NOT supported by q4wine.</p><p>There was some repports about bugs, slows and errors on winetriks and q4wine usage at same time.</p>"));

  if (CoreLib->getSetting("console", "bin").toString().isEmpty()){
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
	  setMeVisible(TRUE);
	} else {
	  setMeVisible(FALSE);
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

  if (!twPrograms->currentItem()){
	lstIcons->clear();
	return;
  }

  QTreeWidgetItem *treeItem = twPrograms->currentItem();
  QStringList result;

  if (treeItem->parent()){
	result=db_icon->getByName(treeItem->parent()->text(0), treeItem->text(0), item->text());
  } else {
	result=db_icon->getByName(treeItem->text(0), "", item->text());
  }

  lblInfo->setText(tr("Program: %1<br> Description: %2").arg(result.at(1)) .arg(result.at(2)));
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

  QList<int> a;
  a.append(CoreLib->getSetting("MainWindow", "splitterSize0", false, 170).toInt());
  a.append(CoreLib->getSetting("MainWindow", "splitterSize1", false, 379).toInt());

  splitter->setSizes(a);

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

#ifdef WITH_ICOUTILS
  val = CoreLib->getSetting("icotool", "wrestool");
  WRESTOOL_BIN=val.toString();
  val = CoreLib->getSetting("icotool", "icotool");
  ICOTOOL_BIN=val.toString();
#endif



   if (CoreLib->getSetting("quickmount", "type", FALSE).toString().isEmpty()){
		QSettings settings(APP_SHORT_NAME, "default");
		settings.beginGroup("quickmount");

	   if (CoreLib->getWhichOut("fuseiso", false).isEmpty()){
		   #ifdef WITH_EMBEDDED_FUSEISO
			   settings.setValue("type", 3);
			   settings.setValue("mount_drive_string", CoreLib->getMountString(3));
			   settings.setValue("mount_image_string", CoreLib->getMountImageString(3));
			   settings.setValue("umount_string", CoreLib->getUmountString(3));
		   #else
			   settings.setValue("type", 0);
			   settings.setValue("mount_drive_string", CoreLib->getMountString(0));
			   settings.setValue("mount_image_string", CoreLib->getMountImageString(0));
			   settings.setValue("umount_string", CoreLib->getUmountString(0));
		   #endif
		} else {
			   settings.setValue("type", 2);
			   settings.setValue("mount_drive_string", CoreLib->getMountString(2));
			   settings.setValue("mount_image_string", CoreLib->getMountImageString(2));
			   settings.setValue("umount_string", CoreLib->getUmountString(2));
		}
		settings.endGroup();
	}

  QString oldDir, oldPrefix;
  oldPrefix = CoreLib->getSetting("LastPrefix", "prefix", false).toString();
  oldDir = CoreLib->getSetting("LastPrefix", "dir", false).toString();

  if (!oldPrefix.isEmpty()){

	cbPrefixes->setCurrentIndex(cbPrefixes->findText(oldPrefix, Qt::MatchExactly));

	QTreeWidgetItem* item;
	QTreeWidgetItem* childItem;
	for(int i=0; i < twPrograms->topLevelItemCount(); i++)
	{
	  item = twPrograms->topLevelItem(i);
	  if (oldPrefix==item->text(0)){
		if (oldDir.isEmpty()){
		  twPrograms->setCurrentItem(item);
		  twPrograms_ItemClick(item, 0);
		  break;
		} else {
		  for (int j=0; j < item->childCount(); j++){
			childItem=item->child(j);
			if (childItem->text(0)==oldDir){
			  twPrograms->setCurrentItem(childItem);
			  twPrograms_ItemClick(childItem, 0);
			}
		  }
		}
	  }
	}
  }

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
  QTreeWidgetItem *treeItem = twPrograms->currentItem();

  if (!twPrograms->currentItem()){
	lstIcons->clear();
	return;
  }

  statusBar()->showMessage(tr("Starting %1 ...").arg(item->text()));
  if (treeItem->parent()){
	if (CoreLib->runIcon(treeItem->parent()->text(0), treeItem->text(0), item->text())){
	  statusBar()->showMessage(tr("%1 started.").arg(item->text()));
	} else {
	  statusBar()->showMessage(tr("%1 fail to start.").arg(item->text()));
	}
  } else {
	if (CoreLib->runIcon(treeItem->text(0), "", item->text())){
	  statusBar()->showMessage(tr("%1 started.").arg(item->text()));
	} else {
	  statusBar()->showMessage(tr("%1 fail to start.").arg(item->text()));
	}

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
  QList<QStringList> iconsList;

  lstIcons->clear();
  lblInfo->setText(tr("Program: <br> Description:"));

  if (!item)
	return;

  if (item->parent()){
	iconsList=db_icon->getByPrefixAndDirName(item->parent()->text(0), item->text(0));
	cbPrefixes->setCurrentIndex(cbPrefixes->findText(item->parent()->text(0), Qt::MatchExactly));
  } else {
	iconsList=db_icon->getByPrefixAndDirName(item->text(0), "");
	cbPrefixes->setCurrentIndex(cbPrefixes->findText(item->text(0), Qt::MatchExactly));
  }


  for (int i = 0; i < iconsList.size(); ++i) {
	iconItem = new QListWidgetItem(lstIcons, 0);
	iconItem->setText(iconsList.at(i).at(1));

	//Seting icon. If no icon or icon file not exists -- setting default
	if (iconsList.at(i).at(3).isEmpty()){
	  iconItem->setIcon(loadIcon("data/exec_wine.png"));
	} else {
	  if (QFile::exists (iconsList.at(i).at(3))){
		iconItem->setIcon(QIcon(iconsList.at(i).at(3)));
	  } else {
		if (iconsList.at(i).at(3)=="wineconsole"){
		  iconItem->setIcon(loadIcon("data/wineconsole.png"));
		} else if (iconsList.at(i).at(3)=="regedit"){
		  iconItem->setIcon(loadIcon("data/regedit.png"));
		} else if (iconsList.at(i).at(3)=="wordpad"){
		  iconItem->setIcon(loadIcon("data/notepad.png"));
		} else if (iconsList.at(i).at(3)=="winecfg"){
		  iconItem->setIcon(loadIcon("data/winecfg.png"));
		} else if (iconsList.at(i).at(3)=="uninstaller"){
		  iconItem->setIcon(loadIcon("data/uninstaller.png"));
		} else if (iconsList.at(i).at(3)=="eject"){
		  iconItem->setIcon(loadIcon("data/eject.png"));
		} else if (iconsList.at(i).at(3)=="explorer"){
		  iconItem->setIcon(loadIcon("data/explorer.png"));
		} else {
		  iconItem->setIcon(loadIcon("data/exec_wine.png"));
		}
	  }
	}
  }

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
	// Inserting root items into programs tree view
	prefixItem = new QTreeWidgetItem(twPrograms);
	prefixItem->setText(0, tr("%1").arg(result.at(i).at(1)));
	prefixItem->setIcon(0, loadIcon("data/wine.png"));
	prefixItem->setExpanded (TRUE);
	twPrograms->addTopLevelItem(prefixItem);

	// Inserting subfolders items into programs tree view
	subresult = db_dir->getFieldsByPrefixId(result.at(i).at(0));
	for (int j = 0; j < subresult.size(); ++j) {
	  subPrefixItem = new QTreeWidgetItem(prefixItem, 0);
	  subPrefixItem->setText(0, tr("%1").arg(subresult.at(j).at(1)));
	  subPrefixItem->setIcon(0, loadIcon("data/folder.png"));
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

  if (tableProc->currentItem()){
	processKillSelected->setEnabled(TRUE);
	processKillWine->setEnabled(TRUE);
	processRenice->setEnabled(TRUE);
  } else {
	processKillSelected->setEnabled(FALSE);
	processKillWine->setEnabled(FALSE);
	processRenice->setEnabled(FALSE);
  }

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

void MainWindow::tableProc_ShowContextMenu(const QPoint point){
  /*
   Function of creation context menu
   */

  if (tableProc->itemAt (point)){
	processKillSelected->setEnabled(TRUE);
	processKillWine->setEnabled(TRUE);
	processRenice->setEnabled(TRUE);
  } else {
	processKillSelected->setEnabled(FALSE);
	processKillWine->setEnabled(FALSE);
	processRenice->setEnabled(FALSE);
  }

  menuProc->exec(QCursor::pos());
  return;

}

void MainWindow::twPrograms_ShowContextMenu(const QPoint){
	/*
		Custom context menu for twPrograms
	*/

	QTreeWidgetItem *treeItem = twPrograms->currentItem();

	QString cdrom_drive, cdrom_mount;
	QStringList result;
	QList<QStringList> images;

	if (!treeItem)
		return;

	this->twPrograms_ItemClick(treeItem, 0);

	if (treeItem->parent()){
		result = db_prefix->getFieldsByPrefixName(treeItem->parent()->text(0));
	} else {
		result = db_prefix->getFieldsByPrefixName(treeItem->text(0));
	}

	xdgOpenPrefixDir->setEnabled(TRUE);
	winefileOpenPrefixDir->setEnabled(TRUE);

	cdrom_drive = result.at(7);
	cdrom_mount = result.at(6);

	if (cdrom_mount.isEmpty()){
		xdgOpenMountDir->setEnabled(FALSE);
		winefileOpenMountDir->setEnabled(FALSE);
	} else {
		xdgOpenMountDir->setEnabled(TRUE);
		winefileOpenMountDir->setEnabled(TRUE);
	}

	QMenu* menuDirMountImages;
	menuDirMountImages = new QMenu(this);

	if (!treeItem->parent()){
		dirRename->setEnabled(FALSE);
		dirDelete->setEnabled(FALSE);
	} else {
		dirRename->setEnabled(TRUE);
		dirDelete->setEnabled(TRUE);
	}

	menuDirMount->clear();
	menuDirMount->setEnabled(FALSE);

	if (!cdrom_mount.isEmpty()){
		menuDirMount->setEnabled(TRUE);
		menuDirMountImages = menuDirMount->addMenu(tr("mount [%1]").arg(CoreLib->getMountedImages(cdrom_mount)));
		if (!cdrom_drive.isEmpty()){
			menuDirMountImages->addAction(QIcon(":/data/drive_menu.png"), cdrom_drive);
			menuDirMountImages->addSeparator();
		} else {
			menuDirMountImages->addAction(QIcon(":/data/drive_menu.png"), tr("[none]"));
			menuDirMountImages->addSeparator();
		}

		images = db_image->getFields();
		for (int i = 0; i < images.size(); ++i) {
			menuDirMountImages->addAction(QIcon(":/data/cdrom_menu.png") , images.at(i).at(0));
		}

		menuDirMount->addSeparator();

		QMenu* menuDirMountRecentImages;
		menuDirMountRecentImages = new QMenu(this);
		menuDirMountRecentImages = menuDirMount->addMenu(tr("mount ..."));

		menuDirMountRecentImages->addAction(iconMountOther);
		menuDirMountRecentImages->addSeparator();

		QSettings settings(APP_SHORT_NAME, "default");
		QStringList files = settings.value("recent_images").toStringList();

		for (int i = 0; i < files.size(); ++i){
			menuDirMountRecentImages->addAction(QIcon(":/data/cdrom_menu.png") , files.at(i).split("/").last());
		}

		connect (menuDirMountRecentImages, SIGNAL(triggered(QAction*)), this, SLOT(menuMountRecentImages_triggered(QAction*)));

		menuDirMount->addSeparator();
		menuDirMount->addAction(dirUnmount);

		connect (menuDirMountImages, SIGNAL(triggered(QAction*)), this, SLOT(menuMountImages_triggered(QAction*)));
	}
	if (!cdrom_mount.isEmpty()){
		dirUnmount->setEnabled(TRUE);
		dirMountOther->setEnabled(TRUE);
		dirUnmount->setText(tr("umount [%1]").arg(cdrom_mount));
	} else {
		dirUnmount->setEnabled(FALSE);
		dirMountOther->setEnabled(FALSE);
		dirUnmount->setText(tr("umount [none]"));
	}

	menuRun->addAction(dirRun);
	menuRun->addSeparator();
	result = db_last_run_icon->getIcons();

	for (int i=0; i<result.size(); ++i){
		if (i >= recentIconsList.size()){
			QAction *lol = new QAction(result.at(i).split("/").last(), this);
			lol->setStatusTip(result.at(i));
			recentIconsList.append(lol);
		} else {
			recentIconsList.at(i)->setText(result.at(i).split("/").last());
			recentIconsList.at(i)->setStatusTip(result.at(i));
		}
		menuRun->addAction(recentIconsList.at(i));
	}

	menuDir->exec(QCursor::pos());
	return;
}

void MainWindow::menuMountImages_triggered ( QAction * action ){
	/*
	 * This slot process menuDirMountImages and menuIconMountImages triggered signal
	 */

 if (action->text()==tr("[none]"))
	 QMessageBox::warning(this, tr("Error"),  tr("No device drive specified in prefix settings."), QMessageBox::Ok);
	 return;

  bool ret;
  if (twPrograms->currentItem()){
	if (twPrograms->currentItem()->parent()){
	  ret=CoreLib->mountImage(action->text(), twPrograms->currentItem()->parent()->text(0));
	} else {
	  ret=CoreLib->mountImage(action->text(), twPrograms->currentItem()->text(0));
	}
  }

  if (ret){
	statusBar()->showMessage(QObject::tr("%1 successfully mounted.").arg(action->text()));
  } else {
	statusBar()->showMessage(QObject::tr("Fail to mount %1.").arg(action->text()));
  }
  return;
}


void MainWindow::menuMountRecentImages_triggered ( QAction * action ){
	/*
	 * This slot process menuDirMountImages and menuIconMountImages triggered signal
	 */

	if (!action)
		return;

	if (action->text().isEmpty())
		return;

	QSettings settings(APP_SHORT_NAME, "default");
	QStringList files = settings.value("recent_images").toStringList();

	for (int i = 0; i < files.size(); ++i){
		if (files.at(i).contains(action->text().split("/").last())){
			bool ret=false;
			if (twPrograms->currentItem()){
				if (twPrograms->currentItem()->parent()){
					ret=CoreLib->mountImage(files.at(i), twPrograms->currentItem()->parent()->text(0));
				} else {
					ret=CoreLib->mountImage(files.at(i), twPrograms->currentItem()->text(0));
				}
			}

			if (ret){
				statusBar()->showMessage(QObject::tr("%1 successfully mounted.").arg(action->text()));
			} else {
				statusBar()->showMessage(QObject::tr("Fail to mount %1.").arg(action->text()));
			}
			break;
		}
	}

  return;
}

void MainWindow::menuRun_triggered ( QAction * action ){

	if (action->text().isEmpty())
		return;

	if (action->text()==tr("Browse ..."))
		return;

	QStringList result = db_last_run_icon->getByExec(action->statusTip());

	if (!twPrograms->currentItem())
		return;



	QTreeWidgetItem *treeItem = twPrograms->currentItem();
	QStringList dataList;


	if (!treeItem)
		return;

	if (!isVisible())
		setMeVisible(TRUE);

	if (isMinimized ())
		showNormal ();

	if (CoreLib->getSetting("advanced", "openRunDialog", false, 0).toInt()==0){
		ExecObject execObj;
		if (treeItem->parent()){
			execObj.prefixid=db_prefix->getId(treeItem->parent()->text(0));
		} else {
			execObj.prefixid=db_prefix->getId(treeItem->text(0));
		}
		execObj.execcmd=action->statusTip();
		execObj.wrkdir=result.at(0);
		execObj.override=result.at(1);
		execObj.winedebug=result.at(2);
		execObj.useconsole=result.at(3);
		execObj.display=result.at(4);
		execObj.cmdargs=result.at(5);
		execObj.desktop=result.at(6);
		execObj.nice=result.at(7);
		CoreLib->runWineBinary(execObj);
	} else {
		Run *run;

		return;

		if (treeItem->parent()){
			run = new Run(treeItem->parent()->text(0), result.at(0), result.at(1), result.at(2), result.at(3), result.at(4), result.at(5), result.at(6), result.at(7).toInt(), action->statusTip());
		} else {
			run = new Run(treeItem->text(0), result.at(0), result.at(1), result.at(2), result.at(3), result.at(4), result.at(5), result.at(6), result.at(7).toInt(), action->statusTip());
		}

		if (run->exec()==QDialog::Accepted)
			CoreLib->runWineBinary(run->execObj);
	}

	return;
}

void MainWindow::lstIcons_ShowContextMenu(const QPoint & iPoint){
  /*
		Function showing context menu
	*/

  QListWidgetItem *iconItem = lstIcons->itemAt(iPoint);
  QTreeWidgetItem *treeItem = twPrograms->currentItem();

  if (iconItem)
	this->lstIcons_ItemClick(iconItem);

  QString cdrom_drive, cdrom_mount;
  QStringList result;
  QList<QStringList> images;

  if (!treeItem)
	return;

  if (treeItem->parent()){
	result = db_prefix->getFieldsByPrefixName(treeItem->parent()->text(0));
  } else {
	result = db_prefix->getFieldsByPrefixName(treeItem->text(0));
  }

  if (result.at(2).isEmpty()){
	xdgOpenPrefixDir->setEnabled(FALSE);
	winefileOpenPrefixDir->setEnabled(FALSE);
  } else {
	xdgOpenPrefixDir->setEnabled(TRUE);
	winefileOpenPrefixDir->setEnabled(TRUE);
  }

  cdrom_drive = result.at(7);
  cdrom_mount = result.at(6);

  if (cdrom_mount.isEmpty()){
	xdgOpenMountDir->setEnabled(FALSE);
	winefileOpenMountDir->setEnabled(FALSE);
  } else {
	xdgOpenMountDir->setEnabled(TRUE);
	winefileOpenMountDir->setEnabled(TRUE);
  }

  menuIconMount->clear();
  menuIconMount->setEnabled(FALSE);

  if (!cdrom_mount.isEmpty()){
	menuIconMount->setEnabled(TRUE);
	QMenu* menuIconMountImages;
	menuIconMountImages = new QMenu(this);
	menuIconMountImages = menuIconMount->addMenu(tr("mount [%1]").arg(CoreLib->getMountedImages(cdrom_mount)));

	if (!cdrom_drive.isEmpty()){
	  menuIconMountImages->addAction(QIcon(":/data/drive_menu.png"), cdrom_drive);
	  menuIconMountImages->addSeparator();
	} else {
		menuIconMountImages->addAction(QIcon(":/data/drive_menu.png"), tr("[none]"));
		menuIconMountImages->addSeparator();
	}

	menuIconMount->addSeparator();

	QMenu* menuIconMountRecentImages;
	menuIconMountRecentImages = new QMenu(this);
	menuIconMountRecentImages = menuIconMount->addMenu(tr("mount ..."));

	menuIconMountRecentImages->addAction(iconMountOther);
	menuIconMountRecentImages->addSeparator();

	QSettings settings(APP_SHORT_NAME, "default");
	QStringList files = settings.value("recent_images").toStringList();

	for (int i = 0; i < files.size(); ++i){
		menuIconMountRecentImages->addAction(QIcon(":/data/cdrom_menu.png") , files.at(i).split("/").last());
	}

	connect (menuIconMountRecentImages, SIGNAL(triggered(QAction*)), this, SLOT(menuMountRecentImages_triggered(QAction*)));

	menuIconMount->addSeparator();
	menuIconMount->addAction(iconUnmount);

	images = db_image->getFields();
	for (int i = 0; i < images.size(); ++i) {
	  menuIconMountImages->addAction(QIcon(":/data/cdrom_menu.png") , images.at(i).at(0));
	}
	connect (menuIconMountImages, SIGNAL(triggered(QAction*)), this, SLOT(menuMountImages_triggered(QAction*)));

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

  if (iconBuffer.names.count()>0){
	iconPaste->setEnabled(TRUE);
  } else {
	iconPaste->setEnabled(FALSE);
  }

  if (lstIcons->selectedItems().count()>0){
	iconDelete->setEnabled(TRUE);
	iconCut->setEnabled(TRUE);
	iconCopy->setEnabled(TRUE);
  } else {
	iconDelete->setEnabled(FALSE);
	iconCut->setEnabled(FALSE);
	iconCopy->setEnabled(FALSE);
  }



  if(iconItem){
	iconRun->setEnabled(TRUE);
	iconOptions->setEnabled(TRUE);
	iconRename->setEnabled(TRUE);
	iconDelete->setEnabled(TRUE);

	if (treeItem->parent()){
	  result=db_icon->getByName(treeItem->parent()->text(0), treeItem->text(0), iconItem->text());
	} else {
	  result=db_icon->getByName(treeItem->text(0), "", iconItem->text());
	}

	if (!result.at(4).isEmpty()){
	  xdgOpenIconDir->setEnabled(TRUE);
	  winefileOpenIconDir->setEnabled(TRUE);
	} else {
	  xdgOpenIconDir->setEnabled(FALSE);
	  winefileOpenIconDir->setEnabled(FALSE);
	}

	menuIcon->exec(QCursor::pos());
  } else {
	xdgOpenIconDir->setEnabled(FALSE);
	winefileOpenIconDir->setEnabled(FALSE);

	//menuRun = new QMenu();
	//menuRun->deleteLater();



	menuRun->addAction(dirRun);
	menuRun->addSeparator();
	result = db_last_run_icon->getIcons();

	for (int i=0; i<result.size(); ++i){
		if (i >= recentIconsList.size()){
			QAction *lol = new QAction(result.at(i).split("/").last(), this);
			lol->setStatusTip(result.at(i));
			recentIconsList.append(lol);
		} else {
			recentIconsList.at(i)->setText(result.at(i).split("/").last());
			recentIconsList.at(i)->setStatusTip(result.at(i));
		}
		menuRun->addAction(recentIconsList.at(i));
	}

	menuIconVoid->exec(QCursor::pos());
  }
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

  QIcon icon = loadIcon("data/q4wine.png");

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

  QTreeWidgetItem *treeItem = twPrograms->currentItem();

  QSettings settings(APP_SHORT_NAME, "default");
  settings.beginGroup("MainWindow");
  settings.setValue("size", size());
  settings.setValue("pos", pos());
  settings.setValue("splitterSize0", splitter->sizes().at(0));
  settings.setValue("splitterSize1", splitter->sizes().at(1));
  settings.endGroup();
  settings.beginGroup("LastPrefix");
  if (treeItem){
	if (treeItem->parent()){
	  settings.setValue("prefix", treeItem->parent()->text(0));
	  settings.setValue("dir", treeItem->text(0));
	} else {
	  settings.setValue("prefix", treeItem->text(0));
	  settings.setValue("dir", "");
	}
  } else {
	settings.setValue("prefix", "");
	settings.setValue("dir", "");
  }
  settings.endGroup();

  return;
}


bool MainWindow::eventFilter(QObject *obj, QEvent *event){

  // twPrograms events
	if (obj == this){
		if (event->type()==QEvent::Resize){
			if (!this->isActiveWindow())
				return false;
		}
	}

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

	  if (keyEvent->key()==Qt::Key_F2){
		if (twPrograms->currentItem())
		  dirRename_Click();
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

	  if (keyEvent->key()==Qt::Key_F2){
		if (lstIcons->currentItem())
		  iconRename_Click();
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


  if (tableProc->currentItem ()){
	processKillSelected->setEnabled(TRUE);
	processKillWine->setEnabled(TRUE);
	processRenice->setEnabled(TRUE);
  } else {
	processKillSelected->setEnabled(FALSE);
	processKillWine->setEnabled(FALSE);
	processRenice->setEnabled(FALSE);
  }


  // Remove unneaded entyes
  numRows = tableProc->rowCount();
  if (numRows > curRows)
	for (int i=curRows; i <= numRows; i++)
	  tableProc->removeRow(curRows);


  lblProcInfo->setText(tr("Total process: %1").arg(numRows));

  return;
}

void MainWindow::cmdCreateFake_Click(){

  QString prefixPath = db_prefix->getPath(cbPrefixes->currentText());
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
  Wizard *createFakeDriveWizard = new Wizard(3, cbPrefixes->currentText());
  if (createFakeDriveWizard->exec()==QDialog::Accepted){
	updateDtabaseConnectedItems(cbPrefixes->currentIndex());
  }
  return;
}

void MainWindow::processKillWine_Click(){
  // Function for killling wineserver for current prefix
  int rowNum;
  rowNum = tableProc->currentRow();

  if (rowNum>=0){
	QString prefixPath;
	prefixPath = tableProc->item(rowNum, 3)->text();

	if (QMessageBox::warning(this, tr("Warning"), tr("This action will send a KILL(-9) signal to all wine proccess with WINEPREFIX='%1'<br><br>Do you really want to proceed?").arg(tableProc->item(rowNum, 3)->text()), QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes){
	  CoreLib->killWineServer(prefixPath);
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

	if (QMessageBox::warning(this, tr("Warning"), tr("This action will send a KILL(-9) signal to proccess '%2' pid: %1<br><br>It is HIGH risk to damage wine normal state.<br><br>Do you really want to proceed?").arg(tableProc->item(rowNum, 0)->text()) .arg(tableProc->item(rowNum, 1)->text()),                   QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes){

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
	  if (db_icon->delIconsByPrefixName(tablePrefix->item(tablePrefix->currentRow(), 0)->text()))
		if(db_dir->delDir(tablePrefix->item(tablePrefix->currentRow(), 0)->text()))
		  db_prefix->delByName(tablePrefix->item(tablePrefix->currentRow(), 0)->text());
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

		Process *exportProcess = new Process(args, CoreLib->getWhichOut("rm"), HOME_PATH, tr("Removing old fake drive.<br>This can take a while..."), tr("Removing old fake drive"));
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

void MainWindow::setMeVisible(bool visible){
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
	setMeVisible(TRUE);

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
	setMeVisible(TRUE);

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
	setMeVisible(TRUE);

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
	setMeVisible(TRUE);

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
	setMeVisible(TRUE);

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

  if (!twPrograms->currentItem())
	return;

  QTreeWidgetItem *treeItem = twPrograms->currentItem();
  QStringList dataList;

  if (!isVisible())
	setMeVisible(TRUE);

  if (isMinimized ())
	showNormal ();

  Run *run;

  if (treeItem->parent()){
	run = new Run(treeItem->parent()->text(0));
  } else {
	run = new Run(treeItem->text(0));
  }

  if (run->exec()==QDialog::Accepted)
	CoreLib->runWineBinary(run->execObj);

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

void MainWindow::mainFirstSteps_Click(){
	CoreLib->openHelpUrl("05-first-steps.html");
}

void MainWindow::mainFAQ_Click(){
	CoreLib->openHelpUrl("00-short-faq.html");
}

void MainWindow::mainIndex_Click(){
	CoreLib->openHelpUrl("index.html");
}

void MainWindow::mainWebsite_Click(){
	CoreLib->openUrl("");
}

void MainWindow::mainDonate_Click(){
	CoreLib->openUrl("donate/");
}

void MainWindow::mainBugs_Click(){
	CoreLib->openUrl("bugs/");
}


void MainWindow::mainExportIcons_Click(){
  /*
	 * main Menu allow export icons
	 */

  if (!isVisible())
	setMeVisible(TRUE);

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

  Process *exportProcess = new Process(args, WRESTOOL_BIN, HOME_PATH, tr("Exporting icon from binary file.<br>This can take a while..."), tr("Exporting icon"), FALSE);

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

	  exportProcess = new Process(args, ICOTOOL_BIN, HOME_PATH, tr("Convering icon from binary file.<br>This can take a while..."), tr("Converting icon"), FALSE);

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

void MainWindow::createMenuActions(){
  /*
	 * Context menu for process manage
	 */

  processKillSelected = new QAction(loadIcon("data/kill.png"), tr("Stop current"), tableProc);
  processKillSelected->setStatusTip(tr("Send TERM signal to selected process"));
  connect(processKillSelected, SIGNAL(triggered()), this, SLOT(processKillSelected_Click()));

  processKillWine = new QAction(loadIcon("data/stop.png"), tr("Stop wine"), tableProc);
  processKillWine->setStatusTip(tr("Send TERM signal to main wine process"));
  connect(processKillWine, SIGNAL(triggered()), this, SLOT(processKillWine_Click()));

  processRefresh = new QAction(loadIcon("data/reload.png"), tr("Refresh list"), tableProc);
  processRefresh->setStatusTip(tr("Refresh process list"));
  connect(processRefresh, SIGNAL(triggered()), this, SLOT(getWineProccessInfo()));

  processRenice = new QAction(tr("Renice"), tableProc);
  processRenice->setStatusTip(tr("Set process priority"));
  connect(processRenice, SIGNAL(triggered()), this, SLOT(processRenice_Click()));

  /*
	 * Context menu for directory manage
	 */

  dirRun = new QAction(QIcon(":/data/folder.png"), tr("Browse ..."), twPrograms);
  dirRun->setStatusTip(tr("Bowse for application"));
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

  dirMountOther = new QAction(QIcon(":/data/folder.png"), tr("Browse ..."), lstIcons);
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

  iconUnmount = new QAction(tr("umount"), lstIcons);
  iconUnmount->setStatusTip(tr("Unmount image"));
  connect(iconUnmount, SIGNAL(triggered()), this, SLOT(iconUnmount_Click()));

  iconMountOther = new QAction(QIcon(":/data/folder.png"), tr("Browse ..."), lstIcons);
  iconMountOther->setStatusTip(tr("Browse for other image"));
  connect(iconMountOther, SIGNAL(triggered()), this, SLOT(iconMountOther_Click()));

  /*
	 * Folder open
	 */

  xdgOpenIconDir = new QAction(tr("Open icon directory"), lstIcons);
  xdgOpenIconDir->setStatusTip(tr("Open directory for current program"));
  connect(xdgOpenIconDir, SIGNAL(triggered()), this, SLOT(xdgOpenIconDir_Click()));

  xdgOpenPrefixDir = new QAction(tr("Open prefix directory"), lstIcons);
  xdgOpenPrefixDir->setStatusTip(tr("Open prefix directory"));
  connect(xdgOpenPrefixDir, SIGNAL(triggered()), this, SLOT(xdgOpenPrefixDir_Click()));

  xdgOpenMountDir = new QAction(tr("Open mount point directory"), lstIcons);
  xdgOpenMountDir->setStatusTip(tr("Open mount point directory for current prefix"));
  connect(xdgOpenMountDir, SIGNAL(triggered()), this, SLOT(xdgOpenMountDir_Click()));

  winefileOpenIconDir = new QAction(tr("Open icon directory"), lstIcons);
  winefileOpenIconDir->setStatusTip(tr("Open directory for current program"));
  connect(winefileOpenIconDir, SIGNAL(triggered()), this, SLOT(winefileOpenIconDir_Click()));

  winefileOpenPrefixDir = new QAction(tr("Open prefix directory"), lstIcons);
  winefileOpenPrefixDir->setStatusTip(tr("Open prefix directory"));
  connect(winefileOpenPrefixDir, SIGNAL(triggered()), this, SLOT(winefileOpenPrefixDir_Click()));

  winefileOpenMountDir = new QAction(tr("Open mount point directory"), lstIcons);
  winefileOpenMountDir->setStatusTip(tr("Open mount point directory for current prefix"));
  connect(winefileOpenMountDir, SIGNAL(triggered()), this, SLOT(winefileOpenMountDir_Click()));

  /*
	 * Context menus for prefix manage
	 */

  prefixAdd = new QAction(loadIcon("data/wizard.png"), tr("Create new"), lstIcons);
  prefixAdd->setStatusTip(tr("Create new prefix"));
  connect(prefixAdd, SIGNAL(triggered()), this, SLOT(prefixAdd_Click()));

  prefixImport = new QAction(loadIcon("data/down.png"), tr("Import prefix"), lstIcons);
  prefixImport->setStatusTip(tr("Import prefix"));
  connect(prefixImport, SIGNAL(triggered()), this, SLOT(prefixImport_Click()));
  prefixImport->setEnabled(FALSE);

  prefixExport = new QAction(loadIcon("data/up.png"), tr("Export prefix"), lstIcons);
  prefixExport->setStatusTip(tr("Export prefix"));
  connect(prefixExport, SIGNAL(triggered()), this, SLOT(prefixExport_Click()));
  prefixExport->setEnabled(FALSE);

  prefixDelete = new QAction(loadIcon("data/kill.png"), tr("Delete prefix"), lstIcons);
  prefixDelete->setStatusTip(tr("Delete prefix"));
  connect(prefixDelete, SIGNAL(triggered()), this, SLOT(prefixDelete_Click()));
  prefixDelete->setEnabled(FALSE);

  prefixSettings = new QAction(loadIcon("data/configure.png"), tr("Edit prefix settings"), lstIcons);
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

  menuIconMount = new QMenu(tr("Mount iso..."));
  //menuIconMount->addAction(tr("Mount iso..."));


  menuIcon = new QMenu(this);
  menuIcon->addAction(iconRun);
  menuIcon->addSeparator();
  menuIcon->addMenu(menuIconMount);
  menuIcon->addSeparator();
  menuIcon->addAction(iconOptions);
  menuIconXdgOpendir = new QMenu(this);
  menuIconXdgOpendir = menuIcon->addMenu(tr("Browser"));
  menuIconXdgOpendir->addAction(xdgOpenIconDir);
  menuIconXdgOpendir->addAction(xdgOpenMountDir);
  menuIconXdgOpendir->addAction(xdgOpenPrefixDir);
  menuIconWineOpendir = new QMenu(this);
  menuIconWineOpendir = menuIcon->addMenu(tr("Wine browser"));
  menuIconWineOpendir->addAction(winefileOpenIconDir);
  menuIconWineOpendir->addAction(winefileOpenMountDir);
  menuIconWineOpendir->addAction(winefileOpenPrefixDir);
  menuIcon->addSeparator();
  menuIcon->addAction(iconCut);
  menuIcon->addAction(iconCopy);
  menuIcon->addAction(iconPaste);
  menuIcon->addAction(iconRename);
  menuIcon->addAction(iconDelete);
  menuIcon->addSeparator();
  menuIcon->addAction(iconAdd);

  menuIconVoid = new QMenu(this);
  menuRun = new QMenu(tr("Run..."), this);

  menuIconVoid->addMenu(menuRun);
  menuIconVoid->addSeparator();
  menuIconVoid->addAction(iconAdd);
  menuIconVoid->addSeparator();
  menuIconVoid->addAction(iconCut);
  menuIconVoid->addAction(iconCopy);
  menuIconVoid->addAction(iconPaste);
  menuIconVoid->addAction(iconDelete);
  menuIconVoid->addSeparator();
  menuIconVoid->addMenu(menuIconMount);
  menuIconVoid->addSeparator();
  menuIconXdgOpendir = new QMenu(this);
  menuIconXdgOpendir = menuIconVoid->addMenu(tr("Browser"));
  menuIconXdgOpendir->addAction(xdgOpenMountDir);
  menuIconXdgOpendir->addAction(xdgOpenPrefixDir);
  menuIconWineOpendir = new QMenu(this);
  menuIconWineOpendir = menuIconVoid->addMenu(tr("Wine browser"));
  menuIconWineOpendir->addAction(winefileOpenMountDir);
  menuIconWineOpendir->addAction(winefileOpenPrefixDir);

  menuDirMount = new QMenu(tr("Mount iso..."));

  menuDir = new QMenu(this);
  menuDir->addAction(dirAdd);
  menuDir->addSeparator();
  menuDir->addMenu(menuDirMount);
  menuDir->addSeparator();
  menuDir->addMenu(menuRun);
  menuDir->addSeparator();
  menuDir->addAction(dirRename);
  menuDir->addSeparator();
  menuDir->addAction(dirDelete);
  menuDir->addSeparator();
  menuIconXdgOpendir = new QMenu(this);
  menuIconXdgOpendir = menuDir->addMenu(tr("Browser"));
  menuIconXdgOpendir->addAction(xdgOpenMountDir);
  menuIconXdgOpendir->addAction(xdgOpenPrefixDir);
  menuIconWineOpendir = new QMenu(this);
  menuIconWineOpendir = menuDir->addMenu(tr("Wine browser"));
  menuIconWineOpendir->addAction(winefileOpenMountDir);
  menuIconWineOpendir->addAction(winefileOpenPrefixDir);

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

void MainWindow::createToolBarActions(){
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

  QList<QListWidgetItem *> icoList = lstIcons->selectedItems();
  QTreeWidgetItem *treeItem = twPrograms->currentItem();

  if (!treeItem)
	return;
  if (icoList.count()<0)
	return;

  if (QMessageBox::warning(this, tr("Delete Icon"), tr("Do you want to delete all selected icons?"),  QMessageBox::Yes, QMessageBox::No	)==QMessageBox::Yes){
	for (int i=0; i<icoList.count(); i++){
	  if(!treeItem->parent()){
		db_icon->delIcon(treeItem->text(0), icoList.at(i)->text());
	  }else{
		db_icon->delIcon(treeItem->parent()->text(0), treeItem->text(0), icoList.at(i)->text());
	  }
	}
	twPrograms_ItemClick(treeItem, 0);
  }
  return;
}

void MainWindow::iconRun_Click(void){

  QListWidgetItem *iconItem=lstIcons->selectedItems().first();
  if (iconItem)
	lstIcons_ItemDoubleClick(iconItem);
  return;
}

void MainWindow::iconRename_Click(void){
  QTreeWidgetItem *treeItem=twPrograms->currentItem();
  QListWidgetItem *iconItem=lstIcons->selectedItems().first();
  QString prefix_name, dir_name;
  bool ok;

  if (!treeItem)
	return;
  if (!iconItem)
	return;

  if (!treeItem->parent()){
	prefix_name = treeItem->text(0);
	dir_name = "";
  } else {
	prefix_name = treeItem->parent()->text(0);
	dir_name = treeItem->text(0);
  }

  QString newName = QInputDialog::getText(this, tr("Enter new icon name"), tr("Icon name:"), QLineEdit::Normal, iconItem->text(), &ok);

  if (ok && !newName.isEmpty()){
	while (db_icon->isExistsByName(prefix_name, dir_name, newName)){
	  newName = QInputDialog::getText(this, tr("Sorry. It seems icon already exists."), tr("Sorry. It seems icon already exists.<br>Please choose another name, or cancel operation."), QLineEdit::Normal, newName, &ok);
	  if ((!ok) || (newName.isEmpty())){
		return;
	  }
	}
	db_icon->renameIcon(iconItem->text(), prefix_name, dir_name, newName);
  }

  twPrograms_ItemClick(treeItem, 0);
  return;
}

void MainWindow::iconAdd_Click(void){
  QTreeWidgetItem *treeItem;
  IconSettings *iconAddWizard;
  treeItem = twPrograms->currentItem();

  if (!treeItem)
	return;

  if (treeItem->parent()){
	iconAddWizard = new IconSettings(treeItem->parent()->text(0), treeItem->text(0));
  } else {
	iconAddWizard = new IconSettings(treeItem->text(0), "");
  }

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
  return;
}

void MainWindow::iconCut_Click(void){
  /*
		This function fill iconBuffer with selected icons names
		and sets other informations required for copy\cut

		see struct iconCopyBuffer definition for details
	*/
  if (!twPrograms->currentItem())
	return;

  QList<QListWidgetItem *> icoList = lstIcons->selectedItems();
  QTreeWidgetItem *treeItem = twPrograms->currentItem();

  // If icon cutted -- set icon disabled style
  // FIXME: It pice works fine, but we nead to usse pixmaps for grayscale, not Qt::ItemIsEnabled flag....
  // FIXME: It is optional, i don't work on it until release... ;)
  //icoList2 = lstIcons->findItems("*", Qt::MatchWrap | Qt::MatchWildcard);

  //	for (int i=0; i<icoList2.count(); i++){
  //		icoList2.at(i)->icon()->addPixmap(QPixmap.alphaChannel (), 0, 0);
  //	}


  // Clearing icon buffer
  iconBuffer.names.clear();
  iconBuffer.dir_name="";
  iconBuffer.prefix_name="";
  iconBuffer.move=true;

  // Fiffing buffer with new items
  for (int i=0; i<icoList.count(); i++){
	iconBuffer.names.append(icoList.at(i)->text());
	//icoList.at(i)->setFlags(Qt::ItemIsEnabled);
  }

  if (treeItem->parent()){
	iconBuffer.prefix_name = treeItem->parent()->text(0);
	iconBuffer.dir_name = treeItem->text(0);
  } else {
	iconBuffer.prefix_name = treeItem->text(0);
	iconBuffer.dir_name = "";
  }

  return;
}

void MainWindow::iconCopy_Click(void){
  /*
	This function fill iconBuffer with selected icons names
	and sets other informations required for copy\cut

	see struct iconCopyBuffer definition for details
	*/

  if (!twPrograms->currentItem())
	return;

  QList<QListWidgetItem *> icoList = lstIcons->selectedItems();
  QTreeWidgetItem *treeItem = twPrograms->currentItem();

  // Clearing icon buffer
  iconBuffer.names.clear();
  iconBuffer.dir_name="";
  iconBuffer.prefix_name="";
  iconBuffer.move=false;

  // Fiffing buffer with new items
  for (int i=0; i<icoList.count(); i++){
	iconBuffer.names.append(icoList.at(i)->text());
  }
  if (treeItem->parent()){
	iconBuffer.prefix_name = treeItem->parent()->text(0);
	iconBuffer.dir_name = treeItem->text(0);
  } else {
	iconBuffer.prefix_name = treeItem->text(0);
	iconBuffer.dir_name = "";
  }
  return;
}

void MainWindow::iconPaste_Click(void){

  if (!twPrograms->currentItem())
	return;

  QTreeWidgetItem *treeItem = twPrograms->currentItem();
  bool fexists=FALSE, ok;
  QString newName, prefix_name, dir_name;

  if (iconBuffer.names.count()>0){
	for (int i=0; i<iconBuffer.names.count(); i++){
	  // Checking for not unic names
	  newName = iconBuffer.names.at(i);
	  fexists=FALSE;
	  ok=FALSE;

	  if (!treeItem->parent()){
		prefix_name = treeItem->text(0);
		dir_name = "";
	  } else {
		prefix_name = treeItem->parent()->text(0);
		dir_name = treeItem->text(0);
	  }

	  while (db_icon->isExistsByName(prefix_name, dir_name, newName)){
		newName = QInputDialog::getText(this, tr("Sorry. It seems icon already exists."), tr("Sorry. It seems icon already exists.<br>Please choose another name, or cancel operation."), QLineEdit::Normal, iconBuffer.names.at(i) , &ok);
		if (!ok){
		  return;
		}
	  }

	  switch (iconBuffer.move){
				case FALSE:
		if (!db_icon->copyIcon(iconBuffer.names.at(i), iconBuffer.prefix_name, iconBuffer.dir_name, newName, prefix_name, dir_name))
		  return;
		break;
				case TRUE:
		if (!db_icon->updateIcon(newName, db_prefix->getId(prefix_name), db_dir->getId(dir_name, prefix_name), db_prefix->getId(iconBuffer.prefix_name), db_dir->getId(iconBuffer.dir_name, iconBuffer.prefix_name), iconBuffer.names.at(i)))
		  return;
		break;
	  }

	}
	iconBuffer.names.clear();
	iconBuffer.dir_name="";
	iconBuffer.prefix_name="";
	iconBuffer.move=false;
  }

  twPrograms_ItemClick(twPrograms->currentItem(), 0);
  return;
}

void MainWindow::iconOption_Click(void){
  QTreeWidgetItem *treeItem = twPrograms->currentItem();
  QListWidgetItem *iconItem = lstIcons->currentItem();

  if (!treeItem)
	return;
  if (!iconItem)
	return;

  IconSettings *iconAddWizard;

  if (treeItem->parent()){
	iconAddWizard = new IconSettings(treeItem->parent()->text(0), treeItem->text(0), iconItem->text());
  } else {
	iconAddWizard = new IconSettings(treeItem->text(0), "", iconItem->text());
  }

  if (iconAddWizard->exec() == QDialog::Accepted){
	// Updating icons view
	twPrograms_ItemClick(treeItem, 0);
  }
  return;
}

void MainWindow::xdgOpenIconDir_Click(void){
  QTreeWidgetItem *treeItem = twPrograms->currentItem();
  QListWidgetItem *iconItem = lstIcons->currentItem();
  QStringList result;
  if (!treeItem)
	return;
  if (!iconItem)
	return;

  if (treeItem->parent()){
	result = db_icon->getByName(treeItem->parent()->text(0), treeItem->text(0), iconItem->text());
  } else {
	result = db_icon->getByName(treeItem->text(0), "", iconItem->text());
  }

  QStringList args;
  args<<result.at(4);
  CoreLib->runProcess(CoreLib->getWhichOut("xdg-open"), args, "", FALSE);

  return;
}

void MainWindow::xdgOpenPrefixDir_Click(void){
  QTreeWidgetItem *treeItem = twPrograms->currentItem();

  QString result;
  if (!treeItem)
	return;

  if (treeItem->parent()){
	result = db_prefix->getPath(treeItem->parent()->text(0));
  } else {
	result = db_prefix->getPath(treeItem->text(0));
  }

  QStringList args;
  args<<result;
  CoreLib->runProcess(CoreLib->getWhichOut("xdg-open"), args, "", FALSE);

  return;
}


void MainWindow::xdgOpenMountDir_Click(void){
  QTreeWidgetItem *treeItem = twPrograms->currentItem();

  QString result;
  if (!treeItem)
	return;

  if (treeItem->parent()){
	result = db_prefix->getMountPath(treeItem->parent()->text(0));
  } else {
	result = db_prefix->getMountPath(treeItem->text(0));
  }

  QStringList args;
  args<<result;
  CoreLib->runProcess(CoreLib->getWhichOut("xdg-open"), args, "", FALSE);
}



void MainWindow::winefileOpenIconDir_Click(void){
  QTreeWidgetItem *treeItem = twPrograms->currentItem();
  QListWidgetItem *iconItem = lstIcons->currentItem();
  QStringList result;
  if (!treeItem)
	return;
  if (!iconItem)
	return;

  if (treeItem->parent()){
	result = db_icon->getByName(treeItem->parent()->text(0), treeItem->text(0), iconItem->text());
	CoreLib->runWineBinary("winefile", result.at(4) + "/", treeItem->parent()->text(0));
  } else {
	result = db_icon->getByName(treeItem->text(0), "", iconItem->text());
	CoreLib->runWineBinary("winefile", result.at(4) + "/", treeItem->text(0));
  }

  return;
}

void MainWindow::winefileOpenPrefixDir_Click(void){
  QTreeWidgetItem *treeItem = twPrograms->currentItem();

  QString result;
  if (!treeItem)
	return;

  if (treeItem->parent()){
	result = db_prefix->getPath(treeItem->parent()->text(0));
	CoreLib->runWineBinary("winefile", result + "/", treeItem->parent()->text(0));
  } else {
	result = db_prefix->getPath(treeItem->text(0));
	CoreLib->runWineBinary("winefile", result + "/", treeItem->text(0));
  }

  return;
}


void MainWindow::winefileOpenMountDir_Click(void){
  QTreeWidgetItem *treeItem = twPrograms->currentItem();

  QString result;
  if (!treeItem)
	return;

  if (treeItem->parent()){
	result = db_prefix->getMountPath(treeItem->parent()->text(0));
	CoreLib->runWineBinary("winefile", result + "/", treeItem->parent()->text(0));
  } else {
	result = db_prefix->getMountPath(treeItem->text(0));
	CoreLib->runWineBinary("winefile", result + "/", treeItem->text(0));
  }

}


void MainWindow::dirAdd_Click(void){
  QTreeWidgetItem *treeItem = twPrograms->currentItem();
  QTreeWidgetItem *prefixItem;

  if (!treeItem->text(0).isEmpty()){
	bool ok;
	QString dirname = QInputDialog::getText(this, tr("Enter new directory name"), tr("Directory name:"), QLineEdit::Normal, "" , &ok);

	if (ok && !dirname.isEmpty()){

	  if (treeItem->parent()){
		if (db_dir->isExistsByName(treeItem->parent()->text(0), dirname)){
		  QMessageBox::warning(this, tr("Error"), tr("Sorry, but directory named %1 already exists.").arg(dirname));
		  return;
		}

		if (!db_dir->addDir(treeItem->parent()->text(0), dirname))
		  return;
		prefixItem = new QTreeWidgetItem(treeItem->parent());
	  } else {
		if (db_dir->isExistsByName(treeItem->text(0), dirname)){
		  QMessageBox::warning(this, tr("Error"), tr("Sorry, but directory named %1 already exists.").arg(dirname));
		  return;
		}

		if (!db_dir->addDir(treeItem->text(0), dirname))
		  return;
		prefixItem = new QTreeWidgetItem(treeItem);
	  }

	  prefixItem->setText(0, dirname);
	  prefixItem->setIcon(0, loadIcon("data/folder.png"));
	}
  }
  return;
}

void MainWindow::dirUnmount_Click(void){
  /*
		Request for unmounting cdrom drve described at wine prefix settings
	*/

  bool ret=false;
  if (twPrograms->currentItem()){
	if (twPrograms->currentItem()->parent()){
	  ret=CoreLib->umountImage(twPrograms->currentItem()->parent()->text(0));
	} else {
	  ret=CoreLib->umountImage(twPrograms->currentItem()->text(0));
	}
  }

  if (ret){
	statusBar()->showMessage(QObject::tr("Drive successfully umounted."));
  } else {
	statusBar()->showMessage(QObject::tr("Fail to umount drive."));
  }
  return;
}

void MainWindow::dirMountOther_Click(void){
  /*
	Request for unmounting cdrom drve described at wine prefix settings
	*/
#ifdef _OS_LINUX_
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open CD Image files"), HOME_PATH, tr("CD image files (*.iso *.nrg *.img *.bin *.mdf)"));
#endif

#ifdef _OS_FREEBSD_
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open ISO Image file"), HOME_PATH, tr("iso files (*.iso)"));
#endif

  if(fileName.isEmpty()){
	return;
  }

  bool ret=false;
  if (twPrograms->currentItem()){
	if (twPrograms->currentItem()->parent()){
	  ret=CoreLib->mountImage(fileName, twPrograms->currentItem()->parent()->text(0));
	} else {
	  ret=CoreLib->mountImage(fileName, twPrograms->currentItem()->text(0));
	}
  }

  if (ret){
	statusBar()->showMessage(QObject::tr("%1 successfully mounted.").arg(fileName));
  } else {
	statusBar()->showMessage(QObject::tr("Fail to mount %1.").arg(fileName));
  }

  QSettings settings(APP_SHORT_NAME, "default");
  QStringList files = settings.value("recent_images").toStringList();
  files.removeAll(fileName);
  files.prepend(fileName);
  while (files.size() > 8)
	  files.removeLast();

  settings.setValue("recent_images", files);

  return;
}

void MainWindow::dirConfigure_Click(void){
  //FIXME: What is this? ;)
  //RunWineUtils("winecfg", twPrograms->currentItem());
  return;
}

void MainWindow::dirInstall_Click(void){
  //FIXME: no install wizard yet...
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
  QTreeWidgetItem *treeItem = twPrograms->currentItem();

  if (!treeItem)
	return;

  if (treeItem->parent()){
	bool ok = FALSE;
	QString newName = QInputDialog::getText(this, tr("Enter new name for directory"), tr("Directory name:"), QLineEdit::Normal, treeItem->text(0) , &ok);

	if (ok && !newName.isEmpty()){
	  while (db_dir->isExistsByName(treeItem->parent()->text(0), newName)){
		newName = QInputDialog::getText(this, tr("Sorry. It seems directory already exists."), tr("Sorry. It seems directory already exists.<br>Please choose another name, or cancel operation."), QLineEdit::Normal, newName, &ok);
		if ((!ok) || (newName.isEmpty())){
		  return;
		}
	  }
	  db_dir->renameDir(treeItem->text(0), treeItem->parent()->text(0), newName);
	  treeItem->setText(0, newName);
	}

  }
  return;
}

void MainWindow::dirDelete_Click(void){
  QTreeWidgetItem *treeItem;
  treeItem = twPrograms->currentItem();

  if (!treeItem)
	return;

  if (treeItem->parent()){

	if (QMessageBox::warning(this, tr("Q4Wine"), tr("Do you really wish delete folder named \"%1\" and all associated icons?\n").arg(twPrograms->currentItem()->text(0)),
							 QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes){

	  if (db_icon->delIcon(treeItem->parent()->text(0), treeItem->text(0), ""))
		if(db_dir->delDir(treeItem->parent()->text(0), treeItem->text(0)))
		  treeItem->parent()->removeChild (treeItem);
	  twPrograms_ItemClick(twPrograms->currentItem(), 0);
	}
  }

  return;
}

void MainWindow::runAutostart(void){
  QList<QStringList> iconsList, prefixList;

  prefixList = db_prefix->getFields();
  for (int i = 0; i < prefixList.size(); ++i) {
	iconsList = db_icon->getByPrefixAndDirName(prefixList.at(i).at(1), "autostart");
	for (int j = 0; j < iconsList.size(); ++j) {
	  CoreLib->runIcon(prefixList.at(i).at(1), "autostart", iconsList.at(j).at(1));
	}
  }
  return;
}

void MainWindow::messageReceived(const QString message) const{
	statusBar()->showMessage(message);
	return;
}
