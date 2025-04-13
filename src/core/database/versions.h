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

#ifndef VERSIONS_H
#define VERSIONS_H

#include <config.h>

#include <QDebug>
#include <QDir>
#include <QList>
#include <QSettings>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <memory>

#include "core/database/table.h"

class Version : public Table {
 public:
  Version();
  QString id_;
  QString name_;
  QString wine_exec_;
  QString wine_server_;
  QString wine_loader_;
  QString wine_dllpath32_;
  QString wine_dllpath64_;

  bool load();
  bool save();
  bool remove();
  void clear();
  QList<Version> load_all(QString filter = "");
};

#endif  // VERSIONS_H
