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

#ifndef DIR_H
#define DIR_H

#include "config.h"

#include <QString>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVariant>

/*!
 * \class Dir
 * \ingroup database
 * \brief This class provide database functions for Dir table.
 *
 */
class Dir
{
public:
   //! Class constructor
	Dir();

   /*! \brief This function gets all table fields by prefixId key value from table.
	*
	* \param  prefixId    A prefixid value.
	* \return Return a list of table fields value or -1 on error.
	*/
    QStringList getDirList(const QString prefix_name) const;

   /*! \brief This function deletes all dirs fields by prefix_name keys value from table.
	*
	* \param  prefix_name    A prefix name value.
	* \return Return true on success.
	*/
	bool delDir(const QString prefix_name, const QString dir_name = "") const;

	/*! \brief This function add dir by name and prefix_name.
	 *
	 * \param  prefix_name    A prefix name value.
	 * \param  dir_name    A name value.
	 * \return Return true if exists.
	 */
	bool addDir(const QString prefix_name, const QString dir_name) const;

	/*! \brief This function check is reccord exists by dir_name or not.
	 *
	 * \param  prefix_name    A prefix name value.
	 * \param  dir_name		 A dir name value.
	 * \return Return true if exists.
	 */
	bool isExistsByName(const QString prefix_name, const QString dir_name) const;

	/*! \brief This function gets prefix id by name.
	 *
	 * \param  dir_name		  A dir name value.
	 * \param  prefix_name    A prefix name value.
	 * \return Return prefix id.
	 */
    QString getId(const QString dir_name, const QString prefix_name) const;

   /*! \brief Rename dir.
	*
	* \param  prefix_name		A prefix name value.
	* \param  dir_name			A dir name value.
	* \param  new_dir_name     A icon name value.
	* \return Return a list of table fields.
	*/
	bool renameDir(const QString dir_name, const QString prefix_name, const QString new_dir_name) const;



};

#endif // DIR_H
