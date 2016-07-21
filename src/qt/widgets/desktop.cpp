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

#include "src/qt/widgets/desktop.hpp"

DesktopWidget::DesktopWidget(QWidget *parent) : QWidget(parent) {
    createWidgets();
}

DesktopWidget::~DesktopWidget() {
}

void DesktopWidget::createWidgets() {
    w_PrefixTree.reset(new PrefixTreeWidget());
    w_IconList.reset(new QListWidget());
    QSplitter* splitter = new QSplitter(this);
    splitter->addWidget(w_PrefixTree.get());
    splitter->addWidget(w_IconList.get());
}

const QIcon DesktopWidget::getIcon(QString name) const {
    return QIcon::fromTheme(name, QIcon(QString(":%1").arg(name)));
}
