/***************************************************************************
 *   Copyright (C) 2008-2013 by Alexey S. Malakhov <brezerk@gmail.com>     *
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

#ifndef WIZARD_H
#define WIZARD_H

#include "memory"

#include <src/q4wine-gui/ui_Wizard.h>

#include <QFileDialog>
#include <QProcess>
#include <QTextCodec>
#include <QMessageBox>
#include <QResizeEvent>
#include <QProgressDialog>
#include <QLibrary>
#include <QDesktopServices>

#include "config.h"
#include "process.h"

#include "src/core/database/prefix.h"
#include "src/core/database/icon.h"
#include "src/core/database/dir.h"
#include "src/core/database/versions.h"
#ifndef _OS_DARWIN_
#include "sysmenu.h"
#endif

#include "q4wine-lib.h"

/*!
 * \class Wizard
 * \ingroup q4wine-gui
 * \brief This class provide wizar dialog functions.
 *
 */
class Wizard : public QDialog, public Ui::Wizard
{
    Q_OBJECT
    public:
        /*! \brief This is class constructor.
         *
         * \param  WizardType	Type of wizard scenario.
         * \param  var1			This variable used for different scenarion actions.
         */
        Wizard(int WizardType, QString var1 = "", QWidget * parent = 0, Qt::WindowFlags f = 0);
        QString getPrefixName();

    private slots:
        //! \brief Next page button click slot.
        void nextWizardPage();

        //! \brief Previos page button click slot.
        void previosWizardPage();

        //! \brief ChekBox change slot.
        void changeBoxState(int state);

        //! \brief ComboBox change slot.
        void comboProxyType_indexChanged(QString text);

        //! \brief cmdHelo click slot.
        void cmdHelp_Click();

        //! \brief mounts profile combobox index cahnge slot
        void comboMountProfiles_currentIndexChanged(int index);

    private:
        //! \brief Function for updateing scene elements.
        void updateScena();

        int Scena, Page, TotalPage;

        //! \brief Function for checking user selected elements.
        bool checkEntry(QString fileName, QString info, bool isFile = true);

        //! Database prefix class defenition.
        Prefix db_prefix;
        Icon db_icon;
        Dir db_dir;

        //! This is need for libq4wine-core.so import.
        QLibrary libq4wine;
        typedef void *CoreLibPrototype (bool);
        CoreLibPrototype *CoreLibClassPointer;
        std::auto_ptr<corelib> CoreLib;

    protected:
        //! \brief Event filter.
        bool eventFilter(QObject *obj, QEvent *event);
        QString var1;

        //! \brief Load icon themes.
        void loadThemeIcons(int Scene);
};

#endif
