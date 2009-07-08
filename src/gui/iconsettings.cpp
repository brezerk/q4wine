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

#include "iconsettings.h"

IconSettings::IconSettings(QString prefix_name, QString dir_name, QString icon_name, QWidget * parent, Qt::WFlags f) : QDialog(parent, f)
{
	/*
	 ** This functions is constructor for IconSettings dialog
	 *
	 * Args:
	 * @newIcon -- This define which type of dialog we'l use "New Icon" or "Edit Icon"
	 * @p_id    -- PrefixId
	 * @p_id    -- PrefixId
	 */
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
	db_prefix = new Prefix();
	db_icon = new Icon();

	this->prefix_name = prefix_name;
	this->dir_name = dir_name;
	this->icon_name = icon_name;
	this->prefix_path = db_prefix->getFieldsByPrefixName(this->prefix_name).at(1);
	if (this->prefix_path.isEmpty()){
		  this->prefix_path = QDir::homePath();
		  this->prefix_path.append("/.wine/drive_c/");
	} else {
		  this->prefix_path.append("/drive_c/");
	}


	// Creating side bar urls
	prefix_urls << QUrl::fromLocalFile(QDir::rootPath());
	prefix_urls << QUrl::fromLocalFile(QDir::homePath());

	if (QDir(this->prefix_path).exists())
	   prefix_urls << QUrl::fromLocalFile(this->prefix_path);

	QString cd_mount = db_prefix->getFieldsByPrefixName(this->prefix_name).at(6);

	if (!cd_mount.isEmpty())
	  if (QDir().exists(cd_mount))
		  prefix_urls << QUrl::fromLocalFile(cd_mount);

	this->loadThemeIcons(this->CoreLib->getSetting("app", "theme", false).toString());

	switch (icon_name.isEmpty()){
		case TRUE:
			lblCaption->setText(tr("Adding new icon"));
			setWindowTitle(tr("Adding new icon"));
		break;
		case FALSE:
			lblCaption->setText(tr("Icon settings"));
			setWindowTitle(tr("Icon settings"));
			getIconReccord();
		break;
	}

	connect(twbGeneral, SIGNAL(currentChanged(int)), this, SLOT(ResizeContent(int)));
	connect(cmdAdd, SIGNAL(clicked()), this, SLOT(cmdAdd_Click()));
	connect(cmdGetProgram, SIGNAL(clicked()), this, SLOT(cmdGetProgram_Click()));
	connect(cmdGetWorkDir, SIGNAL(clicked()), this, SLOT(cmdGetWorkDir_Click()));
	connect(cmdGetIcon, SIGNAL(clicked()), this, SLOT(cmdGetIcon_Click()));
	connect(cmdCancel, SIGNAL(clicked()), this, SLOT(cmdCancel_Click()));
	connect(cmdOk, SIGNAL(clicked()), this, SLOT(cmdOk_Click()));

	connect(cbUseConsole, SIGNAL(stateChanged(int)), this, SLOT(cbUseConsole_stateChanged(int)));

	twDlls->installEventFilter(this);

	cboxDlls->addItems(CoreLib->getWineDlls(db_prefix->getFieldsByPrefixName(prefix_name).at(2)));
	cboxDlls->setMaxVisibleItems (10);

	return;
}

void IconSettings::loadThemeIcons(QString themePath){
	QPixmap pixmap;

	if (!pixmap.load(tr("%1/data/exec.png").arg(themePath))){
		pixmap.load(":data/exec.png");
	}

	lblLogo->setPixmap(pixmap);

	cmdGetProgram->setIcon(loadIcon("data/folder.png", themePath));
	cmdGetWorkDir->setIcon(loadIcon("data/folder.png", themePath));

	cmdGetIcon->setIcon(loadIcon("data/exec_wine.png", themePath));

	return;
}


QIcon IconSettings::loadIcon(QString iconName, QString themePath){
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

void IconSettings::getIconReccord(){
	QStringList iconRec;

	if (this->dir_name.isEmpty()){
		iconRec = db_icon->getByName(this->prefix_name, this->dir_name, this->icon_name);
	} else {
		iconRec = db_icon->getByName(this->prefix_name, this->dir_name, this->icon_name);
	}

	txtName->setText(iconRec.at(1));
	txtCmdArgs->setText(iconRec.at(9));
	txtProgramPath->setText(iconRec.at(10));

	iconPath.clear();

	if (!iconRec.at(3).isEmpty()){
		if (QFile(iconRec.at(3)).exists()){
			cmdGetIcon->setIcon (QIcon(iconRec.at(3)));
			iconPath=iconRec.at(3);
		} else {
			iconPath=iconRec.at(3);
			if (iconPath=="wineconsole"){
				cmdGetIcon->setIcon(loadIcon("data/wineconsole.png", this->CoreLib->getSetting("app", "theme", false).toString()));
			} else if (iconPath=="regedit"){
				cmdGetIcon->setIcon(loadIcon("data/regedit.png", this->CoreLib->getSetting("app", "theme", false).toString()));
			} else if (iconPath=="wordpad"){
				cmdGetIcon->setIcon(loadIcon("data/notepad.png", this->CoreLib->getSetting("app", "theme", false).toString()));
			} else if (iconPath=="winecfg"){
				cmdGetIcon->setIcon(loadIcon("data/winecfg.png", this->CoreLib->getSetting("app", "theme", false).toString()));
			} else if (iconPath=="uninstaller"){
				cmdGetIcon->setIcon(loadIcon("data/uninstaller.png", this->CoreLib->getSetting("app", "theme", false).toString()));
			} else if (iconPath=="eject"){
				cmdGetIcon->setIcon(loadIcon("data/eject.png", this->CoreLib->getSetting("app", "theme", false).toString()));
			} else if (iconPath=="explorer"){
				cmdGetIcon->setIcon(loadIcon("data/explorer.png", this->CoreLib->getSetting("app", "theme", false).toString()));
			} else {
				cmdGetIcon->setIcon(loadIcon("data/exec_wine.png", this->CoreLib->getSetting("app", "theme", false).toString()));
				iconPath="";
			}
		}
	}

	txtDesc->setText(iconRec.at(2));
	txtDisplay->setText(iconRec.at(8));
	txtWinedebug->setText(iconRec.at(6));
	txtWorkDir->setText(iconRec.at(4));
	txtDesktopSize->setText(iconRec.at(11));
	spinNice->setValue(iconRec.at(12).toInt());

	if (iconRec.at(7)=="1"){
		cbUseConsole->setCheckState(Qt::Checked);
		txtWinedebug->setEnabled(TRUE);
	} else {
		cbUseConsole->setCheckState(Qt::Unchecked);
		txtWinedebug->setEnabled(FALSE);
	}

	QStringList override = iconRec.at(5).split(";");

	QString overrideorder;

	for (int i=0; i<override.count()-1; i++){

		QStringList list2 = override.at(i).split("=");
			twDlls->insertRow (0);
			QTableWidgetItem *newItem = new QTableWidgetItem(list2.at(0));
			twDlls->setItem(0, 0, newItem);
			newItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );

			if (list2.at(1)=="n")
				overrideorder = tr("Native");
			if (list2.at(1)=="b")
				overrideorder = tr("Buildin");
			if (list2.at(1)=="n,b")
				overrideorder = tr("Native, Buildin");
			if (list2.at(1)=="b,n")
				overrideorder = tr("Buildin, Native");


			newItem = new QTableWidgetItem(overrideorder);
			twDlls->setItem(0, 1, newItem);
			newItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
	}

	return;
}

bool IconSettings::eventFilter( QObject *object, QEvent *event )
{
   //  firstly, check whether the object is the QTableWidget and if it's a mouse press event
	if (object == twDlls)
		if (event->type() == QEvent::KeyPress)
		{   // if yes, we need to cast the event
		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
		if (keyEvent->key()==Qt::Key_Delete)
			twDlls->removeRow(twDlls->currentRow());

		return true;
		}

	return QWidget::eventFilter(object, event);
}


void IconSettings::ResizeContent(int TabIndex){

	switch (TabIndex){
		case 1:
			twDlls->resizeRowsToContents();
			twDlls->resizeColumnsToContents();
			twDlls->horizontalHeader()->setStretchLastSection(TRUE);
		break;
	}

	return;
}

void IconSettings::resizeEvent (QResizeEvent){
	ResizeContent(2);
	return;
}

/*************************************************************\
|						CheckBox Events								 |
\*************************************************************/

void IconSettings::cbUseConsole_stateChanged(int){
	switch(cbUseConsole->checkState()){
		case Qt::PartiallyChecked:
			txtWinedebug->setEnabled(TRUE);
		break;
		case Qt::Checked:
			txtWinedebug->setEnabled(TRUE);
		break;
		case Qt::Unchecked:
			txtWinedebug->setEnabled(FALSE);
		break;
	}

	return;
}

/*************************************************************\
|						BUTTONS EVENTS								 |
\*************************************************************/

void IconSettings::cmdGetWorkDir_Click(){

	QString fileName, searchPath=this->prefix_path;

	if ((!txtProgramPath->text().isEmpty()) and (QDir().exists(txtProgramPath->text()))){
		searchPath=txtProgramPath->text().left(txtProgramPath->text().length() - txtProgramPath->text().split("/").last().length());;
	}

	if (!QDir(searchPath).exists()){
		if (QDir(this->prefix_path).exists()){
		   searchPath=this->prefix_path;
		} else {
		   searchPath=QDir::homePath();
		}
	 }

	QList<QUrl> add_prefix_urls = this->prefix_urls;
	if ((searchPath != this->prefix_path) && (QDir(searchPath).exists()))
		add_prefix_urls << QUrl::fromLocalFile(searchPath);

	QFileDialog dialog(this);
	  dialog.setFilter(QDir::Dirs | QDir::Hidden);

	  dialog.setFileMode(QFileDialog::Directory);
	  dialog.setWindowTitle(tr("Open Directory"));
	  dialog.setDirectory(searchPath);
	  // This option works only it qt 4.5. In fact this not works correctly with QDir::Hidden,  so I comment it out for a some  time
	  // dialog.setOption(QFileDialog::ShowDirsOnly, true);
	  dialog.setSidebarUrls(add_prefix_urls);

	if (dialog.exec())
		fileName = dialog.selectedFiles().first();

	if(!fileName.isEmpty()){
		txtWorkDir->setText(fileName);
	}
	return;
}

void IconSettings::cmdAdd_Click(){
	if (!cboxDlls->currentText().isEmpty()){
		twDlls->insertRow (0);
		//QTableWidgetItem *newItem = new QTableWidgetItem(cboxDlls->currentText().split(".").at(0));
		QTableWidgetItem *newItem = new QTableWidgetItem(cboxDlls->currentText());
		twDlls->setItem(0, 0, newItem);
		newItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
		newItem = new QTableWidgetItem(cboxOveride->currentText());
		twDlls->setItem(0, 1, newItem);
		newItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
	}

	twDlls->resizeRowsToContents();
	twDlls->resizeColumnsToContents();
	twDlls->horizontalHeader()->setStretchLastSection(TRUE);
	return;
}

void IconSettings::cmdGetProgram_Click(){

	QString fileName, searchPath = this->prefix_path;

	if (!txtProgramPath->text().isEmpty()){
		if (!txtWorkDir->text().isEmpty()){
		searchPath=txtWorkDir->text();
		} else {
		searchPath=txtProgramPath->text().left(txtProgramPath->text().length() - txtProgramPath->text().split("/").last().length());;
		}
	}

	if ((!QDir(searchPath).exists()) or (searchPath.isEmpty())){
		if (QDir(dir_name).exists()){
		   searchPath=this->prefix_path;
		} else {
		   searchPath=QDir::homePath();
		}
	 }

	QList<QUrl> add_prefix_urls = this->prefix_urls;
	if ((searchPath != this->prefix_path) && (QDir(searchPath).exists()))
		add_prefix_urls << QUrl::fromLocalFile(searchPath);

	QFileDialog dialog(this);
	  dialog.setFilter(QDir::Dirs | QDir::Hidden | QDir::Files );
	  dialog.setWindowTitle(tr("Open Exe file"));
	  dialog.setDirectory(searchPath);
	  dialog.setFileMode(QFileDialog::ExistingFile);
	  dialog.setNameFilter(tr("Exe files (*.exe)"));
	  dialog.setSidebarUrls(add_prefix_urls);

	 if (dialog.exec())
		fileName = dialog.selectedFiles().first();

	if(!fileName.isEmpty()){
			QStringList list1 = fileName.split("/");
			txtName->setText(list1.last().left(list1.last().length() - list1.last().split(".").last().length() - 1));
			txtProgramPath->setText(fileName);

			QString wrkDir;
			wrkDir = fileName.left(fileName.length() - list1.last().length());
			txtWorkDir->setText(wrkDir);
	}

	return;
}

void IconSettings::cmdGetIcon_Click(){

	QString fileName, searchPath=this->prefix_path;


	if ((!txtWorkDir->text().isEmpty()) and (QDir(txtWorkDir->text()).exists())){
		searchPath = txtWorkDir->text();
	} else {
		if (QDir(this->prefix_path).exists()){
		   searchPath=this->prefix_path;
		} else {
		   searchPath=QDir::homePath();
		}
	}

	QList<QUrl> add_prefix_urls = this->prefix_urls;
	if ((searchPath != this->prefix_path) && (QDir(searchPath).exists()))
		add_prefix_urls << QUrl::fromLocalFile(searchPath);

	QString addPath;
	addPath=QDir::homePath();
	addPath.append("/.config/");
	addPath.append(APP_SHORT_NAME);
	addPath.append("/icons");

	if ((QDir(addPath).exists()))
		add_prefix_urls << QUrl::fromLocalFile(addPath);

	addPath=QDir::homePath();
	addPath.append("/.local/share/icons/");

	if ((QDir(addPath).exists()))
		add_prefix_urls << QUrl::fromLocalFile(addPath);

	QFileDialog dialog(this);
	  dialog.setFilter(QDir::Dirs | QDir::Hidden | QDir::Files );
	  dialog.setFileMode(QFileDialog::ExistingFile);
	  dialog.setWindowTitle(tr("Open image file"));
	  if ((!iconPath.isEmpty()) and (QFile(iconPath).exists())){
		  QStringList list = iconPath.split("/");
		  searchPath = iconPath.left(iconPath.length() - list.last().length());
	  }
	  dialog.setDirectory(searchPath);

		#ifndef WITH_ICOTOOLS
		dialog.setNameFilter(tr("Image files (*.png *.jpg *.gif *.bmp *.xpm)"));
		#else
		dialog.setNameFilter(tr("Image and Win32 binary files (*.png *.jpg *.gif *.bmp *.xpm *.exe *.dll);;Image files (*.png *.jpg *.gif *.bmp *.xpm);;Win32 Executable (*.exe);;Win32 Shared libraies (*.dll);;Win32 Executable and Shared libraies (*.exe *.dll)"));
		#endif
	  dialog.setSidebarUrls(add_prefix_urls);

	 if (dialog.exec())
		fileName = dialog.selectedFiles().first();

	if(!fileName.isEmpty()){
		if ((fileName.toLower().right(3)!="exe") && (fileName.toLower().right(3)!="dll")){
			cmdGetIcon->setIcon (QIcon(fileName));
		} else {

			QStringList args;
			args << "-x";
			args << "-t" << "14";

			QString tmpDir;
			QStringList list1 = fileName.split("/");

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

			Process *exportProcess = new Process(args, CoreLib->getSetting("icotool", "wrestool").toString(), QDir::homePath(), tr("Exporting icon from binary file.<br>This can take a while..."), tr("Exporting icon"));

			if (exportProcess->exec()==QDialog::Accepted){
			//icotool -x -o ./regedit.png --width=32 --height=32 ./regedit.exe_14_100_0.ico
				args.clear();
				args << "-x";

				// Updating file index
				list = tmp.entryInfoList();

				//Creating file list for converting
				for (int i = 0; i < list.size(); ++i) {
					QFileInfo fileInfo = list.at(i);
					if (fileInfo.fileName().right(3)=="ico")
						args << fileInfo.filePath();
				}

				args << "-o" << tr("%1/").arg(tmpDir);

				//Converting ico files to png

				//Look here, this function checks is some icons found, or not. 5 -- is default number of arguments,
				//if more -- then we have some ico file to convert
				if (args.size()>=4){
					exportProcess = new Process(args, CoreLib->getSetting("icotool", "icotool").toString(), QDir::homePath(), tr("Convering icon from binary file.<br>This can take a while..."), tr("Converting icon"));
					if (exportProcess->exec()==QDialog::Accepted){
						IconsView *iconsView = new IconsView(tmpDir);
						if (iconsView->exec()==QDialog::Accepted){
							fileName=iconsView->selectedFile;
							cmdGetIcon->setIcon (QIcon(fileName));
						} else {
							fileName.clear();
						}
					}
				} else {
					IconsView *iconsView = new IconsView(tmpDir);
					if (iconsView->exec()==QDialog::Accepted){
						fileName=iconsView->selectedFile;
						cmdGetIcon->setIcon (QIcon(fileName));
					} else {
						fileName.clear();
					}
				}
			} else {
				qDebug()<<"wrestool testing Rejected";
				fileName.clear();
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
		}

		if (!fileName.isEmpty())
			iconPath=fileName;
	}

	return;
}

void IconSettings::cmdCancel_Click(){
	reject();
	return;
}

void IconSettings::cmdOk_Click(){

  if (txtProgramPath->text().isEmpty()){
	  QMessageBox::warning(this, tr("Error"), tr("No executiable program selected."));
	  return;
  }

  if (txtName->text().isEmpty()){
	  QMessageBox::warning(this, tr("Error"), tr("No program name specified."));
	  return;
  }

  if (icon_name!=txtName->text()){
	  if (this->dir_name.isEmpty()){
		  if (db_icon->isExistsByName(this->prefix_name, txtName->text())){
			  QMessageBox::warning(this, tr("Error"), tr("Sorry, but icon named %1 already exists.").arg(txtName->text()));
			  return;
		  }
	  } else {
		  if (db_icon->isExistsByName(this->prefix_name, this->dir_name, txtName->text())){
			  QMessageBox::warning(this, tr("Error"), tr("Sorry, but icon named %1 already exists.").arg(txtName->text()));
			  return;
		  }
	  }
  }

  QSqlQuery query;

	QString override;
	for (int i=1; i<=twDlls->rowCount(); i++){
		override.append(tr("%1=").arg(twDlls->item(i-1, 0)->text()));
		if (twDlls->item(i-1, 1)->text()==tr("Native"))
				override.append("n;");
		if (twDlls->item(i-1, 1)->text()==tr("Buildin"))
					override.append("b;");
		if (twDlls->item(i-1, 1)->text()==tr("Native, Buildin"))
			override.append("n,b;");
		if (twDlls->item(i-1, 1)->text()==tr("Buildin, Native"))
			override.append("b,n;");
	}

	QString useconsole;
	if (cbUseConsole->checkState()==Qt::Checked){
		useconsole="1";
	} else {
		useconsole="0";
	}

	switch (this->icon_name.isEmpty()){
		case TRUE:
			db_icon->addIcon(txtCmdArgs->text(), txtProgramPath->text(), iconPath, txtDesc->text(), this->prefix_name, this->dir_name, txtName->text(), override, txtWinedebug->text(), useconsole, txtDisplay->text(), txtWorkDir->text(), txtDesktopSize->text(), spinNice->value());
		break;
		case FALSE:
			db_icon->updateIcon(txtCmdArgs->text(), txtProgramPath->text(), iconPath, txtDesc->text(), this->prefix_name, this->dir_name, txtName->text(), icon_name, override, txtWinedebug->text(), useconsole, txtDisplay->text(), txtWorkDir->text(), txtDesktopSize->text(), spinNice->value());
		break;
	}

	accept();

	return;
}
