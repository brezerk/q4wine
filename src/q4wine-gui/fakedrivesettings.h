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

#ifndef FAKEDRIVESETTINGS_H
#define FAKEDRIVESETTINGS_H

#include <src/q4wine-gui/ui_FakeDriveSettings.h>

#include "config.h"
#include "registry.h"
#include "q4wine-lib.h"

#include "progress.h"
#include "winedrivedialog.h"
#include "drivelistwidgetitem.h"

#include <QSplitter>
#include <QDialog>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QTimer>
#include <QStringListModel>
#include <QMetaType>
#include <QVariant>

Q_DECLARE_METATYPE(QList<int>)

#include "src/core/database/prefix.h"
#include "src/core/database/icon.h"
#include "src/core/database/dir.h"

#ifndef _OS_DARWIN_
#include "sysmenu.h"
#endif

class FakeDriveSettings : public QDialog, public Ui::FakeDriveSettings
{
    Q_OBJECT
public:
    explicit FakeDriveSettings(QString prefixName, QWidget * parent = nullptr, Qt::WindowFlags f =  Qt::WindowFlags());
    void loadPrefixSettings();
    void loadDefaultPrefixSettings();

private slots:
    void waitForWineEnd();
    void waitForWine();

    void optionsTree_itemClicked ( QTreeWidgetItem *item, int);

    void cmdCancel_Click();
    void cmdOk_Click();
    void cmdHelp_Click();

    //! \brief cmdJoystickEdit click slot.
    void cmdJoystickEdit_Click();

    //! \brief cmdJoystickAdd click slot.
    void cmdJoystickAdd_Click();

    //! \brief cmdJoystickDel click slot.
    void cmdJoystickDel_Click();

    //! \brief cmdEdit click slot.
    void cmdWineDriveEdit_Click();

    //! \brief cmdJoystickAdd click slot.
    void cmdWineDriveAdd_Click();

    //! \brief cmdJoystickDel click slot.
    void cmdWineDriveDel_Click();

    void loadSettings();
    void loadDefaultSettings();

private:
    QString prefixName;
    QString desktopFolder, desktopDocuments, desktopMusic, desktopPictures, desktopVideos;
   // bool eventFilter (QObject *object, QEvent *event);
    void loadThemeIcons();
   bool eventFilter(QObject *obj, QEvent *event);
   bool wine64;

    std::unique_ptr<QSplitter> splitter;

    //! This is need for libq4wine-core.so import.
    QLibrary libq4wine;
    typedef void *CoreLibPrototype (bool);
    CoreLibPrototype *CoreLibClassPointer;
    std::unique_ptr<corelib> CoreLib;

    //! Database prefix class definition.
    Prefix db_prefix;
    Dir db_dir;
    Icon db_icon;
    Registry reg;
};

#endif // FAKEDRIVESETTINGS_H
