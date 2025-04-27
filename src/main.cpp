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

#include <QApplication>
#include <QMessageBox>
#include <QTextStream>
#include <QTranslator>

#include "memory"

#ifdef DEBUG

#include <QDebug>
#endif

#include <locale.h>
#include <stdlib.h>

#include <QLibrary>
#include <QString>

#include "db.h"
#include "mainwindow.h"
#include "singleapplication.h"
#include "stdio.h"

// q4wine lib
#include "q4wine-lib.h"

int main(int argc, char *argv[]) {
  SingleApplication app(argc, argv);

  QTextStream QErr(stderr);

  QString exec_binary;
  if (app.arguments().count() > 2) {
    if ((app.arguments().at(1) == "--binary") or
        (app.arguments().at(1) == "-b")) {
      exec_binary = app.arguments().at(2);
      if (app.sendMessage(exec_binary.toUtf8())) return 0;
    }
  }

  if (app.sendMessage("")) return 0;

  //! This is needed for libq4wine-core.so import;
  typedef void *CoreLibPrototype(bool);
  CoreLibPrototype *CoreLibClassPointer;
  std::unique_ptr<corelib> CoreLib;
  QLibrary libq4wine;

  // Loading libq4wine-core.so
#ifdef RELEASE
  libq4wine.setFileName(_CORELIB_PATH_);
#else
  libq4wine.setFileName("../q4wine-lib/libq4wine-core");
#endif

  if (!libq4wine.load()) {
    qDebug() << libq4wine.errorString();
    libq4wine.load();
  }

  // Getting corelib class pointer
  CoreLibClassPointer =
      reinterpret_cast<CoreLibPrototype *>(libq4wine.resolve("createCoreLib"));
  CoreLib.reset(static_cast<corelib *>(CoreLibClassPointer(true)));

  if (!CoreLib.get()) {
    QErr << "[EE] Cannot load shared library." << Qt::endl;
    return -1;
  }

  QTranslator qtt;

#ifdef RELEASE
#ifdef _OS_DARWIN_
  QString l10nPath = QString("%1/%2.app/Contents/l10n")
                         .arg(QDir::currentPath())
                         .arg(APP_SHORT_NAME);
#else
  QString l10nPath = QString("%1/share/%2/l10n")
                         .arg(QString::fromUtf8(APP_PREF))
                         .arg(APP_SHORT_NAME);
#endif
#else
  QString l10nPath = QString("%1/l10n").arg(QString::fromUtf8(APP_BUILD));
#endif

  if (qtt.load(CoreLib->getTranslationLang(), l10nPath)) {
    app.installTranslator(&qtt);
  }

  QTextStream Qcout(stdout);
  int result, startState = 0;

  if (app.arguments().count() > 1) {
    if ((app.arguments().at(1) == "--version") or
        (app.arguments().at(1) == "-v")) {
      Qcout << QString("%1 %2").arg(APP_SHORT_NAME).arg(APP_VERS) << Qt::endl;
      Qcout << QString(
                   "Copyright (C) 2008-2025 by Oleksii S. Malakhov "
                   "<brezerk@gmail.com>")
            << Qt::endl;
      Qcout << QString(
                   "License GPLv3+: GNU GPL version 3 or later "
                   "<http://gnu.org/licenses/gpl.html>.")
            << Qt::endl;
      Qcout << QObject::tr(
                   "This is free software: you are free to change and "
                   "redistribute it.")
            << Qt::endl;
      Qcout << QObject::tr(
                   "There is NO WARRANTY, to the extent permitted by law.")
            << Qt::endl;
      CoreLib->getBuildFlags();
      Qcout << QObject::tr("Author: %1.").arg("Oleksii S. Malakhov")
            << Qt::endl;
      return 0;
    } else if ((app.arguments().at(1) == "--minimize") or
               (app.arguments().at(1) == "-m")) {
      startState = 1;
    } else if ((app.arguments().at(1) == "--binary") or
               (app.arguments().at(1) == "-b")) {
      // startState = 1;
    } else {
      Qcout << QObject::tr("Usage:") << Qt::endl;
      Qcout << QString("  %1 -b <unix_path_to_windown_binary>")
                   .arg(APP_SHORT_NAME)
            << Qt::endl;
      Qcout << QObject::tr("  %1 [KEY]...").arg(APP_SHORT_NAME) << Qt::endl;
      Qcout << QObject::tr(
                   "GUI utility for Wine applications and prefixes management.")
            << Qt::endl
            << Qt::endl;
      Qcout << QObject::tr("KEYs list:") << Qt::endl;
      Qcout << qSetFieldWidth(25) << Qt::left << "  -h,  --help"
            << QObject::tr("display this help and exit") << qSetFieldWidth(0)
            << Qt::endl;
      Qcout << qSetFieldWidth(25) << Qt::left << "  -v,  --version"
            << QObject::tr("output version information and exit")
            << qSetFieldWidth(0) << Qt::endl;
      Qcout << qSetFieldWidth(25) << Qt::left << "  -b,  --binary"
            << QObject::tr("open Q4Wine run dialog for Windows binary")
            << qSetFieldWidth(0) << Qt::endl;
      Qcout << qSetFieldWidth(25) << Qt::left << "  -m,  --minimize"
            << QObject::tr("minimize %1 main window on startup")
                   .arg(APP_SHORT_NAME)
            << qSetFieldWidth(0) << Qt::endl;
      Qcout << Qt::endl;
      Qcout << QObject::tr("Report %1 bugs to %2")
                   .arg(APP_SHORT_NAME)
                   .arg(APP_BUG_EMAIL)
            << Qt::endl;
      Qcout << QObject::tr("%1 homepage: <%2>")
                   .arg(APP_SHORT_NAME)
                   .arg(APP_WEBSITE)
            << Qt::endl;
      Qcout << QObject::tr(
                   "General help using GNU software: "
                   "<http://www.gnu.org/gethelp/>")
            << Qt::endl;
      return 0;
    }
  }

  if (!CoreLib->checkDirs(corelib::getAppConfigLocation())) {
    return -1;
  }

  if (!CoreLib->checkDirs(corelib::getGenericDataLocation(),
                          QStringList() << "wineprefixes")) {
    return -1;
  }

  DataBase db;

  if (!db.checkDb()) {
    QErr << "[EE] Cannot initialize database engine." << Qt::endl;
    return -1;
  }

  if (!CoreLib->isConfigured()) {
    Wizard firstSetupWizard(1);
    if (firstSetupWizard.exec() == QDialog::Rejected) {
      QErr << "[EE] Application not configured! Rerun the setup wizard or "
              "delete broken Q4Wine configuration files."
           << Qt::endl;
      return -1;
    }
  }

  MainWindow mainWin(startState, exec_binary);
  mainWin.show();
  mainWin.raise();
  mainWin.activateWindow();
  QObject::connect(&app, &SingleApplication::instanceStarted, &mainWin,
                   &MainWindow::mainRaiseAndShine);
  result = app.exec();
  db.close();
  return result;
}
