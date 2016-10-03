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

#include "src/qt/widgets/configuration/iconlist.hpp"

ConfigurationIconListWidget::ConfigurationIconListWidget(
        QWidget *parent) : QWidget(parent) {
    layout_.reset(new QVBoxLayout(this));
    layout_->setMargin(0);
    layout_->setSpacing(0);
    createActions();
    createWidgets();
}

ConfigurationIconListWidget::~ConfigurationIconListWidget() {
}

void ConfigurationIconListWidget::createActions() {
    a_Clear.reset(new QAction(getIcon("edit-clear"),
                              tr("Clear search field"),
                               this));
    a_Clear->setStatusTip(tr("Clear search field"));

    a_SortAlpha.reset(new QAction(getIcon("view-sort-descending"),
                               tr("Alphabetic sort descending"),
                               this));
    a_SortAlpha->setStatusTip(tr("Alphabetic sort descending"));

    /*
        sortAlpha->setStatusTip(tr("Alphabetic sort ascending"));
        sortAlpha->setText(tr("Alphabetic sort ascending"));
        sortAlpha->setIcon(CoreLib->loadIcon("view-sort-ascending"));
     */

    txt_Filter.reset(new QLineEdit(this));

    m_ToolBar.reset(new QToolBar(tr("Prefix"), this));
    m_ToolBar->addAction(a_Clear.get());
    m_ToolBar->addWidget(txt_Filter.get());
    m_ToolBar->addAction(a_SortAlpha.get());

    layout_->addWidget(m_ToolBar.get());

    connect(a_Clear.get(),
            SIGNAL(triggered()),
            this,
            SLOT(a_Clear_Click()));
    connect(a_SortAlpha.get(),
            SIGNAL(triggered()),
            this,
            SLOT(a_SortAlpha_Click()));
}

void ConfigurationIconListWidget::createWidgets() {
    w_IconsList.reset(new QListWidget(this));
    layout_->addWidget(w_IconsList.get());
}

const QIcon ConfigurationIconListWidget::getIcon(QString name) const {
    return QIcon::fromTheme(name, QIcon(QString(":%1").arg(name)));
}

void ConfigurationIconListWidget::a_Clear_Click() {
    return;
}

void ConfigurationIconListWidget::a_SortAlpha_Click() {
    return;
}
