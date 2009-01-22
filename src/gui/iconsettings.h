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
#include <QHeaderView>
#include <QTableWidget>

#include "config.h"
#include "iconsview.h"
#include "process.h"

class IconSettings : public QDialog, public Ui::IconSettings
{
	Q_OBJECT
	public:
		IconSettings(bool newIcon, QString prefix_id, QString prefix_dir, QString dir_id, QString icon_name, QString winedll_path = "", QWidget * parent = 0, Qt::WFlags f = 0);

	private slots:
		void ResizeContent(int);
		void cmdAdd_Click();
		void cmdGetProgram_Click();
		void cmdGetIcon_Click();
		void cmdGetWorkDir_Click();
		void cbUseConsole_stateChanged(int);
		void cmdCancel_Click();
		void cmdOk_Click();
		
	private:
		QString prefix_dir, dir_id, prefix_id, iconName, iconId, iconPath;
		bool newIcon;
		void getIconReccord(QString themePath);
		void resizeEvent (QResizeEvent);
		bool eventFilter (QObject *object, QEvent *event);
		void getWineDlls(QString winedll_path);
		
		void loadThemeIcons(QString themePath);
		QIcon loadIcon(QString iconName, QString themePath);
		
		QString WRESTOOL_BIN;
		QString ICOTOOL_BIN;
};

#endif
