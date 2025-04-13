/***************************************************************************
 *   Copyright (C) 2008-2025 by Oleksii S. Malakhov <brezerk@gmail.com>    *
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

#ifndef VERSION_MANAGER_H
#define VERSION_MANAGER_H

#include <QAction>
#include <QDebug>
#include <QInputDialog>
#include <QKeyEvent>
#include <QLineEdit>
#include <QListWidget>
#include <QMenu>
#include <QSplitter>
#include <QTimer>
#include <QToolBar>
#include <QWidget>

// q4wine lib
#include "config.h"
#include "core/database/versions.h"
#include "q4wine-gui/widgets/versionlistwidget.h"
#include "q4wine-lib.h"
#include "src/q4wine-gui/ui_VersionManager.h"

class VersionManager : public QDialog, public Ui::VersionManager {
  Q_OBJECT
 public:
  VersionManager(QWidget *parent = nullptr,
                 Qt::WindowFlags f = Qt::WindowFlags());
  ~VersionManager();
  void setVersionFocus(QString version);
 signals:
  void setVersion(QString version_name);

 private slots:
  void cmdOk_Click();
  void cmdHelp_Click();
  void keyPressEvent(QKeyEvent *event);
  void itemClicked(QListWidgetItem *item);
  void currentItemChanged(QListWidgetItem *item, QListWidgetItem *old_item);
  void searchFilterChange(QString);
  void searchClear_Click();

  void addVersion_Click();
  void delVersion_Click();
  void saveVersion_Click();

  void versionSettingsChanged(QString);
  void restoreItemSelection();

 private:
  //! This is need for libq4wine-core.so import.
  QLibrary libq4wine;
  typedef void *CoreLibPrototype(bool);
  CoreLibPrototype *CoreLibClassPointer;
  std::unique_ptr<corelib> CoreLib;

  void createActions();
  void loadThemeIcons();
  void getVersions();
  bool askSaveChangeds();
  bool saveChanges();
  bool fileExists(QString path, bool check_dir = false);

  // std::unique_ptr<VersionListWidget> listPrefix;
  std::unique_ptr<Version> currentVersion;
  bool currentVersionChanged_;

  std::unique_ptr<QSplitter> splitter;
  std::unique_ptr<QAction> addVersion;
  std::unique_ptr<QAction> delVersion;
  std::unique_ptr<QAction> saveVersion;
  std::unique_ptr<QLineEdit> searchField;
  std::unique_ptr<QAction> searchClear;

 protected:
  bool eventFilter(QObject *obj, QEvent *ev);
};

#endif  // VERSION_MANAGER
