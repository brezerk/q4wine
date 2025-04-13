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

#ifndef PROCESS_H
#define PROCESS_H

#include <QFileDialog>
#include <QLibrary>
#include <QMessageBox>
#include <QProcess>
#include <QStringDecoder>
#include <QTextStream>
#include <clocale>
#include <memory>

#include "config.h"
#include "src/q4wine-lib/ui_Process.h"

#ifdef DEBUG
#include <QDebug>
#endif

class Process : public QDialog, public Ui::Process {
  Q_OBJECT
 public:
  Process(QStringList args, QString exec, QString dir, QString info,
          QString caption, bool showErr = true,
          QStringList env = QProcess::systemEnvironment(),
          QWidget* parent = nullptr, Qt::WindowFlags f = Qt::Widget);

 private slots:
  void slotFinished(int, QProcess::ExitStatus);
  void cmdCancel_clicked();
  void slotError(QProcess::ProcessError);

 private:
  bool showErr;
  std::unique_ptr<QProcess> myProcess;

  QString getLocale();
};

#endif
