/***************************************************************************
 *   Copyright (C) 2008-2016 by Oleksii S. Malakhov <brezerk@gmail.com>     *
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

#ifndef APPTESTWIDGET_H
#define APPTESTWIDGET_H

#include <src/q4wine-gui/ui_AppTestWidget.h>

//Global config
#include "config.h"

//structs
#include "appdbstructs.h"

//widgets
#include "linkitemwidget.h"
#include "commentwidget.h"
#include "lineitemwidget.h"

//Qt inc
#ifdef DEBUG
#include <QDebug>
#endif

class AppTestWidget : public QWidget, public Ui::AppTestWidget
{
Q_OBJECT
public:
	AppTestWidget(const WineAppDBInfo appinfo, QWidget *parent = 0);

public slots:
	void requestParentComment(int id);

signals:
	void itemTrigged(short int, QString, int, int, int);
	void scrollToPos(int);

private:
	/*! \brief sets general application Name
	*
	* \param  name         General application name.
	* \return Nothing.
	*/
	void setAppName(QString name);

	/*! \brief sets general application description and trim it to 255 chars
	*
	* \param  desc  Short Application description.
	* \return Nothing.
	*/
	void setAppDesc(QString desc);

	void addTestResults(QList<WineAppDBTestResult> tests);
	void addBugs(QList<WineAppDBBug> bugs);
	void addComments(QList<WineAppDBComment> comments);
	int appid;
	int verid;
	int testid;
};

#endif // APPTESTWIDGET_H
