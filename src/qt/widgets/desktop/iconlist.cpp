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

#include "src/qt/widgets/desktop/iconlist.hpp"

IconListWidget::IconListWidget(QWidget *parent) : QWidget(parent) {
    layout_.reset(new QVBoxLayout(this));
    layout_->setMargin(0);
    layout_->setSpacing(0);
    createActions();
    createWidgets();
}

IconListWidget::~IconListWidget() {
}

void IconListWidget::createActions() {
    a_Clear.reset(new QAction(getIcon("edit-clear"),
                              tr("Clear search field"),
                               this));
    a_Clear->setStatusTip(tr("Clear search field"));

    a_Mode.reset(new QAction(this));
    a_Mode->setIcon(getIcon("view-list-tree"));
    a_Mode->setText(tr("Icons view mode"));
    a_Mode->setStatusTip(tr("Icons view mode"));

    /*
        viewMode->setIcon(CoreLib->loadIcon("view-list-details"));
        viewMode->setText(tr("Details view mode"));
        viewMode->setStatusTip(tr("Details view mode"));
     */

    a_ZoomIn.reset(new QAction(getIcon("zoom-in"),
                               tr("Zoom In"),
                               this));
    a_ZoomIn->setStatusTip(tr("Zoom In"));

    a_ZoomOut.reset(new QAction(getIcon("zoom-out"),
                               tr("Zoom Out"),
                               this));
    a_ZoomOut->setStatusTip(tr("Zoom Out"));

    a_SortAlpha.reset(new QAction(getIcon("view-sort-descending"),
                               tr("Alphabetic sort descending"),
                               this));
    a_SortAlpha->setStatusTip(tr("Alphabetic sort descending"));

    /*
        sortAlpha->setStatusTip(tr("Alphabetic sort ascending"));
        sortAlpha->setText(tr("Alphabetic sort ascending"));
        sortAlpha->setIcon(CoreLib->loadIcon("view-sort-ascending"));
     */

    a_SortDate.reset(new QAction(getIcon("view-sort-descending"),
                               tr("Sort descending by create order"),
                               this));
    a_SortDate->setStatusTip(tr("Sort descending by create order"));

    /*
        sortCreation->setStatusTip(tr("Sort ascending by create order"));
        sortCreation->setText(tr("Sort ascending by create order"));
        sortCreation->setIcon(CoreLib->loadIcon("view-sort-ascending"));
     */
    txt_Filter.reset(new QLineEdit(this));

    m_ToolBar.reset(new QToolBar(tr("Prefix"), this));
    m_ToolBar->addAction(a_Clear.get());
    m_ToolBar->addWidget(txt_Filter.get());
    m_ToolBar->addAction(a_Mode.get());
    m_ToolBar->addSeparator();
    m_ToolBar->addAction(a_ZoomIn.get());
    m_ToolBar->addAction(a_ZoomOut.get());
    m_ToolBar->addSeparator();
    m_ToolBar->addAction(a_SortAlpha.get());
    m_ToolBar->addAction(a_SortDate.get());

    layout_->addWidget(m_ToolBar.get());

    connect(a_Clear.get(),
            SIGNAL(triggered()),
            this,
            SLOT(a_Clear_Click()));
    connect(a_Mode.get(),
            SIGNAL(triggered()),
            this,
            SLOT(a_Mode_Click()));
    connect(a_ZoomIn.get(),
            SIGNAL(triggered()),
            this,
            SLOT(a_ZoomIn_Click()));
    connect(a_ZoomOut.get(),
            SIGNAL(triggered()),
            this,
            SLOT(a_ZoomOut_Click()));
    connect(a_SortAlpha.get(),
            SIGNAL(triggered()),
            this,
            SLOT(a_SortAlpha_Click()));
    connect(a_SortDate.get(),
            SIGNAL(triggered()),
            this,
            SLOT(a_SortDate_Click()));
}

void IconListWidget::createWidgets() {
    w_IconsList.reset(new QListWidget(this));
    layout_->addWidget(w_IconsList.get());
}

const QIcon IconListWidget::getIcon(QString name) const {
    return QIcon::fromTheme(name, QIcon(QString(":%1").arg(name)));
}

void IconListWidget::a_Clear_Click() {
    return;
}

void IconListWidget::a_Mode_Click() {
    return;
}

void IconListWidget::a_ZoomIn_Click() {
    return;
}

void IconListWidget::a_ZoomOut_Click() {
    return;
}

void IconListWidget::a_SortAlpha_Click() {
    return;
}

void IconListWidget::a_SortDate_Click() {
    return;
}
