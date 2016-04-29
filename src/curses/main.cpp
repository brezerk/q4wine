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

#include <ncurses.h>
#include <menu.h>

#include <string.h>

#include "src/version.h"

#include "lib/db.hpp"

char *choices[] = {
    const_cast<char*>("Choice 1"),
    const_cast<char*>("Choice 2"),
    const_cast<char*>("Choice 3"),
    const_cast<char*>("Choice 4"),
    const_cast<char*>("Exit"),
};

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 4

int main() {
    ITEM **my_items;
    int c;
    MENU *my_menu;
    int n_choices, i;
    //  ITEM *cur_item;

    q4wine::lib::DBEngine* db =
            q4wine::lib::DBEngine::getInstance();
    db->open(":memory:");

    initscr();

    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    n_choices = ARRAY_SIZE(choices);
    my_items = reinterpret_cast<ITEM **>(calloc(n_choices + 1, sizeof(ITEM *)));

    for (i = 0; i < n_choices; ++i)
        my_items[i] = new_item(choices[i], choices[i]);
    my_items[n_choices] = reinterpret_cast<ITEM *>(NULL);

    my_menu = new_menu(reinterpret_cast<ITEM **>(my_items));
    mvprintw(LINES - 2, 0, "F1 to Exit");
    post_menu(my_menu);
    refresh();

    while ((c = getch()) != KEY_F(1)) {
        switch (c) {
        case KEY_DOWN:
            menu_driver(my_menu, REQ_DOWN_ITEM);
            break;
        case KEY_UP:
            menu_driver(my_menu, REQ_UP_ITEM);
            break;
        }
    }

    free_item(my_items[0]);
    free_item(my_items[1]);
    free_menu(my_menu);

    endwin();
}
