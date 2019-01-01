/***************************************************************************
 *   Copyright (C) 2008-2019 by Oleksii S. Malakhov <brezerk@gmail.com>    *
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

#include "db.h"

DataBase::DataBase(QObject * parent): QObject(parent){

    QTextStream QErr(stderr);

#ifdef DEBUG
    qDebug()<<"[ii] Init database engine";
    qDebug()<<"[ii] Loading QSQLITE driver...";
#endif

    if (!QSqlDatabase::drivers().contains("QSQLITE")){
        QErr<<"[EE] "<<"Critical error"<<" : "<<"Unable to load SQLite database driver. You need to compile qt-sql with SQLite database support"<<endl;
        return;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

#ifdef DEBUG
    qDebug()<<"[ii] Loading database file: "<<QString("%1/.config/%2/db/generic.dat").arg(QDir::homePath()).arg(APP_SHORT_NAME);
#endif

    db.setDatabaseName(QString("%1/.config/%2/db/generic.dat").arg(QDir::homePath()).arg(APP_SHORT_NAME));

    if (!db.open()){
        QErr<<"[EE] "<<"Critical error"<<" : "<<QString("Cannot open database file: %1/.config/%2/db/generic.dat ; Error is: %3").arg(QDir::homePath()).arg(APP_SHORT_NAME).arg(db.lastError().text())<<endl;
        return;
    }

    return;
}

bool DataBase::checkDb(){
    /*
    * Checking database tables.
    * If not exists, we try to create them.
    */

    QTextStream QErr(stderr);

    QStringList tables;
    tables << "prefix" << "dir" << "icon" << "images" << "last_run_icon" << "logging" << "providers" << "sysconfig" << "versions";

    QSqlDatabase db = QSqlDatabase::database();

    if (!db.isValid()){
        QErr<<"[EE] "<<tr("No database loaded. Aborting...");
        return false;
    }

    QSqlQuery query;

    for (int i=0; i<tables.size(); ++i){
        QString table=tables.at(i);

#ifdef DEBUG
        qDebug()<<"[ii] Check for table: "<<table;
#endif

        if (db.record(table).isEmpty()){
            if (table == "prefix"){
                if(!query.exec("CREATE TABLE prefix (wine_dllpath TEXT, wine_loader TEXT, wine_server TEXT, wine_exec TEXT, cdrom_mount TEXT, id INTEGER PRIMARY KEY, name TEXT, path TEXT, version TEXT);"))
                    return false;

                // Creating default prefix reccord
                query.prepare("INSERT INTO prefix(id, name) VALUES(NULL, :name);");
                query.bindValue(":name", "Default");
                if (!query.exec())
                    return false;
                query.clear();
            } else if (table == "dir"){
                if(!query.exec("CREATE TABLE dir (id INTEGER PRIMARY KEY, name TEXT, prefix_id NUMERIC);"))
                    return false;
            } else if (table == "icon"){
                if(!query.exec("CREATE TABLE icon (wrkdir TEXT, override TEXT, winedebug TEXT, useconsole NUMERIC, display TEXT, cmdargs TEXT, exec TEXT, icon_path TEXT, desc TEXT, desktop TEXT, nice TEXT, dir_id NUMERIC, id INTEGER PRIMARY KEY, name TEXT, prefix_id NUMERIC);"))
                    return false;
            } else if (table == "images"){
                if(!query.exec("CREATE TABLE images (id INTEGER PRIMARY KEY, name TEXT, path TEXT);"))
                    return false;
            } else if (table == "last_run_icon"){
                if(!query.exec("CREATE TABLE last_run_icon (wrkdir TEXT, override TEXT, winedebug TEXT, useconsole NUMERIC, display TEXT, cmdargs TEXT, exec TEXT, desktop TEXT, nice TEXT, id INTEGER PRIMARY KEY);"))
                    return false;
            } else if (table == "logging"){
                if(!query.exec("CREATE TABLE logging (id INTEGER PRIMARY KEY, name TEXT, exit NUMERIC, stdout TEXT, prefix_id NUMERIC, date NUMERIC);"))
                    return false;
            } else if (table == "providers"){
                if(!query.exec("CREATE TABLE providers (id INTEGER PRIMARY KEY, name TEXT, icon TEXT);"))
                    return false;

                query.prepare("INSERT INTO providers(id, name, icon) VALUES(NULL, :name, :icon);");
                query.bindValue(":name", "System");
                query.bindValue(":icon", "wine");
                if (!query.exec())
                    return false;

                query.bindValue(":name", "Winetricks");
                query.bindValue(":icon", "regedit");
                if (!query.exec())
                    return false;
            } else if (table == "sysconfig"){
                if(!query.exec("CREATE TABLE sysconfig (id INTEGER PRIMARY KEY, name TEXT, type TEXT, icon TEXT, desc TEXT, provider_id INTEGER);"))
                    return false;

                // System items
                query.prepare("INSERT INTO sysconfig(id, name, icon, type, desc, provider_id) VALUES(NULL, :name, :icon, NULL, :desc, 1);");
                query.bindValue(":name", "%CREATE_FAKE%");
                query.bindValue(":icon", "document-new");
                query.bindValue(":desc", "%CREATE_FAKE_DESC%");
                if (!query.exec())
                    return false;

                query.prepare("INSERT INTO sysconfig(id, name, icon, type, desc, provider_id) VALUES(NULL, :name, :icon, NULL, :desc, 1);");
                query.bindValue(":name", "%UPDATE_FAKE%");
                query.bindValue(":icon", "document-edit");
                query.bindValue(":desc", "%UPDATE_FAKE_DESC%");
                if (!query.exec())
                    return false;

                // Wintricks base
                query.prepare("INSERT INTO sysconfig(id, name, icon, type, desc, provider_id) VALUES(NULL, :name, :icon, NULL, :desc, 2);");
                query.bindValue(":name", "%INSTALL_WINETRICKS%");
                query.bindValue(":icon", "download");
                query.bindValue(":desc", "%UPDATE_WINETRICKS_DESC%");
                if (!query.exec())
                    return false;

                query.prepare("INSERT INTO sysconfig(id, name, icon, type, desc, provider_id) VALUES(NULL, :name, :icon, NULL, :desc, 2);");
                query.bindValue(":name", "%REFRESH_WINETRICKS%");
                query.bindValue(":icon", "edit-redo");
                query.bindValue(":desc", "%REFRESH_WINETRICKS_DESC%");
                if (!query.exec())
                    return false;

                query.prepare("INSERT INTO sysconfig(id, name, icon, type, desc, provider_id) VALUES(NULL, :name, :icon, NULL, :desc, 2);");
                query.bindValue(":name", "%ABOUT_WINETRICKS%");
                query.bindValue(":icon", "go-home");
                query.bindValue(":desc", "%ABOUT_WINETRICKS_DESC%");
                if (!query.exec())
                    return false;
            } else if (table == "versions"){
                if(!query.exec("CREATE TABLE versions (wine_dllpath32 TEXT, wine_dllpath64 TEXT, wine_loader TEXT, wine_server TEXT, wine_exec TEXT, id INTEGER PRIMARY KEY, name TEXT);"))
                    return false;

                // Creating default version reccord
                Version vers;
                vers.name_ = "Default";

                // Use previous data is any
                QSettings settings(APP_SHORT_NAME, "default");
                if (settings.value("configure", "no") == "yes"){
                    settings.beginGroup("wine");
                    vers.wine_exec_ = settings.value("WineBin", QString()).toString();
                    vers.wine_server_ = settings.value("ServerBin", QString()).toString();
                    vers.wine_loader_ = settings.value("LoaderBin", QString()).toString();
                    vers.wine_dllpath32_ = settings.value("WineLibs32", QString()).toString();
                    vers.wine_dllpath64_ = settings.value("WineLibs64", QString()).toString();
                    settings.endGroup();
                }
                if (!vers.save())
                    return false;
            }
        }
    }

    return fixup();
}

void DataBase::close(){
   QSqlDatabase db = QSqlDatabase::database();
   db.close();
   return;
}

bool DataBase::fixup(){
    QSqlQuery query;
    if (!query.exec("SELECT arch FROM prefix")){
        if (!query.exec("ALTER TABLE prefix ADD COLUMN arch TEXT")){
            qDebug()<<"[EE] Cannot alter prefix table";
            return false;
        }
    }
    if (!query.exec("SELECT mountpoint_windrive FROM prefix")){
        if (!query.exec("ALTER TABLE prefix ADD COLUMN mountpoint_windrive TEXT")){
            qDebug()<<"[EE] Cannot alter prefix table";
            return false;
        }
    }
    if (!query.exec("SELECT lang FROM icon")){
        if (!query.exec("ALTER TABLE icon ADD COLUMN lang TEXT")){
            qDebug()<<"[EE] Cannot alter icon table";
            return false;
        }
    }
    if (!query.exec("SELECT lang FROM last_run_icon")){
        if (!query.exec("ALTER TABLE last_run_icon ADD COLUMN lang TEXT")){
            qDebug()<<"[EE] Cannot alter last_run_icon table";
            return false;
        }
    }
    if (!query.exec("SELECT prerun FROM icon")){
        if (!query.exec("ALTER TABLE icon ADD COLUMN prerun TEXT")){
            qDebug()<<"[EE] Cannot alter icon table";
            return false;
        }
    }
    if (!query.exec("SELECT postrun FROM icon")){
        if (!query.exec("ALTER TABLE icon ADD COLUMN postrun TEXT")){
            qDebug()<<"[EE] Cannot alter icon table";
            return false;
        }
    }
    if (!query.exec("SELECT run_string FROM prefix")){
        if (!query.exec("ALTER TABLE prefix ADD COLUMN run_string TEXT")){
            qDebug()<<"[EE] Cannot alter prefix table";
            return false;
        }
    }
    if (!query.exec("SELECT version_id FROM prefix")){
        if (!query.exec("ALTER TABLE prefix ADD COLUMN version_id INTEGER")){
            qDebug()<<"[EE] Cannot alter prefix table";
            return false;
        }
        if (!query.exec("UPDATE prefix SET version_id=1")){
            qDebug()<<"[EE] Cannot update prefix table";
            return false;
        }
    }
    query.exec("SELECT icon FROM providers WHERE icon='wine.png'");
    if (query.next()){
        if (!query.exec("UPDATE providers SET icon='regedit' WHERE icon='regedit.png'")){
            qDebug()<<"[EE] Cannot update providers table";
            return false;
        }
        if (!query.exec("UPDATE providers SET icon='wine' WHERE icon='wine.png'")){
            qDebug()<<"[EE] Cannot update providers table";
            return false;
        }
    }
    query.exec("SELECT id FROM sysconfig WHERE icon='go-home'");
    if (!query.next()){
        query.prepare("INSERT INTO sysconfig(id, name, icon, type, desc, provider_id) VALUES(NULL, :name, :icon, NULL, :desc, 2);");
        query.bindValue(":name", "%ABOUT_WINETRICKS%");
        query.bindValue(":icon", "go-home");
        query.bindValue(":desc", "%ABOUT_WINETRICKS_DESC%");
        if (!query.exec()){
            qDebug()<<"[EE] Cannot update sysconfig table";
            return false;
        }
    }
    query.exec("DELETE FROM images WHERE name IS NULL");
    if (!query.exec("SELECT is_installed FROM sysconfig")){
        if (!query.exec("ALTER TABLE sysconfig ADD COLUMN is_installed INTEGER")){
            qDebug()<<"[EE] Cannot alter sysconfig table";
            return false;
        }
        if (!query.exec("UPDATE sysconfig SET is_installed=0")){
            qDebug()<<"[EE] Cannot update sysconfig table";
            return false;
        }
    }
    return true;
}
