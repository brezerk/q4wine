/***************************************************************************
 *   Copyright (C) 2008-2016 by Alexey S. Malakhov <brezerk@gmail.com>     *
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

#include <QWidget>
#include <QToolBar>
#include <QAction>
#include <QListWidget>
#include <QSplitter>
#include <QLineEdit>
#include <QMenu>
#include <QKeyEvent>
#include <QDebug>
#include <QTimer>
#include <QInputDialog>

//q4wine lib
#include "q4wine-lib.h"

#include "config.h"

#include "core/database/versions.h"
#include "q4wine-gui/widgets/versionlistwidget.h"

#include "src/q4wine-gui/ui_VersionManager.h"

class VersionManager : public QDialog, public Ui::VersionManager
{
    Q_OBJECT
public:
    VersionManager(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~VersionManager();
    void setVersionFocus(QString version);
signals:
    void setVersion(QString version_name);

private slots:
    void cmdOk_Click();
    void cmdHelp_Click();
    void keyPressEvent (QKeyEvent * event);
    void itemClicked (QListWidgetItem *item);
    void currentItemChanged (QListWidgetItem *item, QListWidgetItem *old_item);
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
    typedef void *CoreLibPrototype (bool);
    CoreLibPrototype *CoreLibClassPointer;
    std::auto_ptr<corelib> CoreLib;

    void createActions();
    void loadThemeIcons();
    void getVersions();
    bool askSaveChangeds();
    bool saveChanges();
    bool fileExists(QString path, bool check_dir = false);

    //std::auto_ptr<VersionListWidget> listPrefix;
    std::auto_ptr<Version> currentVersion;
    bool currentVersionChanged_;

    std::auto_ptr<QSplitter> splitter;
    std::auto_ptr<QAction> addVersion;
    std::auto_ptr<QAction> delVersion;
    std::auto_ptr<QAction> saveVersion;
    std::auto_ptr<QLineEdit> searchField;
    std::auto_ptr<QAction> searchClear;

protected:
    bool eventFilter(QObject *obj, QEvent *ev);
};

#endif //VERSION_MANAGER
