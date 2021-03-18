/***************************************************************************
 *   Copyright (C) 2008-2021 by Oleksii S. Malakhov <brezerk@gmail.com>    *
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

#include "last_run_icon.h"

Last_Run_Icon::Last_Run_Icon()
{
}

QStringList Last_Run_Icon::getByExec(const QString &exec) const{
    QStringList valuelist;
    QSqlQuery query;
    //                    0       1         2          3           4        5        6        7
    query.prepare("SELECT wrkdir, override, winedebug, useconsole, display, cmdargs, desktop, nice, lang FROM last_run_icon WHERE exec=:exec");
    query.bindValue(":exec", exec);

    if (query.exec()){
        query.first();
        //QStringList values;
        int i=0;
        while (query.value(i).isValid()){
            valuelist.append(query.value(i).toString());
            i++;
        }
    } else {
        qDebug()<<"SqlError: "<<query.lastError()<<query.executedQuery();
        valuelist.clear();
    }

    return valuelist;
}

QStringList Last_Run_Icon::getIcons(){
    QStringList list;
    QSqlQuery query;
    query.prepare("SELECT exec FROM last_run_icon ORDER BY id DESC");

    if (query.exec()){
        while (query.next()) {
            list.append(query.value(0).toString());
        }
    } else {
        qDebug()<<"SqlError: "<<query.lastError()<<query.executedQuery();
        list.clear();
    }

    return list;
}

bool Last_Run_Icon::addIcon(const QString &cmdargs,
                            const QString &exec,
                            const QString &override,
                            const QString &winedebug,
                            const QString &useconsole,
                            const QString &display,
                            const QString &wrkdir,
                            const QString &desktop,
                            const int nice,
                            const QString &lang) const{
    QSqlQuery query;
    query.prepare("DELETE FROM last_run_icon WHERE exec=:exec");
    query.bindValue(":exec", exec);

    if (!query.exec()){
        qDebug()<<"SqlError: "<<query.lastError()<<query.executedQuery();
        return false;
    }

    query.prepare("INSERT INTO last_run_icon(override, winedebug, useconsole, display, cmdargs, exec, wrkdir, desktop, nice, lang) VALUES(:override, :winedebug, :useconsole, :display, :cmdargs, :exec, :wrkdir, :desktop, :nice, :lang);");


    if (cmdargs.isEmpty()){
        query.bindValue(":cmdargs", QVariant(QVariant::String));
    } else {
        query.bindValue(":cmdargs", cmdargs);
    }

    if (exec.isEmpty()){
        query.bindValue(":exec", QVariant(QVariant::String));
    } else {
        query.bindValue(":exec", exec);
    }

    if (override.isEmpty()){
        query.bindValue(":override", QVariant(QVariant::String));
    } else {
        query.bindValue(":override", override);
    }

    if (winedebug.isEmpty()){
        query.bindValue(":winedebug", QVariant(QVariant::String));
    } else {
        query.bindValue(":winedebug", winedebug);
    }

    if (useconsole.isEmpty()){
        query.bindValue(":useconsole", QVariant(QVariant::String));
    } else {
        query.bindValue(":useconsole", useconsole);
    }

    if (display.isEmpty()){
        query.bindValue(":display", QVariant(QVariant::String));
    } else {
        query.bindValue(":display", display);
    }

    if (wrkdir.isEmpty()){
        query.bindValue(":wrkdir", QVariant(QVariant::String));
    } else {
        query.bindValue(":wrkdir", wrkdir);
    }

    if (desktop.isEmpty()){
        query.bindValue(":desktop", QVariant(QVariant::String));
    } else {
        query.bindValue(":desktop", desktop);
    }

    query.bindValue(":nice", nice);

    if (lang.isEmpty()){
        query.bindValue(":lang", QVariant(QVariant::String));
    } else {
        query.bindValue(":lang", lang);
    }

    if (!query.exec()){
        qDebug()<<"SqlError: "<<query.lastError()<<query.executedQuery();
        return false;
    }

    query.prepare("SELECT COUNT(id) FROM last_run_icon");
    if (!query.exec()){
        qDebug()<<"SqlError: "<<query.lastError()<<query.executedQuery();
        return false;
    }

    query.first();
    if (query.isValid()){
        if (query.value(0).toInt()>8){
            query.prepare("DELETE FROM last_run_icon WHERE id=(SELECT MIN(id) FROM last_run_icon)");

            if (!query.exec()){
                qDebug()<<"SqlError: "<<query.lastError()<<query.executedQuery();
                return false;
            }
        }
    }

    return true;
}

