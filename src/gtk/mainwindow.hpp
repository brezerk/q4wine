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

#pragma once

#include <gtkmm.h>

class MainWindow : public Gtk::Window {
 public:
    explicit MainWindow(BaseObjectType* cobject,
                        const Glib::RefPtr<Gtk::Builder>& builder);
    virtual ~MainWindow();

 private:
    // Signal handlers:
    void on_action_file_run();
    void on_action_file_quit();
    void on_action_file_preferences();
    void on_action_tool_desktop();
 protected:
    Glib::RefPtr<Gtk::Builder> builder_;
    Gtk::Statusbar* statusBar_;
};
