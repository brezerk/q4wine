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

#ifndef RUN_H
#define RUN_H

#include <memory>

#include "config.h"

#include "src/q4wine-gui/ui_Run.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QEvent>
#include <QKeyEvent>
#include <QUrl>
#include <QDialog>
#include <QHeaderView>
#include "process.h"

#include "prefix.h"
#include "src/core/database/versions.h"
#include "last_run_icon.h"

#include "q4wine-lib.h"

/*!
 * \class Run
 * \ingroup q4wine-gui
 * \brief This class provide run dialog functions.
 *
 */
class Run : public QDialog, public Ui::Run
{
    Q_OBJECT
    public:
        /*! \brief This is class constructor.
         *
         * \param  prefix_name  Current user selected prefix name.
         */
        Run(QWidget * parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
        void prepare(QString prefix_name, QString wrkdir="", QString override="", QString winedebug="", QString useconsole="", QString display="", QString cmdargs="", QString desktop="", int nice = 0, QString exec = "", QString lang = "");
        ExecObject execObj;
    private slots:
        //! \brief Cancel button click slot.
        void cmdCancel_Click();
        //! \brief Ok button click slot.
        void cmdOk_Click();
        //! \brief Add lib button click slot.
        void cmdAdd_Click();
        //! \brief ComboxBox change slot.
        void comboPrefixes_indexChanged (int);
        //! \brief CheckBox change slot.
        void cbUseConsole_stateChanged(int);
        //! \brief Get program slot.
        void cmdGetProgram_Click();
        //! \brief Get work dir slot.
        void cmdGetWorkDir_Click();
        //! \brief view online help.
        void cmdHelp_Click();
    private:
        QString prefix_name, prefix_dir;

        //! \brief Event filter.
        bool eventFilter (QObject *object, QEvent *event);
        //! \brief Get wine dll list.
        void getWineDlls (QString wine_dllpath);
        //! \brief Get existing prefixes list.
        void getPrefixes (void);

        //! \brief Load theme images.
        void loadThemeIcons();

        //! Database prefix class definition.
        Prefix db_prefix;
        Last_Run_Icon db_last_run_icon;

        //! This is need for libq4wine-core.so import.
        QLibrary libq4wine;
        typedef void *CoreLibPrototype (bool);
        CoreLibPrototype *CoreLibClassPointer;
        std::unique_ptr<corelib> CoreLib;
};

#endif
