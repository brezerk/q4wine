/***************************************************************************
 *   Copyright (C) 2008-2016 by Alexey S. Malakhov <brezerk@gmail.com>     *
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

#pragma once

#include <QWidget>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QVBoxLayout>
#include <QSplitter>
#include <QTreeWidget>
#include <QListWidget>

#include <memory>

class ProgramWidget : public QWidget
{
    Q_OBJECT

 public:
    explicit ProgramWidget(QWidget *parent = 0);
    ~ProgramWidget();

 private:
    void createActions();
    void createWidgets();
    const QIcon getIcon(QString name) const;
};
