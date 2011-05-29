/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010, 2011 by Malakhov Alexey                                 *
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

#ifndef PROCESS_H
#define PROCESS_H

#include <memory>

#include "config.h"

#include "src/q4wine-lib/ui_Process.h"

#include <QProcess>
#include <QFileDialog>
#include <QMessageBox>
#include <QLibrary>
#include <QTextCodec>
#include <QTextStream>

#ifdef DEBUG
#include <QDebug>
#endif

class Process : public QDialog, public Ui::Process
{
	Q_OBJECT
	public:
		Process(QStringList args, QString exec, QString dir, QString info, QString caption, bool showErr = TRUE, QStringList env = QProcess::systemEnvironment(), QWidget * parent = 0, Qt::WFlags f = 0);

	private slots:
		void slotFinished(int, QProcess::ExitStatus);
		void cmdCancel_clicked(void);
		void slotError(QProcess::ProcessError);

	private:
		bool showErr;
		std::auto_ptr<QProcess> myProcess;

		QString getLocale();
};

#endif
