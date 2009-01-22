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
#include <QObject>
#include <QWidget>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>
#include <QKeyEvent>
#include <QSqlQuery>
#include <QHeaderView>
#include <QTableWidget>

#include "config.h"
#include "process.h"



class Run : public QDialog, public Ui::Run
{
	Q_OBJECT
	public:
		Run(QString prefix_id, QString prefix_dir, QString winedll_path, QWidget * parent = 0, Qt::WFlags f = 0);
		ExecObject execObj;
	private slots:
		void cmdCancel_Click();
		void cmdOk_Click();
		void cmdAdd_Click();
		void comboPrefixes_indexChanged (int);
		void cbUseConsole_stateChanged(int);
		void ResizeContent(int TabIndex);
		
		void cmdGetProgram_Click();
		void cmdGetWorkDir_Click();
		
	private:
		QString prefix_id, prefix_dir;
		
		bool eventFilter (QObject *object, QEvent *event);
		void getWineDlls (QString wine_dllpath);
		void getPrefixes (void);
		
		void loadThemeIcons(QString themePath);
		QIcon loadIcon(QString iconName, QString themePath);
};

#endif
