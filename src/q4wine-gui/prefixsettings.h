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


#ifndef PREFIXSETTINGS_H
#define PREFIXSETTINGS_H

#include <memory>

#include <src/q4wine-gui/ui_PrefixSettings.h>

#include <QFileDialog>
#include <QMessageBox>

#include "config.h"
#include "src/core/database/prefix.h"
#include "src/core/database/icon.h"
#include "src/core/database/dir.h"

#include "q4wine-lib.h"

/*!
 * \class PrefixSettings
 * \ingroup q4wine-gui
 * \brief This class provide prefix settings dialog functions.
 *
 */
class PrefixSettings : public QDialog, public Ui::PrefixSettings
{
    Q_OBJECT
    public:
        /*! \brief This is class constructor.
         *
         * \param  prefix_name  Current prefix name.
         */
        PrefixSettings(QString prefix_name, QWidget * parent = 0, Qt::WFlags f = 0);
        PrefixSettings(QWidget * parent = 0, Qt::WFlags f = 0);
        QString getPrefixName();

    private:
        /*! \brief This is an event filter.
         *
         * This event filter hendle button click events
         * \param  prefix_name  Current prefix name.
         */
        bool eventFilter(QObject *obj, QEvent *event);

        /*! \brief This function loads theme images to widgets.
         *
         * \param themePath is a path to user selected theme ;)
         */
        void loadThemeIcons();

        /*!
         * Prefix id, and name
         */
        QString prefix_id, prefix_name;

        //! Database prefix class defenition.
        Prefix db_prefix;
        Icon db_icon;
        Dir db_dir;

        //! This is need for libq4wine-core.so import.
        QLibrary libq4wine;
        typedef void *CoreLibPrototype (bool);
        CoreLibPrototype *CoreLibClassPointer;
        std::auto_ptr<corelib> CoreLib;

        bool addNew;

    private slots:
        /*!
         * \bref Cancle button click slot
         */
        void cmdCancel_Click();

        /*!
         * \bref Ok button click slot
         */
        void cmdOk_Click();

        /*!
         * \bref Help button click slot
         */
        void cmdHelp_Click();

        /*!
         * \brief Get wine cdrom letter
         */
        void getWineCdromLetter();

};

#endif
