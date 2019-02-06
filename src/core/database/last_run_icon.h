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

#ifndef LAST_RUN_ICON_H
#define LAST_RUN_ICON_H

#include "config.h"

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
class Last_Run_Icon
{
public:
    //! Class constructor
    Last_Run_Icon();

    /*! \brief This function gets all icons fields by prefix_name (optional by icon name) keys value from table.
    *
    * \param  exec			A exec binary value.
    * \return Return a list of table fields value.
    */
    QStringList getByExec(const QString &exec) const;

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
    bool addIcon(const QString &cmdargs,
                 const QString &exec,
                 const QString &override="",
                 const QString &winedebug="",
                 const QString &useconsole="",
                 const QString &display="",
                 const QString &wrkdir="",
                 const QString &desktop="",
                 const int nice=0,
                 const QString &lang="") const;

    /*! \brief This function reurns list of recent launched icons.
     *
     * \return Return list of last launched icons.
     */
    QStringList getIcons();

};

#endif // LAST_RUN_ICON_H
