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

#include "src/qt/mainwindow.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    createActions();
    createWidgets();
    createStatusBar();
    readSettings();
}

MainWindow::~MainWindow() {
    writeSettings();
}

void MainWindow::createActions() {
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QToolBar *fileToolBar = addToolBar(tr("File"));

    QAction *act = new QAction(getIcon("document-open"),
                               tr("&Run..."), this);
    act->setShortcuts(QKeySequence::Open);
    act->setStatusTip(tr("Run an existing Windows binary file."));
    //  connect(act, &QAction::triggered, this, &MainWindow::run);
    fileMenu->addAction(act);
    fileToolBar->addAction(act);

    fileMenu->addSeparator();

    act = new QAction(getIcon("document-properties"),
                      tr("&Preferences"), this);
    act->setShortcuts(QKeySequence::Preferences);
    act->setStatusTip(tr("Open application preferences."));
    //  connect(act, &QAction::triggered, this, &MainWindow::preferences);
    fileMenu->addAction(act);
    fileToolBar->addAction(act);

    fileMenu->addSeparator();

    act = fileMenu->addAction(getIcon("application-exit"),
                              tr("E&xit"));
    act->setShortcuts(QKeySequence::Quit);
    act->setStatusTip(tr("Exit the application."));
    connect(act, &QAction::triggered, this, &MainWindow::close);

    QMenu *toolMenu = menuBar()->addMenu(tr("&Tools"));
    QToolBar *toolToolBar = addToolBar(tr("Tools"));

    act = toolMenu->addAction(getIcon("view-list-icons"),
                              tr("&Desktop"));
    act->setStatusTip(tr("Show desktop tab."));
    //  connect(act, &QAction::triggered, this, &MainWindow::desktop);
    toolMenu->addAction(act);
    toolToolBar->addAction(act);

    act = toolMenu->addAction(getIcon("view-list-details"),
                              tr("&Process"));
    act->setStatusTip(tr("Show wine process tab."));
    //  connect(act, &QAction::triggered, this, &MainWindow::process);
    toolMenu->addAction(act);
    toolToolBar->addAction(act);

    act = toolMenu->addAction(getIcon("tools-wizard"),
                              tr("&Configuration"));
    act->setStatusTip(tr("Show configuration tab."));
    //  connect(act, &QAction::triggered, this, &MainWindow::configure);
    toolMenu->addAction(act);
    toolToolBar->addAction(act);

    act = toolMenu->addAction(getIcon("configure"),
                              tr("&Prefix"));
    act->setStatusTip(tr("Show prefix list tab."));
    //  connect(act, &QAction::triggered, this, &MainWindow::prefix);
    toolMenu->addAction(act);
    toolToolBar->addAction(act);

    act = toolMenu->addAction(getIcon("view-history"),
                              tr("&Logs"));
    act->setStatusTip(tr("Show logs tab."));
    //  connect(act, &QAction::triggered, this, &MainWindow::log);
    toolMenu->addAction(act);
    toolToolBar->addAction(act);

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QToolBar *helpToolBar = addToolBar(tr("Help"));

    act = helpMenu->addAction(getIcon("help-about"),
                              tr("&About"));
    act->setStatusTip(tr("About application."));
    //  connect(act, &QAction::triggered, this, &MainWindow::about);
    helpMenu->addAction(act);

    act = helpMenu->addAction(getIcon("help-about"),
                              tr("&About Qt"));
    act->setStatusTip(tr("About Qt."));
    connect(act, &QAction::triggered, this, &QApplication::aboutQt);
    helpMenu->addAction(act);

    act = helpMenu->addAction(getIcon("help-contents"),
                              tr("&Help"));
    act->setStatusTip(tr("Display help contents."));
    //  connect(act, &QAction::triggered, this, &MainWindow::help);
    helpMenu->addAction(act);
    helpToolBar->addAction(act);
}

void MainWindow::createWidgets() {
    QTabWidget* tab = new QTabWidget(this);
    //  QWidget* desktopWidget = new QWidget(tab);
    QWidget* processWidget = new ProcessWidget(tab);
    QWidget* configureWidget = new ConfigurationWidget(tab);
    QWidget* prefixpWidget = new PrefixConfigWidget(tab);
    QWidget* logWidget = new QWidget(tab);
    QWidget* desktop = new DesktopWidget(tab);
    tab->addTab(desktop,
                getIcon("view-list-icons"),
                tr("&Desktop"));
    tab->addTab(processWidget, getIcon("view-list-details"), tr("&Process"));
    tab->addTab(configureWidget, getIcon("tools-wizard"), tr("&Configuration"));
    tab->addTab(prefixpWidget, getIcon("configure"), tr("&Prefix"));
    tab->addTab(logWidget, getIcon("view-history"), tr("&Logs"));
    this->setCentralWidget(tab);
}

void MainWindow::createStatusBar() {
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings() {
    QSettings settings(QCoreApplication::organizationName(),
                       QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry",
                                               QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = \
                QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }
}

QWidget *MainWindow::populateTabDesktop() {
    QTreeWidget* w_PrefixTree = new QTreeWidget();
    QListWidget* w_IconList = new QListWidget();
    QSplitter* splitter = new QSplitter();
    splitter->addWidget(w_PrefixTree);
    splitter->addWidget(w_IconList);
    return splitter;
}

void MainWindow::writeSettings() {
    QSettings settings(QCoreApplication::organizationName(),
                       QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
}

const QIcon MainWindow::getIcon(QString name) const {
    return QIcon::fromTheme(name, QIcon(QString(":%1").arg(name)));
}
