/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010 by Malakhov Alexey                           *
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

#ifndef WINEDRIVEDIALOG_H
#define WINEDRIVEDIALOG_H

#include <ui_WineDriveDialog.h>

#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>

#include <config.h>

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
	WineDriveDialog(QStringList removeLetters, QWidget * parent = 0, Qt::WFlags f = 0);
	WineDriveDialog(QStringList removeLetters, QString driveLetter, QString drivePath, QString driveType, QWidget * parent = 0, Qt::WFlags f = 0);

	//FIXME: Move to get methods
	QString driveDesc;
	QString driveType;

	private slots:
	void cmdOk_Click();
	void cmdCancel_Click();
	void cmdGetDrivePath_Click();
};

#endif // WINEDRIVEDIALOG_H
