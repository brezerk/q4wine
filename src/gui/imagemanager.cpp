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
	setupUi(this);

	QSettings settings(APP_SHORT_NAME, "default");
	settings.beginGroup("app");
	loadThemeIcons(settings.value("theme").toString());
	settings.endGroup();

	connect(cmdOk, SIGNAL(clicked()), this, SLOT(cmdOk_Click()));
	connect(tableImage, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(tableImage_showContextMenu(const QPoint &)));
	connect(tableImage, SIGNAL(clicked(const QModelIndex &)), this, SLOT(update_lblPathInfo(const QModelIndex &)));

	getCDImages();

	return;
}

void ImageManager::update_lblPathInfo(const QModelIndex){

	if (tableImage->currentRow()<0)
		return;

	QSqlQuery query;

	query.prepare("SELECT path FROM images WHERE name=:name");
	query.bindValue(":name", tableImage->item(tableImage->currentRow(), 0)->text());

	query.exec();
	query.first();

	lblPathInfo->setText(tr("%1").arg(query.value(0).toString()));

	return;
}

void ImageManager::tableImage_showContextMenu(const QPoint){
	QMenu menu(this);
		menu.addAction(actionAdd);
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
	
	QSqlQuery query;
	
	qint64 size;

	query.exec("SELECT name, path FROM images ORDER BY name");

	int numRows = tableImage->rowCount();
	int curRows = 0;
	
	while (query.next()) {
			
						curRows++;
	
						if (curRows>numRows){
							tableImage->insertRow (numRows);
							numRows = tableImage->rowCount();
						}

						QFile file (query.value(1).toString());
						if (file.exists ()){
							size = file.size() / 1024 / 1024;
						} else {
							size = 0;
						}
	
						if (tableImage->item(curRows - 1, 0)){
							tableImage->item(curRows - 1, 0)->setText(query.value(0).toString());

							tableImage->item(curRows - 1, 1)->setText(tr("%1 Mb").arg(size));
							if (!file.exists ()){
								tableImage->item(curRows - 1, 0)->setIcon (QIcon(":/data/important.png"));
							} else {
								tableImage->item(curRows - 1, 0)->setIcon (QIcon());
							}
						} else {
							QTableWidgetItem *newItem = new QTableWidgetItem(query.value(0).toString());

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
	
	bool fexists=FALSE, ok;
	QString fileName, newName;
	
	#ifdef _OS_LINUX_
		fileName = QFileDialog::getOpenFileName(this, tr("Open ISO or NRG Image file"), QDir::homePath(), tr("iso and nrg files (*.iso *.nrg)"));
	#endif

	#ifdef _OS_FREEBSD_
		fileName = QFileDialog::getOpenFileName(this, tr("Open ISO Image file"), QDir::homePath(), tr("iso files (*.iso)"));
	#endif

	if(!fileName.isEmpty()){
		QSqlQuery query;

		query.prepare("SELECT name FROM images WHERE name=:name");
		query.bindValue(":name", fileName.split("/").last());
		query.exec();
		query.first();

		fexists=query.isValid();
		query.clear();

		newName = fileName.split("/").last();

		while (fexists){
			newName = QInputDialog::getText(this, tr("Sorry. It seems CD iamge already exists."),
								tr("Sorry. It seems CD image file already exists.<br>Please rename it, or cancel add image operation."), QLineEdit::Normal, fileName.split("/").last() , &ok);
						
			if (!ok){
				return;
			}

			query.prepare("SELECT id FROM images WHERE name=:name");
			query.bindValue(":name", newName);
			query.exec();
			query.first();
			fexists=query.isValid();

			query.clear();
		}

		query.prepare("INSERT INTO images(name, path) VALUES(:name, :path);");
		
		QStringList list1 = fileName.split("/");
			
		;

		query.bindValue(":name", newName);
		query.bindValue(":path", fileName);

		if (!query.exec()){
			QMessageBox::warning(this, tr("Error"), tr("debug: %1").arg(query.lastError().text()));
		}

		query.clear();
		actionRefreshImageList();
	}

	return;
}

void ImageManager::actionRemoveImage(){
	if (tableImage->currentRow()==-1)
		return;

	QSqlQuery query;
		query.prepare("DELETE FROM images WHERE name=:name and path=:path");
		query.bindValue(":name", tableImage->item(tableImage->currentRow(), 0)->text());
		query.bindValue(":path", lblPathInfo->text());
		if (!query.exec()){
			QMessageBox::warning(this, tr("Error"), tr("debug: %1").arg(query.lastError().text()));
		} else {
			qDebug()<<"Woooot!";
		}

	
		query.clear();

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
