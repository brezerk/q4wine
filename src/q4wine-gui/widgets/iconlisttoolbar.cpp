/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010 by Malakhov Alexey                     *
 *   brezerk@gmail.com                                                     *
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

#include "iconlisttoolbar.h"

IconListToolbar::IconListToolbar(QWidget *parent) :
    QWidget(parent)
{
    // Loading libq4wine-core.so
    libq4wine.setFileName("libq4wine-core");

    if (!libq4wine.load()){
          libq4wine.load();
    }

    // Getting corelib calss pointer
    CoreLibClassPointer = (CoreLibPrototype *) libq4wine.resolve("createCoreLib");
    CoreLib.reset((corelib *)CoreLibClassPointer(true));

    this->createActions();

    std::auto_ptr<QToolBar> toolBar (new QToolBar(this));
    toolBar->addAction(searchClear.get());

    searchField.reset (new QLineEdit(this));
    connect(searchField.get(), SIGNAL(textChanged(QString)), this, SIGNAL(searchFilterChange(QString)));

    //connect(searchField.get(), SIGNAL(returnPressed()), this, SLOT(appdbSearch_Click()));
    toolBar->addWidget(searchField.get());

    toolBar->addSeparator();
    toolBar->addAction(viewIcon.get());
    toolBar->addAction(viewList.get());
    toolBar->addSeparator();
    toolBar->addAction(zoomIn.get());
    toolBar->addAction(zoomOut.get());

    std::auto_ptr<QVBoxLayout> layout (new QVBoxLayout(this));
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(toolBar.release());

    this->setLayout(layout.release());
}

void IconListToolbar::createActions(){
    searchClear.reset(new QAction(CoreLib->loadIcon("data/clear-ltr.png"), tr("Clear search field"), this));
    searchClear->setStatusTip(tr("Clear search field"));
    connect(searchClear.get(), SIGNAL(triggered()), this, SLOT(searchClear_Click()));

    viewIcon.reset(new QAction(CoreLib->loadIcon("data/view-list-details.png"), tr("Delete log"), this));
    viewIcon->setStatusTip(tr("Delete current log"));
    connect(viewIcon.get(), SIGNAL(triggered()), this, SLOT(viewIcon_Click()));

    viewList.reset(new QAction(CoreLib->loadIcon("data/view-list-icons.png"), tr("Export log"), this));
    viewList->setStatusTip(tr("Export current log to file"));
    connect(viewList.get(), SIGNAL(triggered()), this, SLOT(viewList_Click()));

    zoomIn.reset(new QAction(CoreLib->loadIcon("data/zoom-in.png"), tr("Export log"), this));
    zoomIn->setStatusTip(tr("Export current log to file"));
    connect(zoomIn.get(), SIGNAL(triggered()), this, SLOT(zoomIn_Click()));

    zoomOut.reset(new QAction(CoreLib->loadIcon("data/zoom-out.png"), tr("Export log"), this));
    zoomOut->setStatusTip(tr("Export current log to file"));
    connect(zoomOut.get(), SIGNAL(triggered()), this, SLOT(zoomOut_Click()));

    return;
}

void IconListToolbar::searchClear_Click(){
    searchField->clear();
}

void IconListToolbar::viewIcon_Click(){
    emit(changeView(0));
}

void IconListToolbar::viewList_Click(){
    emit(changeView(1));
}

void IconListToolbar::zoomIn_Click(){
    emit(changeView(2));
}

void IconListToolbar::zoomOut_Click(){
    emit(changeView(3));
}
