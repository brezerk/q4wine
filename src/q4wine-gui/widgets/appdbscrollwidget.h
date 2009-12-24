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
 ***************************************************************************/

#ifndef APPDBSCROLLWIDGET_H
#define APPDBSCROLLWIDGET_H

#include <memory>

#include "config.h"

#include <QDialog>
#include <QObject>
#include <QWidget>
#include <QString>
#include <QDebug>
#include <QScrollArea>
#include <QVBoxLayout>

#include "appdbstructs.h"
#include "appdbsearchwidget.h"
#include "appdbtestviewwidget.h"
#include "appdbappversionwidget.h"

class AppDBScrollWidget : public QScrollArea
{
	Q_OBJECT
public:
	AppDBScrollWidget(QWidget * parent = 0);
	void addSearchWidget(const WineAppDBInfo appinfo);
	void addTestWidget(const WineAppDBInfo appinfo);
	void addVersionFrame(QList<WineAppDBCategory> list, QString frame_caption, short int action);
	void insertStretch(void);
	void clear(void);

public slots:
	void scrollToPos(int id);

signals:
	void itemTrigged(short int, QString, int, int, int);

private:
	void gotoCommentId(int id);
	void hideAll(void);

	std::auto_ptr<QWidget> contentWidget;
	std::auto_ptr<QVBoxLayout> contentLayout;

};

#endif // APPDBSCROLLWIDGET_H
