/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010 by Malakhov Alexey                           *
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

#ifndef WIZARD_H
#define WIZARD_H

#include "memory"

#include <ui_Wizard.h>

#include <QDebug>
#include <QFileDialog>
#include <QProcess>
#include <QTextCodec>
#include <QMessageBox>
#include <QResizeEvent>
#include <QProgressDialog>
#include <QListWidget>
#include <QStringList>
#include <QInputDialog>
#include <QLibrary>

#include <config.h>
#include "registry.h"
#include "process.h"
#include "winebinlauncher.h"
#include "drivelistwidgetitem.h"

#include "src/core/database/prefix.h"
#include "src/core/database/icon.h"
#include "src/core/database/dir.h"
#include "src/core/registry.h"
#include "winedrivedialog.h"

#include <q4wine-lib/main.h>

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
		Wizard(int WizardType, QString var1 = "", QWidget * parent = 0, Qt::WFlags f = 0);

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

		//! \brief raidioDefault toggled action slot.
		void radioDefault_toggled(bool state);

		//! \brief raidioDefaultGui toggled action slot.
		void radioDefaultGui_toggled(bool state);

		//! \brief raidioFuse toggled action slot.
		void radioFuse_toggled(bool state);

		//! \brief raidioEmbedded toggled action slot.
		void radioEmbedded_toggled(bool state);

	private:
		//! \brief Function for updateing scene elements.
		void updateScena();

		int Scena, Page, TotalPage;

		//! \brief Function for checking user selected elements.
		bool checkEntry(QString fileName, QString info, bool isFile = TRUE);

		//! Database prefix class defenition.
		Prefix db_prefix;
		Icon db_icon;
		Dir db_dir;

		//! This is need for libq4wine-core.so import.
		QLibrary libq4wine;
		typedef void *CoreLibPrototype (bool);
		CoreLibPrototype *CoreLibClassPointer;
		std::auto_ptr<corelib> CoreLib;

		Registry reg;

	protected:
		//! \brief Event filter.
		bool eventFilter(QObject *obj, QEvent *event);
		QString var1;

		//! \brief Load icon themes.
		void loadThemeIcons(QString themePath, int Scene);

		//! \brief Load icon themes.
		QIcon loadIcon(QString iconName, QString themePath);
};

#endif
