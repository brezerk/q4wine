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

#include "iconsettings.h"

IconSettings::IconSettings(bool newIcon, QString prefix_id, QString prefix_dir, QString dir_id, QString icon_name, QString winedll_path, QWidget * parent, Qt::WFlags f) : QDialog(parent, f)
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

	QSettings settings(APP_SHORT_NAME, "default");
	settings.beginGroup("app");
	loadThemeIcons(settings.value("theme").toString());
	settings.endGroup();

	settings.beginGroup("icotool");
	WRESTOOL_BIN = settings.value("wrestool").toString();
	ICOTOOL_BIN = settings.value("icotool").toString();
	settings.endGroup();

	if (prefix_dir.isEmpty()){
		prefix_dir.append(QDir::homePath());
		prefix_dir.append("/.wine/drive_c/");
	}

	this->prefix_id=prefix_id;
	this->prefix_dir=prefix_dir;
	this->dir_id=dir_id;
	this->iconName=icon_name;
	this->newIcon=newIcon;

	switch (newIcon){
		case TRUE:
			lblCaption->setText(tr("Adding new icon"));
			setWindowTitle(tr("Adding new icon"));
		break;
		case FALSE:
			lblCaption->setText(tr("Icon settings"));
			setWindowTitle(tr("Icon settings"));
			settings.beginGroup("app");
				getIconReccord(settings.value("theme").toString());
			settings.endGroup();
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

	getWineDlls(winedll_path);

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

void IconSettings::getIconReccord(QString themePath){
	QSqlQuery query;

	if (!dir_id.isEmpty()){
		query.exec(tr("SELECT name, exec, cmdargs, icon_path, desc, display, winedebug, useconsole, override, id, wrkdir, desktop, nice FROM icon WHERE prefix_id=%1 AND dir_id=%2 and name=\"%3\"") .arg(prefix_id) .arg(dir_id) .arg(iconName));
	} else {
		query.exec(tr("SELECT name, exec, cmdargs, icon_path, desc, display, winedebug, useconsole, override, id, wrkdir, desktop, nice FROM icon WHERE prefix_id=%1 AND name=\"%2\" AND dir_id ISNULL") .arg(prefix_id) .arg(iconName));
	}

	query.first();

	txtName->setText(query.value(0).toString());
	txtProgramPath->setText(query.value(1).toString());
	txtCmdArgs->setText(query.value(2).toString());

	iconPath.clear();



	if (!query.value(3).toString().isEmpty()){
		if (QFile(query.value(3).toString()).exists()){
			cmdGetIcon->setIcon (QIcon(query.value(3).toString()));
			iconPath=query.value(3).toString();
		} else {
			iconPath=query.value(3).toString();
			if (iconPath=="wineconsole"){
				cmdGetIcon->setIcon(loadIcon("data/wineconsole.png", themePath));
			} else if (iconPath=="regedit"){
				cmdGetIcon->setIcon(loadIcon("data/regedit.png", themePath));
			} else if (iconPath=="wordpad"){
				cmdGetIcon->setIcon(loadIcon("data/notepad.png", themePath));
			} else if (iconPath=="winecfg"){
				cmdGetIcon->setIcon(loadIcon("data/winecfg.png", themePath));
			} else if (iconPath=="uninstaller"){
				cmdGetIcon->setIcon(loadIcon("data/uninstaller.png", themePath));
			} else if (iconPath=="eject"){
				cmdGetIcon->setIcon(loadIcon("data/eject.png", themePath));
			} else if (iconPath=="explorer"){
				cmdGetIcon->setIcon(loadIcon("data/explorer.png", themePath));
			} else {
				cmdGetIcon->setIcon(loadIcon("data/exec_wine.png", themePath));
				iconPath="";
			}
		}
	}

	txtDesc->setText(query.value(4).toString());
	txtDisplay->setText(query.value(5).toString());
	txtWinedebug->setText(query.value(6).toString());
	txtWorkDir->setText(query.value(10).toString());
	txtDesktopSize->setText(query.value(11).toString());
	spinNice->setValue(query.value(12).toInt());

	if (query.value(7).toString()=="1"){
		cbUseConsole->setCheckState(Qt::Checked);
		txtWinedebug->setEnabled(TRUE);
	} else {
		cbUseConsole->setCheckState(Qt::Unchecked);
		txtWinedebug->setEnabled(FALSE);
	}

	QStringList override = query.value(8).toString().split(";");

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

	iconId = query.value(9).toString();

	return;
}

void IconSettings::getWineDlls(QString winedll_path){

	if (winedll_path.isEmpty()){
		QSettings settings(APP_SHORT_NAME, "default");
		settings.beginGroup("wine");
			winedll_path=settings.value("WineLibs").toString();
		settings.endGroup();
	}

	QDir dir(winedll_path);
	dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);

	QFileInfoList list = dir.entryInfoList();
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		if (fileInfo.fileName().indexOf(".dll.so")>=0){
			cboxDlls->addItem ( fileInfo.fileName().left(fileInfo.fileName().length()-3));
		}
	}

	cboxDlls->setMaxVisibleItems (10);

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
	QString fileName = QFileDialog::getExistingDirectory(this, tr("Open Directory"), QDir::homePath(),   QFileDialog::DontResolveSymlinks);

	if(!fileName.isEmpty()){
		txtWorkDir->setText(fileName);
	}
	return;
}

void IconSettings::cmdAdd_Click(){
	if (!cboxDlls->currentText().isEmpty()){
		twDlls->insertRow (0);
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

	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Exe file"), prefix_dir, tr("Exe files (*.exe)"));

	if(!fileName.isEmpty()){
			QStringList list1 = fileName.split("/");
			txtName->setText(list1.last());
			txtProgramPath->setText(fileName);

			QString wrkDir;
			wrkDir = fileName.left(fileName.length() - list1.last().length());
			txtWorkDir->setText(wrkDir);
	}

	return;
}

void IconSettings::cmdGetIcon_Click(){

	QString fileName, searchPath;

	if (!txtWorkDir->text().isEmpty()){
	    searchPath = txtWorkDir->text();
	} else {
		if (!iconPath.isEmpty()){
			searchPath = iconPath;
		} else {
		    searchPath = prefix_dir;
		}
	}

	fileName = QFileDialog::getOpenFileName(this, tr("Open image file"), searchPath, tr("Image and Win32 binary files (*.png *.jpg *.gif *.bmp *.exe *.dll);;Image files (*.png *.jpg *.gif *.bmp);;Win32 Executable (*.exe);;Win32 Shared libraies (*.dll);;Win32 Executable and Shared libraies (*.exe *.dll)") );

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

			Process *exportProcess = new Process(args, WRESTOOL_BIN, QDir::homePath(), tr("Exporting icon from binary file.<br>This can take a while..."), tr("Exporting icon"));

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
					exportProcess = new Process(args, ICOTOOL_BIN, QDir::homePath(), tr("Convering icon from binary file.<br>This can take a while..."), tr("Converting icon"));
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
	QSqlQuery query;

	if (iconName!=txtName->text()){
		query.prepare("select id from icon where name=:name and prefix_id=:prefix_id and dir_id=:dir_id");
		query.bindValue(":name", txtName->text());

		if(dir_id.isEmpty())
			query.bindValue(":dir_id", QVariant(QVariant::String));
		else
			query.bindValue(":dir_id", dir_id);

		query.bindValue(":prefix_id", prefix_id);

		query.exec();
		query.first();


		if (query.isValid ()){
			QMessageBox::warning(this, tr("Error"), tr("Sorry, but icon named %1 already exists.").arg(txtName->text()));
			return;
		}
	}

	if (txtProgramPath->text().isEmpty()){
		QMessageBox::warning(this, tr("Error"), tr("No executiable program selected."));
		return;
	}

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



	switch (newIcon){
		case TRUE:
			query.prepare("INSERT INTO icon(override, winedebug, useconsole, display, cmdargs, exec, icon_path, desc, dir_id, id, name, prefix_id, wrkdir, desktop, nice) VALUES(:override, :winedebug, :useconsole, :display, :cmdargs, :exec, :icon_path, :desc, :dir_id, NULL, :name, :prefix_id, :wrkdir, :desktop, :nice);");

			if(dir_id.isEmpty())
				query.bindValue(":dir_id", QVariant(QVariant::String));
			else
				query.bindValue(":dir_id", dir_id);

			query.bindValue(":prefix_id", prefix_id);
		break;
		case FALSE:
			query.prepare("UPDATE icon SET override=:override, winedebug=:winedebug, useconsole=:useconsole, display=:display,  cmdargs=:cmdargs, exec=:exec, icon_path=:icon_path, desc=:desc, name=:name, wrkdir=:wrkdir, desktop=:desktop, nice=:nice WHERE id=:id;");

			query.bindValue(":id", iconId);
		break;
	}

	if (override.isEmpty())
		query.bindValue(":override", QVariant(QVariant::String));
	else
		query.bindValue(":override", override);

	if (txtWinedebug->text().isEmpty())
		query.bindValue(":winedebug", QVariant(QVariant::String));
	else
		query.bindValue(":winedebug", txtWinedebug->text());

	if (txtWorkDir->text().isEmpty())
		query.bindValue(":wrkdir", QVariant(QVariant::String));
	else
		query.bindValue(":wrkdir", txtWorkDir->text());


	if (cbUseConsole->checkState()==Qt::Checked)
		query.bindValue(":useconsole", 1);
	else
		query.bindValue(":useconsole", 0);

	if (txtDisplay->text().isEmpty())
		query.bindValue(":display", QVariant(QVariant::String));
	else
		query.bindValue(":display", txtDisplay->text());

	if (txtCmdArgs->text().isEmpty())
		query.bindValue(":cmdargs", QVariant(QVariant::String));
	else
		query.bindValue(":cmdargs", txtCmdArgs->text());

	if (txtProgramPath->text().isEmpty())
		query.bindValue(":exec", QVariant(QVariant::String));
	else
		query.bindValue(":exec", txtProgramPath->text());

	if (iconPath.isEmpty())
		query.bindValue(":icon_path", QVariant(QVariant::String));
	else
		query.bindValue(":icon_path", iconPath);

	if (txtDesc->text().isEmpty())
		query.bindValue(":desc", QVariant(QVariant::String));
	else
		query.bindValue(":desc", txtDesc->text());

	query.bindValue(":name", txtName->text());

	query.bindValue(":desktop", txtDesktopSize->text());

	query.bindValue(":nice", spinNice->value());

	if (!query.exec()){
		QMessageBox::warning(this, tr("Error"), tr("debug: %1").arg(query.lastError().text()));
	}

	accept();
	return;
}
