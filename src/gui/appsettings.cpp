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

#include "appsettings.h"

AppSettings::AppSettings(QWidget * parent, Qt::WFlags f) : QDialog(parent, f)
{
	setupUi(this);
	
	setWindowTitle(tr("%1 settings").arg(APP_NAME));
	lblCaption->setText(tr("%1 settings").arg(APP_NAME));
	
	connect(cmdCancel, SIGNAL(clicked()), this, SLOT(cmdCancel_Click()));
	connect(cmdOk, SIGNAL(clicked()), this, SLOT(cmdOk_Click()));
	
	connect(comboProxyType, SIGNAL(currentIndexChanged(QString)), this, SLOT(comboProxyType_indexChanged(QString)));
	
	//Installing event filters for get buttuns
	cmdGetWineBin->installEventFilter(this);
	cmdGetWineServerBin->installEventFilter(this);
	cmdGetWineLoaderBin->installEventFilter(this);
	cmdGetWineLibs->installEventFilter(this);
	cmdGetTarBin->installEventFilter(this);
	cmdGetMountBin->installEventFilter(this);
	cmdGetUmountBin->installEventFilter(this);
	cmdGetSudoBin->installEventFilter(this);
	cmdGetNiceBin->installEventFilter(this);
	cmdGetReniceBin->installEventFilter(this);
	cmdGetConsoleBin->installEventFilter(this);
	cmdGetWrestoolBin->installEventFilter(this);
	cmdGetIcotoolBin->installEventFilter(this);
	cmdGetShBin->installEventFilter(this);
	
	QSettings settings(APP_SHORT_NAME, "default");
	
	settings.beginGroup("wine");
		txtWineBin->setText(settings.value("WineBin").toString());
		txtWineServerBin->setText(settings.value("ServerBin").toString());
		txtWineLoaderBin->setText(settings.value("LoaderBin").toString());
		txtWineLibs->setText(settings.value("WineLibs").toString());
	settings.endGroup();
	
	settings.beginGroup("app");
	if (settings.value("showTrareyIcon").toInt()==1){
		chShowTrarey->setCheckState(Qt::Checked);
	} else {
		chShowTrarey->setCheckState(Qt::Unchecked);
	}
	
	
	listThemesView->clear();
	
	QListWidgetItem *iconItem;
	iconItem = new QListWidgetItem(listThemesView, 0);
	iconItem->setText("Default [Aughtor: Xavier Corredor Llano (xavier.corredor.llano@gmail.com); License: GPL v.2.1]");
	iconItem->setIcon(QIcon(":data/wine.png"));
	iconItem->setToolTip("Default");
	listThemesView->setSelectionMode(QAbstractItemView::SingleSelection);
	
	
	
	QString themeDir;
	themeDir.clear();
	themeDir.append(QDir::homePath());
	themeDir.append("/.config/");
	themeDir.append(APP_SHORT_NAME);
	themeDir.append("/theme/");

	
	getThemes(settings.value("theme").toString(), themeDir);
	
	loadThemeIcons(settings.value("theme").toString());
	
	if (settings.value("theme").toString()=="Default"){
		listThemesView->setCurrentItem(iconItem);	
	}
	
	themeDir.clear();
	themeDir.append(APP_PREF);
	themeDir.append("/share/");
	themeDir.append(APP_SHORT_NAME);
	themeDir.append("/theme");
		
	getThemes(settings.value("theme").toString(), themeDir);
	
	getLangs();
	
	comboLangs->setCurrentIndex(comboLangs->findText(settings.value("lang").toString()));
	
	settings.endGroup();	
	
	settings.beginGroup("system");
		txtTarBin->setText(settings.value("tar").toString());
		txtMountBin->setText(settings.value("mount").toString());
		txtUmountBin->setText(settings.value("umount").toString());
		txtSudoBin->setText(settings.value("sudo").toString());
		txtNiceBin->setText(settings.value("nice").toString());
		txtReniceBin->setText(settings.value("renice").toString());
		txtShBin->setText(settings.value("sh").toString());
	settings.endGroup();
	
	settings.beginGroup("console");
		txtConsoleBin->setText(settings.value("bin").toString());
		txtConsoleArgs->setText(settings.value("args").toString());
	settings.endGroup();
	
	settings.beginGroup("icotool");
		txtWrestoolBin->setText(settings.value("wrestool").toString());
		txtIcotoolBin->setText(settings.value("icotool").toString());
	settings.endGroup();
	
	settings.beginGroup("network");
		txtProxyHost->setText(settings.value("host").toString());
		txtProxyPort->setText(settings.value("post").toString());
		txtProxyUser->setText(settings.value("user").toString());
		txtProxyPass->setText(settings.value("pass").toString());
		
		comboProxyType->setCurrentIndex(settings.value("type").toInt());
	
	settings.endGroup();
	
	return;
}

bool AppSettings::eventFilter(QObject *obj, QEvent *event){
	/*
		function for displaying file\dir dialog
	*/

	if (event->type() == QEvent::MouseButtonPress) {
		
		QString file;
		
		if (obj->objectName().right(3)=="Bin"){
			file = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(),   "All files (*)");
		} else {
			file = QFileDialog::getExistingDirectory(this, tr("Open Directory"), QDir::homePath(),   QFileDialog::DontResolveSymlinks);
		}
			
		if (!file.isEmpty()){
			QString a;
			
			a.append("txt");
			a.append(obj->objectName().right(obj->objectName().length()-6));
			
			QLineEdit *lineEdit = findChild<QLineEdit *>(a);
			
			if (lineEdit){
				lineEdit->setText(file);
			} else {
				qDebug("Error");	
			}
		}
	}
	
	return FALSE;	
}

void AppSettings::comboProxyType_indexChanged(QString text){
	if (text==tr("No Proxy")){
		txtProxyHost->setEnabled(FALSE);
		txtProxyPort->setEnabled(FALSE);
		txtProxyUser->setEnabled(FALSE);
		txtProxyPass->setEnabled(FALSE);
	} else {
		txtProxyHost->setEnabled(TRUE);
		txtProxyPort->setEnabled(TRUE);
		txtProxyUser->setEnabled(TRUE);
		txtProxyPass->setEnabled(TRUE);		
	}
	
	return;
}

void AppSettings::getLangs(){
	
	QString themeDir;
	
	themeDir.clear();
	themeDir.append(APP_PREF);
	themeDir.append("/share/");
	themeDir.append(APP_SHORT_NAME);
	themeDir.append("/i18n");
	
	QDir tmp(themeDir);
	tmp.setFilter(QDir::Files | QDir::NoSymLinks);
	
	QFileInfoList list = tmp.entryInfoList();
		
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		if (fileInfo.fileName().right(2)=="qm")
			comboLangs->addItem(fileInfo.fileName());
	}
	
	return;
}

void AppSettings::getThemes(QString selTheme, QString themeDir){
	//Getting installed themes
	
	QString aughtor;
	QString license;
	
	QDir tmp(themeDir);
 	tmp.setFilter(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
	QFileInfoList list = tmp.entryInfoList();
		
	QListWidgetItem *iconItem;
	
	// Getting converted icons from temp directory
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		iconItem = new QListWidgetItem(listThemesView, 0);
		
		
		QFile file(tr("%1/%2/theme.info").arg(themeDir).arg(fileInfo.fileName()));
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
			aughtor = file.readLine();
			aughtor.remove ( "\n" );
			license = file.readLine();
		}
		
		if ((!aughtor.isEmpty()) and (!license.isEmpty())){
			iconItem->setText(tr("%1 [%2; %3]").arg(fileInfo.fileName()).arg(aughtor).arg(license));
		} else {
			iconItem->setText(fileInfo.fileName());
		}
		iconItem->setToolTip(tr("%1/%2").arg(themeDir).arg(fileInfo.fileName()));
		iconItem->setIcon(QIcon(tr("%1/%2/data/wine.png").arg(themeDir).arg(fileInfo.fileName())));
		
		if (!selTheme.isNull()){
			if (selTheme==fileInfo.filePath()){
				
				//Setting selection to selected theme
				listThemesView->setCurrentItem(iconItem);
				
				//Loading pixmaps from theme
				loadThemeIcons(fileInfo.filePath());
			}
		}
	}
	
	return;
}


void AppSettings::loadThemeIcons(QString themePath){
	QPixmap pixmap;
				
	if (!pixmap.load(tr("%1/data/exec.png").arg(themePath))){
		pixmap.load(":data/exec.png");
	}
				
	lblLogo->setPixmap(pixmap);
	
	
	cmdGetWineBin->setIcon(loadIcon("data/folder.png", themePath));
	cmdGetWineServerBin->setIcon(loadIcon("data/folder.png", themePath));
	cmdGetWineLoaderBin->setIcon(loadIcon("data/folder.png", themePath));
	cmdGetWineLibs->setIcon(loadIcon("data/folder.png", themePath));
	
	cmdGetTarBin->setIcon(loadIcon("data/folder.png", themePath));
	cmdGetMountBin->setIcon(loadIcon("data/folder.png", themePath));
	cmdGetUmountBin->setIcon(loadIcon("data/folder.png", themePath));
	cmdGetSudoBin->setIcon(loadIcon("data/folder.png", themePath));
	cmdGetNiceBin->setIcon(loadIcon("data/folder.png", themePath));
	cmdGetReniceBin->setIcon(loadIcon("data/folder.png", themePath));
	cmdGetShBin->setIcon(loadIcon("data/folder.png", themePath));
	
	cmdGetConsoleBin->setIcon(loadIcon("data/folder.png", themePath));
	cmdGetWrestoolBin->setIcon(loadIcon("data/folder.png", themePath));
	cmdGetIcotoolBin->setIcon(loadIcon("data/folder.png", themePath));
 	
	return;
}


QIcon AppSettings::loadIcon(QString iconName, QString themePath){
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


void AppSettings::cmdCancel_Click(){
	reject();
	return;	
}

void AppSettings::cmdOk_Click(){
	
	if (!checkEntry(txtWineBin->text(), "wine"))
		return;
					
	if (!checkEntry(txtWineServerBin->text(), "wine server"))
		return;
					
	if (!checkEntry(txtWineLoaderBin->text(), "wine loader"))
		return;

	if (!checkEntry(txtWineLibs->text(), "wine library", FALSE))
		return;
	
	if (!checkEntry(txtTarBin->text(), "tar"))
		return;
					
	if (!checkEntry(txtMountBin->text(), "mount"))
		return;

	if (!checkEntry(txtUmountBin->text(), "umount"))
		return;

	if (!checkEntry(txtSudoBin->text(), "sudo"))
		return;
					
	if (!checkEntry(txtUmountBin->text(), "nice"))
		return;

	if (!checkEntry(txtUmountBin->text(), "renice"))
		return;

	if (!checkEntry(txtUmountBin->text(), "sh"))
		return;
	
	if (!checkEntry(txtConsoleBin->text(), "console"))
		return;
					
	if (!checkEntry(txtWrestoolBin->text(), "wrestool"))
		return;

	if (!checkEntry(txtIcotoolBin->text(), "icotool"))
		return;
	
	if (comboProxyType->currentText()!=tr("No Proxy")){
		if (txtProxyHost->text().isEmpty()){
			QMessageBox::warning(this, tr("Error"), tr("Sorry, specify proxy host."));
			return;
		}
		if (txtProxyPort->text().isEmpty()){
			QMessageBox::warning(this, tr("Error"), tr("Sorry, specify proxy port."));
			return;
		}
	}
	
	QSettings settings(APP_SHORT_NAME, "default");
	
	settings.beginGroup("wine");
		settings.setValue("WineBin", txtWineBin->text());
		settings.setValue("ServerBin", txtWineServerBin->text());
		settings.setValue("LoaderBin", txtWineLoaderBin->text());
		settings.setValue("WineLibs", txtWineLibs->text());
	settings.endGroup();
	
	settings.beginGroup("app");
	if (chShowTrarey->checkState()==Qt::Checked) {
		settings.setValue("showTrareyIcon", 1);
	} else {
		settings.setValue("showTrareyIcon", 0);
	}
		
	if (listThemesView->currentItem()){
		settings.setValue("theme", listThemesView->currentItem()->toolTip());
	} else {
		settings.setValue("theme", "Default");
	}
		
	settings.setValue("lang", comboLangs->currentText());
	
	settings.endGroup();
	settings.beginGroup("system");
		settings.setValue("tar", txtTarBin->text());
		settings.setValue("mount", txtMountBin->text());
		settings.setValue("umount", txtUmountBin->text());
		settings.setValue("sudo", txtSudoBin->text());
		settings.setValue("nice", txtNiceBin->text());
		settings.setValue("renice", txtReniceBin->text());
		settings.setValue("sh", txtShBin->text());
	settings.endGroup();
	settings.beginGroup("console");
		settings.setValue("bin", txtConsoleBin->text());
		settings.setValue("args", txtConsoleArgs->text());
	settings.endGroup();
	settings.beginGroup("icotool");
		settings.setValue("wrestool", txtWrestoolBin->text());
		settings.setValue("icotool", txtIcotoolBin->text());
	settings.endGroup();
	settings.beginGroup("network");
		settings.setValue("host", txtProxyHost->text());
		settings.setValue("post", txtProxyPort->text());
		settings.setValue("user", txtProxyUser->text());
		settings.setValue("pass", txtProxyPass->text());
		if (comboProxyType->currentText()==tr("No Proxy")){
			settings.setValue("type", 0);
		} else {
			if (comboProxyType->currentText()=="HTTP"){
				settings.setValue("type", 1);
			} else {
				settings.setValue("type", 2);
			}
		}
		
	settings.endGroup();
	accept();
	return;	
}

bool AppSettings::checkEntry(QString fileName, QString info, bool isFile){
	/*
	*	This function check user entry
	*/
	
	if (fileName.isEmpty()){
		switch (isFile){
			case FALSE:
				QMessageBox::warning(this, tr("Error"), tr("Sorry, specify %1 directory.").arg(info));
				break;
			case TRUE:
				QMessageBox::warning(this, tr("Error"), tr("Sorry, specify %1 binary.").arg(info));
				break;
		}
		return FALSE;
	} else {
		if (!QFile::exists(fileName)){
			switch (isFile){
				case FALSE:
					QMessageBox::warning(this, tr("Error"), tr("Sorry, specified %1 directory not exists.").arg(info));
					break;
				case TRUE:
					QMessageBox::warning(this, tr("Error"), tr("Sorry, specified %1 binary not exists.").arg(info));
					break;
			}
			return FALSE;
		}
	}
	
	return TRUE;
}

