/***************************************************************************
 *   Copyright (C) 2008 by Malakhov Alexey                                 *
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
 *   In addition, as a special exception, the copyright holders give       *
 *   permission to link the code of this program with any edition of       *
 *   the Qt library by Trolltech AS, Norway (or with modified versions     *
 *   of Qt that use the same license as Qt), and distribute linked         *
 *   combinations including the two.  You must obey the GNU General        *
 *   Public License in all respects for all of the code used other than    *
 *   Qt.  If you modify this file, you may extend this exception to        *
 *   your version of the file, but you are not obligated to do so.  If     *
 *   you do not wish to do so, delete this exception statement from        *
 *   your version.                                                         *
 ***************************************************************************/

#ifndef WIZARD_H
#define WIZARD_H

#include <ui_Wizard.h>

#include <QDialog>
#include <QObject>
#include <QDebug>
#include <QWidget>
#include <QFileDialog>
#include <QProcess>
#include <QSettings>
#include <QTextCodec>
#include <QMessageBox>
#include <QResizeEvent>
#include <QProgressDialog>
#include <QStringList>
#include <QLibrary>

#include <config.h>
#include "registry.h"
#include "process.h"
#include "winebinlauncher.h"

#include "src/core/database/prefix.h"
#include "src/core/database/icon.h"
#include "src/core/database/dir.h"

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

	private:
		//! \brief Function for updateing scene elements.
		void updateScena();

		int Scena, Page, TotalPage;

		//! \brief Function for checking user selected elements.
		bool checkEntry(QString fileName, QString info, bool isFile = TRUE);

		//! Database prefix class defenition.
		Prefix *db_prefix;
		Icon *db_icon;
		Dir *db_dir;

		//! This is need for libq4wine-core.so import.
		QLibrary libq4wine;
		typedef void *CoreLibPrototype (bool);
		CoreLibPrototype *CoreLibClassPointer;
		corelib *CoreLib;

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
