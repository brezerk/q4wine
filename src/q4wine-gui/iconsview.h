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

#ifndef ICONSVIEW_H
#define ICONSVIEW_H

#include <src/q4wine-gui/ui_IconsView.h>

#include <QAbstractItemView>
#include <QDialog>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <memory>

#include "config.h"

// q4wine lib
#include "q4wine-lib.h"

class IconsView : public QDialog, public Ui::IconsView {
  Q_OBJECT
 public:
  IconsView(QString tmpDir, QWidget *parent = nullptr,
            Qt::WindowFlags f = Qt::WindowFlags());
  QString selectedFile;

 private slots:
  void cmdCancel_Click();
  void cmdOk_Click();

 private:
  //! This is need for libq4wine-core.so import;
  typedef void *CoreLibPrototype(bool);
  CoreLibPrototype *CoreLibClassPointer;
  std::unique_ptr<corelib> CoreLib;
  QLibrary libq4wine;

  QString tempDirectory;
};

#endif
