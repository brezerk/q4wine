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
#ifndef _OS_DARWIN_
#include "sysmenu.h"
#endif

//Qt includes
#include <QDrag>
#include <QMenu>
#include <QMimeData>
#include <QDesktopServices>
#include <QListView>
#include <QClipboard>

#ifdef DEBUG
#include <QDebug>
#endif

//q4wine lib
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
      explicit IconListWidget(QWidget *parent = nullptr);
      ~IconListWidget();

public slots:
      void showFolderContents(QString prefixName="", QString dirName="");
      void setFilterString(QString filterString);
      void changeView(int action);
      void iconPaste_Click(void);
      void runProgramRequest(QString);

private:
      void mousePressEvent(QMouseEvent *event);
#ifndef _OS_DARWIN_
      void mouseMoveEvent(QMouseEvent *event);
#endif
      void dragEnterEvent(QDragEnterEvent *event);
      void dragMoveEvent(QDragMoveEvent *event);
      void dropEvent(QDropEvent *event);
      void contextMenuEvent(QContextMenuEvent *event);
      void keyPressEvent (QKeyEvent * event );

      void setDisplayType(int type);

      QString prefixName, dirName, prefixMediaDrive, prefixMontPoint, filterString;
      QPoint startPos;
      QStringList cdromDevices;
      bool drag, dragstarted;
      int sort_order;
      int view_mode;

      // Database classes
      Prefix db_prefix;
      Dir db_dir;
      Icon db_icon;
      Image db_image;
#ifndef _OS_DARWIN_
      system_menu sys_menu;
#endif

      // Copy/Paste buffer
      iconCopyBuffer iconBuffer;

      //! This is need for libq4wine-core.so import.
      QLibrary libq4wine;
      typedef void *CoreLibPrototype (bool);
      CoreLibPrototype *CoreLibClassPointer;
      std::unique_ptr<corelib> CoreLib;

private slots:
      void showContents(QString filterString);

      void startDrag();
      void startDrop(QList<QUrl> files);

      void itemClicked (QListWidgetItem *item);
      void itemDoubleClicked (QListWidgetItem *item);
      void itemChanged (QListWidgetItem *item, QListWidgetItem *);

      void iconAdd_Click(void);
      void iconRename_Click(void);
      void iconDelete_Click(void);
      void iconRun_Click(void);
      void iconCut_Click(void);
      void iconCopy_Click(void);
      void iconOption_Click(void);
      void iconSearchAppDB_Click(void);
      void iconCopyWrkDir_Click(void);
      void iconCopyProgramPath_Click(void);
      void iconCopyQ4WineCmd_Click(void);


      void menuRunRecent_triggered(QAction* action);
      void menuMount_triggered(QAction*);
      void menuUmount_Click(void);

      void xdgOpenPrefixDir_Click(void);
      void xdgOpenMountDir_Click(void);
      void xdgOpenIconDir_Click(void);

      void winefileOpenPrefixDir_Click(void);
      void winefileOpenMountDir_Click(void);
      void winefileOpenIconDir_Click(void);

      void consoleToPrefixDir_Click(void);
      void consoleToMountDir_Click(void);
      void consoleToIconDir_Click(void);

signals:
      void iconItemClick(QString, QString, QString, QString, QString);
      void changeStatusText(QString);
      void searchRequest(QString);
      void appRunned(bool state);
};

#endif // ICONLISTWIDGET_H
