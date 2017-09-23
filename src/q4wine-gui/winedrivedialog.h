/***************************************************************************
 *   Copyright (C) 2008-2017 by Oleksii S. Malakhov <brezerk@gmail.com>    *
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

#ifndef WINEDRIVEDIALOG_H
#define WINEDRIVEDIALOG_H

#include <src/q4wine-gui/ui_WineDriveDialog.h>

#include <QMessageBox>
#include <QFileDialog>
#include <QLibrary>

#include "config.h"

//q4wine lib
#include "q4wine-lib.h"

/*!
 * \class WineDriveDialog
 * \ingroup q4wine-gui
 * \brief This class provide wizar dialog functions.
 *
 */
class WineDriveDialog : public QDialog, public Ui::WineDriveDialog
{
	Q_OBJECT
public:
	/*! \brief This is class constructor.
	 *
	 * \param  WizardType	Type of wizard scenario.
	 * \param  var1			This variable used for different scenarion actions.
	 */
	WineDriveDialog(QStringList removeLetters, QWidget * parent = 0, Qt::WindowFlags f = 0);
	WineDriveDialog(QStringList removeLetters, QString driveLetter, QString drivePath, QString driveType, QWidget * parent = 0, Qt::WindowFlags f = 0);

	QString getLetter();
	QString getPath();
	QString getType();

private slots:
	void cmdOk_Click();
	void cmdCancel_Click();
	void cmdGetDrivePath_Click();

private:
	//! This is need for libq4wine-core.so import;
	typedef void *CoreLibPrototype (bool);
		CoreLibPrototype *CoreLibClassPointer;
		std::unique_ptr<corelib> CoreLib;
	QLibrary libq4wine;
};

#endif // WINEDRIVEDIALOG_H
