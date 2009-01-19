/***************************************************************************
 *   Copyright (C) 2009 by Pavel Zinin (pashazz)                           *
 *   pzinin@gmail.com                                                      *
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

#ifndef COREMETHODS_H
#define COREMETHODS_H

#include "config.h"
#include "wisitem.h"
#include <QProcess>
#include <QStringList>
#include <QtCore>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDir>
#include <QSqlError>
#include <QXmlStreamReader>

class CoreMethods : public QObject
{
public:
	/* Search for fileName in PATH with /usr/bin/which */
	QString getWhichOut (QString fileName);
	QString getPrefixPath(QString prefixName);
	WisItem getWisInfo (QString fileName);
	QString getSettingValue(QString group, QString key);
	CoreMethods ();
	private:

	QProcess *proc;
};

#endif // COREMETHODS_H
