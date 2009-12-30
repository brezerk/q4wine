/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010 by Malakhov Alexey                            *
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

#include "prefixtreewidget.h"

PrefixTreeWidget::PrefixTreeWidget(QString themeName, QWidget *parent) :
	QTreeWidget(parent)
{
	// Loading libq4wine-core.so
	libq4wine.setFileName("libq4wine-core");

	if (!libq4wine.load()){
		libq4wine.load();
	}

	// Getting corelib calss pointer
	CoreLibClassPointer = (CoreLibPrototype *) libq4wine.resolve("createCoreLib");
	CoreLib.reset((corelib *)CoreLibClassPointer(true));

	this->setHeaderLabel("Current prefixes list");

	connect(this, SIGNAL(itemClicked (QTreeWidgetItem *, int)), this, SLOT(itemClicked (QTreeWidgetItem *, int)));

	this->themeName=themeName;
	this->dirName="";
	this->prefixName="";
	this->prefixMontPoint="";
	this->prefixMediaDrive="";
}

void PrefixTreeWidget::itemClicked (QTreeWidgetItem *item, int){
	if (!item)
		return;

	if (item->parent()){
		this->prefixName=item->parent()->text(0);
		this->dirName=item->text(0);
	} else {
		this->prefixName=item->text(0);
		this->dirName="";
	}

	QStringList result = db_prefix.getFieldsByPrefixName(this->prefixName);
	this->prefixMediaDrive = result.at(7);
	this->prefixMontPoint = result.at(6);

	emit(showFolderContents(this->prefixName, this->dirName, ""));
	return;
}

void PrefixTreeWidget::dirAdd_Click(void){
	bool ok;
	QString dirname = QInputDialog::getText(this, tr("Enter new directory name"), tr("Directory name:"), QLineEdit::Normal, "" , &ok);
	std::auto_ptr<QTreeWidgetItem> treeItem (this->currentItem());

	if (!treeItem.get()){
		treeItem.release();
		return;
	}

	if (ok && !dirname.isEmpty()){
		if (db_dir.isExistsByName(this->prefixName, this->dirName)){
			QMessageBox::warning(this, tr("Error"), tr("Sorry, but directory named %1 already exists.").arg(dirname));
			treeItem.release();
			return;
		}

		if (!db_dir.addDir(this->prefixName, this->dirName)){
			treeItem.release();
			return;
		}
		std::auto_ptr<QTreeWidgetItem> prefixItem (new QTreeWidgetItem(treeItem->parent()));
		prefixItem->setText(0, dirname);
		prefixItem->setIcon(0, loadIcon("data/folder.png"));
		prefixItem.release();
	}
	return;
	treeItem.release();
}



void PrefixTreeWidget::dirRename_Click(void){/*
	QTreeWidgetItem *treeItem = twPrograms->currentItem();

	if (!treeItem)
		return;

	if (treeItem->parent()){
		bool ok = FALSE;
		QString newName = QInputDialog::getText(this, tr("Enter new name for directory"), tr("Directory name:"), QLineEdit::Normal, treeItem->text(0) , &ok);

		if (ok && !newName.isEmpty()){
			while (db_dir.isExistsByName(treeItem->parent()->text(0), newName)){
				newName = QInputDialog::getText(this, tr("Sorry. It seems directory already exists."), tr("Sorry. It seems directory already exists.<br>Please choose another name, or cancel operation."), QLineEdit::Normal, newName, &ok);
				if ((!ok) || (newName.isEmpty())){
					return;
				}
			}
			db_dir.renameDir(treeItem->text(0), treeItem->parent()->text(0), newName);
			treeItem->setText(0, newName);
		}

	}*/
	return;
}

void PrefixTreeWidget::dirDelete_Click(void){
	/*
	QTreeWidgetItem *treeItem;
	treeItem = twPrograms->currentItem();

	if (!treeItem)
		return;

	if (treeItem->parent()){

		if (QMessageBox::warning(this, tr("Q4Wine"), tr("Do you really wish delete folder named \"%1\" and all associated icons?\n").arg(twPrograms->currentItem()->text(0)),
								 QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes){

			if (db_icon.delIcon(treeItem->parent()->text(0), treeItem->text(0), ""))
				if(db_dir.delDir(treeItem->parent()->text(0), treeItem->text(0)))
					treeItem->parent()->removeChild (treeItem);
			twPrograms_ItemClick(twPrograms->currentItem(), 0);
		}
	}
*/
	return;
}

void PrefixTreeWidget::getPrefixes(){
	this->clear();

	QList<QStringList> result = db_prefix.getFields();
	for (int i = 0; i < result.size(); ++i) {
		// Inserting root items into programs tree view
		std::auto_ptr<QTreeWidgetItem> prefixItem (new QTreeWidgetItem(this));
		prefixItem->setText(0, QString("%1").arg(result.at(i).at(1)));
		prefixItem->setIcon(0, loadIcon("data/wine.png"));
		prefixItem->setExpanded (TRUE);
		this->addTopLevelItem(prefixItem.get());

		// Inserting subfolders items into programs tree view
		QList<QStringList> subresult = db_dir.getFieldsByPrefixId(result.at(i).at(0));
		for (int j = 0; j < subresult.size(); ++j) {
			std::auto_ptr<QTreeWidgetItem> subPrefixItem (new QTreeWidgetItem(prefixItem.get(), 0));
			subPrefixItem->setText(0, QString("%1").arg(subresult.at(j).at(1)));
			subPrefixItem->setIcon(0, loadIcon("data/folder.png"));
			subPrefixItem.release();
		}

		prefixItem.release();
	}
	return;
}

void PrefixTreeWidget::contextMenuEvent (QContextMenuEvent * event){
	if (this->prefixName.isEmpty())
		return;

	this->itemClicked (this->currentItem(), 0);

	std::auto_ptr<QMenu> menu (new QMenu(this));
	std::auto_ptr<QMenu> menuMount (new QMenu(tr("Mount iso..."), this));

	if (this->prefixMontPoint.isEmpty()){
		menuMount->setEnabled(false);
	} else {
		std::auto_ptr<QMenu> submenuMount (new QMenu(tr("Mount [%1]").arg(CoreLib->getMountedImages(this->prefixMontPoint)), this));
		std::auto_ptr<QAction> entry;

		if (this->prefixMediaDrive.isEmpty()){
			entry.reset (new QAction(loadIcon("/data/drive_menu.png"), tr("[none]"), this));
			entry->setStatusTip(tr("No media was set in prefix settings."));
			submenuMount->addAction(entry.release());
		} else {
			entry.reset (new QAction(loadIcon("/data/drive_menu.png"), this->prefixMediaDrive, this));
			entry->setStatusTip(tr("Mount media drive."));
			submenuMount->addAction(entry.release());
		}

		QList<QStringList> images = db_image.getFields();

		if (images.size()>0)
			submenuMount->addSeparator();

		for (int i = 0; i < images.size(); ++i) {
			entry.reset (new QAction(loadIcon("/data/cdrom_menu.png"), images.at(i).at(0), this));
			entry->setStatusTip(tr("Mount media image."));
			submenuMount->addAction(entry.release());
		}

		connect(submenuMount.get(), SIGNAL(triggered(QAction*)), this, SLOT(menuMount_triggered(QAction*)));
		menuMount->addMenu(submenuMount.release());
		menuMount->addSeparator();

		submenuMount.reset (new QMenu(tr("Mount ..."), this));

		entry.reset (new QAction(loadIcon("/data/folder.png"), tr("Browse..."), this));
		entry->setStatusTip(tr("Browse for media image."));
		submenuMount->addAction(entry.release());

		QStringList files = CoreLib->getSetting("", "recent_images", false).toStringList();

		if (files.size()>0)
			submenuMount->addSeparator();

		for (int i = 0; i < files.size(); ++i){
			entry.reset (new QAction(loadIcon("/data/cdrom_menu.png"),files.at(i).split("/").last(), this));
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
		entry->setStatusTip(tr("Create new directory"));
		connect(entry.get(), SIGNAL(triggered()), this, SLOT(dirAdd_Click()));
		menu->addAction(entry.release());
		menu->addSeparator();

		entry.reset(new QAction(tr("Delete"), this));
		entry->setStatusTip(tr("Delete current icon"));
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

	menu->exec(QCursor::pos());
	return;
}


QIcon PrefixTreeWidget::loadIcon(QString iconName){
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


void PrefixTreeWidget::menuRun_triggered(QAction* action){
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

void PrefixTreeWidget::menuMount_triggered(QAction* action){
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

void PrefixTreeWidget::menuUmount_Click(void){
	if (CoreLib->umountImage(this->prefixName)){
		emit(changeStatusText(tr("Drive successfully umounted.")));
	} else {
		emit(changeStatusText(tr("Fail to umount drive.")));
	}
	return;
}

void PrefixTreeWidget::xdgOpenPrefixDir_Click(void){
	CoreLib->openPrefixDirectry(this->prefixName);
	return;
}

void PrefixTreeWidget::xdgOpenMountDir_Click(void){
	CoreLib->openUrl(this->prefixMontPoint);
	return;
}

void PrefixTreeWidget::winefileOpenPrefixDir_Click(void){
	QString result = db_prefix.getPath(this->prefixName);
	CoreLib->runWineBinary("winefile", result + "/", this->prefixName);
	return;
}

void PrefixTreeWidget::winefileOpenMountDir_Click(void){
	CoreLib->runWineBinary("winefile", this->prefixMontPoint + "/", this->prefixName);
	return;
}
