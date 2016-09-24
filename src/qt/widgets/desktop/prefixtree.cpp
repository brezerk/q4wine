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

#include "src/qt/widgets/desktop/prefixtree.hpp"

PrefixTreeWidget::PrefixTreeWidget(QWidget *parent) : QWidget(parent) {
    layout_.reset(new QVBoxLayout(this));
    layout_->setMargin(0);
    layout_->setSpacing(0);
    createActions();
    createWidgets();
}

PrefixTreeWidget::~PrefixTreeWidget() {
}

void PrefixTreeWidget::createActions() {
    a_TreeState.reset(new QAction(this));
    a_TreeState->setIcon(getIcon("view-list-tree"));
    a_TreeState->setText(tr("Expand prefix tree"));
    a_TreeState->setStatusTip(tr("Expand prefix tree"));

    /*
    treeState->setIcon(CoreLib->loadIcon("view-list-details"));
    treeState->setText(tr("Collapse prefix tree"));
    treeState->setStatusTip(tr("Collapse prefix tree"));
    */

    a_Import.reset(new QAction(getIcon("document-import"),
                               tr("Import prefixes"),
                               this));
    a_Import->setStatusTip(tr("Import prefixes from "
                              "~/.local/share/wineprefixes/"));

    a_Export.reset(new QAction(getIcon("document-export"),
                               tr("Export prefixes"),
                               this));
    a_Export->setStatusTip(tr("Export prefixes to "
                              "~/.local/share/wineprefixes/"));

    m_ToolBar.reset(new QToolBar(tr("Prefix"), this));
    m_ToolBar->addAction(a_TreeState.get());
    m_ToolBar->addSeparator();
    m_ToolBar->addAction(a_Import.get());
    m_ToolBar->addAction(a_Export.get());
    layout_->addWidget(m_ToolBar.get());

    connect(a_TreeState.get(),
            SIGNAL(triggered()),
            this,
            SLOT(a_TreeState_Click()));
    connect(a_Import.get(),
            SIGNAL(triggered()),
            this,
            SLOT(a_Import_Click()));
    connect(a_Export.get(),
            SIGNAL(triggered()),
            this,
            SLOT(a_Export_Click()));
}

void PrefixTreeWidget::createWidgets() {
    w_PrefixTree.reset(new QTreeWidget(this));
    w_PrefixTree->header()->hide();
    layout_->addWidget(w_PrefixTree.get());
}

const QIcon PrefixTreeWidget::getIcon(QString name) const {
    return QIcon::fromTheme(name, QIcon(QString(":%1").arg(name)));
}

void PrefixTreeWidget::a_TreeState_Click() {
    return;
}

void PrefixTreeWidget::a_Export_Click() {
    return;
}

void PrefixTreeWidget::a_Import_Click() {
    return;
}
