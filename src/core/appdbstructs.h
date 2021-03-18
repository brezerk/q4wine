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

#ifndef APPDBSTRUCTS_H
#define APPDBSTRUCTS_H

#include <QString>
#include <QList>

struct WineAppDBVersionInfo {
	int id;
	QString appver;
	QString winever;
	QString rating;
};

struct WineAppDBTestResult {
	int id;
	bool current;
	QString distrib;
	QString date;
	QString winever;
	bool run;
	bool install;
	QString rating;
};

struct WineAppDBComment {
	int id;
	QString topic;
	QString date;
	QString author;
	int parent_id;
	QString message;
};

struct WineAppDBBug {
	int id;
	QString desc;
	QString status;
	QString resolution;
};

struct WineAppDBCategory {
	int id;
	QString name;
	QString desc;
	bool enabled;
};

struct WineAppDBInfo {
	int id;
	int test_id;
	int ver_id;
	QString name;
	QString desc;
	QString url;
	QString appver;
	QString winever;
	QString rating;
	QString works;
	QString notworks;
	QString nottested;
	QString comment;
	QString license;
	QList<WineAppDBTestResult> tests;
	QList<WineAppDBBug> bugs;
	QList<WineAppDBCategory> categorys;
	QList<WineAppDBComment> comments;
	QList<WineAppDBVersionInfo> versions;
};


#endif // APPDBSTRUCTS_H
