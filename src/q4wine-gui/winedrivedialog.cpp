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

#include "winedrivedialog.h"

WineDriveDialog::WineDriveDialog(QStringList removeLetters, QWidget *parent,
                                 Qt::WindowFlags f)
    : QDialog(parent, f) {
  setupUi(this);

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

  setWindowIcon(CoreLib->loadIcon(
      CoreLib->getSetting("app", "icon", false, "q4wine").toString()));

  if (removeLetters.count() > 0) {
    for (int i = 0; i < removeLetters.count(); i++) {
      if (cbDriveLetter->findText(removeLetters.at(i), Qt::MatchExactly) >= 0) {
        cbDriveLetter->removeItem(cbDriveLetter->findText(removeLetters.at(i)));
      }
    }
  }

  if (cbDriveLetter->count() > 2) {
    cbDriveLetter->setCurrentIndex(2);
  }

  connect(cmdCancel, SIGNAL(clicked()), this, SLOT(cmdCancel_Click()));
  connect(cmdOk, SIGNAL(clicked()), this, SLOT(cmdOk_Click()));
  connect(cmdGetDrivePath, SIGNAL(clicked()), this,
          SLOT(cmdGetDrivePath_Click()));

  cmdGetDrivePath->setIcon(CoreLib->loadIcon("document-open"));

  cmdOk->setFocus(Qt::ActiveWindowFocusReason);
}

WineDriveDialog::WineDriveDialog(QStringList removeLetters, QString driveLetter,
                                 QString drivePath, QString driveType,
                                 QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f) {
  setupUi(this);

  // Loading libq4wine-core.so
  libq4wine.setFileName(_CORELIB_PATH_);

  if (!libq4wine.load()) {
    libq4wine.load();
  }

  // Getting corelib class pointer
  CoreLibClassPointer =
      reinterpret_cast<CoreLibPrototype *>(libq4wine.resolve("createCoreLib"));
  CoreLib.reset(static_cast<corelib *>(CoreLibClassPointer(true)));

  if (removeLetters.count() > 0) {
    for (int i = 0; i < removeLetters.count(); i++) {
      if (cbDriveLetter->findText(removeLetters.at(i), Qt::MatchExactly) >= 0) {
        cbDriveLetter->removeItem(cbDriveLetter->findText(removeLetters.at(i)));
      }
    }
  }

  if (cbDriveLetter->findText(driveLetter, Qt::MatchExactly) >= 0) {
    cbDriveLetter->setCurrentIndex(
        cbDriveLetter->findText(driveLetter, Qt::MatchExactly));
  }

  txtDrivePath->setText(drivePath);

  if (cbDriveType->findText(driveType, Qt::MatchExactly) >= 0) {
    cbDriveType->setCurrentIndex(
        cbDriveType->findText(driveType, Qt::MatchExactly));
  }

  if (driveLetter == "C:") {
    cbDriveLetter->setEnabled(false);
    cbDriveType->setEnabled(false);
    cmdGetDrivePath->setEnabled(false);
    txtDrivePath->setEnabled(false);
  }

  connect(cmdCancel, SIGNAL(clicked()), this, SLOT(cmdCancel_Click()));
  connect(cmdOk, SIGNAL(clicked()), this, SLOT(cmdOk_Click()));
  connect(cmdGetDrivePath, SIGNAL(clicked()), this,
          SLOT(cmdGetDrivePath_Click()));

  cmdGetDrivePath->setIcon(CoreLib->loadIcon("document-open"));

  cmdOk->setFocus(Qt::ActiveWindowFocusReason);
}

QString WineDriveDialog::getLetter() { return cbDriveLetter->currentText(); }

QString WineDriveDialog::getPath() { return txtDrivePath->text(); }

QString WineDriveDialog::getType() { return cbDriveType->currentText(); }

void WineDriveDialog::cmdCancel_Click() { reject(); }

void WineDriveDialog::cmdOk_Click() {
  if (cbDriveLetter->currentText() == "C:") {
    accept();
    return;
  }

  if (txtDrivePath->text().isEmpty()) {
    QMessageBox::warning(this, tr("Error"),
                         tr("Sorry, you need to set existing drive directory."),
                         QMessageBox::Ok);
    return;
  }

  if (!QDir().exists(txtDrivePath->text())) {
    QMessageBox::warning(this, tr("Error"),
                         tr("Sorry, drive directory does not exist."),
                         QMessageBox::Ok);
    return;
  }

  accept();
}

void WineDriveDialog::cmdGetDrivePath_Click() {
  QString fileName;
  QString searchPath = txtDrivePath->text();

  QFileDialog dialog(this);
  dialog.setFilter(QDir::Dirs | QDir::Hidden);

  if (CoreLib->getSetting("advanced", "useNativeFileDialog", false, 1) == 0) {
    dialog.setOptions(QFileDialog::DontUseNativeDialog);
  }

  dialog.setFileMode(QFileDialog::Directory);
  dialog.setWindowTitle(tr("Open Directory"));
  if (!searchPath.isEmpty()) {
    if (QDir(searchPath).exists()) {
      dialog.setDirectory(searchPath);
    } else {
      dialog.setDirectory(QDir::homePath());
    }
  } else {
    dialog.setDirectory(QDir::homePath());
  }

  if (dialog.exec()) fileName = dialog.selectedFiles().first();

  if (!fileName.isEmpty()) {
    txtDrivePath->setText(fileName);
  }

  return;
}
