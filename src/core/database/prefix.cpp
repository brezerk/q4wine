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

#include "prefix.h"

Prefix::Prefix()
{
	this->_TABLE="prefix";
}

QList<QStringList> Prefix::getFields(const QStringList fields) const{
    QList<QStringList> valuelist;

    if (fields.size()==0){
	valuelist.append(QStringList("-1"));
	return valuelist;
    }

    QString sqlquery="SELECT ";

    for (int i = 0; i < fields.size(); ++i){
	sqlquery.append(fields.at(i));
	if (i < (fields.size() - 1))
	    sqlquery.append(", ");
    }
	sqlquery.append(" FROM ");
	sqlquery.append(this->_TABLE);
	sqlquery.append(" ORDER BY id");

    QSqlQuery query;
    if (query.exec(sqlquery)){
	while (query.next()) {
	    QStringList values;
	    for (int i = 0; i < fields.size(); ++i){
	       values.append(query.value(i).toString());
	    }
	    valuelist.append(values);
	}
    } else {
	qDebug()<<"SqlError: "<<query.lastError();
	valuelist.clear();
	valuelist.append(QStringList("-1"));
	return valuelist;
    }

    return valuelist;
}

QList<QStringList> Prefix::getFields(void) const{
	QList<QStringList> valuelist;

	QString sqlquery="SELECT id, name, path, wine_dllpath, wine_loader, wine_server, wine_exec, cdrom_mount, cdrom_drive FROM prefix ORDER BY id";

	QSqlQuery query;
	if (query.exec(sqlquery)){
	while (query.next()) {
		QStringList values;
		values.append(query.value(0).toString());
		values.append(query.value(1).toString());
		values.append(query.value(2).toString());
		values.append(query.value(3).toString());
		values.append(query.value(4).toString());
		values.append(query.value(5).toString());
		values.append(query.value(6).toString());
		values.append(query.value(7).toString());
		values.append(query.value(8).toString());
		valuelist.append(values);
	}
	} else {
	qDebug()<<"SqlError: "<<query.lastError();
	valuelist.clear();
	valuelist.append(QStringList("-1"));
	return valuelist;
	}

	return valuelist;
}

QList<QStringList> Prefix::getFieldsByKey(const QStringList fields, const QStringList keys,  const QStringList vals) const{
    QList<QStringList> valuelist;

    if ((fields.size()==0) or (keys.size()==0) or (vals.size()==0) or (vals.size()!=keys.size())){
	valuelist.append(QStringList("-1"));
	return valuelist;
    }

    QString sqlquery="SELECT ";

    for (int i = 0; i < fields.size(); ++i){
	sqlquery.append(fields.at(i));
	if (i < (fields.size() - 1))
	    sqlquery.append(", ");
    }

	sqlquery.append(" FROM ");
	sqlquery.append(this->_TABLE);
	sqlquery.append(" WHERE ");

    for (int i = 0; i < keys.size(); ++i){
	sqlquery.append(keys.at(i));
	sqlquery.append("=");
	sqlquery.append(vals.at(i));
	if (i < (keys.size() - 1))
	    sqlquery.append(", ");
    }

    sqlquery.append(" ORDER BY id");

    QSqlQuery query;
    if (query.exec(sqlquery)){
	while (query.next()) {
	    QStringList values;
	    for (int i = 0; i < fields.size(); ++i){
	       values.append(query.value(i).toString());
	    }
	    valuelist.append(values);
	}
    } else {
	qDebug()<<"SqlError: "<<query.lastError();
	valuelist.clear();
	valuelist.append(QStringList("-1"));
	return valuelist;
    }
    return valuelist;
}

QStringList Prefix::getFieldsByPrefixName(const QString prefixName) const{
	QStringList values;

	QString sqlquery="SELECT id, path, wine_dllpath, wine_loader, wine_server, wine_exec, cdrom_mount, cdrom_drive FROM prefix WHERE name='";
	sqlquery.append(prefixName);
	sqlquery.append("' ORDER BY id");

	QSqlQuery query;
	if (query.exec(sqlquery)){
		query.first();
		if (query.isValid()){
			values.append(query.value(0).toString());
			values.append(query.value(1).toString());
			values.append(query.value(2).toString());
			values.append(query.value(3).toString());
			values.append(query.value(4).toString());
			values.append(query.value(5).toString());
			values.append(query.value(6).toString());
			values.append(query.value(7).toString());
		}
	} else {
		qDebug()<<"SqlError: "<<query.lastError();
		values.clear();
	}
	return values;
}

bool Prefix::updateQuery(QSqlQuery *sqlQuery) const{
	if (!sqlQuery->exec()){
		qDebug()<<"SqlError: "<<sqlQuery->lastError();
		return false;
	}
	return true;
}
