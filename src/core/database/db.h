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

/*!
 * \defgroup database Q4Wine database core
 * \brief Database core pakage provide general database functions for q4wine.
 */

#ifndef DB_H
#define DB_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRecord>
#include <QStringList>
#include <QSqlQuery>
#include <QDebug>
#include <QDir>
#include <QSqlRelation>
#include <QMessageBox>
#include <QSqlRelationalTableModel>

#include "config.h"

/*!
 * \class DataBase
 * \ingroup database
 * \brief This class provide general database functions for q4wine.
 *
 * It is not useful by itself, but it creates database connections
 * and provides basic database check.
 *
 */
class DataBase
{
public:
   //! Constructor
   DataBase();

   /*! \brief This function tries to check database structure.
    *
    * It gets process values: pid, name, nice priority and WINEPREFIX environment variable.
    * \param  tables  List of tables name.
    * \return Return true on success otherwise false.
    */
   bool checkDb(const QStringList tables) const;

   /*! \brief This function close database connection.
    */
   void close();
};

#endif // DB_H
