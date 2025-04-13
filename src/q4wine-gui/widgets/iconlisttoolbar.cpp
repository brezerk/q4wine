/***************************************************************************
 *   Copyright (C) 2008-2025 by Oleksii S. Malakhov <brezerk@gmail.com>    *
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

IconListToolbar::IconListToolbar(QWidget *parent) : QWidget(parent) {
  // Loading libq4wine-core.so
#ifdef RELEASE
  libq4wine.setFileName(_CORELIB_PATH_);
#else
  libq4wine.setFileName("../q4wine-lib/libq4wine-core");
#endif

  if (!libq4wine.load()) {
    libq4wine.load();
  }

  // Getting corelib class pointer
  CoreLibClassPointer =
      reinterpret_cast<CoreLibPrototype *>(libq4wine.resolve("createCoreLib"));
  CoreLib.reset(static_cast<corelib *>(CoreLibClassPointer(true)));

  this->sort_order =
      CoreLib
          ->getSetting("IconWidget", "IconSort", false, D_SORT_TYPE_BY_DATE_ASC)
          .toInt();
  this->view_mode =
      CoreLib->getSetting("IconWidget", "ViewMode", false, D_VIEW_MODE_ICON)
          .toInt();
  this->createActions();

  std::unique_ptr<QToolBar> toolBar(new QToolBar(this));

  toolBar->setIconSize(QSize(24, 24));
  toolBar->addAction(searchClear.get());

  searchField.reset(new QLineEdit(this));
  connect(searchField.get(), SIGNAL(textChanged(QString)), this,
          SIGNAL(searchFilterChange(QString)));

  // connect(searchField.get(), SIGNAL(returnPressed()), this,
  // SLOT(appdbSearch_Click()));
  toolBar->addWidget(searchField.get());

  toolBar->addSeparator();
  toolBar->addAction(viewMode.get());
  toolBar->addSeparator();
  toolBar->addAction(zoomIn.get());
  toolBar->addAction(zoomOut.get());
  toolBar->addSeparator();
  toolBar->addAction(sortAlpha.get());
  toolBar->addAction(sortCreation.get());

  std::unique_ptr<QVBoxLayout> layout(new QVBoxLayout(this));
  layout->setSpacing(0);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(toolBar.release());

  this->setLayout(layout.release());
}

void IconListToolbar::createActions() {
  searchClear.reset(new QAction(CoreLib->loadIcon("edit-clear"),
                                tr("Clear search field"), this));
  searchClear->setStatusTip(tr("Clear search field"));
  connect(searchClear.get(), SIGNAL(triggered()), this,
          SLOT(searchClear_Click()));

  viewMode.reset(new QAction(this));
  connect(viewMode.get(), SIGNAL(triggered()), this, SLOT(viewMode_Click()));

  if (this->view_mode == D_VIEW_MODE_LIST) {
    viewMode->setIcon(CoreLib->loadIcon("view-list-icons"));
    viewMode->setText(tr("Icons view mode"));
    viewMode->setStatusTip(tr("Icons view mode"));
  } else {
    viewMode->setIcon(CoreLib->loadIcon("view-list-details"));
    viewMode->setText(tr("Details view mode"));
    viewMode->setStatusTip(tr("Details view mode"));
  }

  zoomIn.reset(new QAction(CoreLib->loadIcon("zoom-in"), tr("Zoom In"), this));
  zoomIn->setStatusTip(tr("Zoom In"));
  zoomIn->setShortcut(Qt::Key_Plus);
  connect(zoomIn.get(), SIGNAL(triggered()), this, SLOT(zoomIn_Click()));

  zoomOut.reset(
      new QAction(CoreLib->loadIcon("zoom-out"), tr("Zoom Out"), this));
  zoomOut->setStatusTip(tr("Zoom Out"));
  zoomOut->setShortcut(Qt::Key_Minus);
  connect(zoomOut.get(), SIGNAL(triggered()), this, SLOT(zoomOut_Click()));

  sortAlpha.reset(new QAction(this));
  connect(sortAlpha.get(), SIGNAL(triggered()), this, SLOT(sortAlpha_Click()));

  if (this->sort_order == D_SORT_TYPE_BY_NAME_ASC) {
    sortAlpha->setStatusTip(tr("Alphabetic sort descending"));
    sortAlpha->setText(tr("Alphabetic sort descending"));
    sortAlpha->setIcon(CoreLib->loadIcon("view-sort-descending"));
  } else {
    sortAlpha->setStatusTip(tr("Alphabetic sort ascending"));
    sortAlpha->setText(tr("Alphabetic sort ascending"));
    sortAlpha->setIcon(CoreLib->loadIcon("view-sort-ascending"));
  }

  sortCreation.reset(new QAction(this));
  connect(sortCreation.get(), SIGNAL(triggered()), this,
          SLOT(sortCreation_Click()));

  if (this->sort_order == D_SORT_TYPE_BY_DATE_ASC) {
    sortCreation->setStatusTip(tr("Sort descending by create order"));
    sortCreation->setText(tr("Sort descending by create order"));
    sortCreation->setIcon(CoreLib->loadIcon("view-sort-descending"));
  } else {
    sortCreation->setStatusTip(tr("Sort ascending by create order"));
    sortCreation->setText(tr("Sort ascending by create order"));
    sortCreation->setIcon(CoreLib->loadIcon("view-sort-ascending"));
  }

  return;
}

void IconListToolbar::searchClear_Click() { searchField->clear(); }

void IconListToolbar::viewMode_Click() {
  if (this->view_mode == D_VIEW_MODE_ICON) {
    this->view_mode = D_VIEW_MODE_LIST;
    viewMode->setIcon(CoreLib->loadIcon("view-list-icons"));
    viewMode->setText(tr("Icons view mode"));
    viewMode->setStatusTip(tr("Icons view mode"));
    emit(changeView(0));
  } else {
    this->view_mode = D_VIEW_MODE_ICON;
    viewMode->setIcon(CoreLib->loadIcon("view-list-details"));
    viewMode->setText(tr("Details view mode"));
    viewMode->setStatusTip(tr("Details view mode"));
    emit(changeView(1));
  }
}

void IconListToolbar::zoomIn_Click() { emit(changeView(2)); }

void IconListToolbar::zoomOut_Click() { emit(changeView(3)); }

void IconListToolbar::sortAlpha_Click() {
  emit(changeView(4));
  if (this->sort_order == D_SORT_TYPE_BY_NAME_ASC) {
    this->sort_order = D_SORT_TYPE_BY_NAME_DSC;
    sortAlpha->setStatusTip(tr("Alphabetic sort ascending"));
    sortAlpha->setText(tr("Alphabetic sort ascending"));
    sortAlpha->setIcon(CoreLib->loadIcon("view-sort-ascending"));
  } else {
    this->sort_order = D_SORT_TYPE_BY_NAME_ASC;
    sortAlpha->setStatusTip(tr("Alphabetic sort descending"));
    sortAlpha->setText(tr("Alphabetic sort descending"));
    sortAlpha->setIcon(CoreLib->loadIcon("view-sort-descending"));
  }
  sortCreation->setStatusTip(tr("Sort ascending by create order"));
  sortCreation->setText(tr("Sort ascending by create order"));
  sortCreation->setIcon(CoreLib->loadIcon("view-sort-ascending"));
}

void IconListToolbar::sortCreation_Click() {
  emit(changeView(5));
  if (this->sort_order == D_SORT_TYPE_BY_DATE_ASC) {
    this->sort_order = D_SORT_TYPE_BY_DATE_DSC;
    sortCreation->setStatusTip(tr("Sort ascending by create order"));
    sortCreation->setText(tr("Sort ascending by create order"));
    sortCreation->setIcon(CoreLib->loadIcon("view-sort-ascending"));
  } else {
    this->sort_order = D_SORT_TYPE_BY_DATE_ASC;
    sortCreation->setStatusTip(tr("Sort descending by create order"));
    sortCreation->setText(tr("Sort descending by create order"));
    sortCreation->setIcon(CoreLib->loadIcon("view-sort-descending"));
  }
  sortAlpha->setStatusTip(tr("Alphabetic sort ascending"));
  sortAlpha->setText(tr("Alphabetic sort ascending"));
  sortAlpha->setIcon(CoreLib->loadIcon("view-sort-ascending"));
}
