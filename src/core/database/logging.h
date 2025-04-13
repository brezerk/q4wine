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

#ifndef LOGGING_H
#define LOGGING_H

#include <config.h>

#include <QDateTime>
#include <QDebug>
#include <QList>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <memory>

class Logging {
 public:
  Logging();

  void addLogRecord(int prefix_id, QString program, int exit, QString stdout,
                    qint64 date);

  void clearLogs(void);
  void deleteLogs(QString prefix_name);
  void deleteLogs(QString prefix_name, QString app_name);
  void deleteLogs(QString prefix_name, QString app_name, QString date);

  QStringList getApps(QString prefix_name);
  QHash<qint64, int> getDates(QString prefix_name, QString app_name);
  QString getLogs(QString prefix_name, QString app_name, QString date);
};

#endif  // LOGGING_H
