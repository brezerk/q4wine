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

#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <gtkmm/button.h>

#include "src/version.h"

#include "lib/db.hpp"

int main(int argc, char *argv[]) {
    q4wine::lib::DBEngine* db =
            q4wine::lib::DBEngine::getInstance();
    db->open(":memory:");

    auto app = Gtk::Application::create(argc, argv,
                                        "ua.org.brezblock.q4wine");
    Gtk::Window window;
    Gtk::Button m_button("Hello World");
    window.set_default_size(200, 200);
    window.add(m_button);
    m_button.show();

    return app->run(window);
}
