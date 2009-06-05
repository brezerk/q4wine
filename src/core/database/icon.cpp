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

#include "icon.h"

Icon::Icon()
{
  	this->_TABLE="icon";
}

QList<QStringList> Icon::getIconsInfo(const QString prefix_id, const QString dir_id) const{
	QList<QStringList> valuelist;
	QSqlQuery query;
	//                    0   1     2     3          4       5         6          7           8        9        10    11       12
	query.prepare("SELECT id, name, desc, icon_path, wrkdir, override, winedebug, useconsole, display, cmdargs, exec, desktop, nice FROM icon WHERE prefix_id=:prefix_id AND dir_id=:dir_id");
	query.bindValue(":prefix_id", prefix_id);
	query.bindValue(":dir_id", dir_id);

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
		valuelist.clear();
	}

	return valuelist;
}

QList<QStringList> Icon::getIconsInfo(const QString prefix_id) const{
	QList<QStringList> valuelist;
	QSqlQuery query;
	//                    0   1     2     3          4       5         6          7           8        9        10    11       12
	query.prepare("SELECT id, name, desc, icon_path, wrkdir, override, winedebug, useconsole, display, cmdargs, exec, desktop, nice FROM icon WHERE prefix_id=:prefix_id AND dir_id ISNULL");
	query.bindValue(":prefix_id", prefix_id);

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
		valuelist.clear();
	}

	return valuelist;
}

QList<QStringList> Icon::getIconsInfoByNames(const QString prefix_name, const QString dir_name, const QString icon_name) const{
	QList<QStringList> valuelist;
	QSqlQuery query;
	//                         0   1     2     3          4       5         6          7           8        9        10    11       12    13         14
	if (icon_name.isEmpty()){
		query.prepare("SELECT id, name, desc, icon_path, wrkdir, override, winedebug, useconsole, display, cmdargs, exec, desktop, nice, prefix_id, dir_id FROM icon WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name) AND dir_id=(SELECT id FROM dir WHERE name=:dir_name AND prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name1))");
	} else {
		query.prepare("SELECT id, name, desc, icon_path, wrkdir, override, winedebug, useconsole, display, cmdargs, exec, desktop, nice, prefix_id, dir_id FROM icon WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name) AND dir_id=(SELECT id FROM dir WHERE name=:dir_name AND prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name1)) AND name=:icon_name");
		query.bindValue(":icon_name", icon_name);
	}
	query.bindValue(":prefix_name", prefix_name);
	query.bindValue(":prefix_name1", prefix_name);
	query.bindValue(":dir_name", dir_name);

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
		qDebug()<<"SqlError: "<<query.lastError()<<query.executedQuery();
		valuelist.clear();
	}

	return valuelist;
}


QList<QStringList> Icon::getIconsInfoByPrefixName(const QString prefix_name, const QString icon_name) const{
	QList<QStringList> valuelist;
	QSqlQuery query;
	//                         0   1     2     3          4       5         6          7           8        9        10    11       12    13         14
	QString sqlquery = "SELECT id, name, desc, icon_path, wrkdir, override, winedebug, useconsole, display, cmdargs, exec, desktop, nice, prefix_id, dir_id FROM icon WHERE prefix_id=(SELECT id FROM prefix WHERE name='";
	sqlquery.append(prefix_name);
	sqlquery.append("' ) AND dir_id ISNULL");
	if (!icon_name.isEmpty()){
	   sqlquery.append(" AND name='");
	   sqlquery.append(icon_name);
	   sqlquery.append("'");
	}

	if (query.exec(sqlquery)){
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
		valuelist.clear();
	}

	return valuelist;
}

bool Icon::delIconsByPrefixName(const QString prefix_name) const{
	QSqlQuery query;
	query.prepare("DELETE FROM icon WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name)");
	query.bindValue(":prefix_name", prefix_name);

	if (!query.exec()){
		qDebug()<<"SqlError: "<<query.lastError()<<query.executedQuery();
		return false;
	}
	return true;
}


bool Icon::delIcon(const QString prefix_name, const QString icon_name) const{
	QSqlQuery query;
	query.prepare("DELETE FROM icon WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name) AND dir_id ISNULL AND name=:icon_name");
	query.bindValue(":prefix_name", prefix_name);
	query.bindValue(":icon_name", icon_name);

	if (!query.exec()){
		qDebug()<<"SqlError: "<<query.lastError()<<query.executedQuery();
		return false;
	}
	return true;
}

bool Icon::delIcon(const QString prefix_name, const QString dir_name, const QString icon_name) const{
	QSqlQuery query;
	query.prepare("DELETE FROM icon WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name) AND dir_id=(SELECT id FROM dir WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name1) AND name=:dir_name) AND name=:icon_name");
	query.bindValue(":prefix_name", prefix_name);
	query.bindValue(":prefix_name1", prefix_name);
	query.bindValue(":dir_name", dir_name);
	query.bindValue(":icon_name", icon_name);

	if (!query.exec()){
		qDebug()<<"SqlError: "<<query.lastError()<<query.executedQuery();
		return false;
	}
	return true;
}

bool Icon::isExistsByName(const QString prefix_name, const QString icon_name) const{
	QSqlQuery query;
	query.prepare("SELECT id FROM icon WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name) AND dir_id ISNULL AND name=:icon_name");
	query.bindValue(":prefix_name", prefix_name);
	query.bindValue(":icon_name", icon_name);

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

bool Icon::isExistsByName(const QString prefix_name, const QString dir_name, const QString icon_name) const{
	QSqlQuery query;
	query.prepare("SELECT id FROM icon WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name) AND dir_id=(SELECT id FROM dir WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name1) AND name=:dir_name) AND name=:icon_name");
	query.bindValue(":prefix_name", prefix_name);
	query.bindValue(":prefix_name1", prefix_name);
	query.bindValue(":dir_name", dir_name);
	query.bindValue(":icon_name", icon_name);

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
