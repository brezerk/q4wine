/***************************************************************************
 *   Copyright (C) 2008 by Malakhov Alexey                                 *
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
 *   In addition, as a special exception, the copyright holders give       *
 *   permission to link the code of this program with any edition of       *
 *   the Qt library by Trolltech AS, Norway (or with modified versions     *
 *   of Qt that use the same license as Qt), and distribute linked         *
 *   combinations including the two.  You must obey the GNU General        *
 *   Public License in all respects for all of the code used other than    *
 *   Qt.  If you modify this file, you may extend this exception to        *
 *   your version of the file, but you are not obligated to do so.  If     *
 *   you do not wish to do so, delete this exception statement from        *
 *   your version.                                                         *
 ***************************************************************************/

#ifndef INITDB_H
#define INITDB_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QStringList>
#include <QSqlQuery>
#include <QDebug>
#include <QDir>
#include <QSqlRelation>
#include <QMessageBox>
#include <QSqlRelationalTableModel>




bool initDb()
{
	/* 
	 * Databace engine init
	 */
	
	
	if (!QSqlDatabase::drivers().contains("QSQLITE")){
		qDebug()<<"[EE] Unable to load database. This program needs the SQLITE driver";
		return FALSE;
	}
	
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(QMessageBox::tr("%1/.q4wine/db/generic.dat").arg(QDir::homePath()));

	if (!db.open()){
		qDebug()<<"[EE] Sorry, i can't open database file by path: ~/.q4wine/db/generic.dat\".\n\n"
					"May be database is not correctly installed...";
					
		return FALSE;
	}
		
	return TRUE;	
}

bool createDb(){
	/* 
	 * create en empty database structure.
	 * Nead for first setup or recovery
	 */
	
	QSqlDatabase db = QSqlDatabase::database();
	
	//db.setDatabaseName(QMessageBox::tr("%1/.q4wine/db/generic.dat").arg(QDir::homePath()));

	if (!db.open()){
		qDebug()<<"[EE] Sorry, i can't open database file by path: ~/.q4wine/db/generic.dat\".\n\n"
				"May be database is not correctly installed...";
					
		return FALSE;
	}
	
	QSqlQuery query;
			
	if(!query.exec("CREATE TABLE prefix (wine_dllpath TEXT, wine_loader TEXT, wine_server TEXT, wine_exec TEXT, cdrom_mount TEXT, cdrom_drive TEXT, id INTEGER PRIMARY KEY, name TEXT, path TEXT, version TEXT);"))
	{
		qDebug() << "Failed to create table 'dir'" << query.lastError();
		return FALSE;
	}
	
	if(!query.exec("CREATE TABLE icon (wrkdir TEXT, override TEXT, winedebug TEXT, useconsole NUMERIC, display TEXT, mount TEXT, image TEXT, cmdargs TEXT, exec TEXT, icon_path TEXT, desc TEXT, dir_id NUMERIC, id INTEGER PRIMARY KEY, name TEXT, prefix_id NUMERIC);"))
	{
		qDebug() << "Failed to create table 'dir'" << query.lastError();
		return FALSE;
	}
	
	if(!query.exec("CREATE TABLE dir (id INTEGER PRIMARY KEY, name TEXT, prefix_id NUMERIC);"))
	{
		qDebug() << "Failed to create table 'dir'" << query.lastError();
		return FALSE;
	}


	return TRUE;	
}


#endif
