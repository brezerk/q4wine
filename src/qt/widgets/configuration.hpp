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
#include <QVBoxLayout>
#include <QSplitter>
#include <QTreeWidget>
#include <QListWidget>
#include <QLabel>
#include <QIcon>
#include <QSplitter>
#include <QGridLayout>
#include <QVBoxLayout>

#include <memory>

#include "src/qt/widgets/desktop/prefixtree.hpp"
#include "src/qt/widgets/desktop/iconlist.hpp"

class ConfigurationWidget : public QWidget
{
    Q_OBJECT

 public:
    explicit ConfigurationWidget(QWidget *parent = 0);
    ~ConfigurationWidget();

 private:
    void createWidgets();
    const QIcon getIcon(QString name) const;

    std::shared_ptr<PrefixTreeWidget> w_PrefixTree;
    std::shared_ptr<IconListWidget> w_IconList;

    std::shared_ptr<QLabel> lbl_IconInfoName,
                            lbl_IconInfoArgs,
                            lbl_IconInfoDesc,
                            lbl_IconInfoTerminal,
                            lbl_IconInfoDesktopSize;
};
