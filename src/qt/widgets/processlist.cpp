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

#include "src/qt/widgets/processlist.hpp"

ProcessListWidget::ProcessListWidget(QWidget *parent) : QWidget(parent) {
    layout_.reset(new QVBoxLayout(this));
    layout_->setMargin(0);
    layout_->setSpacing(0);
    createActions();
    createWidgets();
}

ProcessListWidget::~ProcessListWidget() {
}

void ProcessListWidget::createActions() {
    a_Kill.reset(new QAction(getIcon("process-stop"),
                              tr("Stop current"),
                               this));
    a_Kill->setStatusTip(tr("Send TERM signal to selected process"));


    a_KillAll.reset(new QAction(getIcon("system-shutdown"),
                               tr("Stop wine prefix"),
                               this));
    a_KillAll->setStatusTip(tr("Send TERM signal to main wine process for "
                               "selected prefix"));

    a_Refresh.reset(new QAction(getIcon("view-refresh"),
                               tr("Refresh List"),
                               this));
    a_Refresh->setStatusTip(tr("Refresh process list"));


    m_ToolBar.reset(new QToolBar(tr("Process list"), this));
    m_ToolBar->addAction(a_Kill.get());
    m_ToolBar->addAction(a_KillAll.get());
    m_ToolBar->addSeparator();
    m_ToolBar->addAction(a_Refresh.get());

    layout_->addWidget(m_ToolBar.get());

    connect(a_Kill.get(),
            SIGNAL(triggered()),
            this,
            SLOT(a_Kill_Click()));
    connect(a_KillAll.get(),
            SIGNAL(triggered()),
            this,
            SLOT(a_KillAll_Click()));
    connect(a_Refresh.get(),
            SIGNAL(triggered()),
            this,
            SLOT(a_Refresh_Click()));
}

void ProcessListWidget::createWidgets() {
    w_ProcessList.reset(new QTableView(this));

    model_.reset(new QStandardItemModel());
    model_->setColumnCount(4);
    model_->setHeaderData(0, Qt::Horizontal, tr("PID"),
                          Qt::DisplayRole);
    model_->setHeaderData(1, Qt::Horizontal, tr("Name"),
                          Qt::DisplayRole);
    model_->setHeaderData(2, Qt::Horizontal, tr("Ni"),
                          Qt::DisplayRole);
    model_->setHeaderData(3, Qt::Horizontal, tr("Prefix path"),
                          Qt::DisplayRole);

    w_ProcessList->setContextMenuPolicy(Qt::CustomContextMenu);
    w_ProcessList->setModel(model_.get());

    layout_->addWidget(w_ProcessList.get());
}

const QIcon ProcessListWidget::getIcon(QString name) const {
    return QIcon::fromTheme(name, QIcon(QString(":%1").arg(name)));
}

void ProcessListWidget::a_Kill_Click() {
    return;
}

void ProcessListWidget::a_KillAll_Click() {
    return;
}

void ProcessListWidget::a_Refresh_Click() {
    return;
}
