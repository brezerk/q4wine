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

#ifndef DRAGLISTWIDGET_H
#define DRAGLISTWIDGET_H

#include "memory"

#include "config.h"

#include "src/core/database/prefix.h"
#include "src/core/database/dir.h"
#include "src/core/database/icon.h"
#include "src/core/database/last_run_icon.h"

#include "run.h"

#include "iconsettings.h"

#include <QListWidget>
#include <QMouseEvent>
#include <QPoint>
#include <QApplication>
#include <QUrl>
#include <QAction>
#include <QMenu>
#include <QLibrary>
#ifdef DEBUG
#include <QDebug>
#endif

#include <q4wine-lib/main.h>

struct iconCopyBuffer {
	QString dir_name;
	QString prefix_name;
	bool move;
	QStringList names;
};

class DragListWidget : public QListWidget
{
	Q_OBJECT
public:
	DragListWidget(QString themeName, QWidget *parent = 0);

public slots:
	void showFolderContents(QString prefixName, QString dirName, QString filter);

private:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dropEvent(QDropEvent *event);
	void contextMenuEvent(QContextMenuEvent *event);

	QIcon loadIcon(QString iconName);

	QString themeName;
	QPoint startPos;
	bool drag, dragstarted;
	QString prefixName, dirName;

	// Database classes
	Prefix db_prefix;
	Dir db_dir;
	Icon db_icon;

	// Copy/Paste buffer
	iconCopyBuffer iconBuffer;

	//! This is need for libq4wine-core.so import.
	QLibrary libq4wine;
	typedef void *CoreLibPrototype (bool);
	CoreLibPrototype *CoreLibClassPointer;
	std::auto_ptr<corelib> CoreLib;

private slots:
	void itemClicked (QListWidgetItem *item);
	void itemDoubleClicked (QListWidgetItem *item);

	void iconAdd_Click(void);
	void iconRename_Click(void);
	void iconDelete_Click(void);
	void iconRun_Click(void);
	void iconCut_Click(void);
	void iconCopy_Click(void);
	void iconPaste_Click(void);
	void iconOption_Click(void);

	void menuRun_triggered(QAction*);

signals:
	void startDrag();
	void startDrop(QList<QUrl> files);
	void iconItemClick(QString, QString, QString, QString, QString);
	void changeStatusText(QString);
};

#endif // DRAGLISTWIDGET_H