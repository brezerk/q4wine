/***************************************************************************
 *   Copyright (C) 2008-2018 by Oleksii S. Malakhov <brezerk@gmail.com>    *
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

#include "core/database/versions.h"

Version::Version() : Table() {
    clear();
}

void Version::clear(){
    id_.clear();
    name_.clear();
    wine_exec_.clear();
    wine_loader_.clear();
    wine_server_.clear();
    wine_dllpath32_.clear();
    wine_dllpath64_.clear();
}

bool Version::load(){
    if ((id_.isEmpty()) && name_.isEmpty()){
        return false;
    }

    QSqlQuery query;
    QString sql = "SELECT id, name, wine_dllpath32, wine_dllpath64, wine_loader, wine_server, wine_exec FROM versions WHERE ";

    if (!id_.isEmpty()){
        sql.append("id=:id");
    } else {
        sql.append("name=:name");
    }

    query.prepare(sql);

    if (!id_.isEmpty()){
        query.bindValue(":id", id_);
    } else {
        query.bindValue(":name", name_);
    }

    if (query.exec()){
        query.first();
        if (query.isValid()){
             id_ = query.value(0).toString();
             name_ = query.value(1).toString();
             wine_dllpath32_ = query.value(2).toString();
             wine_dllpath64_ = query.value(3).toString();
             wine_loader_ = query.value(4).toString();
             wine_server_ = query.value(5).toString();
             wine_exec_ = query.value(6).toString();
             query.clear();
             return true;
        }
    } else {
        qDebug()<<"SqlError: "<<query.lastError();
    }
    query.clear();
    return false;
}

QList<Version> Version::load_all(QString filter){
    QList<Version> versions;
    QSqlQuery query;
    QString sql = "SELECT id, name, wine_dllpath32, wine_dllpath64, wine_loader, wine_server, wine_exec FROM versions";
    if (!filter.isEmpty()){
        sql.append(QString(" WHERE name like '%%1%'").arg(filter));
    }
    query.prepare(sql);
    if (query.exec()){
        while (query.next()) {
            Version ver;
            ver.id_ = query.value(0).toString();
            ver.name_ = query.value(1).toString();
            ver.wine_dllpath32_ = query.value(2).toString();
            ver.wine_dllpath64_ = query.value(3).toString();
            ver.wine_loader_ = query.value(4).toString();
            ver.wine_server_ = query.value(5).toString();
            ver.wine_exec_ = query.value(6).toString();
            versions.append(ver);
        }
    }
    return versions;
}

bool Version::save(){
    QString sql;
    if (!id_.isEmpty()){
        sql = "UPDATE versions SET name=:name, wine_dllpath32=:wine_dllpath32, wine_dllpath64=:wine_dllpath64, wine_loader=:wine_loader, wine_server=:wine_server, wine_exec=:wine_exec WHERE id=:id";
    } else {
        sql = "INSERT INTO versions(name, wine_exec, wine_server, wine_loader, wine_dllpath32, wine_dllpath64) VALUES(:name, :wine_exec, :wine_server, :wine_loader, :wine_dllpath32, :wine_dllpath64)";
    }
    QSqlQuery query;

    query.prepare(sql);

    if (!id_.isEmpty())
        query.bindValue(":id", id_);

    query.bindValue(":name", name_);

    if (wine_exec_.isEmpty()){
        query.bindValue(":wine_exec", QVariant(QVariant::String));
    } else {
        query.bindValue(":wine_exec", wine_exec_);
    }

    if (wine_server_.isEmpty()){
        query.bindValue(":wine_server", QVariant(QVariant::String));
    } else {
        query.bindValue(":wine_server", wine_server_);
    }

    if (wine_loader_.isEmpty()){
        query.bindValue(":wine_loader", QVariant(QVariant::String));
    } else {
        query.bindValue(":wine_loader", wine_loader_);
    }

    if (wine_dllpath32_.isEmpty()){
        query.bindValue(":wine_dllpath32", QVariant(QVariant::String));
    } else {
        query.bindValue(":wine_dllpath32", wine_dllpath32_);
    }

    if (wine_dllpath64_.isEmpty()){
        query.bindValue(":wine_dllpath64", QVariant(QVariant::String));
    } else {
        query.bindValue(":wine_dllpath64", wine_dllpath64_);
    }

    if (!query.exec()){
        qDebug()<<"SqlError: "<<query.lastError()<<query.executedQuery();
        return false;
    }

    return true;
}

bool Version::remove(){
    if ((id_.isEmpty()) && name_.isEmpty()){
        return false;
    }
    QSqlQuery query;
    QString sql = "DELETE FROM versions WHERE ";

    if (!id_.isEmpty()){
        sql.append("id=:id");
    } else {
        sql.append("name=:name");
    }

    query.prepare(sql);

    if (!id_.isEmpty()){
        query.bindValue(":id", id_);
    } else {
        query.bindValue(":name", name_);
    }

    if (!query.exec()){
        qDebug()<<"SqlError: "<<query.lastError()<<query.executedQuery();
        return false;
    }

    return true;
}
