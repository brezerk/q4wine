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

#ifndef APPDBSEARCHWIDGET_H
#define APPDBSEARCHWIDGET_H

#include <ui_AppDBSearchWidget.h>

#include "memory"

#include "config.h"

#include <QDialog>
#include <QObject>
#include <QWidget>
#include <QString>
#include <QDebug>

#include "appdbappversionwidget.h"
#include "appdbstructs.h"

/*!
 * \class AppDBSearchWidget
 * \ingroup widgets
 * \brief This class provide database functions for AppDB search widget.
 *
 */
class AppDBSearchWidget : public QWidget, public Ui::AppDBSearchWidget
{
Q_OBJECT
public:
	/*! \brief class constructor
	*
	* \param  name         General application name.
	* \param  desc  Short  Application description.
	* \param  versions     An QList of QStringList witch describes tested app versions.
	* \param  url	       Application url to open.
	*/

	AppDBSearchWidget(QString name, QString desc, const int appid, QList<WineAppDBVersionInfo> versions, QWidget *parent = 0);

	//! \brief class destructor;
	~AppDBSearchWidget();
signals:
	 void itemTrigged(short int, QString, int, int, int);

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

	int appid;
protected:
	//! \brief Event filter.
	bool eventFilter(QObject *obj, QEvent *event);
};

#endif // APPDBSEARCHWIDGET_H
