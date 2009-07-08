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
#ifndef WINEBINLAUNCHER_H
#define WINEBINLAUNCHER_H

#include <config.h>

#include <QString>
#include <QStringList>
#include <QVariant>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDir>
#include <QTime>
#include <QProcess>
#include <QDebug>
#include <QObject>
#include <QProgressDialog>
#include <QMessageBox>
#include <QTextCodec>
#include <QSettings>

#include <ui_Process.h>

struct wine_prefix{
	QString path;
	QString wine_dllpath;
	QString wine_loader;
	QString wine_exec;
	QString wine_server;
};

class WineBinLauncher : public QDialog, public Ui::Process
{
	Q_OBJECT
	public:
		WineBinLauncher(QString prefix_name = "Default", QWidget * parent = 0, Qt::WFlags f = 0);
		void run_exec(QObject *parent, QString exe, QString args = "", bool KeepRunning = FALSE);
		void appendWineExe(QString wine_append);
		QProcess *myProcess;

	private:
		wine_prefix prefix;
		QString regfile_image;
		
		QString DEFAULT_WINE_BIN, WINE_APPEND, DEFAULT_WINE_SERVER, DEFAULT_WINE_LOADER, DEFAULT_WINE_LIBS, SH_BIN;

	private slots:
		void slotFinished(int, QProcess::ExitStatus);
		void cmdCancel_clicked(void);

};

#endif
