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

#include "src/qt/widgets/prefix.hpp"

PrefixConfigWidget::PrefixConfigWidget(
        QWidget *parent) : QWidget(parent) {
    layout_.reset(new QVBoxLayout(this));
    layout_->setMargin(0);
    layout_->setSpacing(0);
    createActions();
    createWidgets();
}

PrefixConfigWidget::~PrefixConfigWidget() {
}

void PrefixConfigWidget::createActions() {
    a_New.reset(new QAction(getIcon("document-new"),
                              tr("Create new prefix"),
                               this));
    a_New->setStatusTip(tr("Create new prefix"));

    a_Import.reset(new QAction(getIcon("document-import"),
                              tr("Import prefix from tar archive"),
                               this));
    a_Import->setStatusTip(tr("Import prefix from tar archive"));

    a_Export.reset(new QAction(getIcon("document-export"),
                              tr("Export prefix as tar archive"),
                               this));
    a_Export->setStatusTip(tr("Export prefix as tar archive"));

    a_Delete.reset(new QAction(getIcon("edit-delete"),
                              tr("Delete prefix"),
                               this));
    a_Delete->setStatusTip(tr("Delete prefix"));

    a_Edit.reset(new QAction(getIcon("document-edit"),
                              tr("Edit prefix"),
                               this));
    a_Edit->setStatusTip(tr("Edit prefix"));

    a_Configure.reset(new QAction(getIcon("document-properties"),
                              tr("Set up prefix fake drive"),
                               this));
    a_Configure->setStatusTip(tr("Set up prefix fake drive"));

    a_ConfigureVersions.reset(new QAction(getIcon("configure"),
                              tr("Configure wine versions"),
                               this));
    a_ConfigureVersions->setStatusTip(tr("Configure wine versions"));

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
    m_ToolBar->addAction(a_New.get());
    m_ToolBar->addSeparator();
    m_ToolBar->addAction(a_Import.get());
    m_ToolBar->addAction(a_Export.get());
    m_ToolBar->addSeparator();
    m_ToolBar->addAction(a_Delete.get());
    m_ToolBar->addSeparator();
    m_ToolBar->addAction(a_Edit.get());
    m_ToolBar->addSeparator();
    m_ToolBar->addAction(a_Configure.get());
    m_ToolBar->addAction(a_ConfigureVersions.get());
    m_ToolBar->addSeparator();
    m_ToolBar->addAction(a_Clear.get());
    m_ToolBar->addWidget(txt_Filter.get());
    m_ToolBar->addAction(a_SortAlpha.get());

    layout_->addWidget(m_ToolBar.get());
    connect(a_New.get(),
            SIGNAL(triggered()),
            this,
            SLOT(a_New_Click()));
    connect(a_Import.get(),
            SIGNAL(triggered()),
            this,
            SLOT(a_Import_Click()));
    connect(a_Export.get(),
            SIGNAL(triggered()),
            this,
            SLOT(a_Export_Click()));
    connect(a_Delete.get(),
            SIGNAL(triggered()),
            this,
            SLOT(a_Delete_Click()));
    connect(a_Edit.get(),
            SIGNAL(triggered()),
            this,
            SLOT(a_Edit_Click()));
    connect(a_Configure.get(),
            SIGNAL(triggered()),
            this,
            SLOT(a_Configure_Click()));
    connect(a_ConfigureVersions.get(),
            SIGNAL(triggered()),
            this,
            SLOT(a_ConfigureVersions_Click()));
    connect(a_Clear.get(),
            SIGNAL(triggered()),
            this,
            SLOT(a_Clear_Click()));
    connect(a_SortAlpha.get(),
            SIGNAL(triggered()),
            this,
            SLOT(a_SortAlpha_Click()));
}

void PrefixConfigWidget::createWidgets() {
    tbl_Prefixes.reset(new QTableView(this));

    model_.reset(new QStandardItemModel());
    model_->setColumnCount(4);
    model_->setHeaderData(0, Qt::Horizontal, tr("Name"),
                          Qt::DisplayRole);
    model_->setHeaderData(1, Qt::Horizontal, tr("Version"),
                          Qt::DisplayRole);
    model_->setHeaderData(2, Qt::Horizontal, tr("Arch"),
                          Qt::DisplayRole);
    model_->setHeaderData(3, Qt::Horizontal, tr("Prefix path"),
                          Qt::DisplayRole);


    tbl_Prefixes->setContextMenuPolicy(Qt::CustomContextMenu);
    tbl_Prefixes->setModel(model_.get());

    tbl_Prefixes->horizontalHeader()->setSectionResizeMode(
                3, QHeaderView::Stretch);

    layout_->addWidget(tbl_Prefixes.get());
}

const QIcon PrefixConfigWidget::getIcon(QString name) const {
    return QIcon::fromTheme(name, QIcon(QString(":%1").arg(name)));
}

void PrefixConfigWidget::a_Clear_Click() {
    return;
}

void PrefixConfigWidget::a_SortAlpha_Click() {
    return;
}

void PrefixConfigWidget::a_New_Click() {
    return;
}

void PrefixConfigWidget::a_Import_Click() {
    return;
}

void PrefixConfigWidget::a_Export_Click() {
    return;
}

void PrefixConfigWidget::a_Delete_Click() {
    return;
}

void PrefixConfigWidget::a_Edit_Click() {
    return;
}

void PrefixConfigWidget::a_Configure_Click() {
    return;
}

void PrefixConfigWidget::a_ConfigureVersions_Click() {
    return;
}
