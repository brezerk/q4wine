/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010 by Malakhov Alexey                     *
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

#include "iconlistwidget.h"

IconListWidget::IconListWidget(QWidget *parent) : QListWidget (parent)
{
	  // Loading libq4wine-core.so
	  libq4wine.setFileName("libq4wine-core");

	  if (!libq4wine.load()){
			libq4wine.load();
	  }

	  // Getting corelib calss pointer
	  CoreLibClassPointer = (CoreLibPrototype *) libq4wine.resolve("createCoreLib");
	  CoreLib.reset((corelib *)CoreLibClassPointer(true));

	  setAcceptDrops(true);
	  setViewMode(QListView::IconMode);
	  setGridSize(QSize(86, 86));
	  setResizeMode(QListView::Adjust);
	  setWrapping(true);
	  setWordWrap(true);
	  setAcceptDrops(true);
	  setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	  setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	  setMovement(QListView::Snap);
	  setDragDropMode(QAbstractItemView::InternalMove);
	  setSelectionMode(QAbstractItemView::ContiguousSelection);
	  setIconSize(QSize(32, 32));

	  connect(this, SIGNAL(itemClicked (QListWidgetItem *)), this, SLOT(itemClicked (QListWidgetItem *)));
	  connect(this, SIGNAL(itemDoubleClicked (QListWidgetItem *)), this, SLOT(itemDoubleClicked (QListWidgetItem *)));

	  this->dirName="";
	  this->prefixName="";
	  this->prefixMontPoint="";
	  this->prefixMediaDrive="";
	  this->filterString="";
}

void IconListWidget::showFolderContents(QString prefixName, QString dirName){
	this->prefixName=prefixName;
	this->dirName=dirName;

	this->showContents(this->filterString);
	return;
}

void IconListWidget::showContents(QString filterString){
	  this->clear();
	  emit(iconItemClick("","","","",""));

	  QList<QStringList> iconsList=db_icon.getByPrefixAndDirName(this->prefixName, this->dirName, filterString);

	  for (int i = 0; i < iconsList.size(); ++i) {
			std::auto_ptr<QListWidgetItem> iconItem (new QListWidgetItem(this, 0));
			iconItem->setText(iconsList.at(i).at(1));

			//Seting icon. If no icon or icon file not exists -- setting default
			if (iconsList.at(i).at(3).isEmpty()){
				  iconItem->setIcon(CoreLib->loadIcon("data/exec_wine.png"));
			} else {
				  if (QFile::exists (iconsList.at(i).at(3))){
						iconItem->setIcon(QIcon(iconsList.at(i).at(3)));
				  } else {
						if (iconsList.at(i).at(3)=="wineconsole"){
							  iconItem->setIcon(CoreLib->loadIcon("data/wineconsole.png"));
						} else if (iconsList.at(i).at(3)=="regedit"){
							  iconItem->setIcon(CoreLib->loadIcon("data/regedit.png"));
						} else if (iconsList.at(i).at(3)=="wordpad"){
							  iconItem->setIcon(CoreLib->loadIcon("data/notepad.png"));
						} else if (iconsList.at(i).at(3)=="winecfg"){
							  iconItem->setIcon(CoreLib->loadIcon("data/winecfg.png"));
						} else if (iconsList.at(i).at(3)=="uninstaller"){
							  iconItem->setIcon(CoreLib->loadIcon("data/uninstaller.png"));
						} else if (iconsList.at(i).at(3)=="eject"){
							  iconItem->setIcon(CoreLib->loadIcon("data/eject.png"));
						} else if (iconsList.at(i).at(3)=="explorer"){
							  iconItem->setIcon(CoreLib->loadIcon("data/explorer.png"));
						} else {
							  iconItem->setIcon(CoreLib->loadIcon("data/exec_wine.png"));
						}
				  }
			}
			iconItem.release();
	  }

	  QStringList result = db_prefix.getFieldsByPrefixName(this->prefixName);
	  this->prefixMediaDrive = result.at(7);
	  this->prefixMontPoint = result.at(6);

	  return;
}

void IconListWidget::setFilterString(QString filterString){
	this->filterString=filterString;
	this->showContents(filterString);
	return;
}

void IconListWidget::startDrag(){
	  if (this->prefixName.isEmpty())
			return;

	  QList<QListWidgetItem *> items = this->selectedItems ();

	  if (items.count()>0){
			QString fileName;
			std::auto_ptr<QMimeData> mimeData(new QMimeData());
			QList<QUrl> urls;

			for (int i=0; i<items.count(); i++){
				  fileName = CoreLib->createDesktopFile(this->prefixName, this->dirName, items.at(i)->text());
				  urls<<QUrl::fromLocalFile(fileName);
			}

			mimeData->setUrls(urls);
			std::auto_ptr<QDrag> drag(new QDrag(this));
			drag->setMimeData(mimeData.release());
			drag->setPixmap(items.at(0)->icon().pixmap(32));
			drag->start(Qt::MoveAction);
			drag.release();
	  }
}

void IconListWidget::startDrop(QList<QUrl> files){
	  if (this->prefixName.isEmpty())
			return;

	  for (int i=0; i < files.count(); i++){
			if (files.at(i).toLocalFile().contains(".exe", Qt::CaseInsensitive) || files.at(i).toLocalFile().contains(".bat", Qt::CaseInsensitive) || files.at(i).toLocalFile().contains(".com", Qt::CaseInsensitive)){
				  QString file="", fileName="";
				  QStringList list1;
				  file = files.at(i).toLocalFile();
				  list1 = file.split("/");
				  fileName=list1.last().left(list1.last().length() - list1.last().split(".").last().length() - 1);

				  while (db_icon.isExistsByName(this->prefixName, this->dirName, fileName)){
						bool ok=false;
						fileName = QInputDialog::getText(this, tr("Sorry. It seems icon already exists."), tr("Sorry. It seems icon already exists.<br>Please choose another name, or cancel operation."), QLineEdit::Normal, fileName , &ok);

						if (!ok){
							  this->showContents("");
							  return;
						}
				  }

				  if (files.at(i).toLocalFile().contains(".bat", Qt::CaseInsensitive)){
					  QString localfile=files.at(i).toLocalFile();
						file = "--backend=user ";
						file.append(localfile.split("/").last());
						db_icon.addIcon(file, "wineconsole", "", "", this->prefixName, this->dirName, fileName, "", "", "", "", localfile.left(localfile.length() - localfile.split("/").last().length()), "", 0);
				  } else {
						db_icon.addIcon("", file, "", "", this->prefixName, this->dirName, fileName, "", "", "", "", file.left(file.length() - file.split("/").last().length()), "", 0);
				  }
			}
	  }
	  this->showContents("");
}

void IconListWidget::itemClicked (QListWidgetItem *item){
	  if (!item)
			return;

	  QStringList result=db_icon.getByName(this->prefixName, this->dirName, item->text());
	  emit(iconItemClick(result.at(10).split('/').last().split('\\').last(), result.at(9), result.at(2), result.at(7), result.at(11)));
}

void IconListWidget::itemDoubleClicked (QListWidgetItem *item){
	  if (!item)
			return;

	  emit(changeStatusText(tr("Prepare to run wine binary...")));
	  if (CoreLib->runIcon(this->prefixName, this->dirName, item->text())){
			emit(changeStatusText(tr("%1 started.").arg(item->text())));
	  } else {
			emit(changeStatusText(tr("%1 fail to start.").arg(item->text())));
	  }
	  return;
}

void IconListWidget::mousePressEvent(QMouseEvent *event){
	  if (itemAt(event->pos())){
			if (event->button() == Qt::LeftButton)
				  startPos = event->pos();
			drag=false;
			dragstarted=true;
	  } else {
			dragstarted=false;
	  }

	  if (QApplication::keyboardModifiers()==Qt::CTRL){
			std::auto_ptr<QListWidgetItem> item (this->itemAt(event->x(), event->y()));
			if (item.get()){
				  QListWidget::itemClicked ( item.get() );
				  if (!item->isSelected()){
						item->setSelected(true);
				  } else {
						item->setSelected(false);
				  }
			}
			item.release();
	  } else {
			QListWidget::mousePressEvent(event);
	  }
}

void IconListWidget::mouseMoveEvent(QMouseEvent *event){
	  if (itemAt(event->pos()) && dragstarted){
			if (event->buttons() & Qt::LeftButton){
				  int distance = (event->pos() - startPos).manhattanLength();
				  if (distance > QApplication::startDragDistance())
						if (currentItem() && !drag){
						emit startDrag();
						drag=true;
				  }
				  return;
			}
	  }
	  QListWidget::mouseMoveEvent(event);
}

void IconListWidget::dragEnterEvent(QDragEnterEvent *event){
	  if (event->mimeData()->hasFormat("text/uri-list"))
	  {
			QList<QUrl> list = event->mimeData()->urls();
			for (int i=0; i < list.count(); i++){
				  //Accept only .exe, .bat or .com files
				  if (list.at(i).toLocalFile().contains(".exe", Qt::CaseInsensitive) || list.at(i).toLocalFile().contains(".bat", Qt::CaseInsensitive) || list.at(i).toLocalFile().contains(".com", Qt::CaseInsensitive)){
						event->acceptProposedAction();
						break;
				  }
			}
	  }
}

void IconListWidget::dragMoveEvent(QDragMoveEvent *event){
	  std::auto_ptr<IconListWidget> source (qobject_cast<IconListWidget *>(event->source()));
	  if (source.get() && source.get() != this){
			event->setDropAction(Qt::MoveAction);
			event->accept();
	  }
	  source.release();
}

void IconListWidget::dropEvent(QDropEvent *event){
	  std::auto_ptr<IconListWidget> source (qobject_cast<IconListWidget *>(event->source()));
	  if (source.get() && source.get() != this){
			event->setDropAction(Qt::MoveAction);
			event->accept();
	  }
	  source.release();
	  emit(startDrop(event->mimeData()->urls()));
}

void IconListWidget::contextMenuEvent (QContextMenuEvent * event){
	  if (this->prefixName.isEmpty())
			return;

	  bool isItemSelected=false, isOnItemEvent=false;

	  std::auto_ptr<QListWidgetItem> item (this->itemAt(event->pos().x(), event->pos().y()));
	  if (item.get()){
			isOnItemEvent=true;
	  } else {
			isOnItemEvent=false;
	  }
	  item.release();

	  if (this->selectedItems().count()>0){
			isItemSelected=true;
	  } else {
			isItemSelected=false;
	  }

	  std::auto_ptr<QMenu> menu (new QMenu(this));

	  std::auto_ptr<QMenu> menuMount (new QMenu(tr("Mount iso..."), this));

	  if (this->prefixMontPoint.isEmpty()){
			menuMount->setEnabled(false);
	  } else {
			std::auto_ptr<QMenu> submenuMount (new QMenu(tr("Mount [%1]").arg(CoreLib->getMountedImages(this->prefixMontPoint)), this));
			std::auto_ptr<QAction> entry;

			if (this->prefixMediaDrive.isEmpty()){
				  entry.reset (new QAction(CoreLib->loadIcon("/data/drive_menu.png"), tr("[none]"), this));
				  entry->setStatusTip(tr("No media was set in prefix settings."));
				  submenuMount->addAction(entry.release());
			} else {
				  entry.reset (new QAction(CoreLib->loadIcon("/data/drive_menu.png"), this->prefixMediaDrive, this));
				  entry->setStatusTip(tr("Mount media drive."));
				  submenuMount->addAction(entry.release());
			}

			QList<QStringList> images = db_image.getFields();

			if (images.size()>0)
				  submenuMount->addSeparator();

			for (int i = 0; i < images.size(); ++i) {
				  entry.reset (new QAction(CoreLib->loadIcon("/data/cdrom_menu.png"), images.at(i).at(0), this));
				  entry->setStatusTip(tr("Mount media image."));
				  submenuMount->addAction(entry.release());
			}

			connect(submenuMount.get(), SIGNAL(triggered(QAction*)), this, SLOT(menuMount_triggered(QAction*)));
			menuMount->addMenu(submenuMount.release());
			menuMount->addSeparator();

			submenuMount.reset (new QMenu(tr("Mount ..."), this));

			entry.reset (new QAction(CoreLib->loadIcon("/data/folder.png"), tr("Browse..."), this));
			entry->setStatusTip(tr("Browse for media image."));
			submenuMount->addAction(entry.release());

			QStringList files = CoreLib->getSetting("", "recent_images", false).toStringList();

			if (files.size()>0)
				  submenuMount->addSeparator();

			for (int i = 0; i < files.size(); ++i){
				  entry.reset (new QAction(CoreLib->loadIcon("/data/cdrom_menu.png"),files.at(i).split("/").last(), this));
				  entry->setStatusTip(files.at(i));
				  submenuMount->addAction(entry.release());
			}

			connect(submenuMount.get(), SIGNAL(triggered(QAction*)), this, SLOT(menuMount_triggered(QAction*)));
			menuMount->addMenu(submenuMount.release());
			menuMount->addSeparator();

			entry.reset (new QAction(tr("Unmount [%1]").arg(this->prefixMontPoint), this));
			entry->setStatusTip(tr("Unmount media."));
			connect(entry.get(), SIGNAL(triggered()), this, SLOT(menuUmount_Click()));
			menuMount->addAction(entry.release());

	  }

	  if (isOnItemEvent){
			std::auto_ptr<QAction> entry (new QAction(tr("Run"), this));
			entry->setStatusTip(tr("Run current icon"));
			connect(entry.get(), SIGNAL(triggered()), this, SLOT(iconRun_Click()));
			menu->addAction(entry.release());

			menu->addSeparator();

			entry.reset(new QAction(tr("Options"), this));
			entry->setStatusTip(tr("Modify current icon options"));
			connect(entry.get(), SIGNAL(triggered()), this, SLOT(iconOption_Click()));
			menu->addAction(entry.release());

			menu->addSeparator();

			entry.reset(new QAction(tr("Cut"), this));
			entry->setStatusTip(tr("Cut selected icons to buffer"));
			connect(entry.get(), SIGNAL(triggered()), this, SLOT(iconCut_Click()));
			menu->addAction(entry.release());

			entry.reset(new QAction(tr("Copy"), this));
			entry->setStatusTip(tr("Copy selected icons to buffer"));
			connect(entry.get(), SIGNAL(triggered()), this, SLOT(iconCopy_Click()));
			menu->addAction(entry.release());

			entry.reset(new QAction(tr("Paste"), this));
			entry->setStatusTip(tr("Paste selected icons from buffer to selected folder"));
			if (iconBuffer.names.count()>0){
				  entry->setEnabled(true);
			} else {
				  entry->setEnabled(false);
			}
			connect(entry.get(), SIGNAL(triggered()), this, SLOT(iconPaste_Click()));
			menu->addAction(entry.release());

			entry.reset(new QAction(tr("Rename"), this));
			entry->setStatusTip(tr("Rename current icon"));
			connect(entry.get(), SIGNAL(triggered()), this, SLOT(iconRename_Click()));
			menu->addAction(entry.release());

			entry.reset(new QAction(tr("Delete"), this));
			entry->setStatusTip(tr("Delete current icon"));
			connect(entry.get(), SIGNAL(triggered()), this, SLOT(iconDelete_Click()));
			menu->addAction(entry.release());

			std::auto_ptr<QMenu> subMenu (new QMenu(tr("Browser"), this));

			entry.reset(new QAction(tr("Open application directory"), this));
			entry->setStatusTip(tr("Open application directory in system file browser"));
			connect(entry.get(), SIGNAL(triggered()), this, SLOT(xdgOpenIconDir_Click()));
			subMenu->addAction(entry.release());

			entry.reset(new QAction(tr("Open prefix directory"), this));
			entry->setStatusTip(tr("Open prefix directory in system file browser"));
			connect(entry.get(), SIGNAL(triggered()), this, SLOT(xdgOpenPrefixDir_Click()));
			subMenu->addAction(entry.release());

			entry.reset(new QAction(tr("Open mount point directory"), this));
			entry->setStatusTip(tr("Open mount point directory in system file browser"));
			if (this->prefixMontPoint.isEmpty())
				  entry->setEnabled(false);
			connect(entry.get(), SIGNAL(triggered()), this, SLOT(xdgOpenMountDir_Click()));
			subMenu->addAction(entry.release());

			menu->addSeparator();
			menu->addMenu(subMenu.release());

			subMenu.reset(new QMenu(tr("Wine Browser"), this));

			entry.reset(new QAction(tr("Open application directory"), this));
			entry->setStatusTip(tr("Open application directory in wine file browser"));
			connect(entry.get(), SIGNAL(triggered()), this, SLOT(winefileOpenIconDir_Click()));
			subMenu->addAction(entry.release());

			entry.reset(new QAction(tr("Open prefix directory"), this));
			entry->setStatusTip(tr("Open prefix directory in wine file browser"));
			connect(entry.get(), SIGNAL(triggered()), this, SLOT(winefileOpenPrefixDir_Click()));
			subMenu->addAction(entry.release());

			entry.reset(new QAction(tr("Open mount point directory"), this));
			entry->setStatusTip(tr("Open mount point directory in wine file browser"));
			if (this->prefixMontPoint.isEmpty())
				  entry->setEnabled(false);
			connect(entry.get(), SIGNAL(triggered()), this, SLOT(winefileOpenMountDir_Click()));
			subMenu->addAction(entry.release());

			menu->addMenu(subMenu.release());

			menu->addSeparator();
			entry.reset(new QAction(tr("Search in Wine AppDB"), this));
			entry->setStatusTip(tr("Search for application name in wine AppDB"));
#ifdef WITH_WINEAPPDB
			connect(entry.get(), SIGNAL(triggered()), this, SLOT(iconSearchAppDB_Click()));
#else
			entry->setEnabled(false);
#endif
			menu->addAction(entry.release());


	  } else {
			// Default menu
			std::auto_ptr<QMenu> menuRun (new QMenu(tr("Run..."), this));

			std::auto_ptr<QAction> entry (new QAction(CoreLib->loadIcon("data/folder.png"), tr("Browse ..."), this));
			entry->setStatusTip(tr("Browse for other image"));

			connect(menuRun.get(), SIGNAL(triggered(QAction*)), this, SLOT(menuRun_triggered(QAction*)));
			menuRun->addAction(entry.release());

			Last_Run_Icon db_last_run_icon;
			QStringList result = db_last_run_icon.getIcons();

			if (result.size()>0){
				  menuRun->addSeparator();
				  for (int i=0; i<result.size(); ++i){
						entry.reset(new QAction(result.at(i).split("/").last(), this));
						entry->setStatusTip(result.at(i));
						menuRun->addAction(entry.release());
				  }
			}

			menu->addMenu(menuRun.release());
			menu->addSeparator();

			entry.reset(new QAction(tr("New"), this));
			entry->setStatusTip(tr("Create new icon"));
			connect(entry.get(), SIGNAL(triggered()), this, SLOT(iconAdd_Click()));
			menu->addAction(entry.release());
			menu->addSeparator();

			entry.reset(new QAction(tr("Cut"), this));
			entry->setStatusTip(tr("Cut selected icons to buffer"));
			entry->setEnabled(isItemSelected);
			connect(entry.get(), SIGNAL(triggered()), this, SLOT(iconCut_Click()));
			menu->addAction(entry.release());

			entry.reset(new QAction(tr("Copy"), this));
			entry->setStatusTip(tr("Copy selected icons to buffer"));
			entry->setEnabled(isItemSelected);
			connect(entry.get(), SIGNAL(triggered()), this, SLOT(iconCopy_Click()));
			menu->addAction(entry.release());

			entry.reset(new QAction(tr("Paste"), this));
			entry->setStatusTip(tr("Paste selected icons from buffer to selected folder"));
			if (iconBuffer.names.count()>0){
				  entry->setEnabled(true);
			} else {
				  entry->setEnabled(false);
			}
			connect(entry.get(), SIGNAL(triggered()), this, SLOT(iconPaste_Click()));
			menu->addAction(entry.release());

			entry.reset(new QAction(tr("Delete"), this));
			entry->setStatusTip(tr("Delete current icon"));
			entry->setEnabled(isItemSelected);
			connect(entry.get(), SIGNAL(triggered()), this, SLOT(iconDelete_Click()));
			menu->addAction(entry.release());

			menu->addSeparator();
			menu->addMenu(menuMount.release());

			std::auto_ptr<QMenu> subMenu (new QMenu(tr("Browser"), this));
			entry.reset(new QAction(tr("Open prefix directory"), this));
			entry->setStatusTip(tr("Open prefix directory in system file browser"));
			connect(entry.get(), SIGNAL(triggered()), this, SLOT(xdgOpenPrefixDir_Click()));
			subMenu->addAction(entry.release());

			entry.reset(new QAction(tr("Open mount point directory"), this));
			entry->setStatusTip(tr("Open mount point directory in system file browser"));
			if (this->prefixMontPoint.isEmpty())
				  entry->setEnabled(false);
			connect(entry.get(), SIGNAL(triggered()), this, SLOT(xdgOpenMountDir_Click()));
			subMenu->addAction(entry.release());

			menu->addSeparator();
			menu->addMenu(subMenu.release());

			subMenu.reset(new QMenu(tr("Wine Browser"), this));
			entry.reset(new QAction(tr("Open prefix directory"), this));
			entry->setStatusTip(tr("Open prefix directory in wine file browser"));
			connect(entry.get(), SIGNAL(triggered()), this, SLOT(winefileOpenPrefixDir_Click()));
			subMenu->addAction(entry.release());

			entry.reset(new QAction(tr("Open mount point directory"), this));
			entry->setStatusTip(tr("Open mount point directory in wine file browser"));
			if (this->prefixMontPoint.isEmpty())
				  entry->setEnabled(false);
			connect(entry.get(), SIGNAL(triggered()), this, SLOT(winefileOpenMountDir_Click()));
			subMenu->addAction(entry.release());

			menu->addMenu(subMenu.release());

	  }
	  menu->exec(QCursor::pos());
	  return;
}

void IconListWidget::keyPressEvent (QKeyEvent * event){

	if (event->key()==Qt::Key_Tab){
		//FIXME: emmit change focus signal to mainwin
		//txtIconFilter->setFocus();
		//keyEvent.release();
		return;
	}

	if (event->key()==Qt::Key_Return){
		if (this->currentItem())
			this->itemDoubleClicked(this->currentItem());
		return;
	}

	if (event->key()==Qt::Key_Delete){
		if (this->currentItem())
			this->iconDelete_Click();
		return;
	}

	if (event->key()==Qt::Key_F2){
		if (this->currentItem())
			this->iconRename_Click();
		return;
	}

	return;
}

void IconListWidget::iconAdd_Click(void){
	  IconSettings iconAddWizard(this->prefixName, this->dirName);
	  if (iconAddWizard.exec() == QDialog::Accepted){
			this->showContents("");
	  }
	  return;
}

void IconListWidget::iconRename_Click(void){
	  bool ok=false;

	  std::auto_ptr<QListWidgetItem> iconItem (selectedItems().first());
	  if (!iconItem.get()){
			iconItem.release();
			return;
	  }

	  QString newName = QInputDialog::getText(this, tr("Enter new icon name"), tr("Icon name:"), QLineEdit::Normal, iconItem->text(), &ok);
	  if (ok && !newName.isEmpty()){
			while (db_icon.isExistsByName(this->prefixName, this->dirName, newName)){
				  newName = QInputDialog::getText(this, tr("Sorry. It seems icon already exists."), tr("Sorry. It seems icon already exists.<br>Please choose another name, or cancel operation."), QLineEdit::Normal, newName, &ok);
				  if ((!ok) || (newName.isEmpty())){
						iconItem.release();
						return;
				  }
			}
			db_icon.renameIcon(iconItem->text(), this->prefixName, this->dirName, newName);
	  }
	  iconItem.release();
	  this->showContents(this->filterString);
	  return;
}

void IconListWidget::iconDelete_Click(void){
	  /*
	  * This function delete add selected icons
	  */
	  QList<QListWidgetItem *> icoList = this->selectedItems();

	  if (icoList.count()<0)
			return;

	  if (QMessageBox::warning(this, tr("Delete Icon"), tr("Do you want to delete all selected icons?"),  QMessageBox::Yes, QMessageBox::No	)==QMessageBox::Yes){
			for (int i=0; i<icoList.count(); i++){
				  db_icon.delIcon(this->prefixName, this->dirName, icoList.at(i)->text());
			}
			this->showContents(this->filterString);
	  }
	  return;
}

void IconListWidget::iconRun_Click(void){
	  std::auto_ptr<QListWidgetItem> iconItem (this->selectedItems().first());
	  if (iconItem.get())
			this->itemDoubleClicked(iconItem.get());

	  iconItem.release();
	  return;
}

void IconListWidget::iconCut_Click(void){
	  /*
			This function fill iconBuffer with selected icons names
			and sets other informations required for copy\cut

			see struct iconCopyBuffer definition for details
	  */

	  QList<QListWidgetItem *> icoList = this->selectedItems();
	  if (icoList.count()<=0)
			return;

	  // If icon cutted -- set icon disabled style
	  // FIXME: It pice works fine, but we nead to use pixmaps for grayscale, not Qt::ItemIsEnabled flag....
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

	  iconBuffer.prefix_name = this->prefixName;
	  iconBuffer.dir_name = this->dirName;
	  return;
}

void IconListWidget::iconCopy_Click(void){
	  /*
	  * This function fill iconBuffer with selected icons names
	  * and sets other informations required for copy\cut
	  *
	  * see struct iconCopyBuffer definition for details
	  */

	  QList<QListWidgetItem *> icoList = this->selectedItems();
	  if (icoList.count()<=0)
			return;

	  // Clearing icon buffer
	  iconBuffer.names.clear();
	  iconBuffer.dir_name="";
	  iconBuffer.prefix_name="";
	  iconBuffer.move=false;

	  // Fiffing buffer with new items
	  for (int i=0; i<icoList.count(); i++){
			iconBuffer.names.append(icoList.at(i)->text());
	  }

	  iconBuffer.prefix_name = this->prefixName;
	  iconBuffer.dir_name = this->dirName;
	  return;
}

void IconListWidget::iconPaste_Click(void){
	  bool fexists=false, ok=false;
	  QString newName="";

	  if (iconBuffer.names.count()>0){
			for (int i=0; i<iconBuffer.names.count(); i++){
				  // Checking for not unic names
				  newName = iconBuffer.names.at(i);
				  fexists=false;
				  ok=false;

				  while (db_icon.isExistsByName(this->prefixName, this->dirName, newName)){
						newName = QInputDialog::getText(this, tr("Sorry. It seems icon already exists."), tr("Sorry. It seems icon already exists.<br>Please choose another name, or cancel operation."), QLineEdit::Normal, iconBuffer.names.at(i) , &ok);
						if (!ok){
							  return;
						}
				  }

				  switch (iconBuffer.move){
   case false:
						if (!db_icon.copyIcon(iconBuffer.names.at(i), iconBuffer.prefix_name, iconBuffer.dir_name, newName, this->prefixName, this->dirName))
							  return;
						break;
   case true:
						if (!db_icon.updateIcon(newName, db_prefix.getId(this->prefixName), db_dir.getId(this->dirName, this->prefixName), db_prefix.getId(iconBuffer.prefix_name), db_dir.getId(iconBuffer.dir_name, iconBuffer.prefix_name), iconBuffer.names.at(i)))
							  return;
						break;
				  }

			}
			iconBuffer.names.clear();
			iconBuffer.dir_name="";
			iconBuffer.prefix_name="";
			iconBuffer.move=false;
	  }

	  this->showContents("");
	  return;
}

void IconListWidget::iconOption_Click(void){
	  std::auto_ptr<QListWidgetItem> iconItem (this->selectedItems().first());
	  if (!iconItem.get())
			return;

	  IconSettings iconAddWizard(this->prefixName, this->dirName, iconItem->text());
	  if (iconAddWizard.exec() == QDialog::Accepted){
			this->showContents(this->filterString);
	  }
	  iconItem.release();
	  return;
}

void IconListWidget::iconSearchAppDB_Click(void){
	std::auto_ptr<QListWidgetItem> iconItem (this->selectedItems().first());
	if (!iconItem.get())
		  return;

	emit(searchRequest(iconItem->text()));
	iconItem.release();
	return;
}

void IconListWidget::menuRun_triggered(QAction* action){
	  if (action->text().isEmpty())
			return;

	  if (action->text()==tr("Browse ...")){
			Run run;
			run.prepare(this->prefixName);

			if (run.exec()==QDialog::Accepted)
				  CoreLib->runWineBinary(run.execObj);
			return;
	  }

	  Last_Run_Icon db_last_run_icon;
	  QStringList result = db_last_run_icon.getByExec(action->statusTip());

#ifdef DEBUG
	  qDebug()<<"Config key: advanced.openRunDialog="<<CoreLib->getSetting("advanced", "openRunDialog", false, 0).toString();
#endif

	  if (CoreLib->getSetting("advanced", "openRunDialog", false, 0).toInt()==0){
			ExecObject execObj;
			execObj.prefixid=db_prefix.getId(this->prefixName);
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
			Run run;
			run.prepare(this->prefixName, result.at(0), result.at(1), result.at(2), result.at(3), result.at(4), result.at(5), result.at(6), result.at(7).toInt(), action->statusTip());
			if (run.exec()==QDialog::Accepted)
				  CoreLib->runWineBinary(run.execObj);
	  }
	  return;
}

void IconListWidget::menuMount_triggered(QAction* action){
	  /*
	   * This slot process menuDirMountImages and menuIconMountImages triggered signal
	   */

	  if (action->text()==tr("[none]")){
			QMessageBox::warning(this, tr("Error"),  tr("No device drive specified in prefix settings."), QMessageBox::Ok);
			return;
	  } else if (action->text()==tr("Browse...")) {
			/*
			Request for unmounting cdrom drve described at wine prefix settings
			*/
#ifdef _OS_LINUX_
			QString fileName = QFileDialog::getOpenFileName(this, tr("Open CD Image files"), QDir::homePath(), tr("CD image files (*.iso *.nrg *.img *.bin *.mdf)"));
#endif

#ifdef _OS_FREEBSD_
			QString fileName = QFileDialog::getOpenFileName(this, tr("Open ISO Image file"), QDir::homePath(), tr("iso files (*.iso)"));
#endif

			if(fileName.isEmpty()){
				  return;
			}

			if (CoreLib->mountImage(fileName, this->prefixName)){
				  emit(changeStatusText(tr("%1 successfully mounted.").arg(fileName)));
			} else {
				  emit(changeStatusText(tr("Fail to mount %1.").arg(fileName)));
			}

			CoreLib->updateRecentImagesList(fileName);
			return;
	  }

	  bool ret=false;
	  if (action->statusTip().contains("/")){
			ret=CoreLib->mountImage(action->statusTip(), this->prefixName);
	  } else {
			ret=CoreLib->mountImage(action->text(), this->prefixName);
	  }

	  if (ret){
			emit(changeStatusText(tr("%1 successfully mounted.").arg(action->text())));
	  } else {
			emit(changeStatusText(tr("Fail to mount %1.").arg(action->text())));
	  }
	  return;
}

void IconListWidget::menuUmount_Click(void){
	  if (CoreLib->umountImage(this->prefixName)){
			emit(changeStatusText(tr("Drive successfully umounted.")));
	  } else {
			emit(changeStatusText(tr("Fail to umount drive.")));
	  }
	  return;
}

void IconListWidget::xdgOpenPrefixDir_Click(void){
     QDesktopServices::openUrl(QUrl(QString("file://%1").arg(db_prefix.getPath(this->prefixName)), QUrl::TolerantMode));
	  return;
}

void IconListWidget::xdgOpenMountDir_Click(void){
    QDesktopServices::openUrl(QUrl(QString("file://%1").arg(this->prefixMontPoint), QUrl::TolerantMode));
    return;
}

void IconListWidget::xdgOpenIconDir_Click(void){
	  QCursor cur;

	  std::auto_ptr<QListWidgetItem> item (this->currentItem());
	  if (!item.get()){
			item.release();
			return;
	  }

	  QString result = db_icon.getByName(this->prefixName, this->dirName, item->text()).at(4);

	  if (result.isEmpty()){
          emit(changeStatusText(tr("Error: \"%1\" is an embedded wine binary.").arg(item->text())));
	  } else {
          QDesktopServices::openUrl(QUrl(QString("file://%1").arg(result), QUrl::TolerantMode));
	  }

	  item.release();
	  return;
}

void IconListWidget::winefileOpenPrefixDir_Click(void){
	  QString result = db_prefix.getPath(this->prefixName);
	  CoreLib->runWineBinary("winefile", result + "/", this->prefixName);
	  return;
}

void IconListWidget::winefileOpenMountDir_Click(void){
	  CoreLib->runWineBinary("winefile", this->prefixMontPoint + "/", this->prefixName);
	  return;
}

void IconListWidget::winefileOpenIconDir_Click(void){
	  QCursor cur;

	  std::auto_ptr<QListWidgetItem> item (this->currentItem());
	  if (!item.get()){
			item.release();
			return;
	  }

	  QString result = db_icon.getByName(this->prefixName, this->dirName, item->text()).at(4);

	  if (result.isEmpty()){
			emit(changeStatusText(tr("Error: \"%1\" is an embedded wine binary.").arg(item->text())));
	  } else {
			CoreLib->runWineBinary("winefile", result + "/", this->prefixName);
	  }

	  item.release();
	  return;
}
