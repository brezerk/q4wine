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
#include <QTextEdit>
#include <QLabel>

#include <memory>

class PrefixTreeWidget : public QWidget
{
    Q_OBJECT

 public:
    explicit PrefixTreeWidget(QWidget *parent = 0);
    ~PrefixTreeWidget();

 private:
    void createActions();
    void createWidgets();
    const QIcon getIcon(QString name) const;

    std::shared_ptr<QMenu> m_Prefix, m_Icon;

    std::shared_ptr<QAction> a_TreeSate,
                           a_Export,
                           a_Import,
                           a_IconSearchClear,
                           a_IconViewMode,
                           a_IconZoomIn,
                           a_IconZoomOut,
                           a_IconSortName,
                           a_IconSortDate;

    std::shared_ptr<QTextEdit> txt_IconSearch;

    std::shared_ptr<QTreeWidget> w_PrefixTree;
    std::shared_ptr<QListWidget> w_IconList;

    std::shared_ptr<QLabel> lbl_IconInfoName,
                            lbl_IconInfoArgs,
                            lbl_IconInfoDesc,
                            lbl_IconInfoTerminal,
                            lbl_IconInfoDesktopSize;
};
