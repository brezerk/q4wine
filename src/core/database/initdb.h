/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010 by Malakhov Alexey                                 *
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

#ifndef INITDB_H
#define INITDB_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>

bool initDb(){
   /*
   * DataBase engine init
   */

   if (!QSqlDatabase::drivers().contains("QSQLITE")){
	QMessageBox::warning(0, QObject::tr("Critical error"), QObject::tr("Unable to load database SQLITE driver. You need to compile qt-sql with sqlite database support"));
	return FALSE;
   }

   QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
   db.setDatabaseName(QString("%1/.config/%2/db/generic.dat").arg(QDir::homePath()).arg(APP_SHORT_NAME));

   if (!db.open()){
	QMessageBox::warning(0, QObject::tr("Critical error"), QObject::tr("Sorry, i can't open database file: %1/.config/%2/db/generic.dat ; Error is: %3").arg(QDir::homePath()).arg(APP_SHORT_NAME).arg(db.lastError().text()));
	return FALSE;
   }

   return TRUE;
}

#endif
