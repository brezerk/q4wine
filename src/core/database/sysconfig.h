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

#ifndef SYSCONFIG_H
#define SYSCONFIG_H

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
 * \class Sysconfig
 * \ingroup database
 * \brief This class provide database functions for sysconfig table.
 *
 */
struct ProviderItem {
    int id;
    QString name;
    QString icon;
};

struct SysconfigItem {
    int id;
    QString name;
    QString icon;
    QString type;
    QString desc;
    int provider_id;
    bool is_installed;
};

class Sysconfig
{

  public:
     /*! \brief This is class constructor.
      */
      Sysconfig();

      QList<ProviderItem> getProviders(void) const;
      QStringList getProviderSubtypes(int provider_id) const;
      QList<SysconfigItem> getItems(QString provider, QString type, int sort_order, QString filter) const;
      bool add_item(QString name, QString icon, QString desc, QString type, int provider_id, bool is_installed);
      bool drop_items(int provider_id);

      bool begin();
      bool commit();
      bool vacuum();

protected:
      /*! \brief This function executes requested query.
      *
      * \param  SQL Query
      * \return Return true on success
      */
      bool updateQuery(QSqlQuery *sqlQuery) const;

};

#endif // SYSCONFIG_H
