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

#include <Elementary.hh>

#include "src/version.h"

#include "lib/db.hpp"

EAPI_MAIN int elm_main(int argc, char *argv[]) {
    q4wine::lib::DBEngine* db =
            q4wine::lib::DBEngine::getInstance();
    db->open(":memory:");

    elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_HIDDEN);

    ::elm::win_standard win;
    win.title_set("Hello, World!");
    win.autohide_set(true);

    ::elm::bg bg(efl::eo::parent = win);
    bg.size_hint_weight_set(EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    win.resize_object_add(bg);
    bg.visible_set(true);

    ::elm::button btn(efl::eo::parent = win);
    btn.text_set("elm.text", "Hello World!");
    btn.size_set(120, 30);
    btn.position_set(60, 15);
    btn.size_hint_weight_set(EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    btn.size_hint_align_set(EVAS_HINT_FILL, EVAS_HINT_FILL);
    btn.visible_set(true);

    auto on_click = std::bind([] () { elm_exit(); });

    btn.callback_clicked_add(on_click);

    win.size_set(240, 60);
    win.visible_set(true);

    elm_run();
    return 0;
}
ELM_MAIN()
