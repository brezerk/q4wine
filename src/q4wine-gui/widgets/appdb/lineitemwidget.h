/***************************************************************************
 *   Copyright (C) 2008-2017 by Oleksii S. Malakhov <brezerk@gmail.com>     *
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

#ifndef LINEITEMWIDGET_H
#define LINEITEMWIDGET_H

//System
#include <memory>

//Global config
#include "config.h"

//Qt inc
#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QEvent>

#ifdef DEBUG
#include <QDebug>
#endif

//Structs
#include "appdbstructs.h"

/*!
 * \class LineItemWidget
 * \ingroup widgets
 * \brief This class provide database functions for AppDB search widget.
 *
 */

class LineItemWidget : public QWidget
{
Q_OBJECT
public:
	//! Class constructor
	LineItemWidget(const short int action, QWidget *parent = 0);

	//! Class destructor
	~LineItemWidget();

	void addLabel(const QString text, const short int width=-1, const short int aligment=0, const bool worldwarp=false);
	void insertStretch(void);
	void setBold(void);

	void setAction(short int action);
	void setAppId(int id);
	void setAppVerId(int id);
	void setTestId(int id);
	void setEnabled(bool enable);

private:
	std::auto_ptr<QBoxLayout> contentLayout;
	int appid;
	int verid;
	int testid;
	short int action;

signals:
	void itemTrigged(short int, QString, int, int, int);

protected:
	//! \brief Event filter.
	bool eventFilter(QObject *obj, QEvent *event);

};

#endif // LINEITEMWIDGET_H
