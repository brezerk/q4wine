/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010, 2011 by Malakhov Alexey                                 *
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
 ***************************************************************************/

#ifndef XMLPARSER_H
#define XMLPARSER_H

#include "config.h"

#include <QObject>
#include <QRegExp>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>

#ifdef DEBUG
#include <QDebug>
#endif

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
	int parseIOSream(QString file);

	QList<WineAppDBInfo> getAppSearchInfoList();
	WineAppDBInfo getAppSearchInfo();

	short int getPageCount(void);
	short int getPageCurrent(void);

	QList<WineAppDBCategory> getSubCategorysList(void);
	QList<WineAppDBCategory> getCategorysList(void);
	QList<WineAppDBCategory> getAppsList(void);

private:
	short int page_current;
	short int page_count;
	QString action;

	QList<WineAppDBInfo> _APPDB_SEARCH_INFO;
	WineAppDBInfo appdb_appinfo;

	QList<WineAppDBCategory> appdb_subcategorys;
	QList<WineAppDBCategory> appdb_categorys;
	QList<WineAppDBCategory> appdb_apps;

	void parseAppSearchEntry(const QDomElement &element);
	void parseAppCategoryEntry(const QDomElement &element);
	int parseMessageEntry(const QDomElement &element);
	void parsePages(const QDomElement &element);

	void parseApp(const QDomElement &element);
	void parseAppVersion(const QDomElement &element, WineAppDBInfo &appinfo);

	WineAppDBCategory parseCategory(const QDomElement &element);
	void parseTestResults(const QDomElement &element, WineAppDBInfo &appinfo);
	void parseBug(const QDomElement &element, WineAppDBInfo &appinfo);
	void parseTest(const QDomElement &element, WineAppDBInfo &appinfo);
	void parseComment(const QDomElement &element, WineAppDBInfo &appinfo);



	QString getChildNodeData(const QDomNode &childNode);

	void clear();
};

#endif // XMLPARSER_H
