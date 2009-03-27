/***************************************************************************
 *   Copyright (C) 2009 by Pavel Zinin (pashazz)                            *
 *   pzinin@gmail.com                                                     *
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

#ifndef WINETRICKS_H
#define WINETRICKS_H

#include "config.h"
#include "winetrickshelp.h"
#include <ui_winetricks.h>
#include <QtGui/QDialog>
#include <QStringList>
#include <QProcess>
#include <QMessageBox>

#include "coremethods.h"
#include "process.h"

class winetricks : public QDialog, public Ui::winetricks {
	Q_OBJECT
	Q_DISABLE_COPY(winetricks)
public:
	winetricks(QString prefixName, QWidget * parent = 0, Qt::WFlags f = 0);
	virtual ~winetricks();
	QString _prefixName;

protected:
	//virtual void changeEvent(QEvent *e);

	private slots:
		void run_winetricks();
		void install_winetricks();
                void run_help();
	private:
		QString console_bin;
		QString console_args;
		QString prefix_path;
		QString winetricks_bin;

		void downloadwinetricks();
		void removefile(QString);
		CoreMethods *core;
};

#endif // WINETRICKS_H
