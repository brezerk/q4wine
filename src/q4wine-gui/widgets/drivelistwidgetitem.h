/***************************************************************************
 *   Copyright (C) 2008-2010 by Malakhov Alexey                            *
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

#ifndef DRIVELISTWIDGETITEM_H
#define DRIVELISTWIDGETITEM_H

#include "config.h"

#include <QIcon>
#include <QListWidget>
#include <QDebug>
#include <QString>
#include <QListWidgetItem>

class DriveListWidgetItem : public QObject, public QListWidgetItem
{
Q_OBJECT
public:
	DriveListWidgetItem(QString themeName, QListWidget * parent = 0, int type = QListWidgetItem::UserType);

	void setDrive(QString letter, QString path, QString type);
	QString getLetter();
	QString getPath();
	QString getType();
signals:

public slots:

private:
	QString themeName, letter, path, type;
	QString getDrivePic(QString driveType);
	QIcon loadIcon(QString iconName);

};

#endif // DRIVELISTWIDGETITEM_H
