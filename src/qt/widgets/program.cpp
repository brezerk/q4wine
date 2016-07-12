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

#include "src/qt/widgets/program.hpp"

ProgramWidget::ProgramWidget(QWidget *parent) : QWidget(parent) {
    createActions();
    createWidgets();
}

ProgramWidget::~ProgramWidget() {
}

void ProgramWidget::createActions() {
}

void ProgramWidget::createWidgets() {
}

const QIcon ProgramWidget::getIcon(QString name) const {
    return QIcon::fromTheme(name, QIcon(QString(":%1").arg(name)));
}
