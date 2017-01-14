/***************************************************************************
 *   Copyright (C) 2008-2017 by Oleksii S. Malakhov <brezerk@gmail.com>     *
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

#ifndef PREFIX_H
#define PREFIX_H

#include <config.h>

#include <memory>

#include <QList>
#include <QString>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>
#include <QVariant>
#include <QSettings>

/*!
 * \class Prefix
 * \ingroup database
 * \brief This class provide database functions for prefix table.
 *
 */
class Prefix
{

  public:
     /*! \brief This is calss constructor.
      */
      Prefix();

     /*! \brief This function gets all table fields by prefix id key value from table.
      *
      * \param  prefix_id Prefix id key value.
      * \return Return a list of table fields value or -1 on error.
      */
      QHash<QString,QString> getByName(const QString prefix_name) const;
      QString getMountPoint(const QString prefix_name) const;
      QChar getMountPointWindrive(const QString prefix_name) const;
      QStringList getPrefixList(void) const;

     /*! \brief This function gets prefix id by name.
      *
      * \param  prefix_name    A prefix name value.
      * \return Return prefix id.
      */
     QString getId(const QString prefix_name) const;

     /*! \brief This function gets prefix path by name.
      *
      * \param  prefix_name    A prefix name value.
      * \return Return prefix path.
      */
      QString getPath(const QString prefix_name) const;
      QString getName(const QString prefix_path) const;

     /*! \brief This function deletes all prefix fields by prefix_name keys value from table.
      *
      * \param  prefix_name    A prefix name value.
      * \return Return true on success.
      */
      bool delByName(const QString prefix_name) const;

     /*! \brief This function deletes all prefix fields by prefix_name keys value from table.
      *
      * \param  prefix_name    A prefix name value.
      * \param  prefix_path    A prefix path value.
      * \param  wine_exec	   A wine executable binary.
      * \param  wine_server    A wine server binary.
      * \param  wine_loader    A wine loader binary.
      * \param  wine_dllpath   A wine library directory.
      * \param  cdrom_mount    A cdrom mount directory.
      * \return Return true on success.
      */
      bool addPrefix(const QString prefix_name, const QString prefix_path="", const QString wine_exec="", const QString wine_server="", const QString wine_loader="", const QString wine_dllpath="", const QString cdrom_mount="", const QString arch="", const QString mountpoint_windrive="", const QString run_string="", const QString version_id="") const;

     /*! \brief This function check is reccord exists by prefix name or not.
      *
      * \param  name    A prefix name value.
      * \return Return true if exists.
      */
      bool isExistsByName(const QString prefix_name) const;

      /*! \brief This function deletes all prefix fields by prefix_name keys value from table.
      *
      * \param  prefix_name        A prefix name value.
      * \param  prefix_path        A prefix path value.
      * \param  wine_exec	       A wine executable binary.
      * \param  wine_server        A wine server binary.
      * \param  wine_loader        A wine loader binary.
      * \param  wine_dllpath       A wine library directory.
      * \param  cdrom_mount        A cdrom mount directory.
      * \param  old_prefix_name	   A cdrom drive.
      * \param  mountpoint_windrive The windows drive to use for this mountpoint
      * \return Return true on success.
      */
      bool updatePrefix(const QString prefix_name, const QString prefix_path, const QString wine_exec, const QString wine_server, const QString wine_loader, const QString wine_dllpath, const QString cdrom_mount, const QString old_prefix_name, const QString arch, const QString mountpoint_windrive, const QString run_string, const QString version_id) const;

      void fixPrefixPath();
protected:
      /*! \brief This function executes requested query.
      *
      * \param  SQL Query
      * \return Return true on success
      */
      bool updateQuery(QSqlQuery *sqlQuery) const;

};

#endif // PREFIX_H
