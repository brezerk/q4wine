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

#ifndef PREFIXTREEWIDGET_H
#define PREFIXTREEWIDGET_H

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
#include "prefixsettings.h"
#include "fakedrivesettings.h"

#ifndef _OS_DARWIN_
#include "sysmenu.h"
#endif

//Qt includes
#include <QTreeWidgetItem>
#include <QMenu>
#include <QMimeData>
#include <QDesktopServices>

#ifdef DEBUG
#include <QDebug>
#endif

//q4wine lib
#include "q4wine-lib.h"

class PrefixTreeWidget : public QTreeWidget
{
Q_OBJECT
public:
      explicit PrefixTreeWidget(QWidget *parent = nullptr);
      ~PrefixTreeWidget();

private:
      void contextMenuEvent(QContextMenuEvent *event);
      void keyReleaseEvent ( QKeyEvent * event );


      virtual bool dropMimeData(QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action);
      QStringList mimeTypes() const;
      Qt::DropActions supportedDropActions () const;
      void moveDesktopFile(QUrl url, QString prefixName, QString dirName);
      void dragEnterEvent(QDragEnterEvent *event);

      QString prefixName, dirName, prefixMontPoint;
      QStringList cdromDevices;

      //! This is need for libq4wine-core.so import.
      QLibrary libq4wine;
      typedef void *CoreLibPrototype (bool);
      CoreLibPrototype *CoreLibClassPointer;
      std::unique_ptr<corelib> CoreLib;

      // Database classes
      Image db_image;
      Prefix db_prefix;
      Dir db_dir;
      Icon db_icon;
#ifndef _OS_DARWIN_
      system_menu sys_menu;
#endif

private slots:
      void itemClicked (QTreeWidgetItem *item, int);

      void dirAdd_Click(void);
      void dirRename_Click(void);
      void dirDelete_Click(void);
      void menuRun_triggered(QAction*);
      void menuMount_triggered(QAction*);
      void menuUmount_Click(void);

      void xdgOpenPrefixDir_Click(void);
      void xdgOpenMountDir_Click(void);

      void winefileOpenPrefixDir_Click(void);
      void winefileOpenMountDir_Click(void);

      void menuCreatePrefix_Click(void);
      void menuSetupPrefix_Click(void);
      void menuConfigurePrefix_Click(void);
      void menuDeletePrefix_Click(void);

      void consoleToPrefixDir_Click(void);
      void consoleToMountDir_Click(void);

signals:
      void showFolderContents(QString, QString);
      void changeStatusText(QString);
      void prefixIndexChanged(QString);
      void updateDatabaseConnections();
      void setSearchFocus(void);
      void setTabIndex (int);
      void pasteAction(void);

public slots:
      void getPrefixes();
      void setDefaultFocus(QString prefixName, QString dirName="");

      void expandTree();
      void collapseTree();

};

#endif // PREFIXTREEWIDGET_H
