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

#include "src/lib/db.hpp"
#include <string>

namespace q4wine {
namespace lib {

DBEngine* DBEngine::DBEngine_instance(NULL);

DBEngine* DBEngine::getInstance() {
    return DBEngine_instance ? DBEngine_instance : (
                                   DBEngine_instance = new DBEngine());
}

DBEngine::DBEngine() : is_open_(false) {
}

bool DBEngine::open(std::string name) {
    if (sqlite3_open("/tmp/q4wine.db", &db_)) {
        std::cerr << sqlite3_errmsg(db_) << " " << name << std::endl;
        is_open_ = false;
    } else {
        std::cout << "Open database " << name << std::endl;
        is_open_ = true;
        name_ = name;
    }
    return is_open_;
}

DBEngine::~DBEngine() {
    std::cout << "Closing DB" << std::endl;
    sqlite3_close(db_);
}

}  // namespace lib
}  // namespace q4wine
