/***************************************************************************
 *   Copyright (C) 2008-2016 by Oleksii S. Malakhov <brezerk@gmail.com>     *
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

#ifndef WINEPROCESSWIDGET_H
#define WINEPROCESSWIDGET_H

//System
#include <memory>

//Global config
#include "config.h"

//Windows
#include "wizard.h"

//Qt includes
#include <QStandardItemModel>
#include <QTableWidgetItem>
#include <QTableView>
#include <QToolBar>
#include <QTimer>
#include <QMenu>
#include <QHeaderView>
#include <QInputDialog>

#ifdef DEBUG
#include <QDebug>
#endif

//q4wine lib
#include "q4wine-lib.h"

class WineProcessWidget : public QWidget
{
Q_OBJECT
public:
	explicit WineProcessWidget(QWidget *parent = 0);

signals:
	void changeStatusText(QString);

public slots:
	void stopTimer(void);
	void startTimer(void);

private:
	std::auto_ptr<QTimer> timer;
	std::auto_ptr<QStandardItemModel> model;

	//! This is need for libq4wine-core.so import.
	QLibrary libq4wine;
	typedef void *CoreLibPrototype (bool);
	CoreLibPrototype *CoreLibClassPointer;
	std::auto_ptr<corelib> CoreLib;

	void createActions();

	std::auto_ptr<QTableView> procTable;
	std::auto_ptr<QLabel> lblInfo;

	std::auto_ptr<QMenu> menu;
	std::auto_ptr<QAction> procKillSelected;
	std::auto_ptr<QAction> procKillWine;
	std::auto_ptr<QAction> procRefresh;
	std::auto_ptr<QAction> procRenice;

private slots:
	void getWineProcesssInfo(void);
	void customContextMenuRequested(const QPoint &pos);
	void itemClicked(const QModelIndex &);

	void procKillSelected_Click(void);
	void procKillWine_Click(void);
	void procRenice_Click(void);
};

#endif // WINEPROCESSVIEW_H
