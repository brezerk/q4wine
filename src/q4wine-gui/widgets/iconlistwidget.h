/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010 by Malakhov Alexey                     *
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

#ifndef ICONLISTWIDGET_H
#define ICONLISTWIDGET_H

//System
#include <memory>

//Global config
#include "config.h"

//Database
#include "prefix.h"
#include "dir.h"
#include "icon.h"
#include "last_run_icon.h"

//Windows
#include "run.h"
#include "iconsettings.h"

//Qt includes
#include <QMenu>
#include <QDesktopServices>
#include <QListView>

#ifdef DEBUG
#include <QDebug>
#endif

//q4ine lib
#include "q4wine-lib.h"

struct iconCopyBuffer {
	  QString dir_name;
	  QString prefix_name;
	  bool move;
	  QStringList names;
};

class IconListWidget : public QListWidget
{
	  Q_OBJECT
public:
	  explicit IconListWidget(QWidget *parent = 0);
      ~IconListWidget();

public slots:
	  void showFolderContents(QString prefixName="", QString dirName="");
	  void setFilterString(QString filterString);
      void changeView(int action);
      void iconPaste_Click(void);

private:
	  void mousePressEvent(QMouseEvent *event);
	  void mouseMoveEvent(QMouseEvent *event);
	  void dragEnterEvent(QDragEnterEvent *event);
	  void dragMoveEvent(QDragMoveEvent *event);
	  void dropEvent(QDropEvent *event);
	  void contextMenuEvent(QContextMenuEvent *event);
      void keyPressEvent (QKeyEvent * event );

      void setDisplayType(bool icon=true);

	  QString prefixName, dirName, prefixMediaDrive, prefixMontPoint, filterString;
	  QPoint startPos;
      bool drag, dragstarted;

	  // Database classes
	  Prefix db_prefix;
	  Dir db_dir;
	  Icon db_icon;
	  Image db_image;

	  // Copy/Paste buffer
	  iconCopyBuffer iconBuffer;

	  //! This is need for libq4wine-core.so import.
	  QLibrary libq4wine;
	  typedef void *CoreLibPrototype (bool);
	  CoreLibPrototype *CoreLibClassPointer;
	  std::auto_ptr<corelib> CoreLib;

private slots:
	  void showContents(QString filterString);

	  void startDrag();
	  void startDrop(QList<QUrl> files);

	  void itemClicked (QListWidgetItem *item);
	  void itemDoubleClicked (QListWidgetItem *item);

	  void iconAdd_Click(void);
	  void iconRename_Click(void);
	  void iconDelete_Click(void);
	  void iconRun_Click(void);
	  void iconCut_Click(void);
	  void iconCopy_Click(void);
	  void iconOption_Click(void);
	  void iconSearchAppDB_Click(void);

	  void menuRun_triggered(QAction*);
	  void menuMount_triggered(QAction*);
	  void menuUmount_Click(void);

	  void xdgOpenPrefixDir_Click(void);
	  void xdgOpenMountDir_Click(void);
	  void xdgOpenIconDir_Click(void);

	  void winefileOpenPrefixDir_Click(void);
	  void winefileOpenMountDir_Click(void);
	  void winefileOpenIconDir_Click(void);

signals:
	  void iconItemClick(QString, QString, QString, QString, QString);
	  void changeStatusText(QString);
	  void searchRequest(QString);
};

#endif // ICONLISTWIDGET_H
