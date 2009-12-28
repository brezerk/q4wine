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
 ***************************************************************************/

#ifndef ICONSVIEW_H
#define ICONSVIEW_H

#include "memory"

#include <ui_IconsView.h>

#include <QDialog>
#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QSizePolicy>
#include <QTableWidget>

class IconsView : public QDialog, public Ui::IconsView
{
	Q_OBJECT
	public:
		IconsView(QString tmpDir, QWidget * parent = 0, Qt::WFlags f = 0);
		QString selectedFile;

	private slots:
		void cmdCancel_Click();
		void cmdOk_Click();

	private:
		QString tempDirectory;
};

#endif
