/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010 by Malakhov Alexey                                 *
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

#ifndef LINKITEMWIDGET_H
#define LINKITEMWIDGET_H

//Global config
#include "config.h"

//Qt inc
#include <QLabel>
#include <QEvent>

class LinkItemWidget : public QLabel
{
	Q_OBJECT
public:
	LinkItemWidget(QString text, short int action = 0, QWidget *parent = 0);
	~LinkItemWidget();

	void setEnabled(bool enable);
	void setBold(bool enable);
	void setAction(short int action);
	void setSearchUrl(QString url);
	void setAppId(int id);
	void setVerId(int id);
	void setCatId(int id);
	void setPage(short int id);
	void setParentId(int id);

private:
	short int action;
	int appid;
	int verid;
	int catid;
	int page;
	int parentid;
	QString search;

signals:
	void itemTrigged(short int, QString, int, int, int);
	void requestParentComment(int id);

private:
	//! \brief Event filter.
	bool eventFilter(QObject *obj, QEvent *event);
};

#endif // LINKITEMWIDGET_H
