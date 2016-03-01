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

#include <stdint.h>
#include <sqlite3.h>
#include <string>
#include <memory>
#include <iostream>

#include "src/lib/defines.hpp"

namespace q4wine {
namespace lib {

/*! \class DBEngine dbengine.hpp <q4wine/src/lib/dbengine.hpp>
 * \brief SQLite3 engine
 *
 * \author Alexey S. Malakhov <brezerk@gmail.com>
 */
class DBEngine {
 public:
     /**
     * @brief getInstance
     * Check current instance. If it is NULL
     * then create new one.
     * Otherwise return existing one.
     *
     * @return instance of DBEngine
     */
    static DBEngine* getInstance();

        /*! Disable copy */
    DBEngine(DBEngine const&) = delete;
    /*! Disable copy on =*/
    DBEngine& operator=(DBEngine const&) = delete;
    /*! Constructs an DBObject object.*/
    DBEngine();
    /*! Destroys this DBEngine object. */
    ~DBEngine();

    bool open(std::string name);

 protected:
    /*! Instance of DBEngine object. */
    static DBEngine* DBEngine_instance;

 private:
    std::string name_;
    sqlite3* db_;
    bool is_open_;
};

}  // namespace lib
}  // namespace q4wine
