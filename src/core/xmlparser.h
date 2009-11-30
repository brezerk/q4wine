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

#ifndef XMLPARSER_H
#define XMLPARSER_H

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
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>

#include "appdbstructs.h"

/*!
 * \class Registry
 * \ingroup registry
 * \brief This class provide general registry functions for q4wine.
 *
 * This class can create registry files to export via wine regedit.
 * Also this class can read keys value from wine registry.
 *
 */

class XmlParser : public QObject
{
Q_OBJECT
public:
	XmlParser(void);
	int parseIOSream(QString filename);
	QList<WineAppDBInfo> _APPDB_SEARCH_INFO;
	WineAppDBTestInfo _APPDB_TEST_INFO;

	short int _ACTION;
	short int _PAGE_CURRENT;
	short int _PAGE_COUNT;
private:
	bool parseEntry(const QDomElement &element);
	void parseApp(const QDomElement &element);
	void parseAppTest(const QDomElement &element);
	void parseTestList(const QDomElement &element);
	void parseTest(const QDomElement &element);
	void parseBugList(const QDomElement &element);
	void parseBug(const QDomElement &element);
	void parseAppVersionsList(const QDomElement &element, WineAppDBInfo &appinfo);
	void parseAppVersion(const QDomElement &element, WineAppDBInfo &appinfo);
	QString getChildNodeData(const QDomNode &childNode);
	void clear();
};

#endif // XMLPARSER_H
