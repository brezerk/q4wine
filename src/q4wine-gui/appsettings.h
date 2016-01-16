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

#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <memory>
#include <src/q4wine-gui/ui_AppSettings.h>

#include "config.h"
#include "versions.h"
#include "q4wine-lib.h"

#ifndef _OS_DARWIN_
#include "sysmenu.h"
#endif

#include <QSplitter>

class AppSettings : public QDialog, public Ui::AppSettings
{
    Q_OBJECT
    public:
        AppSettings(QWidget * parent = 0, Qt::WindowFlags f = 0);

    private slots:
        void cmdCancel_Click();
        void cmdOk_Click();
        void cmdVersionManager_Click();
        void comboProxyType_indexChanged(QString text);
        //! \brief cmdHelo click slot.
        void cmdHelp_Click();
        void comboMountProfiles_currentIndexChanged(int index);
        void cbShowTray_stateChanged ( int state );
        void optionsTree_itemClicked ( QTreeWidgetItem *item, int);
        void cmdDesktopMenu_Regen_Click();
        void cmdDesktopMenu_Remove_Click();

    private:
        bool eventFilter (QObject *object, QEvent *event);
        void getLangs();
        bool checkEntry(QString fileName, QString info, bool isFile = true);
        void loadThemeIcons();

        std::auto_ptr<QSplitter> splitter;

        //! This is need for libq4wine-core.so import.
        QLibrary libq4wine;
        typedef void *CoreLibPrototype (bool);
        CoreLibPrototype *CoreLibClassPointer;
        std::auto_ptr<corelib> CoreLib;
};

#endif

