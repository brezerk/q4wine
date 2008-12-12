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

#include "config.h"

#include "iconsview.h"
 
IconsView::IconsView(QString tmpDir, QWidget * parent, Qt::WFlags f) : QDialog(parent, f)
{
	setupUi(this);

	
	tempDirectory=tmpDir;
	
	QDir tmp(tmpDir);
	tmp.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	QFileInfoList list = tmp.entryInfoList();
		
	QListWidgetItem *iconItem;
	lstIcons->clear();
	lstIcons->setSelectionMode(QAbstractItemView::SingleSelection);
	
	// Getting converted icons from temp directory
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		if (fileInfo.fileName().right(3)=="png"){
			iconItem = new QListWidgetItem(lstIcons, 0);
			iconItem->setText(fileInfo.fileName());
			iconItem->setIcon(QIcon(fileInfo.filePath()));
		}
	}
	
	connect(cmdCancel, SIGNAL(clicked()), this, SLOT(cmdCancel_Click()));
	connect(cmdOk, SIGNAL(clicked()), this, SLOT(cmdOk_Click()));
	
	return;
}

void IconsView::cmdCancel_Click(){
	reject();
	return;	
}

void IconsView::cmdOk_Click(){
	//Getting selected icons
	
	//FIXME: For multiple icon export
	//QList<QListWidgetItem *> icoList = lstIcons->selectedItems();
		
	//for (int i=0; i<icoList.count(); i++){
	//		iconBuffer.names.append(icoList.at(i)->text());
	//	}
	
	if (lstIcons->currentItem()){
		QFile file;
	
		QString sourceFile, saveFile, saveFileName;
		bool ok=FALSE;
		
		sourceFile.clear();
		sourceFile.append(tempDirectory);
		sourceFile.append("/");
		sourceFile.append(lstIcons->currentItem()->text());
		
		
		if (cbDefaultExport->checkState()==Qt::Checked){
		saveFile.clear();
		saveFile.append(QDir::homePath());
		saveFile.append("/.config/");
		saveFile.append(APP_SHORT_NAME);
		saveFile.append("/icons/");
		saveFile.append(lstIcons->currentItem()->text());
		
		saveFileName=lstIcons->currentItem()->text();
		
		
		QMessageBox message;
			message.setText(tr("Sorry. It seems file already exists.<br>Replace existent or rename current?"));
			message.setWindowTitle(tr("Exporting icon"));
			message.setIcon(QMessageBox::Question);
			message.addButton(tr("Rename"),QMessageBox::ResetRole);
			message.addButton(tr("Use existing"), QMessageBox::ActionRole);
			message.addButton(tr("Replace"), QMessageBox::DestructiveRole);
			message.addButton(tr("Cancel"), QMessageBox::RejectRole);
					
			while (QFile::exists (saveFile)){
				switch (message.exec()){
					case 0:
						saveFileName = QInputDialog::getText(this, tr("Sorry. It seems file already exists."),
																		tr("Replace existent or rename current?"), QLineEdit::Normal,
																	saveFileName , &ok);
						if ((!saveFileName.isEmpty()) && (ok)){
							saveFile.clear();
							saveFile.append(QDir::homePath());
							saveFile.append("/.config/");
							saveFile.append(APP_SHORT_NAME);
							saveFile.append("/icons/");
							saveFile.append(saveFileName);
						} else {
							reject();
							return;
						}
					break;
					case 1:
						saveFile.clear();
						saveFile.append(QDir::homePath());
						saveFile.append("/.config/");
						saveFile.append(APP_SHORT_NAME);
						saveFile.append("/icons/");
						saveFile.append(saveFileName);
						selectedFile=saveFile;
						accept();
						return;
					break;
					case 2:
						if (!file.remove(saveFile)){
							QMessageBox::warning(this, tr("Error"), tr("Sorry, i can't remove file: <br>%1.").arg(saveFile), QMessageBox::Ok);
						}
					break;
					case 3:
						reject();
						return;
					break;
				}		
		}
		
		} else {
			saveFile.clear();
			saveFile.append(QDir::homePath());
			saveFile.append("/.config/");
			saveFile.append(APP_SHORT_NAME);
			saveFile.append("/icons/");
			saveFile = QFileDialog::getSaveFileName(this, tr("Select file to save"), saveFile , tr("Images (*.png)"));
	
			if (saveFile.isEmpty()){
				reject();
				return;
			}
		}
		
		if (!file.copy(sourceFile, saveFile)){
			QMessageBox::warning(this, tr("Error"), tr("Sorry, i can't create file: <br>%1.").arg(saveFile), QMessageBox::Ok);
		}
		
		selectedFile=saveFile;
		accept();
		return;
		
	} else {
		reject();
		return;
	}
}

