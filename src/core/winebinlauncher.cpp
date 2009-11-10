/***************************************************************************
 *   Copyright (C) 2008,2009 by Malakhov Alexey                            *
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

#include "winebinlauncher.h"

WineBinLauncher::WineBinLauncher(QString prefix_name, QWidget * parent, Qt::WFlags f): QDialog(parent, f){
	// Getting prefix settings by prefix_name
	setupUi(this);

	this->setWindowTitle (tr("Please wait"));

	QSqlQuery query;

	query.prepare("SELECT path, wine_dllpath, wine_loader, wine_exec, wine_server FROM prefix WHERE name=:prefix_name");
	query.bindValue(":prefix_name", prefix_name);
	if (!query.exec()){
		qDebug()<<"Can't get prefix info at Registry constructor"<<query.lastError();
		return;
	}
	query.first();

	this->prefix.path = query.value(0).toString();
	this->prefix.wine_dllpath = query.value(1).toString();
	this->prefix.wine_loader = query.value(2).toString();
	this->prefix.wine_exec = query.value(3).toString();
	this->prefix.wine_server = query.value(4).toString();

	query.clear();

	// Getting default patchs
	QSettings settings(APP_SHORT_NAME, "default");

	settings.beginGroup("wine");
		DEFAULT_WINE_BIN=settings.value("WineBin").toString();
		DEFAULT_WINE_SERVER=settings.value("ServerBin").toString();
		DEFAULT_WINE_LOADER=settings.value("LoaderBin").toString();
		DEFAULT_WINE_LIBS=settings.value("WineLibs").toString();
	settings.endGroup();

	settings.beginGroup("system");
		SH_BIN=settings.value("sh").toString();
	settings.endGroup();


	myProcess = new QProcess(parent);

	connect(myProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotFinished(int, QProcess::ExitStatus)));

	connect(cmdCancel, SIGNAL(clicked()), this, SLOT(cmdCancel_clicked()));

	return;
}

void WineBinLauncher::appendWineExe(QString wine_append){
	//This is neaded for owerride /usr/bin/wine to /usr/bin/wineprefixcreate (for example)
	WINE_APPEND = wine_append;
	return;
}

void WineBinLauncher::run_exec(QObject *parent, QString exe, QString exe_args, bool KeepRunning){

	// Exec an a wine binary
	QStringList args;
	QString envargs;

	// qDebug()<<exe<<exe_args;

	lblInfo->setText(tr("Running: %1 %2").arg(WINE_APPEND).arg(exe_args));

	args.append("-c");

	if (!this->prefix.path.isEmpty()){
		envargs.append(QString(" WINEPREFIX=%1 ").arg(this->prefix.path));
	} else {
		envargs.append(QString(" WINEPREFIX=%1/.wine ").arg(QDir::homePath()));
	}

	if (!this->prefix.wine_dllpath.isEmpty()){
		envargs.append(QString(" WINEDLLPATH=%1 ").arg(this->prefix.wine_dllpath));
	} else {
		envargs.append(QString(" WINEDLLPATH=%1 ").arg(DEFAULT_WINE_LIBS));
	}

	if (!this->prefix.wine_loader.isEmpty()){
		envargs.append(QString(" WINELOADER=%1 ").arg(this->prefix.wine_loader));
	} else {
		envargs.append(QString(" WINELOADER=%1 ").arg(DEFAULT_WINE_LOADER));
	}

	if (!this->prefix.wine_server.isEmpty()){
		envargs.append(QString(" WINESERVER=%1 ").arg(this->prefix.wine_server));
	} else {
		envargs.append(QString(" WINESERVER=%1 ").arg(DEFAULT_WINE_SERVER));
	}

	if (!this->prefix.wine_exec.isEmpty()){
		envargs.append(this->prefix.wine_exec);
	} else {
		envargs.append(DEFAULT_WINE_BIN);
	}

	if (!WINE_APPEND.isEmpty()){
		envargs.append(WINE_APPEND);
	}

	envargs.append(" ");
	envargs.append(exe);
	envargs.append(" ");
	envargs.append(exe_args);

	args.append(envargs);

	myProcess->setWorkingDirectory(QDir::homePath());
	myProcess->start( SH_BIN, args);

	return;

}

void WineBinLauncher::cmdCancel_clicked(void){
	myProcess->kill();
	reject();
	return;
}

void WineBinLauncher::slotFinished(int, QProcess::ExitStatus){

	if (myProcess->exitCode()!=0){

		QString lang;
		// Getting env LANG variable
		lang = getenv("LANG");
		lang = lang.split(".").at(1);

		if (lang.isNull())
			lang = "UTF8";

		QTextCodec *codec = QTextCodec::codecForName(lang.toAscii());
		QString string = codec->toUnicode(myProcess->readAllStandardError());
		if (!string.isEmpty()){
			QMessageBox::warning(this, tr("Error"), tr("It seems procces fail.<br><br>Error log:<br>%1").arg(string));
		} else {
			QMessageBox::warning(this, tr("Error"), tr("It seems procces fail.<br><br>Cant read STDERR message.<br>%1").arg(string));

		}
		reject ();
		return;
	}
	accept();
	return;
}


