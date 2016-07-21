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
#include <QListWidget>
#include <QLineEdit>
#include <QLabel>

#include <memory>

class IconListWidget : public QWidget
{
    Q_OBJECT

 public:
    explicit IconListWidget(QWidget *parent = 0);
    ~IconListWidget();

 private:
    void createActions();
    void createWidgets();
    const QIcon getIcon(QString name) const;

    std::shared_ptr<QToolBar> m_ToolBar;

    std::shared_ptr<QAction> a_Clear,
                           a_Mode,
                           a_ZoomIn,
                           a_ZoomOut,
                           a_SortAlpha,
                           a_SortDate;

    std::shared_ptr<QLineEdit> txt_Filter;

    std::shared_ptr<QListWidget> w_IconsList;

    std::shared_ptr<QVBoxLayout> layout_;

 private slots:
    void a_Clear_Click();
    void a_Mode_Click();
    void a_ZoomIn_Click();
    void a_ZoomOut_Click();
    void a_SortAlpha_Click();
    void a_SortDate_Click();
};
