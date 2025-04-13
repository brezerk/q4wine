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

#ifndef PROGRESS_H
#define PROGRESS_H

#include <QDialog>
#include <QDir>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QWidget>
#include <memory>

#include "config.h"
#include "src/q4wine-lib/ui_Process.h"

#ifdef DEBUG
#include <QDebug>
#endif

#include "dir.h"
#include "icon.h"
#include "prefix.h"

// q4wine lib
#include "q4wine-lib.h"

class Progress : public QDialog, public Ui::Process {
  Q_OBJECT
 public:
  explicit Progress(int action, QString path = "", QWidget *parent = nullptr,
                    Qt::WindowFlags f = Qt::WindowFlags());

 signals:

 public slots:

 private:
  int action, max, current;

  QLibrary libq4wine;
  typedef void *CoreLibPrototype(bool);
  CoreLibPrototype *CoreLibClassPointer;
  std::unique_ptr<corelib> CoreLib;

  Dir db_dir;
  Icon db_icon;
  Prefix db_prefix;

  QString path;

  int importIcons(QString folder);
  // void removeEmptyFolders(QString folder);
  void parseDesktopFile(QString file, QString dirName);
  QStringList iconDirs;
  QStringList iconFiles;

  std::unique_ptr<QTimer> t;

 private slots:
  void runAction();
  void cmdCancel_Click();
};

#endif  // PROGRESS_H
