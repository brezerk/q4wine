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

#include "src/qt/widgets/process.hpp"

ProcessWidget::ProcessWidget(QWidget *parent) : QWidget(parent) {
    createWidgets();
}

ProcessWidget::~ProcessWidget() {
}

void ProcessWidget::createWidgets() {
    w_PrefixTree.reset(new PrefixTreeWidget());
    w_ProcessList.reset(new ProcessListWidget());

    QSplitter* splitter = new QSplitter(this);
    splitter->addWidget(w_PrefixTree.get());
    splitter->addWidget(w_ProcessList.get());
    splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QList<int> sizes;
    sizes << 150 << 400;
    splitter->setSizes(sizes);

    lbl_ProcessCount.reset(new QLabel());
    lbl_PrefixCount.reset(new QLabel());

    QGridLayout* layout = new QGridLayout();
    layout->setMargin(0);
    layout->setSpacing(4);
    layout->addWidget(splitter,
                      0, 0, 1, 2);
    layout->addWidget(lbl_ProcessCount.get(),
                      1, 0);
    layout->addWidget(lbl_PrefixCount.get(),
                      1, 1);
    this->setLayout(layout);

    lbl_ProcessCount->setText(tr("Total process count:"));
    lbl_PrefixCount->setText(tr("Running prefixes:"));
}

const QIcon ProcessWidget::getIcon(QString name) const {
    return QIcon::fromTheme(name, QIcon(QString(":%1").arg(name)));
}
