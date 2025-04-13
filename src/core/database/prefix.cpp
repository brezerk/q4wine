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

#include "prefix.h"

Prefix::Prefix() {}

void Prefix::fixPrefixPath() {
  QSqlQuery query;
  query.prepare("SELECT id, path FROM prefix");

  if (query.exec()) {
    while (query.next()) {
      if (query.value(0).isValid()) {
        QString path = query.value(1).toString();
        if (path.length() > 1) {
          if (path.right(1) == "/") {
            path = path.left(path.length() - 1);
            QSqlQuery fixquery;
            fixquery.prepare("UPDATE prefix SET path=:path WHERE id=:id");
            fixquery.bindValue(":path", path);
            fixquery.bindValue(":id", query.value(0).toString());
            if (!fixquery.exec()) {
              qDebug() << "SqlError: " << fixquery.lastError();
            }
          }
        }
      }
    }
  } else {
    qDebug() << "SqlError: " << query.lastError();
  }
  query.clear();
}

QString Prefix::getId(const QString &prefix_name) const {
  QString value;
  QSqlQuery query;

  query.prepare("SELECT id FROM prefix WHERE name=:prefix_name");
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
  query.clear();
  return value;
}

QString Prefix::getPath(const QString &prefix_name) const {
  QString value;

  if (prefix_name == "Default") {
    value = QDir::homePath();
    value.append("/.wine");
    return value;
  }

  QSqlQuery query;

  query.prepare("SELECT path FROM prefix WHERE name=:prefix_name");
  query.bindValue(":prefix_name", prefix_name);

  if (query.exec()) {
    query.first();
    if (query.isValid()) {
      if (query.value(0).toString().isEmpty()) {
        value = QDir::homePath();
        value.append("/.wine");
      } else {
        value.append(query.value(0).toString());
      }
    }
  } else {
    qDebug() << "SqlError: " << query.lastError();
  }
  query.clear();
  return value;
}

QString Prefix::getName(const QString &prefix_path) const {
  QString value;
  QSqlQuery query;
  query.prepare("SELECT name FROM prefix WHERE path=:prefix_path");
  query.bindValue(":prefix_path", prefix_path);

  if (query.exec()) {
    query.first();
    if (query.isValid()) {
      value.append(query.value(0).toString());
    }
  } else {
    qDebug() << "SqlError: " << query.lastError();
  }
  query.clear();
  return value;
}

QHash<QString, QString> Prefix::getByName(const QString &prefix_name) const {
  QHash<QString, QString> values;

  QSqlQuery query;
  QSettings settings(APP_SHORT_NAME, "default");
  settings.beginGroup("wine");

  query.prepare(
      "SELECT path, wine_dllpath, wine_loader, wine_server, wine_exec, "
      "cdrom_mount, id, name, arch, mountpoint_windrive, run_string, "
      "version_id FROM prefix WHERE name=:prefix_name");
  query.bindValue(":prefix_name", prefix_name);

  if (query.exec()) {
    query.first();
    if (query.isValid()) {
      if (prefix_name == "Default") {
        values.insert("path", QString("%1/.wine").arg(QDir::homePath()));
      } else {
        if (query.value(0).toString().isEmpty()) {
          values.insert("path", QString("%1/.wine").arg(QDir::homePath()));
        } else {
          values.insert("path", query.value(0).toString());
        }
      }
      values.insert("libs", query.value(1).toString());
      values.insert("loader", query.value(2).toString());
      values.insert("server", query.value(3).toString());
      values.insert("bin", query.value(4).toString());
      values.insert("mount", query.value(5).toString());
      values.insert("id", query.value(6).toString());
      values.insert("name", query.value(7).toString());
      values.insert("arch", query.value(8).toString());
      values.insert("mountpoint_windrive", query.value(9).toString());
      if (!query.value(10).toString().isEmpty()) {
        values.insert("run_string", query.value(10).toString());
      } else {
        values.insert("run_string", RUN_STRING_TEMPLATE_DEFAULT);
      }
      values.insert("version_id", query.value(11).toString());
    }
  } else {
    qDebug() << "SqlError: " << query.lastError();
  }
  settings.endGroup();
  query.clear();

  return values;
}

QString Prefix::getMountPoint(const QString &prefix_name) const {
  QString value;
  QSqlQuery query;

  query.prepare("SELECT cdrom_mount FROM prefix WHERE name=:prefix_name");
  query.bindValue(":prefix_name", prefix_name);

  if (query.exec()) {
    query.first();
    if (query.isValid()) {
      value = query.value(0).toString();
    }
  } else {
    qDebug() << "SqlError: " << query.lastError();
  }
  query.clear();
  return value;
}

QChar Prefix::getMountPointWindrive(const QString &prefix_name) const {
  QString value;
  QSqlQuery query;

  query.prepare(
      "SELECT mountpoint_windrive FROM prefix WHERE name=:prefix_name");
  query.bindValue(":prefix_name", prefix_name);

  if (query.exec()) {
    query.first();
    if (query.isValid()) {
      value = query.value(0).toString();
    }
  } else {
    qDebug() << "SqlError: " << query.lastError();
    return QChar();
  }
  query.clear();
  return value[0];
}

QStringList Prefix::getPrefixList(void) const {
  QStringList value;
  QSqlQuery query;

  query.prepare("SELECT name FROM prefix ORDER BY name");

  if (query.exec()) {
    while (query.next()) {
      if (query.value(0).isValid()) value.append(query.value(0).toString());
    }
  } else {
    qDebug() << "SqlError: " << query.lastError();
  }
  query.clear();
  return value;
}

bool Prefix::updateQuery(QSqlQuery *sqlQuery) const {
  if (!sqlQuery->exec()) {
    qDebug() << "SqlError: " << sqlQuery->lastError();
    return false;
  }
  return true;
}

bool Prefix::delByName(const QString &prefix_name) const {
  QSqlQuery query;
  query.prepare(
      "DELETE FROM prefix WHERE id=(SELECT id FROM prefix WHERE "
      "name=:prefix_name )");
  query.bindValue(":prefix_name", prefix_name);

  if (!query.exec()) {
    qDebug() << "SqlError: " << query.lastError() << query.executedQuery();
    return false;
  }
  return true;
}

bool Prefix::addPrefix(const QString &prefix_name, const QString &prefix_path,
                       const QString &wine_exec, const QString &wine_server,
                       const QString &wine_loader, const QString &wine_dllpath,
                       const QString &cdrom_mount, const QString &arch,
                       const QString &mountpoint_windrive,
                       const QString &run_string,
                       const QString &version_id) const {
  QSqlQuery query;
  query.prepare(
      "INSERT INTO prefix(name, path, wine_exec, wine_server, wine_loader, "
      "wine_dllpath, cdrom_mount, arch, mountpoint_windrive, run_string, "
      "version_id) VALUES(:prefix_name, :prefix_path, :wine_exec, "
      ":wine_server, :wine_loader, :wine_dllpath, :cdrom_mount, :arch, "
      ":mountpoint_windrive, :run_string, :version_id);");

  query.bindValue(":prefix_name", prefix_name);

  if (prefix_path.isEmpty()) {
    query.bindValue(":prefix_path", QVariant(QVariant::String));
  } else {
    query.bindValue(":prefix_path", prefix_path);
  }

  if (wine_exec.isEmpty()) {
    query.bindValue(":wine_exec", QVariant(QVariant::String));
  } else {
    query.bindValue(":wine_exec", wine_exec);
  }

  if (wine_server.isEmpty()) {
    query.bindValue(":wine_server", QVariant(QVariant::String));
  } else {
    query.bindValue(":wine_server", wine_server);
  }

  if (wine_loader.isEmpty()) {
    query.bindValue(":wine_loader", QVariant(QVariant::String));
  } else {
    query.bindValue(":wine_loader", wine_loader);
  }

  if (wine_dllpath.isEmpty()) {
    query.bindValue(":wine_dllpath", QVariant(QVariant::String));
  } else {
    query.bindValue(":wine_dllpath", wine_dllpath);
  }

  if (cdrom_mount.isEmpty()) {
    query.bindValue(":cdrom_mount", QVariant(QVariant::String));
  } else {
    query.bindValue(":cdrom_mount", cdrom_mount);
  }

  if (arch == QObject::tr("Default")) {
    query.bindValue(":arch", QVariant(QVariant::String));
  } else {
    query.bindValue(":arch", arch);
  }

  if (mountpoint_windrive.isEmpty()) {
    query.bindValue(":mountpoint_windrive", QVariant(QVariant::String));
  } else {
    query.bindValue(":mountpoint_windrive", mountpoint_windrive);
  }

  if (run_string.isEmpty()) {
    query.bindValue(":run_string", QVariant(QVariant::String));
  } else {
    query.bindValue(":run_string", run_string);
  }

  if (version_id.isEmpty()) {
    query.bindValue(":version_id", QVariant(QVariant::String));
  } else {
    query.bindValue(":version_id", version_id);
  }

  if (!query.exec()) {
    qDebug() << "SqlError: " << query.lastError() << query.executedQuery();
    return false;
  }

  return true;
}

bool Prefix::updatePrefix(
    const QString &prefix_name, const QString &prefix_path,
    const QString &wine_exec, const QString &wine_server,
    const QString &wine_loader, const QString &wine_dllpath,
    const QString &cdrom_mount, const QString &old_prefix_name,
    const QString &arch, const QString &mountpoint_windrive,
    const QString &run_string, const QString &version_id) const {
  QSqlQuery query;
  query.prepare(
      "UPDATE prefix SET wine_dllpath=:wine_dllpath, wine_loader=:wine_loader, "
      "wine_server=:wine_server, wine_exec=:wine_exec, "
      "cdrom_mount=:cdrom_mount, arch=:arch, name=:prefix_name, "
      "mountpoint_windrive=:mountpoint_windrive, path=:prefix_path, "
      "run_string=:run_string, version_id=:version_id WHERE "
      "name=:old_prefix_name");

  query.bindValue(":prefix_name", prefix_name);
  query.bindValue(":old_prefix_name", old_prefix_name);

  if (prefix_path.isEmpty()) {
    query.bindValue(":prefix_path", QVariant(QVariant::String));
  } else {
    query.bindValue(":prefix_path", prefix_path);
  }

  if (wine_exec.isEmpty()) {
    query.bindValue(":wine_exec", QVariant(QVariant::String));
  } else {
    query.bindValue(":wine_exec", wine_exec);
  }

  if (wine_server.isEmpty()) {
    query.bindValue(":wine_server", QVariant(QVariant::String));
  } else {
    query.bindValue(":wine_server", wine_server);
  }

  if (wine_loader.isEmpty()) {
    query.bindValue(":wine_loader", QVariant(QVariant::String));
  } else {
    query.bindValue(":wine_loader", wine_loader);
  }

  if (wine_dllpath.isEmpty()) {
    query.bindValue(":wine_dllpath", QVariant(QVariant::String));
  } else {
    query.bindValue(":wine_dllpath", wine_dllpath);
  }

  if (cdrom_mount.isEmpty()) {
    query.bindValue(":cdrom_mount", QVariant(QVariant::String));
  } else {
    query.bindValue(":cdrom_mount", cdrom_mount);
  }

  if (arch == QObject::tr("Default")) {
    query.bindValue(":arch", QVariant(QVariant::String));
  } else {
    query.bindValue(":arch", arch);
  }

  if (mountpoint_windrive.isEmpty()) {
    query.bindValue(":mountpoint_windrive", QVariant(QVariant::String));
  } else {
    query.bindValue(":mountpoint_windrive", mountpoint_windrive);
  }

  if (run_string.isEmpty()) {
    query.bindValue(":run_string", QVariant(QVariant::String));
  } else {
    query.bindValue(":run_string", run_string);
  }

  if (version_id.isEmpty()) {
    query.bindValue(":version_id", QVariant(QVariant::String));
  } else {
    query.bindValue(":version_id", version_id);
  }

  if (!query.exec()) {
    qDebug() << "SqlError: " << query.lastError() << query.executedQuery();
    return false;
  }

  return true;
}

bool Prefix::isExistsByName(const QString &prefix_name) const {
  QSqlQuery query;
  query.prepare("SELECT id FROM prefix WHERE name=:prefix_name");
  query.bindValue(":prefix_name", prefix_name);

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
