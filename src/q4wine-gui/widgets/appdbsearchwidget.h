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

#ifndef APPDBSEARCHWIDGET_H
#define APPDBSEARCHWIDGET_H

#include <ui_AppDBSearchWidget.h>

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

	AppDBSearchWidget(QString name, QString desc, QList<WineAppDBVersionInfo> &versions, const int appid, QWidget *parent = 0);

	//! \brief class destructor;
	~AppDBSearchWidget();
signals:
	 void linkTrigged(short int action, QString search, int value);
	 void versionTrigged(short int action, int appid, int verid, int testid);
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

	//! \brief This holds url description
	int _APPID;
protected:
	//! \brief Event filter.
	bool eventFilter(QObject *obj, QEvent *event);
};

#endif // APPDBSEARCHWIDGET_H
