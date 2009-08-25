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

#include "last_run_icon.h"

Last_Run_Icon::Last_Run_Icon()
{
	this->_TABLE="last_run_icon";
}

/*
QList<QStringList> Last_Run_Icon::getIconsInfo(const QString prefix_id, const QString dir_id) const{
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

QList<QStringList> Last_Run_Icon::getIconsInfo(const QString prefix_id) const{
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

QList<QStringList> Last_Run_Icon::getIconsInfoByNames(const QString prefix_name, const QString dir_name, const QString icon_name) const{
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

QList<QStringList> Last_Run_Icon::getByPrefixAndDirName(const QString prefix_name, const QString dir_name) const{
	QList<QStringList> valuelist;
	QSqlQuery query;
	//                         0   1     2     3          4       5         6          7           8        9        10    11       12    13         14
	if (dir_name.isEmpty()){
		query.prepare("SELECT id, name, desc, icon_path, wrkdir, override, winedebug, useconsole, display, cmdargs, exec, desktop, nice, prefix_id, dir_id FROM icon WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name) AND dir_id ISNULL");
	} else {
		query.prepare("SELECT id, name, desc, icon_path, wrkdir, override, winedebug, useconsole, display, cmdargs, exec, desktop, nice, prefix_id, dir_id FROM icon WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name) AND dir_id=(SELECT id FROM dir WHERE name=:dir_name AND prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name1))");
		query.bindValue(":dir_name", dir_name);
		query.bindValue(":prefix_name1", prefix_name);
	}
	query.bindValue(":prefix_name", prefix_name);



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
*/

QStringList Last_Run_Icon::getByExec(const QString exec) const{
	QStringList valuelist;
	QSqlQuery query;
	//                    0       1         2          3           4        5        6        7
	query.prepare("SELECT wrkdir, override, winedebug, useconsole, display, cmdargs, desktop, nice FROM last_run_icon WHERE exec=:exec");
	query.bindValue(":exec", exec);

	if (query.exec()){
		query.first();
			//QStringList values;
			int i=0;
			while (query.value(i).isValid()){
				valuelist.append(query.value(i).toString());
				i++;
			}
	} else {
		qDebug()<<"SqlError: "<<query.lastError()<<query.executedQuery();
		valuelist.clear();
	}

	return valuelist;
}

QStringList Last_Run_Icon::getIcons(){
	QStringList list;
	QSqlQuery query;
	query.prepare("SELECT exec FROM last_run_icon ORDER BY id DESC");

	if (query.exec()){
		int i=0;
		while (query.next()) {
			i++;
			list.append(query.value(0).toString());
		}
	} else {
		qDebug()<<"SqlError: "<<query.lastError()<<query.executedQuery();
		list.clear();
	}


	return list;
}

bool Last_Run_Icon::addIcon(const QString cmdargs, const QString exec, QString override, const QString winedebug, const QString useconsole, const QString display, const QString wrkdir, const QString desktop, const int nice) const{
	QSqlQuery query;
	query.prepare("DELETE FROM last_run_icon WHERE exec=:exec");
	query.bindValue(":exec", exec);

	if (!query.exec()){
		qDebug()<<"SqlError: "<<query.lastError()<<query.executedQuery();
		return false;
	}

	query.prepare("INSERT INTO last_run_icon(override, winedebug, useconsole, display, cmdargs, exec, wrkdir, desktop, nice) VALUES(:override, :winedebug, :useconsole, :display, :cmdargs, :exec, :wrkdir, :desktop, :nice);");


	if (cmdargs.isEmpty()){
		query.bindValue(":cmdargs", QVariant(QVariant::String));
	} else {
		query.bindValue(":cmdargs", cmdargs);
	}

	if (exec.isEmpty()){
		query.bindValue(":exec", QVariant(QVariant::String));
	} else {
		query.bindValue(":exec", exec);
	}

	if (override.isEmpty()){
		query.bindValue(":override", QVariant(QVariant::String));
	} else {
		query.bindValue(":override", override);
	}

	if (winedebug.isEmpty()){
		query.bindValue(":winedebug", QVariant(QVariant::String));
	} else {
		query.bindValue(":winedebug", winedebug);
	}

	if (useconsole.isEmpty()){
		query.bindValue(":useconsole", QVariant(QVariant::String));
	} else {
		query.bindValue(":useconsole", useconsole);
	}

	if (display.isEmpty()){
		query.bindValue(":display", QVariant(QVariant::String));
	} else {
		query.bindValue(":display", display);
	}

	if (wrkdir.isEmpty()){
		query.bindValue(":wrkdir", QVariant(QVariant::String));
	} else {
		query.bindValue(":wrkdir", wrkdir);
	}

	if (desktop.isEmpty()){
		query.bindValue(":desktop", QVariant(QVariant::String));
	} else {
		query.bindValue(":desktop", desktop);
	}

	query.bindValue(":nice", nice);

	if (!query.exec()){
		qDebug()<<"SqlError: "<<query.lastError()<<query.executedQuery();
		return false;
	}

	query.prepare("SELECT COUNT(id) FROM last_run_icon");
	if (!query.exec()){
		qDebug()<<"SqlError: "<<query.lastError()<<query.executedQuery();
		return false;
	}

	query.first();
	if (query.isValid()){
		if (query.value(0).toInt()>8){
			query.prepare("DELETE FROM last_run_icon WHERE id=(SELECT MIN(id) FROM last_run_icon)");

			if (!query.exec()){
				qDebug()<<"SqlError: "<<query.lastError()<<query.executedQuery();
				return false;
			}
		}
	}

	return true;
}

