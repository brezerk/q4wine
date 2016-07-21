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

#include "src/qt/widgets/prefixtree.hpp"

PrefixTreeWidget::PrefixTreeWidget(QWidget *parent) : QWidget(parent) {
    createActions();
    createWidgets();
}

PrefixTreeWidget::~PrefixTreeWidget() {
}

void PrefixTreeWidget::createActions() {
    treeState.reset(new QAction(this));
    if (this->tree_state == D_TREE_COLLAPSE){
        treeState->setIcon(CoreLib->loadIcon("view-list-tree"));
        treeState->setText(tr("Expand prefix tree"));
        treeState->setStatusTip(tr("Expand prefix tree"));
        emit(collapseTree());
    } else {
        treeState->setIcon(CoreLib->loadIcon("view-list-details"));
        treeState->setText(tr("Collapse prefix tree"));
        treeState->setStatusTip(tr("Collapse prefix tree"));
        emit(expandTree());
    }

    connect(treeState.get(), SIGNAL(triggered()), this, SLOT(treeState_Click()));

    prefixImport.reset(new QAction(CoreLib->loadIcon("document-import"), tr("Import prefixes"), this));
    prefixImport->setStatusTip(tr("Import prefixes from ~/.local/share/wineprefixes/"));
    connect(prefixImport.get(), SIGNAL(triggered()), this, SLOT(prefixImport_Click()));

    prefixExport.reset(new QAction(CoreLib->loadIcon("document-export"), tr("Export prefixes"), this));
    prefixExport->setStatusTip(tr("Export prefixes to ~/.local/share/wineprefixes/"));
    connect(prefixExport.get(), SIGNAL(triggered()), this, SLOT(prefixExport_Click()));
}

void PrefixTreeWidget::createWidgets() {
    w_PrefixTree.reset(new QTreeWidget());
    w_IconList.reset(new QListWidget());
}

const QIcon PrefixTreeWidget::getIcon(QString name) const {
    return QIcon::fromTheme(name, QIcon(QString(":%1").arg(name)));
}
