/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010, 2011 by Malakhov Alexey                                 *
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

#include "db.h"

DataBase::DataBase(QObject * parent): QObject(parent){

    QTextStream QErr(stderr);

#ifdef DEBUG
    qDebug()<<"[ii] Init database engine";
    qDebug()<<"[ii] Loading QSQLITE driver...";
#endif

    if (!QSqlDatabase::drivers().contains("QSQLITE")){
        QErr<<"[EE] "<<tr("Critical error")<<" : "<<"Unable to load database SQLITE driver. You need to compile qt-sql with sqlite database support"<<endl;
        return;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

#ifdef DEBUG
    qDebug()<<"[ii] Loading database file: "<<QString("%1/.config/%2/db/generic.dat").arg(QDir::homePath()).arg(APP_SHORT_NAME);
#endif

    db.setDatabaseName(QString("%1/.config/%2/db/generic.dat").arg(QDir::homePath()).arg(APP_SHORT_NAME));

    if (!db.open()){
        QErr<<"[EE] "<<tr("Critical error")<<" : "<<tr("Can not open database file: %1/.config/%2/db/generic.dat ; Error is: %3").arg(QDir::homePath()).arg(APP_SHORT_NAME).arg(db.lastError().text())<<endl;
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
    tables << "prefix" << "dir" << "icon" << "images" << "last_run_icon" << "logging" << "providers" << "sysconfig";

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
                query.bindValue(":icon", "wine.png");
                if (!query.exec())
                    return false;

                query.bindValue(":name", "Winetricks");
                query.bindValue(":icon", "regedit.png");
                if (!query.exec())
                    return false;
            } else if (table == "sysconfig"){
                if(!query.exec("CREATE TABLE sysconfig (id INTEGER PRIMARY KEY, name TEXT, type TEXT, icon TEXT, desc TEXT, provider_id INTEGER);"))
                    return false;

                // System items
                query.prepare("INSERT INTO sysconfig(id, name, icon, type, desc, provider_id) VALUES(NULL, :name, :icon, NULL, :desc, 1);");
                query.bindValue(":name", "%CREATE_FAKE%");
                query.bindValue(":icon", "fakedrive-new.png");
                query.bindValue(":desc", "%CREATE_FAKE_DESC%");
                if (!query.exec())
                    return false;

                query.prepare("INSERT INTO sysconfig(id, name, icon, type, desc, provider_id) VALUES(NULL, :name, :icon, NULL, :desc, 1);");
                query.bindValue(":name", "%UPDATE_FAKE%");
                query.bindValue(":icon", "fakedrive-update.png");
                query.bindValue(":desc", "%UPDATE_FAKE_DESC%");
                if (!query.exec())
                    return false;

                // Wintricks base
                query.prepare("INSERT INTO sysconfig(id, name, icon, type, desc, provider_id) VALUES(NULL, :name, :icon, NULL, :desc, 2);");
                query.bindValue(":name", "%INSTALL_WINETRICKS%");
                query.bindValue(":icon", "winetricks-install.png");
                query.bindValue(":desc", "%UPDATE_WINETRICKS_DESC%");
                if (!query.exec())
                    return false;

                query.prepare("INSERT INTO sysconfig(id, name, icon, type, desc, provider_id) VALUES(NULL, :name, :icon, NULL, :desc, 2);");
                query.bindValue(":name", "%REFRESH_WINETRICKS%");
                query.bindValue(":icon", "folder-downloads.png");
                query.bindValue(":desc", "%REFRESH_WINETRICKS_DESC%");
                if (!query.exec())
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
    return true;
}
