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
#include <QLineEdit>
#include <QLabel>
#include <QTimer>
#include <QStandardItemModel>
#include <QTableView>
#include <QTableWidgetItem>

#include <memory>

class ProcessListWidget : public QWidget
{
    Q_OBJECT

 public:
    explicit ProcessListWidget(QWidget *parent = 0);
    ~ProcessListWidget();

 private:
    void createActions();
    void createWidgets();
    const QIcon getIcon(QString name) const;

    std::shared_ptr<QToolBar> m_ToolBar;

    std::shared_ptr<QAction> a_Kill,
                           a_KillAll,
                           a_Refresh;

    std::shared_ptr<QTableView> w_ProcessList;

    std::shared_ptr<QVBoxLayout> layout_;

    std::shared_ptr<QStandardItemModel> model_;

 private slots:
    void a_Kill_Click();
    void a_KillAll_Click();
    void a_Refresh_Click();
};
