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


#ifndef PREFIXSETTINGS_H
#define PREFIXSETTINGS_H

#include <ui_PrefixSettings.h>

#include <QDialog>
#include <QObject>
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QLibrary>
#include <QDebug>
#include <QSettings>
#include <QStringList>

#include "config.h"
#include "src/core/database/prefix.h"

#include <q4wine-lib/main.h>

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
		void loadThemeIcons(QString themePath);

		/*! \brief This function returns load theme image item by user selected theme
		 *
		 * \param iconName icon name
		 * \param themePath is a path to user selected theme ;)
		 */
		QIcon loadIcon(QString iconName, QString themePath);

		/*!
		 * Prefix id, and name
		 */
		QString prefix_id, prefix_name;

		//! Database prefix class defenition.
		Prefix *db_prefix;

		//! This is need for libq4wine-core.so import.
		QLibrary libq4wine;
		typedef void *CoreLibPrototype (bool);
		CoreLibPrototype *CoreLibClassPointer;
		corelib *CoreLib;

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


};

#endif
