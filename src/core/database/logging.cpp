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

#include "logging.h"

Logging::Logging() {}

void Logging::clearLogs(void) {
  QSqlQuery query;
  query.exec("DELETE FROM logging");
  if (!query.exec()) {
    qDebug() << "clearLogs.SqlError: " << query.lastError()
             << query.executedQuery();
  }
  query.exec("VACUUM");
  if (!query.exec()) {
    qDebug() << "clearLogs.SqlError: " << query.lastError()
             << query.executedQuery();
  }
  return;
}

void Logging::deleteLogs(QString prefix_name) {
  QSqlQuery query;
  query.prepare(
      "DELETE FROM logging WHERE prefix_id=(SELECT id FROM prefix WHERE "
      "name=:prefix_name);");
  query.bindValue(":prefix_name", prefix_name);

  if (!query.exec()) {
    qDebug() << "SqlError: " << query.lastError() << query.executedQuery();
  }
  return;
}

void Logging::deleteLogs(QString prefix_name, QString app_name) {
  QSqlQuery query;
  query.prepare(
      "DELETE FROM logging WHERE prefix_id=(SELECT id FROM prefix WHERE "
      "name=:prefix_name) AND name=:app_name;");
  query.bindValue(":prefix_name", prefix_name);
  query.bindValue(":app_name", app_name);

  if (!query.exec()) {
    qDebug() << "SqlError: " << query.lastError() << query.executedQuery();
  }
  return;
}

void Logging::deleteLogs(QString prefix_name, QString app_name, QString date) {
  QSqlQuery query;
  query.prepare(
      "DELETE FROM logging WHERE prefix_id=(SELECT id FROM prefix WHERE "
      "name=:prefix_name) AND name=:app_name AND date=:date;");
  query.bindValue(":prefix_name", prefix_name);
  query.bindValue(":app_name", app_name);
  QDateTime dt = QDateTime::fromString(date, "dd.MM.yyyy hh:mm:ss");
  qint64 timestamp = dt.toSecsSinceEpoch();
  query.bindValue(":date", timestamp);

  if (!query.exec()) {
    qDebug() << "SqlError: " << query.lastError() << query.executedQuery();
  }
  return;
}

void Logging::addLogRecord(int prefix_id, QString program, int exit,
                           QString stdout, qint64 date) {
  QSqlQuery query;

  query.prepare(
      "INSERT INTO logging(name, exit, stdout, prefix_id, date) VALUES (:name, "
      ":exit, :stdout, :prefix_id, :date);");
  query.bindValue(":name", program);
  query.bindValue(":exit", exit);
  query.bindValue(":stdout", stdout);
  query.bindValue(":prefix_id", prefix_id);
  query.bindValue(":date", date);

  if (!query.exec()) {
    qDebug() << "SqlError: " << query.lastError() << query.executedQuery();
  }
  return;
}

QStringList Logging::getApps(QString prefix_name) {
  QStringList list;

  QSqlQuery query;
  query.prepare(
      "SELECT name FROM logging WHERE prefix_id=(SELECT id FROM prefix WHERE "
      "name=:prefix_name) GROUP BY name ORDER BY name");
  query.bindValue(":prefix_name", prefix_name);

  if (query.exec()) {
    while (query.next()) {
      if (query.value(0).isValid()) list.append(query.value(0).toString());
    }
  } else {
    qDebug() << "SqlError: " << query.lastError();
  }
  query.clear();

  return list;
}

QHash<qint64, int> Logging::getDates(QString prefix_name, QString app_name) {
  QHash<qint64, int> list;

  QSqlQuery query;
  query.prepare(
      "SELECT date, exit FROM logging WHERE prefix_id=(SELECT id FROM prefix "
      "WHERE name=:prefix_name) AND name=:app_name ORDER BY date DESC");
  query.bindValue(":prefix_name", prefix_name);
  query.bindValue(":app_name", app_name);

  if (query.exec()) {
    while (query.next()) {
      if (query.value(0).isValid()) {
        list.insert(query.value(0).toULongLong(), query.value(1).toInt());
      }
    }
  } else {
    qDebug() << "SqlError: " << query.lastError();
  }
  query.clear();

  return list;
}

QString Logging::getLogs(QString prefix_name, QString app_name, QString date) {
  QString list;

  QSqlQuery query;
  query.prepare(
      "SELECT stdout FROM logging WHERE prefix_id=(SELECT id FROM prefix WHERE "
      "name=:prefix_name) AND name=:app_name AND date=:date");
  query.bindValue(":prefix_name", prefix_name);
  query.bindValue(":app_name", app_name);
  QDateTime dt = QDateTime::fromString(date, "dd.MM.yyyy hh:mm:ss");
  qint64 timestamp = dt.toSecsSinceEpoch();
  query.bindValue(":date", timestamp);

  if (query.exec()) {
    query.first();
    if (query.value(0).isValid()) list = query.value(0).toString();
  } else {
    qDebug() << "SqlError: " << query.lastError();
  }
  query.clear();

  return list;
}
