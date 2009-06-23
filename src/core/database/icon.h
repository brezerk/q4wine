/***************************************************************************
 *   Copyright (C) 2008, 2009 by Malakhov Alexey                           *
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

#ifndef ICON_H
#define ICON_H

#include "config.h"
#include "prefix.h"

#include <QList>
#include <QString>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVariant>

/*!
 * \class Icon
 * \ingroup database
 * \brief This class provide database functions for Icon table.
 *
 */
class Icon : public Prefix
{
public:
	//! Class constructor
    Icon();

   /*! \brief This function gets all icons fields by prefix_id and dir_id key value from table.
	*
	* \param  prefix_id    A prefixid value.
	* \param  dir_id	   A prefixid value.
	* \return Return a list of table fields value or -1 on error.
	*/
	QList<QStringList> getIconsInfo(const QString prefix_id, const QString dir_id) const;

   /*! \brief This function gets all icons fields by prefixId key value from table.
	*
	* \param  prefix_id      A prefix id value.
	* \return Return a list of table fields value or -1 on error.
	*/
	QList<QStringList> getIconsInfo(const QString prefix_id) const;

   /*! \brief This function gets all icons fields by prefix_name and dir_name (optional by icon name) keys value from table.
	*
	* \param  prefix_name    A prefix name value.
	* \param  dir_name		 A dir name value.
	* \param  icon_name      A icon name value (optional).
	* \return Return a list of table fields value or -1 on error.
	*/
	QList<QStringList> getIconsInfoByNames(const QString prefix_name, const QString dir_name, const QString icon_name = "") const;

	QStringList getByName(const QString prefix_name, const QString dir_name, const QString icon_name = "") const;

	bool copyIcon(const QString icon_name, const QString prefix_name, const QString dir_name, const QString new_icon_name, const QString new_prefix_name, const QString new_dir_name) const;

	bool renameIcon(const QString icon_name, const QString prefix_name, const QString dir_name, const QString new_icon_name) const;



   /*! \brief This function gets all icons fields by prefix_name (optional by icon name) keys value from table.
	*
	* \param  prefix_name    A prefix name value.
	* \param  icon_name      A icon name value (optional).
	* \return Return a list of table fields value or -1 on error.
	*/
	QList<QStringList> getIconsInfoByPrefixName(const QString prefix_name, const QString icon_name = "") const;

   /*! \brief This function deletes all icons fields by prefix_name keys value from table.
	*
	* \param  prefix_name    A prefix name value.
	* \return Return true on success.
	*/
	bool delIconsByPrefixName(const QString prefix_name) const;

   /*! \brief This function delete icon field by prefix_name key value from table.
	*
	* \param  prefix_name    A prefix name value.
	* \return Return true on success.
	*/
	bool delIcon(const QString prefix_name, const QString icon_name) const;

   /*! \brief This function delete icon field by prefix_name and dir_name key value from table.
	*
	* \param  prefix_name    A prefix name value.
	* \param  dir_name		 A dir name value.
	* \return Return true on success.
	*/
	bool delIcon(const QString prefix_name, const QString dir_name, const QString icon_name) const;

	/*! \brief This function check is reccord exists by icon_name or not.
	 *
	 * \param  prefix_name    A prefix name value.
	 * \param  name    A name value.
	 * \return Return true if exists.
	 */
	bool isExistsByName(const QString prefix_name, const QString icon_name) const;

	/*! \brief This function check is reccord exists by icon_name or not.
	 *
	 * \param  prefix_name    A prefix name value.
  	 * \param  dir_name		 A dir name value.
	 * \param  icon_name    A name value.
	 * \return Return true if exists.
	 */
	bool isExistsByName(const QString prefix_name, const QString dir_name, const QString icon_name) const;

	/*! \brief This function add icon record.
	 *
	 * \param  cmdargs		A command line arguments value.
  	 * \param  exec			A exec binary value.
	 * \param  icon_path	A icon name or path value.
  	 * \param  desc			A description value.
  	 * \param  prefix_name  A dir name value.
  	 * \param  dir_name		A dir name value.
  	 * \param  name			A dir name value.
	 * \return Return true on success.
	 */
	bool addIcon(const QString cmdargs, const QString exec, const QString icon_path, const QString desc, const QString prefix_name, const QString dir_name, const QString name, const QString override="", const QString winedebug="", const QString useconsole="", const QString display="", const QString wrkdir="", const QString desktop="", const int nice=0) const;

  	/*! \brief This function updates icon record by name, prefix_name and dir_name.
	 *
	 * \param  cmdargs		A command line arguments value.
  	 * \param  exec			A exec binary value.
	 * \param  icon_path	A icon name or path value.
  	 * \param  desc			A description value.
  	 * \param  prefix_name  A dir name value.
  	 * \param  dir_name		A dir name value.
  	 * \param  name			A dir name value.
	 * \return Return true on success.
	 */
	bool updateIcon(const QString cmdargs, const QString exec, const QString icon_path, const QString desc, const QString prefix_name, const QString dir_name, const QString name, const QString icon_name, const QString override="", const QString winedebug="", const QString useconsole="", const QString display="", const QString wrkdir="", const QString desktop="", const int nice=0) const;

	/*! \brief This function updates name, prefix_name and dir_name by old_name.
	 *
	 * \param  icon_name		A icon name value.
  	 * \param  prefix_name		A dir name value.
  	 * \param  dir_name			A dir name value.
  	 * \param  old_icon_name	A old icon name value.
	 * \return Return true on success.
	 */
	bool updateIcon(const QString icon_name, const QString prefix_id, const QString dir_id, const QString old_prefix_id, const QString old_dir_id, const QString old_icon_name) const;



 };

#endif // ICON_H
