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

#include "prefixsettings.h"

PrefixSettings::PrefixSettings(QString prefix_name, QWidget * parent, Qt::WFlags f) : QDialog(parent, f)
{
	setupUi(this);
	
	this->prefix_name=prefix_name;
	
	QSettings settings(APP_NAME, "default");
	settings.beginGroup("app");
		loadThemeIcons(settings.value("theme").toString());
	settings.endGroup();	
	
	QSqlQuery query;
	query.prepare("select id, wine_dllpath, wine_loader, wine_server, wine_exec, cdrom_mount, cdrom_drive, name, path from prefix where name=:name");
	query.bindValue(":name", prefix_name);
	if (!query.exec()){
		#ifdef DEBUG
			qDebug()<<"WARNING: SQL error at MainWindow::PrefixSettings()\nINFO:\n"<<query.executedQuery()<<"\n"<<query.lastError();
		#endif
		return;
	}
	query.first();
	
	if (query.value(0).toString().isNull())
		return;
	
	prefix_id=query.value(0).toString();
	
	txtWineLibs->setText(query.value(1).toString());
	txtWineLoaderBin->setText(query.value(2).toString());
	txtWineServerBin->setText(query.value(3).toString());
	txtWineBin->setText(query.value(4).toString());
		
	//comboDeviceList
	txtMountPoint->setText(query.value(5).toString());
	
	if (prefix_name=="Default"){
		txtPrefixName->setEnabled(FALSE);
		txtPrefixPath->setEnabled(FALSE);
		cmdGetPrefixPath->setEnabled(FALSE);
	} else {
		cmdGetPrefixPath->installEventFilter(this);	
	}
	
	txtPrefixName->setText(query.value(7).toString());
	txtPrefixPath->setText(query.value(8).toString());
	
	getprocDevices();
	
	comboDeviceList->setCurrentIndex (comboDeviceList->findText(query.value(6).toString()));
	
	connect(cmdCancel, SIGNAL(clicked()), this, SLOT(cmdCancel_Click()));
	connect(cmdOk, SIGNAL(clicked()), this, SLOT(cmdOk_Click()));
	
	cmdGetWineBin->installEventFilter(this);
	cmdGetWineServerBin->installEventFilter(this);
	cmdGetWineLoaderBin->installEventFilter(this);
	cmdGetWineLibs->installEventFilter(this);
	
	cmdGetMountPoint->installEventFilter(this);
	
	return;
}



void PrefixSettings::loadThemeIcons(QString themePath){
	QPixmap pixmap;
				
	if (!pixmap.load(tr("%1/data/exec.png").arg(themePath))){
		pixmap.load(":data/exec.png");
	}
				
	lblLogo->setPixmap(pixmap);
		
	cmdGetWineBin->setIcon(loadIcon("data/folder.png", themePath));
	cmdGetWineServerBin->setIcon(loadIcon("data/folder.png", themePath));
	cmdGetWineLoaderBin->setIcon(loadIcon("data/folder.png", themePath));
	cmdGetWineLibs->setIcon(loadIcon("data/folder.png", themePath));

	cmdGetMountPoint->setIcon(loadIcon("data/folder.png", themePath));
	cmdGetPrefixPath->setIcon(loadIcon("data/folder.png", themePath));
		
	return;
}


QIcon PrefixSettings::loadIcon(QString iconName, QString themePath){
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

void PrefixSettings::getprocDevices(){
	/*
		Getting divice names at /proc/diskstats
	*/

	QString name, procstat, path, prefix;;
		
	QFile file("/etc/fstab");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		QMessageBox::warning(this, tr("Error"), tr("Sorry, i can't access to /etc/fstab"));
	}
		
	//int indOf;
	comboDeviceList->addItem(tr("<none>"));

	while (1) {
		QByteArray line = file.readLine();
		
		if (line.isEmpty())
			break;
		
		if (line.indexOf("cdrom")>=0){
			QList<QByteArray> array = line.split(' ').at(0).split('\t');
				comboDeviceList->addItem(array.at(0));
		}
	}
	file.close();
	
	return;
}

void PrefixSettings::cmdCancel_Click(){
	reject();
	return;	
}

void PrefixSettings::cmdOk_Click(){
	QSqlQuery query;
	
	if (prefix_name!=txtPrefixName->text()){
	
		query.prepare("select id from prefix where name=:name");
		query.bindValue(":name", txtPrefixName->text());
		query.exec();
		query.first();
		
		if (query.isValid ()){
			QMessageBox::warning(this, tr("Error"), tr("Sorry, but prefix named %1 already exists.").arg(txtPrefixName->text()));
			query.clear();
			return;
		}
		
		query.clear();
	
	}
	
	query.prepare("UPDATE prefix SET wine_dllpath=:wine_dllpath, wine_loader=:wine_loader, wine_server=:wine_server, wine_exec=:wine_exec, cdrom_mount=:cdrom_mount, cdrom_drive=:cdrom_drive, name=:name, path=:path WHERE id=:id;");
	query.bindValue(":wine_dllpath", txtWineLibs->text());
	query.bindValue(":wine_loader", txtWineLoaderBin->text());
	query.bindValue(":wine_server", txtWineServerBin->text());
	query.bindValue(":wine_exec", txtWineBin->text());

	if (txtMountPoint->text().endsWith ("/"))
		txtMountPoint->setText(txtMountPoint->text().mid(0, txtMountPoint->text().length()-1));

	query.bindValue(":cdrom_mount", txtMountPoint->text());
	if (comboDeviceList->currentText()!=tr("<none>")){
		query.bindValue(":cdrom_drive", comboDeviceList->currentText());
	} else {
		query.bindValue(":cdrom_drive", QVariant(QVariant::String));
	}
	query.bindValue(":name", txtPrefixName->text());
	query.bindValue(":path", txtPrefixPath->text());
	query.bindValue(":id", prefix_id);
	
	if (!query.exec()){
		#ifdef DEBUG
			qDebug()<<"WARNING: SQL error at MainWindow::PrefixSettings::cmdOd_Click()\nINFO:\n"<<query.executedQuery()<<"\n"<<query.lastError();
		#endif
		return;
	}
	
	query.clear();
	
	accept();
	return;
}

bool PrefixSettings::eventFilter(QObject *obj, QEvent *event){
	/*
	 * Select folder dialog
	 */

	if (event->type() == QEvent::MouseButtonPress) {
		
		QString file;
		
		if (obj->objectName().right(3)=="Bin"){
			file = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(),   "All files (*.*)");
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


