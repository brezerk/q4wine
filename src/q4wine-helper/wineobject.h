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

#ifndef WINEOBJECT_H
#define WINEOBJECT_H

#ifdef __GNU__
#define PATH_MAX 4096
#endif

#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <QLocalSocket>
#include <QObject>
#include <QProcess>
#include <QTime>
#include <memory>

#include "config.h"
#include "core/database/versions.h"
#include "logging.h"
#include "prefix.h"
#include "q4wine-lib.h"

class WineObject : public QObject {
  Q_OBJECT
 public:
  explicit WineObject(QObject *parent = nullptr);

  bool setPrefix(const QString &prefix);

  void setProgramBinary(const QString &binary);
  void setProgramArgs(const QString &args);
  void setProgramDisplay(const QString &display);
  void setProgramDebug(const QString &debug);
  void setProgramNice(const int nice);
  void setProgramDesktop(const QString &desktop);
  void setProgramOverride(const QString &override);
  void setProgramWrkdir(const QString &wrkdir);
  void setProgramLang(const QString &lang);
  void setPreRun(const QString &path);
  void setPostRun(const QString &path);

  void setUseConsole(const int console);
  void setOverrideDll(const QString &dll_list);
  int run();

 private:
  int runSys();
  int runScript(const QString &script_path, const bool pre_run = true);
  void logStdout(const int status);

  //! This is need for libq4wine-core.so import;
  typedef void *CoreLibPrototype(bool);
  CoreLibPrototype *CoreLibClassPointer;
  std::unique_ptr<corelib> CoreLib;
  QLibrary libq4wine;

  QString createEnvString(void);
  void sendMessage(const QString &message);

  Prefix db_prefix;
  Logging db_logging;

  QString stdout;
  QString prefixName;
  QString prefixPath;
  QString prefixArch;
  QString prefixDllPath;
  int prefixId;
  QString versionId;
  QString prefixLoader;
  QString prefixServer;
  QString prefixBinary;
  QString prefixRunString;

  QString programBinary;
  QString programBinaryName;
  QString programArgs;
  QString programDisplay;
  QString programDebug;
  QString programWrkDir;
  int programNice;
  QString programDesktop;
  QString overrideDllList;

  QString programLang;

  QString prerun_script;
  QString postrun_script;

  bool useConsole, logEnabled;

 signals:

 public slots:
};

#endif  // WINEOBJECT_H
