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

#include "config.h"

#include <QTimer>
#include <QDialog>
#include <QObject>
#include <QWidget>
#include <QString>
#include <QDebug>
#include <QScrollArea>
#include <QVBoxLayout>

#include "xmlparser.h"
#include "appdbstructs.h"
#include "appdbheaderwidget.h"
#include "appdbsearchwidget.h"
#include "appdbappversionwidget.h"

class AppDBScrollWidget : public QScrollArea
{
	Q_OBJECT
public:
	AppDBScrollWidget(AppDBHeaderWidget *appdbHeader, QWidget * parent = 0);
	void startSearch(short int action, QString search);

public slots:
	void versionTrigged(short int action, int appid, int verid, int testid);
	void linkTrigged(short int action, QString search, int value);

private slots:
	void update();

private:
	void addSearchWidget(WineAppDBInfo appinfo);
	void insertStretch(void);
	void clear(void);
	void hideAll(void);
	QTimer *timer;
	QWidget *contentWidget;
	QVBoxLayout *contentLayout;
	AppDBHeaderWidget *appdbHeader;

	short int _ACTION;
	QString _SEARCH;
};

#endif // APPDBSCROLLWIDGET_H
