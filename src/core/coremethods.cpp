/***************************************************************************
 *   Copyright (C) 2009 by Pavel Zinin (pashazz)                           *
 *   pzinin@gmail.com                                                      *
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

#include "coremethods.h"

CoreMethods::CoreMethods()
{
    ;
}

QString CoreMethods::getWhichOut(QString fileName){
	/*
	 * Getting 'whitch' output;
	 */

	proc = new QProcess(this);

	QStringList args;

	args<<fileName;

	proc->setWorkingDirectory (QDir::homePath());
	proc->start("/usr/bin/which", args, QIODevice::ReadOnly);
	proc->waitForFinished();

	QString string = proc->readAllStandardOutput();
	delete proc;

	if (!string.isEmpty()){
		return string.trimmed();
	}

	return "";
}

QString CoreMethods::getPrefixPath(QString prefixName, bool check_exists){
	/*
	 * Getting prefix path by its name
	 */

	//Check if FakeDive exists?
	QString prefixPath;
	QSqlQuery query;
	query.prepare("SELECT path FROM prefix WHERE name=:name");
	query.bindValue(":name", prefixName);
	if (!query.exec()){
		qDebug()<<"SQL Error: CoreMethods::getPrefixPath"<<query.executedQuery()<<"\n"<<query.lastError();
		query.clear();
		return "";
	}
	query.first();
	prefixPath = query.value(0).toString();
	query.clear();

	if (prefixPath.isEmpty()){
		prefixPath = QDir::homePath();
		prefixPath.append("/.wine");
	}

	if (check_exists){
		QDir fake_dir (prefixPath);
		if (!fake_dir.exists()){
			QMessageBox::warning(0, tr("Error"), tr("Sorry, specified prefix %1 directory not exists.").arg(prefixPath), QMessageBox::Ok);
				return "";
		}
	}

	return prefixPath;
}

QString CoreMethods::getSettingValue(QString group, QString key){
	/*
	 * Getting setting key value
	 */

	QString value;

	QSettings settings(APP_SHORT_NAME, "default");

	settings.beginGroup(group);
		value = settings.value(key).toString();
	settings.endGroup();

	return value;
}

WisItem  CoreMethods::getWisInfo(QString fileName) {
	WisItem obj; //to return

	QFile file (fileName);
	if (!file.open(QIODevice::ReadOnly)) {
		qDebug()<<tr ("cannot open file ") + file.fileName() + tr (" because ") + file.errorString();
		obj.error = true;
		return obj;
	}

	QXmlStreamReader reader (&file);
		
        reader.readNext(); //startDocument
	qDebug() << "first" << reader.tokenString(); //it is
        reader.readNext(); //q4wine_pack
        qDebug() << "q4wine_pack";

	while (!reader.atEnd()) {
		if (reader.hasError()) {
			qDebug () << tr ("File format error!");
			break;
		}
				qDebug()<<reader.readNext();
				if (!reader.isWhitespace()){
					QString name, text;
					name = reader.name().toString();
					text = reader.readElementText().trimmed();

					
					qDebug()<< tr ("Token detected:  '%1' contains '%2'").arg(name).arg(text);
					
					if (name == "q4wine_pack"){
						qDebug () << tr ("Q4wine package");
					}

					if (name == "name"){
						qDebug () << tr ("name");
						obj.name = text;
					}

					if (name  == "author"){
						qDebug () << tr ("author ") + text ;
						obj.author = text;
					}
					if (name == "description"){
						qDebug () << tr ("description of package");
						obj.description = text;
					}

					if (name == "download"){
						qDebug () << tr (" download URL");
						obj.download = text;
					}

					if (name == "site"){
						qDebug () << tr ("site");
						obj.site = text;
					}

					if (name == "install"){
						qDebug () << tr ("install path");
						obj.install = text;
					}

					if (name == "contact"){
						qDebug () << tr ("contact info");
						obj.contact = text;
					}
				}
	}
	file.close();
	return obj;
}


