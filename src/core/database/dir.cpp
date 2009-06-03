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

#include "dir.h"

Dir::Dir()
{
  	this->_TABLE="dir";
}

QList<QStringList> Dir::getFieldsByPrefixId(const QString prefixId) const{
	QList<QStringList> valuelist;

	QString sqlquery="SELECT id, name, prefix_id FROM dir WHERE prefix_id=";
	sqlquery.append(prefixId);

	QSqlQuery query;
	if (query.exec(sqlquery)){
		while (query.next()) {
			QStringList values;
			values.append(query.value(0).toString());
			values.append(query.value(1).toString());
			values.append(query.value(2).toString());
			valuelist.append(values);
		}
	} else {
		qDebug()<<"SqlError: "<<query.lastError();
		valuelist.clear();
	}
	return valuelist;
}
