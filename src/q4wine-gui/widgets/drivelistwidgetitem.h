/***************************************************************************
 *   Copyright (C) 2008-2021 by Oleksii S. Malakhov <brezerk@gmail.com>    *
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

//Global config
#include "config.h"

//Qt inc
#include <QListWidgetItem>

//q4wine lib
#include "q4wine-lib.h"

class DriveListWidgetItem : public QObject, public QListWidgetItem
{
Q_OBJECT
public:
	DriveListWidgetItem(QListWidget * parent = nullptr, int type = QListWidgetItem::UserType);

	void setDrive(QString letter, QString path, QString type);
	QString getLetter();
	QString getPath();
	QString getType();
signals:

public slots:

private:
	QString themeName, letter, path, type;
	QString getDrivePic(QString driveType);

	//! This is need for libq4wine-core.so import.
	QLibrary libq4wine;
	typedef void *CoreLibPrototype (bool);
	CoreLibPrototype *CoreLibClassPointer;
	std::unique_ptr<corelib> CoreLib;

};

#endif // DRIVELISTWIDGETITEM_H
