/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010 by Malakhov Alexey                                 *
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

#include "draglistwidget.h"

DragListWidget::DragListWidget(QString themeName, QWidget *parent) : QListWidget (parent)
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

	this->themeName=themeName;

	connect(this, SIGNAL(itemClicked (QListWidgetItem *)), this, SLOT(itemClicked (QListWidgetItem *)));
	connect(this, SIGNAL(itemDoubleClicked (QListWidgetItem *)), this, SLOT(itemDoubleClicked (QListWidgetItem *)));
}

void DragListWidget::showFolderContents(QString prefixName, QString dirName, QString filter){
	this->clear();
	emit(iconItemClick("","","","",""));

	this->prefixName=prefixName;
	this->dirName=dirName;

	QList<QStringList> iconsList=db_icon.getByPrefixAndDirName(prefixName, dirName, filter);

	for (int i = 0; i < iconsList.size(); ++i) {
		std::auto_ptr<QListWidgetItem> iconItem (new QListWidgetItem(this, 0));
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
		iconItem.release();
	}
	return;
}

void DragListWidget::itemClicked (QListWidgetItem *item){
	if (!item)
		return;

	QStringList result=db_icon.getByName(this->prefixName, this->dirName, item->text());
	emit(iconItemClick(result.at(10).split('/').last().split('\\').last(), result.at(9), result.at(2), result.at(7), result.at(11)));
}

void DragListWidget::itemDoubleClicked (QListWidgetItem *item){
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

void DragListWidget::mousePressEvent(QMouseEvent *event){
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

void DragListWidget::mouseMoveEvent(QMouseEvent *event){
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

void DragListWidget::dragEnterEvent(QDragEnterEvent *event){
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

void DragListWidget::dragMoveEvent(QDragMoveEvent *event){
	std::auto_ptr<DragListWidget> source (qobject_cast<DragListWidget *>(event->source()));
	if (source.get() && source.get() != this){
		event->setDropAction(Qt::MoveAction);
		event->accept();
	}
	source.release();
}

void DragListWidget::dropEvent(QDropEvent *event){
	std::auto_ptr<DragListWidget> source (qobject_cast<DragListWidget *>(event->source()));
	if (source.get() && source.get() != this){
		event->setDropAction(Qt::MoveAction);
		event->accept();
	}
	source.release();
	emit(startDrop(event->mimeData()->urls()));
}

void DragListWidget::contextMenuEvent (QContextMenuEvent * event){
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
	std::auto_ptr<QMenu> submenuMount (new QMenu(tr("Mount [none]"), this));

	std::auto_ptr<QAction> entry (new QAction(tr("Run"), this));
	entry->setStatusTip(tr("umount [drive]"));
	connect(entry.get(), SIGNAL(triggered()), this, SLOT(iconUmount_Click()));
	submenuMount->addAction(entry.release());

	menuMount->addMenu(submenuMount.release());

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
	} else {
		// Default menu
		std::auto_ptr<QMenu> menuRun (new QMenu(tr("Run..."), this));

		std::auto_ptr<QAction> entry (new QAction(loadIcon("data/folder.png"), tr("Browse ..."), this));
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

	}
	menu->exec(QCursor::pos());
	return;
}

QIcon DragListWidget::loadIcon(QString iconName){
	// Function tryes to load icon image from theme dir
	// If it fails -> load default from rsource file
	QIcon icon;
	if ((!this->themeName.isEmpty()) and (this->themeName!="Default")){
		icon.addFile(QString("%1/%2").arg(this->themeName).arg(iconName));
		if (icon.isNull()){
			icon.addFile(QString(":/%1").arg(iconName));
		}
	} else {
		icon.addFile(QString(":/%1").arg(iconName));
	}
	return icon;
}

void DragListWidget::iconAdd_Click(void){
	IconSettings iconAddWizard(this->prefixName, this->dirName);
	if (iconAddWizard.exec() == QDialog::Accepted){
		this->showFolderContents(this->prefixName, this->dirName, "");
	}
	return;
}

void DragListWidget::iconRename_Click(void){
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
	this->showFolderContents(this->prefixName, this->dirName, "");
	return;
}

void DragListWidget::iconDelete_Click(void){
	/*
		This function delete add selected icons
	*/
	QList<QListWidgetItem *> icoList = this->selectedItems();

	if (icoList.count()<0)
		return;

	if (QMessageBox::warning(this, tr("Delete Icon"), tr("Do you want to delete all selected icons?"),  QMessageBox::Yes, QMessageBox::No	)==QMessageBox::Yes){
		for (int i=0; i<icoList.count(); i++){
			db_icon.delIcon(this->prefixName, this->dirName, icoList.at(i)->text());
		}
		this->showFolderContents(this->prefixName, this->dirName, "");
	}
	return;
}

void DragListWidget::iconRun_Click(void){
	std::auto_ptr<QListWidgetItem> iconItem (this->selectedItems().first());
	if (iconItem.get())
		this->itemDoubleClicked(iconItem.get());

	iconItem.release();
	return;
}

void DragListWidget::iconCut_Click(void){
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

void DragListWidget::iconCopy_Click(void){
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

void DragListWidget::iconPaste_Click(void){
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

	this->showFolderContents(this->prefixName, this->dirName, "");
	return;
}

void DragListWidget::iconOption_Click(void){
	std::auto_ptr<QListWidgetItem> iconItem (this->selectedItems().first());
	if (!iconItem.get())
		return;

	IconSettings iconAddWizard(this->prefixName, this->dirName, iconItem->text());
	if (iconAddWizard.exec() == QDialog::Accepted){
		this->showFolderContents(this->prefixName, this->dirName, "");
	}
	iconItem.release();
	return;
}

void DragListWidget::menuRun_triggered(QAction* action){
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

//QMenu DragListWidget::createCustomMenu(){
	/*
	// OpenDir
	std::auto_ptr<QAction> xdgOpenIconDir;
	std::auto_ptr<QAction> xdgOpenPrefixDir;
	std::auto_ptr<QAction> xdgOpenMountDir;
	std::auto_ptr<QAction> winefileOpenIconDir;
	std::auto_ptr<QAction> winefileOpenPrefixDir;
	std::auto_ptr<QAction> winefileOpenMountDir;

	// Icons control for context menu
	std::auto_ptr<QAction> iconRun;
	std::auto_ptr<QAction> iconAdd;
	std::auto_ptr<QAction> iconRename;
	std::auto_ptr<QAction> iconDelete;
	std::auto_ptr<QAction> iconOptions;
	std::auto_ptr<QAction> iconCopy;
	std::auto_ptr<QAction> iconCut;
	std::auto_ptr<QAction> iconPaste;
	std::auto_ptr<QAction> iconMount;
	std::auto_ptr<QAction> iconUnmount;
	std::auto_ptr<QAction> iconMountOther;
   */

	/*
	iconRun.reset(new QAction(tr("Run"), lstIcons.get()));
	iconRun->setStatusTip(tr("Create new icon"));
	connect(iconRun.get(), SIGNAL(triggered()), this, SLOT(iconRun_Click()));

	iconAdd.reset(new QAction(tr("New"), lstIcons.get()));
	iconAdd->setStatusTip(tr("Create new icon"));
	connect(iconAdd.get(), SIGNAL(triggered()), this, SLOT(iconAdd_Click()));

	iconCut.reset(new QAction(tr("Cut"), lstIcons.get()));
	iconCut->setStatusTip(tr("Cut selected icons to buffer"));
	connect(iconCut.get(), SIGNAL(triggered()), this, SLOT(iconCut_Click()));

	iconCopy.reset(new QAction(tr("Copy"), lstIcons.get()));
	iconCopy->setStatusTip(tr("Copy selected icons to buffer"));
	connect(iconCopy.get(), SIGNAL(triggered()), this, SLOT(iconCopy_Click()));

	iconPaste.reset(new QAction(tr("Paste"), lstIcons.get()));
	iconPaste->setStatusTip(tr("Paste selected icons from buffer to selected folder"));
	connect(iconPaste.get(), SIGNAL(triggered()), this, SLOT(iconPaste_Click()));

	iconRename.reset(new QAction(tr("Rename"), lstIcons.get()));
	iconRename->setStatusTip(tr("Rename current icon"));
	connect(iconRename.get(), SIGNAL(triggered()), this, SLOT(iconRename_Click()));

	iconDelete.reset(new QAction(tr("Delete"), lstIcons.get()));
	iconDelete->setStatusTip(tr("Delete current icon"));
	connect(iconDelete.get(), SIGNAL(triggered()), this, SLOT(iconDelete_Click()));

	iconOptions.reset(new QAction(tr("Options"), lstIcons.get()));
	iconOptions->setStatusTip(tr("Modify current icon options"));
	connect(iconOptions.get(), SIGNAL(triggered()), this, SLOT(iconOption_Click()));

	iconMount.reset(new QAction(tr("mount"), lstIcons.get()));
	iconMount->setStatusTip(tr("Mount image from icon options"));

	iconUnmount.reset(new QAction(tr("umount"), lstIcons.get()));
	iconUnmount->setStatusTip(tr("Unmount image"));
	connect(iconUnmount.get(), SIGNAL(triggered()), this, SLOT(iconUnmount_Click()));

	iconMountOther.reset(new QAction(QIcon(":/data/folder.png"), tr("Browse ..."), lstIcons.get()));
	iconMountOther->setStatusTip(tr("Browse for other image"));
	connect(iconMountOther.get(), SIGNAL(triggered()), this, SLOT(iconMountOther_Click()));
	*/
//}
