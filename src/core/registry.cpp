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

#include "registry.h"

Registry::Registry() {
  // Loading libq4wine-core.so
#ifdef RELEASE
  libq4wine.setFileName(_CORELIB_PATH_);
#else
  libq4wine.setFileName("../q4wine-lib/libq4wine-core");
#endif

  if (!libq4wine.load()) {
    libq4wine.load();
  }

  // Getting corelib class pointer
  CoreLibClassPointer =
      reinterpret_cast<CoreLibPrototype *>(libq4wine.resolve("createCoreLib"));
  CoreLib.reset(static_cast<corelib *>(CoreLibClassPointer(true)));
  return;
}

Registry::Registry(const QString &prefixPath) : Registry() {
  regfile.append(prefixPath);
  regfile.append("/");
  return;
}

bool Registry::init() {
  regfile_image = "REGEDIT4";
  return true;
}

void Registry::append(const QString &reg_keys) {
  // regfile_image.append(reg_keys);
  Q_UNUSED(reg_keys)
  return;
}

void Registry::set(const QString &path, const QString &key,
                   const QString &value, const QString &hkey) {
  QString regString;
  regString.append("\n\n[");
  regString.append(hkey);
  regString.append("\\");
  regString.append(path);
  regString.append("]\n");
  if (!key.isEmpty()) {
    regString.append("\"");
    regString.append(key);
    regString.append("\"=\"");
    regString.append(value);
    regString.append("\"");
  } else {
    regString.append(value);
  }

  regfile_image.append(regString);
  return;
}

void Registry::unset(const QString &path, const QString &key,
                     const QString &hkey) {
  QString regString;
  regString.append("\n\n[");
  regString.append(hkey);
  regString.append("\\");
  regString.append(path);
  regString.append("]\n\"");
  regString.append(key);
  regString.append("\"=-");

  regfile_image.append(regString);
  return;
}

void Registry::unsetPath(const QString &path, const QString &hkey) {
  QString regString;
  regString.append("\n\n[-");
  regString.append(hkey);
  regString.append("\\");
  regString.append(path);
  regString.append("]");

  regfile_image.append(regString);
  return;
}

bool Registry::exec(QObject *parent, const QString &prefix_path,
                    const QString &prefix_name, const QString &post_run) {
  // This function wrights regfile_image into file, then run regedit.exe and
  // import this file. Also, clean files before end

  QDir dir(prefix_path);

  if (!dir.exists()) {
    qDebug() << "[ee] Fake drive dir does not exist!";
    return false;
  } else {
    dir.setPath(QString("%1/drive_c/temp").arg(dir.path()));
    if (!dir.exists()) {
      if (!dir.mkdir(dir.path())) {
        qDebug() << "[ee] Cannot create tmp dir for fake drive dir!";
        return false;
      }
    }
  }

  QString full_file_path = QString("%1/XXXXXX.reg").arg(dir.path());
  QTemporaryFile file(full_file_path);
  file.open();
  file.write(regfile_image.toLatin1());  // write to stderr
  file.close();

  ExecObject execObj;
  execObj.cmdargs = QString("C:/temp/%1").arg(QFileInfo(file).fileName());
  execObj.execcmd = "regedit.exe";
  if (!post_run.isEmpty()) execObj.postrun = post_run;

  bool ret = CoreLib->runWineBinary(execObj, prefix_name, false);

  file.remove();

  return ret;
}

QStringList Registry::readKeys(const QString &sysfile, const QString &path,
                               const QStringList &keys) const {
  QStringList ret;
  QString searchPath;

  for (int i = 0; i < keys.count(); i++) {
    ret.append(QString());
  }

  searchPath = "[";
  searchPath.append(path);
  searchPath.append("]");
  searchPath.replace("\\", "\\\\");

  QString sfile = regfile;
  sfile.append(sysfile);
  sfile.append(".reg");

  QFile file(sfile);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << " [EE] Cannot open reg file: " << regfile;
    return ret;
  }

  bool readFlag = false;

  while (!file.atEnd()) {
    QByteArray line = file.readLine();
    if ((line.indexOf(searchPath.toUtf8()) > -1) and (!readFlag)) {
      readFlag = true;
    }

    if (readFlag) {
      QList<QByteArray> key = line.trimmed().split('=');
      int index = keys.indexOf(key.at(0));
      if (index > -1) {
        ret.replace(index, key.at(1).mid(1, (key.at(1).length() - 2)));
      }
    }

    if (((line == "\n") or (line.isEmpty())) and (readFlag)) readFlag = false;
  }

  return ret;
}

QStringList Registry::readExcludedKeys(const QString &sysfile,
                                       const QString &path,
                                       const QStringList &keys,
                                       const int count) const {
  QStringList ret;
  QString searchPath;

  searchPath = "[";
  searchPath.append(path);
  searchPath.append("]");
  searchPath.replace("\\", "\\\\");

  QString sfile = regfile;
  sfile.append(sysfile);
  sfile.append(".reg");

  QFile file(sfile);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << " [EE] Cannot open reg file: " << regfile;
    return ret;
  }

  bool readFlag = false;
  int read = 1;

  while (!file.atEnd()) {
    QByteArray line = file.readLine();

    if ((readFlag) && (!line.trimmed().isEmpty())) {
      if (line.startsWith("#time")) continue;
      QList<QByteArray> key = line.trimmed().split('=');
      int index = keys.indexOf(key.at(0));
      if (index == -1) {
        ret.append(line.trimmed());
        read++;
        if (read > count) return ret;
      }
    } else {
      if ((line.indexOf(searchPath.toUtf8()) > -1) and (!readFlag))
        readFlag = true;
    }

    if (((line == "\n") or (line.isEmpty())) and (readFlag)) readFlag = false;
  }
  return ret;
}
