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

#include "src/qt/widgets/configuration.hpp"

ConfigurationWidget::ConfigurationWidget(QWidget *parent) : QWidget(parent) {
    createWidgets();
}

ConfigurationWidget::~ConfigurationWidget() {
}

void ConfigurationWidget::createWidgets() {
    w_ConfigTree.reset(new ConfigTreeWidget());
    w_ConfigIconList.reset(new ConfigurationIconListWidget());

    QSplitter* splitter = new QSplitter(this);
    splitter->addWidget(w_ConfigTree.get());
    splitter->addWidget(w_ConfigIconList.get());
    splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QList<int> sizes;
    sizes << 150 << 400;
    splitter->setSizes(sizes);

    lbl_IconInfoName.reset(new QLabel());
    lbl_IconInfoDesc.reset(new QLabel());

    QGridLayout* layout = new QGridLayout();
    layout->setMargin(0);
    layout->setSpacing(4);
    layout->addWidget(splitter,
                      0, 0);
    layout->addWidget(lbl_IconInfoName.get(),
                      1, 0);
    layout->addWidget(lbl_IconInfoDesc.get(),
                      3, 0);
    this->setLayout(layout);

    lbl_IconInfoName->setText(tr("Name:"));
    lbl_IconInfoDesc->setText(tr("Some loooooooooooooooooooooooooooooooooooooo"
                                 "oooooooooooooooooooooooooooooooooong desc"));
}

const QIcon ConfigurationWidget::getIcon(QString name) const {
    return QIcon::fromTheme(name, QIcon(QString(":%1").arg(name)));
}
