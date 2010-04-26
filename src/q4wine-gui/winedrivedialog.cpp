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

#include "winedrivedialog.h"

WineDriveDialog::WineDriveDialog(QStringList removeLetters, QWidget * parent, Qt::WFlags f) : QDialog(parent, f)
{
	setupUi(this);

	// Loading libq4wine-core.so
	libq4wine.setFileName("libq4wine-core");

	if (!libq4wine.load()){
		libq4wine.load();
	}

	// Getting corelib calss pointer
	CoreLibClassPointer = (CoreLibPrototype *) libq4wine.resolve("createCoreLib");
	CoreLib.reset((corelib *)CoreLibClassPointer(true));

	if (removeLetters.count()>0){
		for (int i=0; i<removeLetters.count(); i++){
			if (cbDriveLetter->findText(removeLetters.at(i), Qt::MatchExactly)>=0){
				cbDriveLetter->removeItem(cbDriveLetter->findText(removeLetters.at(i)));
			}
		}
	}

	if (cbDriveLetter->count()>2){
		cbDriveLetter->setCurrentIndex(2);
	}

	connect(cmdCancel, SIGNAL(clicked()), this, SLOT(cmdCancel_Click()));
	connect(cmdOk, SIGNAL(clicked()), this, SLOT(cmdOk_Click()));
	connect(cmdGetDrivePath, SIGNAL(clicked()), this, SLOT(cmdGetDrivePath_Click()));

	cmdGetDrivePath->setIcon(CoreLib->loadIcon("data/folder.png"));

	cmdOk->setFocus(Qt::ActiveWindowFocusReason);

}

WineDriveDialog::WineDriveDialog(QStringList removeLetters, QString driveLetter, QString drivePath, QString driveType, QWidget * parent, Qt::WFlags f) : QDialog(parent, f)
{
	setupUi(this);

	// Loading libq4wine-core.so
	libq4wine.setFileName("libq4wine-core");

	if (!libq4wine.load()){
		libq4wine.load();
	}

	// Getting corelib calss pointer
	CoreLibClassPointer = (CoreLibPrototype *) libq4wine.resolve("createCoreLib");
	CoreLib.reset((corelib *)CoreLibClassPointer(true));

	if (removeLetters.count()>0){
		for (int i=0; i<removeLetters.count(); i++){
			if (cbDriveLetter->findText(removeLetters.at(i), Qt::MatchExactly)>=0){
				cbDriveLetter->removeItem(cbDriveLetter->findText(removeLetters.at(i)));
			}
		}
	}

	if (cbDriveLetter->findText(driveLetter, Qt::MatchExactly)>=0){
				cbDriveLetter->setCurrentIndex(cbDriveLetter->findText(driveLetter, Qt::MatchExactly));
	}

	txtDrivePath->setText(drivePath);


	if (cbDriveType->findText(driveType, Qt::MatchExactly)>=0){
				cbDriveType->setCurrentIndex(cbDriveType->findText(driveType, Qt::MatchExactly));
	}

	if (driveLetter=="C:"){
		cbDriveLetter->setEnabled(FALSE);
		cbDriveType->setEnabled(FALSE);
		cmdGetDrivePath->setEnabled(FALSE);
		txtDrivePath->setEnabled(FALSE);
	}

	connect(cmdCancel, SIGNAL(clicked()), this, SLOT(cmdCancel_Click()));
	connect(cmdOk, SIGNAL(clicked()), this, SLOT(cmdOk_Click()));
	connect(cmdGetDrivePath, SIGNAL(clicked()), this, SLOT(cmdGetDrivePath_Click()));

	cmdGetDrivePath->setIcon(CoreLib->loadIcon("data/folder.png"));

	cmdOk->setFocus(Qt::ActiveWindowFocusReason);
}

QString WineDriveDialog::getLetter(){
	return cbDriveLetter->currentText();
}

QString WineDriveDialog::getPath(){
	return txtDrivePath->text();
}

QString WineDriveDialog::getType(){
	return cbDriveType->currentText();
}

void WineDriveDialog::cmdCancel_Click(){
	reject();
}

void WineDriveDialog::cmdOk_Click(){
	if (cbDriveLetter->currentText()=="C:"){
		accept();
		return;
	}

	if (txtDrivePath->text().isEmpty()){
		QMessageBox::warning(this, tr("Error"), tr("Sorry, you need to set existing drive directory."), QMessageBox::Ok);
		return;
	}

	if (!QDir().exists(txtDrivePath->text())){
		QMessageBox::warning(this, tr("Error"), tr("Sorry, drive directory not exists."), QMessageBox::Ok);
		return;
	}

	accept();
}

void WineDriveDialog::cmdGetDrivePath_Click(){
	QString fileName;
	QString searchPath=txtDrivePath->text();

	QFileDialog dialog(this);
	  dialog.setFilter(QDir::Dirs | QDir::Hidden);

#if QT_VERSION >= 0x040500
      if (CoreLib->getSetting("advanced", "dontUseNativeFileDialog", false, 0)==1){
          dialog.setOptions(QFileDialog::DontUseNativeDialog);
      }
#endif

	  dialog.setFileMode(QFileDialog::Directory);
	  dialog.setWindowTitle(tr("Open Directory"));
	  if (!searchPath.isEmpty()){
		  if (QDir(searchPath).exists()){
			  dialog.setDirectory(searchPath);
		  } else {
			  dialog.setDirectory(QDir::homePath());
		  }
	  } else {
		  dialog.setDirectory(QDir::homePath());
	  }

	if (dialog.exec())
		fileName = dialog.selectedFiles().first();

	if(!fileName.isEmpty()){
		txtDrivePath->setText(fileName);
	}

	return;
}
