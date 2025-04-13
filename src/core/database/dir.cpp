/***************************************************************************
 *   Copyright (C) 2008-2025 by Oleksii S. Malakhov <brezerk@gmail.com>    *
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

#include "dir.h"

Dir::Dir() {}

QStringList Dir::getDirList(const QString &prefix_name) const {
  QStringList valuelist;

  QSqlQuery query;
  query.prepare(
      "SELECT name FROM dir WHERE prefix_id=(SELECT id FROM prefix WHERE "
      "name=:prefix_name)");
  query.bindValue(":prefix_name", prefix_name);

  if (query.exec()) {
    while (query.next()) {
      if (query.value(0).isValid()) {
        valuelist.append(query.value(0).toString());
      }
    }
  } else {
    qDebug() << "SqlError: " << query.lastError();
    valuelist.clear();
  }
  return valuelist;
}

bool Dir::delDir(const QString &prefix_name, const QString &dir_name) const {
  QSqlQuery query;
  if (dir_name.isEmpty()) {
    query.prepare(
        "DELETE FROM dir WHERE prefix_id=(SELECT id FROM prefix WHERE "
        "name=:prefix_name)");
  } else {
    query.prepare(
        "DELETE FROM dir WHERE prefix_id=(SELECT id FROM prefix WHERE "
        "name=:prefix_name) and name=:dir_name");
    query.bindValue(":dir_name", dir_name);
  }
  query.bindValue(":prefix_name", prefix_name);

  if (!query.exec()) {
    qDebug() << "SqlError: " << query.lastError() << query.executedQuery();
    return false;
  }
  return true;
}

bool Dir::addDir(const QString &prefix_name, const QString &dir_name) const {
  QSqlQuery query;
  query.prepare(
      "INSERT INTO dir(name, prefix_id) VALUES(:name, (SELECT id FROM prefix "
      "WHERE name=:prefix_name))");
  query.bindValue(":prefix_name", prefix_name);
  query.bindValue(":name", dir_name);

  if (!query.exec()) {
    qDebug() << "SqlError: " << query.lastError() << query.executedQuery();
    return false;
  }
  return true;
}

bool Dir::isExistsByName(const QString &prefix_name,
                         const QString &dir_name) const {
  QSqlQuery query;
  query.prepare(
      "SELECT id FROM dir WHERE prefix_id=(SELECT id FROM prefix WHERE "
      "name=:prefix_name) AND name=:dir_name");
  query.bindValue(":prefix_name", prefix_name);
  query.bindValue(":dir_name", dir_name);

  if (!query.exec()) {
    qDebug() << "SqlError: " << query.lastError() << query.executedQuery();
    return false;
  }

  query.first();
  if (query.isValid()) {
    return true;
  }

  return false;
}

QString Dir::getId(const QString &dir_name, const QString &prefix_name) const {
  QString value;
  QSqlQuery query;

  query.prepare(
      "SELECT id FROM dir WHERE name=:dir_name and prefix_id=(SELECT id FROM "
      "prefix WHERE name=:prefix_name)");
  query.bindValue(":dir_name", dir_name);
  query.bindValue(":prefix_name", prefix_name);

  if (query.exec()) {
    query.first();
    if (query.isValid()) {
      int i = 0;
      while (query.value(i).isValid()) {
        value.append(query.value(i).toString());
        i++;
      }
    }
  } else {
    qDebug() << "SqlError: " << query.lastError();
  }
  return value;
}

bool Dir::renameDir(const QString &dir_name, const QString &prefix_name,
                    const QString &new_dir_name) const {
  QSqlQuery query;
  query.prepare(
      "UPDATE dir SET name=:new_dir_name WHERE name=:dir_name AND "
      "prefix_id=(SELECT id FROM prefix WHERE name=:prefix_name)");
  query.bindValue(":prefix_name", prefix_name);
  query.bindValue(":new_dir_name", new_dir_name);
  query.bindValue(":dir_name", dir_name);

  if (!query.exec()) {
    qDebug() << "SqlError: " << query.lastError() << query.executedQuery();
    return false;
  }

  return true;
}
