/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010 by Malakhov Alexey                           *
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

#include "image.h"

Image::Image()
{

}

QList<QStringList> Image::getFields(void) const{
	QList<QStringList> valuelist;
	//                      0     1
	QSqlQuery query("SELECT name, path FROM images ORDER BY name");
	if (query.exec()){
		while (query.next()) {
			QStringList values;
			int i=0;
			while (query.value(i).isValid()){
				values.append(query.value(i).toString());
				i++;
			}
			valuelist.append(values);
		}
	} else {
		qDebug()<<"SqlError: "<<query.lastError();
	}
	return valuelist;
}


QString Image::getPath(const QString name) const{
	QString result;
	QSqlQuery query("SELECT path FROM images WHERE name=:name ORDER BY name");
	query.bindValue(":name", name);
	if (query.exec()){
		query.next();
		result = query.value(0).toString();
	} else {
		qDebug()<<"SqlError: "<<query.lastError();
	}
	return result;
}

bool Image::isExistsByName(const QString name) const{
	QSqlQuery query;
	query.prepare("SELECT id FROM images WHERE name=:name");
	query.bindValue(":name", name);
	if (!query.exec()){
		qDebug()<<"SqlError: "<<query.lastError()<<query.executedQuery();
		return false;
	}
	query.first();
	if (query.isValid()){
		return true;
	}
	return false;
}

bool Image::addImage(const QString name, const QString path) const{
  	QSqlQuery query;
	query.prepare("INSERT INTO images(name, path) VALUES(:name, :path)");
	query.bindValue("name", name);
	query.bindValue("path", path);

	if (!query.exec()){
		qDebug()<<"SqlError: "<<query.lastError()<<query.executedQuery();
		return false;
	}
	return true;
}

bool Image::renameImage(const QString name, const QString old_name) const{
  	QSqlQuery query;
	query.prepare("UPDATE images SET name=:name WHERE name=:old_name");
	query.bindValue("name", name);
	query.bindValue("old_name", old_name);

	if (!query.exec()){
		qDebug()<<"SqlError: "<<query.lastError()<<query.executedQuery();
		return false;
	}
	return true;
}

bool Image::delImage(const QString name) const{
  	QSqlQuery query;
	query.prepare("DELETE FROM images WHERE name=:name");
	query.bindValue("name", name);

	if (!query.exec()){
		qDebug()<<"SqlError: "<<query.lastError()<<query.executedQuery();
		return false;
	}
	return true;
}

