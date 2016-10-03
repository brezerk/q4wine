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
#include <QStandardItemModel>
#include <QTableView>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QLineEdit>
#include <QLabel>

#include <memory>

class PrefixConfigWidget : public QWidget
{
    Q_OBJECT

 public:
    explicit PrefixConfigWidget(QWidget *parent = 0);
    ~PrefixConfigWidget();

 private:
    void createActions();
    void createWidgets();
    const QIcon getIcon(QString name) const;

    std::shared_ptr<QToolBar> m_ToolBar;

    std::shared_ptr<QAction> a_Clear,
                           a_SortAlpha,
                           a_New,
                           a_Import,
                           a_Export,
                           a_Delete,
                           a_Edit,
                           a_Configure,
                           a_ConfigureVersions;

    std::shared_ptr<QLineEdit> txt_Filter;

    std::shared_ptr<QTableView> tbl_Prefixes;

    std::shared_ptr<QVBoxLayout> layout_;
    std::shared_ptr<QStandardItemModel> model_;

 private slots:
    void a_Clear_Click();
    void a_SortAlpha_Click();
    void a_New_Click();
    void a_Import_Click();
    void a_Export_Click();
    void a_Delete_Click();
    void a_Edit_Click();
    void a_Configure_Click();
    void a_ConfigureVersions_Click();

};
