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

#include "src/gtk/mainwindow.hpp"
#include <gtkmm.h>
#include <iostream>

MainWindow::MainWindow(BaseObjectType* cobject,
                       const Glib::RefPtr<Gtk::Builder>& builder) :
    Gtk::Window(cobject), builder_(builder) {
    Gtk::ImageMenuItem* act;
    builder->get_widget("appStatusBar", statusBar_);
    builder->get_widget("menuActionExit", act);
    act->signal_activate().connect(
                sigc::mem_fun(*this, &MainWindow::on_action_file_quit));
    act = NULL;

    statusBar_->push("Ready");
}

MainWindow::~MainWindow() {
}

void MainWindow::on_action_file_quit() {
    hide();  // Closes the main window to stop the app->run().
}

void MainWindow::on_action_file_run() {
    std::cout << "A Run menu item was selected." << std::endl;
}

void MainWindow::on_action_file_preferences() {
    std::cout << "A menu item was selected." << std::endl;
}

void MainWindow::on_action_tool_desktop() {
    std::cout << "The toggle menu item was selected." << std::endl;
}
