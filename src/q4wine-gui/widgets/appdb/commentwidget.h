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

#ifndef COMMENTWIDGET_H
#define COMMENTWIDGET_H

#include <src/q4wine-gui/ui_CommentWidget.h>

//System
#include <memory>

//Global config
#include "config.h"

//Qt inc
#ifdef DEBUG
#include <QDebug>
#endif

//Structs
#include "appdbstructs.h"

//Widgets
#include "linkitemwidget.h"
#include "lineitemwidget.h"

class CommentWidget : public QFrame, public Ui::CommentWidget
{
	Q_OBJECT
public:
	CommentWidget(const WineAppDBComment comment, QWidget * parent = 0);
	void setId(int id);
	void setParentId(int id);
	bool isId(int id);

signals:
	void itemTrigged(short int, QString, int, int, int);
	void requestParentComment(int id);

private:
	void setTopic(QString topic, int type);
	void setDate(QString autor, QString date);
	void setMessage(QString message);
	int id;
	int parentid;
};

#endif // COMMENTWIDGET_H
