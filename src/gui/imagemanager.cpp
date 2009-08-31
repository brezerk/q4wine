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
#include "imagemanager.h"

ImageManager::ImageManager(QWidget * parent, Qt::WFlags f) : QDialog(parent, f)
{
	//setModal(true);
	setupUi(this);

	// Loading libq4wine-core.so
	libq4wine.setFileName("libq4wine-core");

	if (!libq4wine.load()){
		libq4wine.load();
	}

	// Getting corelib calss pointer
	CoreLibClassPointer = (CoreLibPrototype *) libq4wine.resolve("createCoreLib");
	CoreLib = (corelib *)CoreLibClassPointer(true);

	// Creating database classes
	db_image = new Image();

	loadThemeIcons(CoreLib->getSetting("app", "theme", false).toString());

	connect(cmdOk, SIGNAL(clicked()), this, SLOT(cmdOk_Click()));
	connect(cmdHelp, SIGNAL(clicked()), this, SLOT(cmdHelp_Click()));
	connect(tableImage, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(tableImage_showContextMenu(const QPoint &)));
	connect(tableImage, SIGNAL(clicked(const QModelIndex &)), this, SLOT(update_lblPathInfo(const QModelIndex &)));

	getCDImages();
	return;
}

void ImageManager::update_lblPathInfo(const QModelIndex){

	if (tableImage->currentRow()<0)
		return;
	lblPathInfo->setText(tr("File path: %1").arg(db_image->getPath(tableImage->item(tableImage->currentRow(), 0)->text())));

	return;
}

void ImageManager::tableImage_showContextMenu(const QPoint){
	QMenu menu(this);
	menu.addAction(actionAdd);
	menu.addAction(actionRename);
	menu.addSeparator();
	menu.addAction(actionRefresh);
	menu.addSeparator();
	if (tableImage->currentRow()>=0){
		actionRemove->setEnabled(TRUE);
	} else {
		actionRemove->setEnabled(FALSE);
	}
	menu.addAction(actionRemove);
	menu.exec(QCursor::pos());
	return;
}


void ImageManager::getCDImages(void){
	qint64 size = 0;
	QList<QStringList> result = db_image->getFields();

	int numRows = tableImage->rowCount();
	int curRows = 0;

	for (int i = 0; i < result.size(); ++i) {
		curRows++;

		if (curRows>numRows){
			tableImage->insertRow (numRows);
			numRows = tableImage->rowCount();
		}

		QFile file (result.at(i).at(1));
		if (file.exists ()){
			size = file.size() / 1024 / 1024;
		} else {
			size = 0;
		}

		if (tableImage->item(curRows - 1, 0)){
			tableImage->item(curRows - 1, 0)->setText(result.at(i).at(0));
			tableImage->item(curRows - 1, 1)->setText(tr("%1 Mb").arg(size));
				if (!file.exists ()){
					tableImage->item(curRows - 1, 0)->setIcon (QIcon(":/data/important.png"));
				} else {
					tableImage->item(curRows - 1, 0)->setIcon (QIcon());
				}
		} else {
			QTableWidgetItem *newItem = new QTableWidgetItem(result.at(i).at(0));
			tableImage->setItem(curRows - 1, 0, newItem);
			if (!file.exists ()){
				newItem->setIcon ( QIcon(":/data/important.png") );
			} else {
				newItem->setIcon ( QIcon() );
			}
			newItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
			newItem = new QTableWidgetItem(tr("%1 Mb").arg(size));
			tableImage->setItem(curRows - 1, 1, newItem);
			newItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
		}
	}

	numRows = tableImage->rowCount();

	if (numRows > curRows)
		for (int i=curRows; i <= numRows; i++)
			tableImage->removeRow(curRows);

	tableImage->resizeRowsToContents();
	tableImage->resizeColumnsToContents();
	tableImage->horizontalHeader()->setStretchLastSection(TRUE);

	return;
}

void ImageManager::loadThemeIcons(QString themePath){
	QPixmap pixmap;

	if (!pixmap.load(tr("%1/data/iso_manager.png").arg(themePath))){
		pixmap.load(":data/iso_manager.png");
	}

	lblLogo->setPixmap(pixmap);

	managerToolBar = new QToolBar(tlbManager);

	actionAdd = managerToolBar->addAction (loadIcon("data/add.png", themePath), tr("Add image"));
	connect(actionAdd, SIGNAL(triggered()), this, SLOT(actionAddImage()));

	actionRename = managerToolBar->addAction (loadIcon("data/configure.png", themePath), tr("Rename image"));
	connect(actionRename, SIGNAL(triggered()), this, SLOT(actionRenameImage()));

	managerToolBar->addSeparator ();

	actionRemove = managerToolBar->addAction (loadIcon("data/remove.png", themePath), tr("Remove image"));
	connect(actionRemove, SIGNAL(triggered()), this, SLOT(actionRemoveImage()));

	managerToolBar->addSeparator ();

	actionRefresh = managerToolBar->addAction (loadIcon("data/reload.png", themePath), tr("Refresh image list"));
	connect(actionRefresh, SIGNAL(triggered()), this, SLOT(actionRefreshImageList()));

	return;
}

void ImageManager::cmdOk_Click(){
	accept();
	return;
}


void ImageManager::actionAddImage(){
	bool ok;
	QString fileName, newName;
	#ifdef _OS_LINUX_
		fileName = QFileDialog::getOpenFileName(this, tr("Open ISO or NRG Image file"), QDir::homePath(), tr("iso and nrg files (*.iso *.nrg)"));
	#endif

	#ifdef _OS_FREEBSD_
		fileName = QFileDialog::getOpenFileName(this, tr("Open ISO Image file"), QDir::homePath(), tr("iso files (*.iso)"));
	#endif

	if(!fileName.isEmpty()){
		newName = fileName.split("/").last();
		while (db_image->isExistsByName(newName)){
			newName = QInputDialog::getText(this, tr("Sorry. It seems CD iamge already exists."), tr("Sorry. It seems CD image file already exists.<br>Please rename it, or cancel add image operation."), QLineEdit::Normal, fileName.split("/").last() , &ok);
			if (!ok){
				return;
			}
		}
		if (!db_image->addImage(newName, fileName))
			return;
		actionRefreshImageList();
	}
	return;
}

void ImageManager::actionRenameImage(){
	if (tableImage->currentRow()==-1)
		return;

	bool ok;
	QString newName = QInputDialog::getText(this, tr("Enter new name"), tr("Enter new name:"), QLineEdit::Normal, tableImage->item(tableImage->currentRow(), 0)->text(), &ok);
	while (db_image->isExistsByName(newName) or (newName.isEmpty())){
		if (!ok){
			return;
		}
		if (newName.isEmpty()){
			newName = QInputDialog::getText(this, tr("Sorry. CD iamge name can not be empty."), tr("Sorry. CD iamge name can not be empty.<br>Please enter new name, or cancel rename image operation."), QLineEdit::Normal, newName, &ok);
		} else {
			newName = QInputDialog::getText(this, tr("Sorry. It seems CD iamge already exists."), tr("Sorry. It seems CD image file already exists.<br>Please rename it, or cancel rename image operation."), QLineEdit::Normal, newName, &ok);
		}
	}
	if (!db_image->renameImage(newName, tableImage->item(tableImage->currentRow(), 0)->text()))
		return;
	actionRefreshImageList();
	return;
}

void ImageManager::actionRemoveImage(){
	if (tableImage->currentRow()==-1)
		return;
	if (!db_image->delImage(tableImage->item(tableImage->currentRow(), 0)->text()))
		return;
	actionRefreshImageList();
	return;
}

void ImageManager::actionRefreshImageList(){
	getCDImages();
	return;
}

QIcon ImageManager::loadIcon(QString iconName, QString themePath){
	// Function tryes to load icon image from theme dir
	// If it fails -> load default from rsource file
	QIcon icon;
	if ((!themePath.isEmpty()) and (themePath!="Default")){
		icon.addFile(tr("%1/%2").arg(themePath).arg(iconName));
		if (icon.isNull()){
			icon.addFile(tr(":/%1").arg(iconName));
		}
	} else {
		icon.addFile(tr(":/%1").arg(iconName));
	}
	return icon;
}

void ImageManager::cmdHelp_Click(){
	CoreLib->openHelpUrl("09-image-manager.html");
}

