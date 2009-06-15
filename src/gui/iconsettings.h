/***************************************************************************
 *   Copyright (C) 2008, 2009 by Malakhov Alexey                           *
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


#ifndef ICONSETTINGS_H
#define ICONSETTINGS_H

#include <ui_IconSettings.h>

#include <QDialog>
#include <QObject>
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlRelation>
#include <QSettings>
#include <QKeyEvent>
#include <QLibrary>
#include <QUrl>
#include <QHeaderView>
#include <QTableWidget>

#include "config.h"
#include "iconsview.h"
#include "process.h"

#include "src/core/database/prefix.h"
#include "src/core/database/icon.h"

#include <q4wine-lib/main.h>


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

	private:
		QString prefix_name, dir_name, icon_name, prefix_path, iconPath;

		/*! \brief This function gets icon parms from database.
		 */
		void getIconReccord();
		void resizeEvent (QResizeEvent);
		bool eventFilter (QObject *object, QEvent *event);

		void loadThemeIcons(QString themePath);
		QIcon loadIcon(QString iconName, QString themePath);

		//! Side bar URLs list.
		QList<QUrl> prefix_urls;

		//! Database prefix class defenition.
		Prefix *db_prefix;
		Icon *db_icon;

		//! This is need for libq4wine-core.so import.
		QLibrary libq4wine;
		typedef void *CoreLibPrototype (bool);
		CoreLibPrototype *CoreLibClassPointer;
		corelib *CoreLib;
};

#endif
