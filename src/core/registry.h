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

/*!
 * \defgroup registry Q4Wine registry core
 * \brief Registry core pakage provide general registry functions like reading and wrighting for wine registry files.
 */

#ifndef REGISTRY_H
#define REGISTRY_H

#include <config.h>

#include <unistd.h>

#include <QString>
#include <QStringList>
#include <QVariant>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDir>
#include <QTime>
#include <QProcess>
#include <QDebug>
#include <QObject>
#include <QSettings>

#include "winebinlauncher.h"

/*!
 * \class Registry
 * \ingroup registry
 * \brief This class provide general registry functions for q4wine.
 *
 * This class can create registry files to export via wine regedit.
 * Also this class can read keys value from wine registry.
 *
 */
class Registry: public QObject
{
	Q_OBJECT
public:
	/*! \brief This is empty calss constructor.
	  */
	Registry();
	/*! \brief This is calss constructor.
	  * \param prefixPath Base prefix path. This is need to get wine system registry file path.
	  */
	Registry(QString prefixPath);

	/*! \brief Creates heade information for  exported reg file.
	  *
	  * \return ture on success.
	  */
	bool init();

	/*! \brief Appends path and keys to exported reg file.
	  * \param reg_keys QString of keys and it's values.
	  */
	void append(QString reg_keys);

	/*! \brief Executes regedit to import exported file into wine registry.
	  *
	  * \return ture on success.
	  */
	bool exec(QObject *parent, QString prefix_name = "Default");

	/*! \brief Read registry keys value from regfile by path.
	  * \param path Registry path.
	  * \param keys List of kays to be readed from registry.
	  *
	  * \return List of readed key's value. Index of readed keys identical to requested keys.
	  */
	QStringList readKeys(const QString sysfile, const QString path, const QStringList keys) const;

private:
	QString regfile;
	QString regfile_image;
};

#endif
