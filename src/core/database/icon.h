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
	 * \param  name    A name value.
	 * \return Return true if exists.
	 */
	bool isExistsByName(const QString prefix_name, const QString dir_name, const QString icon_name) const;
};

#endif // ICON_H
