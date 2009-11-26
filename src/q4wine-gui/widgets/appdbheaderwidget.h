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

#ifndef APPDBHEADERWIDGET_H
#define APPDBHEADERWIDGET_H

#include "config.h"

#include <QObject>
#include <QWidget>
#include <QFrame>
#include <QString>
#include <QLabel>
#include <QEvent>
#include <QVariant>
#include <QDebug>
#include <QVBoxLayout>

#include "appdblinkitemwidget.h"

class AppDBHeaderWidget : public QFrame
{
	Q_OBJECT
public:
	AppDBHeaderWidget(QWidget * parent = 0);
	void addLabel(QString info);
	void addLink(QString text, bool enabled = true, short int action = 0, QString search = "", int value = 0);
	void setLayout(short int direction);
	void insertStretch(void);
	void createPagesList(short int count, short int current, QString search);
	void clear(void);
	void hideAll(void);

signals:
	void linkTrigged(short int action, QString search, int value);

private:
	QBoxLayout *contentLayout;
	short int pages_len;

};

#endif // APPDBHEADERWIDGET_H
