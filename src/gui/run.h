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

#ifndef RUN_H
#define RUN_H

#include <ui_Run.h>

#include <QDialog>
#include <QLibrary>
#include <QObject>
#include <QWidget>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>
#include <QUrl>
#include <QKeyEvent>
#include <QSqlQuery>
#include <QHeaderView>
#include <QTableWidget>

#include "config.h"
#include "process.h"

#include "src/core/database/prefix.h"
#include "src/core/database/icon.h"
#include "src/core/database/dir.h"

#include <q4wine-lib/main.h>

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
		Run(QString prefix_name, QWidget * parent = 0, Qt::WFlags f = 0);
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
		//! \brief Resize window content slot.
		void ResizeContent(int TabIndex);
		//! \brief Get program slot.
		void cmdGetProgram_Click();
		//! \brief Get work dir slot.
		void cmdGetWorkDir_Click();
	private:
		QString prefix_name, prefix_dir;

		//! \brief Event filter.
		bool eventFilter (QObject *object, QEvent *event);
		//! \brief Get wine dll list.
		void getWineDlls (QString wine_dllpath);
		//! \brief Get exesting prefixes list.
		void getPrefixes (void);

		//! \brief Load theme images.
		void loadThemeIcons(QString themePath);
		//! \brief Load icon.
		QIcon loadIcon(QString iconName, QString themePath);

		//! Database prefix class defenition.
		Prefix *db_prefix;

		//! This is need for libq4wine-core.so import.
		QLibrary libq4wine;
		typedef void *CoreLibPrototype (bool);
		CoreLibPrototype *CoreLibClassPointer;
		corelib *CoreLib;
};

#endif
