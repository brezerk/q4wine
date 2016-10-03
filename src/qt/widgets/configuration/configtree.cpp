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

#include "src/qt/widgets/configuration/configtree.hpp"

ConfigTreeWidget::ConfigTreeWidget(
        QWidget *parent) : QWidget(parent) {
    layout_.reset(new QVBoxLayout(this));
    layout_->setMargin(0);
    layout_->setSpacing(0);
    createActions();
    createWidgets();
}

ConfigTreeWidget::~ConfigTreeWidget() {
}

void ConfigTreeWidget::createActions() {
    cb_Prefix.reset(new QComboBox(this));
    cb_Prefix->setSizePolicy(QSizePolicy::MinimumExpanding,
                             QSizePolicy::Maximum);
    cb_Prefix->addItem(tr("Default"), 0);

    a_PrefixControl.reset(new QAction(this));
    a_PrefixControl->setIcon(getIcon("view-list-tree"));
    a_PrefixControl->setText(tr("Manage prefixes"));
    a_PrefixControl->setStatusTip(tr("Manage prefixes"));

    m_ToolBar.reset(new QToolBar(tr("Prefix"), this));
    m_ToolBar->addWidget(cb_Prefix.get());
    m_ToolBar->addAction(a_PrefixControl.get());
    layout_->addWidget(m_ToolBar.get());

    connect(a_PrefixControl.get(),
            SIGNAL(triggered()),
            this,
            SLOT(a_PrefixControl_Click()));
}

void ConfigTreeWidget::createWidgets() {
    w_ConfigTree.reset(new QTreeWidget(this));
    w_ConfigTree->header()->hide();
    layout_->addWidget(w_ConfigTree.get());
}

const QIcon ConfigTreeWidget::getIcon(QString name) const {
    return QIcon::fromTheme(name, QIcon(QString(":%1").arg(name)));
}

void ConfigTreeWidget::a_PrefixControl_Click() {
    return;
}
