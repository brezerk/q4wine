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

#include "registry.h"

Registry::Registry(){
	return;
}

Registry::Registry(QString prefixPath){
	regfile.append(prefixPath);
	regfile.append("/");
	return;
}

bool Registry::init(){
	regfile_image="WINE Registry Version 2\n\n";
	return TRUE;
}

void Registry::append(QString reg_keys){
	regfile_image.append(reg_keys);
	return;
}

bool Registry::exec(QObject *parent, QString prefix_name){

	//This function wrights regfile_image into file, then run regedit.exe and import this file.
	//Also, clean files before end

	QTime midnight(0, 0, 0);
	qsrand(midnight.secsTo(QTime::currentTime()));

	int file_name = qrand() % 10000;
	QString full_file_path = QObject::tr("%1/.config/%2/tmp/%3.reg").arg(QDir::homePath()).arg(APP_SHORT_NAME).arg(file_name);

	QFile file(full_file_path);
	file.open(QIODevice::WriteOnly);
	file.write(regfile_image.toAscii());        // write to stderr
	file.close();

	WineBinLauncher *launcher;

	launcher = new WineBinLauncher(prefix_name);
	launcher->show();
	launcher->run_exec(parent, "regedit.exe", full_file_path, TRUE);

	if (launcher->exec()==QDialog::Accepted){
		return TRUE;
	} else {
		return FALSE;
	}

}

QStringList Registry::readKeys(const QString sysfile, const QString path, const QStringList keys) const{
	QStringList ret;
	QString searchPath;

	for (int i=0; i<keys.count(); i++){
		ret.append("");
	}

	searchPath="[";
	searchPath.append(path);
	searchPath.append("]");
	searchPath.replace("\\","\\\\");

	QString sfile = regfile;
	sfile.append(sysfile);
	sfile.append(".reg");

	QFile file(sfile);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		qDebug()<<" [EE] Can't open reg file: "<<regfile;
		return ret;
	}

	bool readFlag=false;

	while (!file.atEnd()) {
		QByteArray line = file.readLine();
		if ((line.indexOf(searchPath)>-1) and (!readFlag)){
			readFlag=true;
		}

		if (readFlag){
			QList<QByteArray> key = line.trimmed().split('=');
			int index = keys.indexOf(key.at(0));
			if (index>-1){
				ret.replace(index, key.at(1).mid(1, (key.at(1).length()-2)));
			}
		}

		if (((line=="\n") or (line.isEmpty())) and (readFlag))
			readFlag=false;
	}
	return ret;
}

