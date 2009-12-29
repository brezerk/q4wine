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

QList<QStringList> Icon::getByPrefixAndDirName(const QString prefix_name, const QString dir_name, const QString filter) const{
	QList<QStringList> valuelist;
	QSqlQuery query;

	//                         0   1     2     3          4       5         6          7           8        9        10    11       12    13         14
	if (dir_name.isEmpty()){
		if (filter.isEmpty()){
			query.prepare("SELECT id, name, desc, icon_path, wrkdir, override, winedebug, useconsole, display, cmdargs, exec, desktop, nice, prefix_id, dir_id FROM icon WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name) AND dir_id ISNULL");
		} else {
			query.prepare(QString("SELECT id, name, desc, icon_path, wrkdir, override, winedebug, useconsole, display, cmdargs, exec, desktop, nice, prefix_id, dir_id FROM icon WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name) AND dir_id ISNULL AND name LIKE \"%%1%\"").arg(filter));
		}
	} else {
		if (filter.isEmpty()){
			query.prepare("SELECT id, name, desc, icon_path, wrkdir, override, winedebug, useconsole, display, cmdargs, exec, desktop, nice, prefix_id, dir_id FROM icon WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name) AND dir_id=(SELECT id FROM dir WHERE name=:dir_name AND prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name1))");
		} else {
			query.prepare(QString("SELECT id, name, desc, icon_path, wrkdir, override, winedebug, useconsole, display, cmdargs, exec, desktop, nice, prefix_id, dir_id FROM icon WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name) AND dir_id=(SELECT id FROM dir WHERE name=:dir_name AND prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name1)) AND name LIKE \"%%1%\"").arg(filter));
		}
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


QStringList Icon::getByName(const QString prefix_name, const QString dir_name, const QString icon_name) const{
	QStringList valuelist;
	QSqlQuery query;
	//                         0   1     2     3          4       5         6          7           8        9        10    11       12    13         14
	if (dir_name.isEmpty()){
		query.prepare("SELECT id, name, desc, icon_path, wrkdir, override, winedebug, useconsole, display, cmdargs, exec, desktop, nice, prefix_id, dir_id FROM icon WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name) AND dir_id ISNULL AND name=:icon_name");
	} else {
		query.prepare("SELECT id, name, desc, icon_path, wrkdir, override, winedebug, useconsole, display, cmdargs, exec, desktop, nice, prefix_id, dir_id FROM icon WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name) AND dir_id=(SELECT id FROM dir WHERE name=:dir_name AND prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name1)) AND name=:icon_name");
		query.bindValue(":prefix_name1", prefix_name);
		query.bindValue(":dir_name", dir_name);
	}
	query.bindValue(":icon_name", icon_name);
	query.bindValue(":prefix_name", prefix_name);

	if (query.exec()){
		query.first();
		//QStringList values;
		int i=0;
		while (query.value(i).isValid()){
			valuelist.append(query.value(i).toString());
			i++;
		}
		//valuelist.append(values);
	} else {
		qDebug()<<"SqlError: "<<query.lastError()<<query.executedQuery();
		valuelist.clear();
	}

	//qDebug()<<"stage: "<<valuelist;
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

/*
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
}*/

bool Icon::delIcon(const QString prefix_name, const QString dir_name, const QString icon_name) const{
	QSqlQuery query;
	if (dir_name.isEmpty()){
		query.prepare("DELETE FROM icon WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name) AND dir_id ISNULL AND name=:icon_name");
		query.bindValue(":prefix_name", prefix_name);
		query.bindValue(":icon_name", icon_name);
	} else {
		if (!icon_name.isEmpty()){
			query.prepare("DELETE FROM icon WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name) AND dir_id=(SELECT id FROM dir WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name1) AND name=:dir_name) AND name=:icon_name");
			query.bindValue(":icon_name", icon_name);
		} else {
			query.prepare("DELETE FROM icon WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name) AND dir_id=(SELECT id FROM dir WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name1) AND name=:dir_name)");
		}
		query.bindValue(":prefix_name", prefix_name);
		query.bindValue(":prefix_name1", prefix_name);
		query.bindValue(":dir_name", dir_name);
	}

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
	if (!dir_name.isEmpty()){
		query.prepare("SELECT id FROM icon WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name) AND dir_id=(SELECT id FROM dir WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name1) AND name=:dir_name) AND name=:icon_name");
		query.bindValue(":dir_name", dir_name);
		query.bindValue(":prefix_name1", prefix_name);
	} else {
		query.prepare("SELECT id FROM icon WHERE prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name) AND dir_id ISNULL AND name=:icon_name");
	}
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

bool Icon::addIcon(const QString cmdargs, const QString exec, const QString icon_path, const QString desc, const QString prefix_name, const QString dir_name, const QString name, const QString override, const QString winedebug, const QString useconsole, const QString display, const QString wrkdir, const QString desktop, const int nice) const{
	QSqlQuery query;
	query.prepare("INSERT INTO icon(override, winedebug, useconsole, display, cmdargs, exec, icon_path, desc, dir_id, id, name, prefix_id, wrkdir, desktop, nice) VALUES(:override, :winedebug, :useconsole, :display, :cmdargs, :exec, :icon_path, :desc, (SELECT id FROM dir WHERE name=:dir_name AND prefix_id=(SELECT id FROM prefix WHERE name=:prefix_dir_name)), NULL, :name, (SELECT id FROM prefix WHERE name=:prefix_name), :wrkdir, :desktop, :nice);");

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

	if (icon_path.isEmpty()){
		query.bindValue(":icon_path", QVariant(QVariant::String));
	} else {
		query.bindValue(":icon_path", icon_path);
	}

	if (desc.isEmpty()){
		query.bindValue(":desc", QVariant(QVariant::String));
	} else {
		query.bindValue(":desc", desc);
	}

	query.bindValue(":prefix_name", prefix_name);
	query.bindValue(":prefix_dir_name", prefix_name);

	if (dir_name.isEmpty()){
		query.bindValue(":dir_name", QVariant(QVariant::String));
	} else {
		query.bindValue(":dir_name", dir_name);
	}

	if (name.isEmpty()){
		query.bindValue(":name", QVariant(QVariant::String));
	} else {
		query.bindValue(":name", name);
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

	return true;
}

bool Icon::updateIcon(const QString icon_name, const QString prefix_id, const QString dir_id, const QString old_prefix_id, const QString old_dir_id, const QString old_icon_name) const{
	QSqlQuery query;
	if (!old_dir_id.isEmpty()){
		query.prepare("UPDATE icon SET name=:icon_name, prefix_id=:prefix_id, dir_id=:dir_id WHERE name=:old_icon_name and prefix_id=:old_prefix_id and dir_id=:old_dir_id");
		query.bindValue(":old_dir_id", old_dir_id);
	} else {
		query.prepare("UPDATE icon SET name=:icon_name, prefix_id=:prefix_id, dir_id=:dir_id WHERE name=:old_icon_name and prefix_id=:old_prefix_id and dir_id ISNULL");
	}

	query.bindValue(":icon_name", icon_name);
	query.bindValue(":old_icon_name", old_icon_name);
	query.bindValue(":prefix_id", prefix_id);
	query.bindValue(":dir_id", dir_id);
	query.bindValue(":old_prefix_id", old_prefix_id);

	if (!query.exec()){
		qDebug()<<"SqlError: "<<query.lastError()<<query.executedQuery();
		return false;
	}

	return true;
}

bool Icon::copyIcon(const QString icon_name, const QString prefix_name, const QString dir_name, const QString new_icon_name, const QString new_prefix_name, const QString new_dir_name) const{
	QStringList iconRec = this->getByName(prefix_name, dir_name, icon_name);
	return this->addIcon(iconRec.at(9), iconRec.at(10), iconRec.at(3), iconRec.at(2), new_prefix_name, new_dir_name, new_icon_name, iconRec.at(5), iconRec.at(6), iconRec.at(7), iconRec.at(8), iconRec.at(4), iconRec.at(11), iconRec.at(12).toInt());
}

bool Icon::renameIcon(const QString icon_name, const QString prefix_name, const QString dir_name, const QString new_icon_name) const{
	QSqlQuery query;
	if (!dir_name.isEmpty()){
		query.prepare("UPDATE icon SET name=:new_icon_name WHERE name=:icon_name and dir_id=(SELECT id FROM dir WHERE name=:dir_name AND prefix_id=(SELECT id FROM prefix WHERE name=:prefix_dir_name)) and prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name)");
		query.bindValue(":prefix_dir_name", prefix_name);
		query.bindValue(":dir_name", dir_name);
	} else {
		query.prepare("UPDATE icon SET name=:new_icon_name WHERE name=:icon_name and dir_id ISNULL and prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name)");
	}
	query.bindValue(":prefix_name", prefix_name);
	query.bindValue(":new_icon_name", new_icon_name);
	query.bindValue(":icon_name", icon_name);

	if (!query.exec()){
		qDebug()<<"SqlError: "<<query.lastError()<<query.executedQuery();
		return false;
	}

	return true;
}

bool Icon::updateIcon(const QString cmdargs, const QString exec, const QString icon_path, const QString desc, const QString prefix_name, const QString dir_name, const QString name, const QString icon_name, const QString override, const QString winedebug, const QString useconsole, const QString display, const QString wrkdir, const QString desktop, const int nice) const{
	QSqlQuery query;
	if (!dir_name.isEmpty()){
		query.prepare("UPDATE icon SET override=:override, winedebug=:winedebug, useconsole=:useconsole, display=:display,  cmdargs=:cmdargs, exec=:exec, icon_path=:icon_path, desc=:desc, name=:name, wrkdir=:wrkdir, desktop=:desktop, nice=:nice WHERE name=:icon_name and dir_id=(SELECT id FROM dir WHERE name=:dir_name AND prefix_id=(SELECT id FROM prefix WHERE name=:prefix_dir_name)) and prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name)");
		query.bindValue(":prefix_dir_name", prefix_name);

		if (dir_name.isEmpty()){
			query.bindValue(":dir_name", QVariant(QVariant::String));
		} else {
			query.bindValue(":dir_name", dir_name);
		}
	} else {
		query.prepare("UPDATE icon SET override=:override, winedebug=:winedebug, useconsole=:useconsole, display=:display,  cmdargs=:cmdargs, exec=:exec, icon_path=:icon_path, desc=:desc, name=:name, wrkdir=:wrkdir, desktop=:desktop, nice=:nice WHERE name=:icon_name and dir_id IS NULL and prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name)");
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

	if (icon_path.isEmpty()){
		query.bindValue(":icon_path", QVariant(QVariant::String));
	} else {
		query.bindValue(":icon_path", icon_path);
	}

	if (desc.isEmpty()){
		query.bindValue(":desc", QVariant(QVariant::String));
	} else {
		query.bindValue(":desc", desc);
	}

	if (name.isEmpty()){
		query.bindValue(":name", QVariant(QVariant::String));
	} else {
		query.bindValue(":name", name);
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
	query.bindValue(":icon_name", icon_name);
	query.bindValue(":prefix_name", prefix_name);

	if (!query.exec()){
		qDebug()<<"SqlError: "<<query.lastError()<<query.executedQuery();
		return false;
	}

	return true;
}

