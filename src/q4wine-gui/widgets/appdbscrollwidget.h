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

#ifndef APPDBSCROLLWIDGET_H
#define APPDBSCROLLWIDGET_H

#include <memory>

#include "config.h"

#include <QTimer>
#include <QDialog>
#include <QObject>
#include <QWidget>
#include <QString>
#include <QDebug>
#include <QScrollArea>
#include <QVBoxLayout>

#include "httpcore.h"

#include "xmlparser.h"
#include "appdbstructs.h"
#include "appdbheaderwidget.h"
#include "appdbsearchwidget.h"
#include "appdbtestviewwidget.h"
#include "appdbappversionwidget.h"

class AppDBScrollWidget : public QScrollArea
{
	Q_OBJECT
public:
	AppDBScrollWidget(AppDBHeaderWidget *appdbHeader, QWidget * parent = 0);

	//FIXME: Use signal\slot model!!!
	void startSearch(QString search);

public slots:
	void versionTrigged(short int action, int appid, int verid, int testid);
	void linkTrigged(short int action, QString search="", int val1=0, int val2=0);
	void pageReaded(void);

private slots:
	void update();

private:
	int appid;
	int catid;
	int verid;
	int testid;
	int page;

	void addSearchWidget(const WineAppDBInfo appinfo);
	void addTestWidget(const WineAppDBInfo *appinfo);
	void addVersionFrame(QList<WineAppDBCategory> list, QString frame_caption, short int action);
	void gotoCommentId(int id);
	void showXmlError(int id);
	void insertStretch(void);
	void clear(void);
	void hideAll(void);

	std::auto_ptr<QTimer> timer;
	std::auto_ptr<QWidget> contentWidget;
	std::auto_ptr<QVBoxLayout> contentLayout;

	//FIXME: See no 8 draft page 133-253
	//std::auto_ptr<AppDBHeaderWidget> appdbHeader;
	AppDBHeaderWidget *appdbHeader;

	std::auto_ptr<XmlParser> xmlparser;
	std::auto_ptr<HttpCore> httpcore;

	short int _ACTION;
	QString _SEARCH;
};

#endif // APPDBSCROLLWIDGET_H
