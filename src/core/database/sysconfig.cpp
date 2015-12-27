/***************************************************************************
 *   Copyright (C) 2008-2013 by Alexey S. Malakhov <brezerk@gmail.com>     *
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

#include "sysconfig.h"

Sysconfig::Sysconfig()
{

}

QList<ProviderItem> Sysconfig::getProviders(void) const{
    QList<ProviderItem> providers;
    QSqlQuery query;
    query.prepare("SELECT id, name, icon FROM providers");

    if (query.exec()){
        while (query.next()) {
            ProviderItem provider;
            provider.id = query.value(0).toInt();
            provider.name = query.value(1).toString();
            provider.icon = query.value(2).toString();
            providers.append(provider);
        }
    }
    query.clear();
    return providers;
}

QStringList Sysconfig::getProviderSubtypes(int provider_id) const{
    QStringList subtypes;
    QSqlQuery query;
    query.prepare("SELECT type FROM sysconfig WHERE provider_id=:provider_id GROUP BY type");
    query.bindValue(":provider_id", provider_id);
    if (query.exec()){
        while (query.next()) {
            subtypes.append(query.value(0).toString());
        }
    }
    query.clear();
    return subtypes;
}

QList<SysconfigItem> Sysconfig::getItems(QString provider, QString type, int sort_order, QString filter) const{
    QList<SysconfigItem> items;
    QSqlQuery query;
    QString sql, order;

    if (sort_order == D_SORT_TYPE_BY_NAME_ASC){
        order = " ORDER BY name ASC ";
    } else if (sort_order == D_SORT_TYPE_BY_NAME_DSC){
        order = " ORDER BY name DESC ";
    }

    if (type.isEmpty()){
        if (filter.isEmpty()){
            sql = "SELECT id, name, icon, desc FROM sysconfig WHERE provider_id=(SELECT id FROM providers WHERE name=:provider) AND type IS NULL";
        } else {
            sql = QString("SELECT id, name, icon, desc FROM sysconfig WHERE provider_id=(SELECT id FROM providers WHERE name=:provider) AND type IS NULL AND name LIKE \"%%1%\"").arg(filter);
        }
        sql.append(order);
        query.prepare(sql);
    } else {
        if (filter.isEmpty()){
            sql = "SELECT id, name, icon, desc FROM sysconfig WHERE provider_id=(SELECT id FROM providers WHERE name=:provider) AND type=:type";
        } else {
            sql = QString("SELECT id, name, icon, desc FROM sysconfig WHERE provider_id=(SELECT id FROM providers WHERE name=:provider) AND type=:type AND name LIKE \"%%1%\"").arg(filter);
        }
        sql.append(order);
        query.prepare(sql);
        query.bindValue(":type", type);
    }
    query.bindValue(":provider", provider);
    if (query.exec()){
        while (query.next()) {
            SysconfigItem item;
            item.id = query.value(0).toInt();
            item.name = query.value(1).toString();
            item.icon = query.value(2).toString();
            item.desc = query.value(3).toString();
            items.append(item);
        }
    }
    query.clear();
    return items;
}

bool Sysconfig::add_item(QString name, QString icon, QString desc, QString type, int provider_id){
    QSqlQuery query;
    if (type.isEmpty()){
        query.prepare("INSERT INTO sysconfig(id, name, icon, type, desc, provider_id) VALUES(NULL, :name, :icon, NULL, :desc, :provider_id);");
    } else {
        query.prepare("INSERT INTO sysconfig(id, name, icon, type, desc, provider_id) VALUES(NULL, :name, :icon, :type, :desc, :provider_id);");
        query.bindValue(":type", type);
    }
    query.bindValue(":name", name);
    query.bindValue(":icon", icon);
    query.bindValue(":desc", desc);
    query.bindValue(":provider_id", provider_id);
    if (!query.exec()){
        qDebug() << "Error sysconfig::add_item: " << query.lastError().text();
        return false;
    }
    return true;
}

bool Sysconfig::drop_items(int provider_id){
    QSqlQuery query;
    query.prepare("DELETE FROM sysconfig WHERE provider_id=:provider_id AND type IS NOT NULL");
    query.bindValue(":provider_id", provider_id);
    if (!query.exec())
        return false;
    return true;
}
