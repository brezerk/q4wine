/***************************************************************************
 *   Copyright (C) 2008-2020 by Oleksii S. Malakhov <brezerk@gmail.com>    *
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

/*!
 * \defgroup database Q4Wine database core
 * \brief Database core package provides general database functions for Q4Wine.
 */

#ifndef DB_H
#define DB_H

#include <QStandardPaths>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>
#include <QTextStream>
#include <QString>
#include <QVariant>
#include <QDebug>
#include <QDir>
#include <QObject>
#include <QSettings>

#include "config.h"
#include "core/database/versions.h"

/*!
 * \class DataBase
 * \ingroup database
 * \brief This class provide general database functions for q4wine.
 *
 * It is not useful by itself, but it creates database connections
 * and provides basic database check.
 *
 */
class DataBase : public QObject
{
    Q_OBJECT
public:
    //! Constructor
    explicit DataBase(QObject * parent = 0);

    /*! \brief This function tries to check database structure.
	*
	* It gets process values: pid, name, nice priority and WINEPREFIX environment variable.
	* \param  tables  List of tables name.
	* \return Return true on success otherwise false.
	*/
    bool checkDb();

    /*! \brief This function close database connection.
	*/
    void close();
    bool fixup();

};

#endif // DB_H
