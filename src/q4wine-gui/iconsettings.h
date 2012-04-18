/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010, 2011 by Malakhov Alexey                           *
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

#ifndef ICONSETTINGS_H
#define ICONSETTINGS_H

#include <memory>

#include <src/q4wine-gui/ui_IconSettings.h>

//Global config
#include "config.h"

//Windows
#include "iconsview.h"
#include "process.h"

//Db
#include "prefix.h"
#include "icon.h"

//Qt inc

#include <QUrl>
#include <QEvent>
#include <QResizeEvent>
#include <QKeyEvent>
#include <QHeaderView>
#include <QTableWidgetItem>

#include "q4wine-lib.h"

/*!
 * \class IconSettings
 * \ingroup q4wine-gui
 * \brief This class provide icon settings dialog functions.
 *
 */
class IconSettings : public QDialog, public Ui::IconSettings
{
    Q_OBJECT
    public:
        /*! \brief This is class constructor.
         *
         * \param  prefix_name  Current prefix name.
         * \param  dir_name		Current directory name.
         * \param  icon_name	Current directory name.
         */
        IconSettings(QString prefix_name, QString dir_name, QString icon_name = "", QWidget * parent = 0, Qt::WFlags f = 0);

    private slots:
        /*! \brief This slot function resize dialog content.
         */
        void ResizeContent(int);

        /*! \brief This slot function adds selected dll to override list.
         */
        void cmdAdd_Click();

        /*! \brief This slot function gets path to executable win32 binary.
         */
        void cmdGetProgram_Click();

        /*! \brief This slot function gets and\or extract icons.
         */
        void cmdGetIcon_Click();

        /*! \brief This slot function gets work directory.
         */
        void cmdGetWorkDir_Click();

        /*! \brief This slot function trigger use console state.
         */
        void cbUseConsole_stateChanged(int);

        /*! \brief This slot for cancel button.
         */
        void cmdCancel_Click();

        /*! \brief This slot for ok button.
         */
        void cmdOk_Click();

        /*! \brief This slot for help button.
         */
        void cmdHelp_Click();

        /*! \brief This slot function gets program icon.
         */
        void getProgramIcon(QString name);

        /*! \brief This slot function gets path to Pre Run scrip or binary.
         */
        void cmdGetPreRun_Click();

        /*! \brief This slot function gets path to Post Run scrip or binary.
         */
        void cmdGetPostRun_Click();

    private:
        QString prefix_name, dir_name, icon_name, prefix_path, iconPath;

        /*! \brief This function gets icon parms from database.
         */
        void getIconReccord();
        void resizeEvent (QResizeEvent);
        bool eventFilter (QObject *object, QEvent *event);

        void loadThemeIcons();

        //! Side bar URLs list.
        QList<QUrl> prefix_urls;

        //! Database prefix class defenition.
        Prefix db_prefix;
        Icon db_icon;

        //! This is need for libq4wine-core.so import.
        QLibrary libq4wine;
        typedef void *CoreLibPrototype (bool);
        CoreLibPrototype *CoreLibClassPointer;
        std::auto_ptr<corelib> CoreLib;
};

#endif
