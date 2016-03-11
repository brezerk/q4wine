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
#include <initializer_list>
#include <typeinfo>
#include <algorithm>
#include <map>
#include <vector>
#include <utility>

#include "src/lib/defines.hpp"

namespace q4wine {
namespace lib {

typedef std::map<std::string, std::string> result;
typedef std::pair<std::string, std::string> result_p;
typedef std::vector<result> rows;

/*! \class DBEngine db.hpp <q4wine/src/lib/db.hpp>
 * \brief SQLite3 engine
 *
 * \author Alexey S. Malakhov <brezerk@gmail.com>
 */
class DBEngine {
 public:
     /**
     * \brief getInstance
     * Check current instance. If it is NULL
     * then create new one.
     * Otherwise return existing one.
     *
     * \return instance of DBEngine
     */
    static DBEngine* getInstance();

    /*!
     * \brief open Open SQLite database
     * \param name A database file name
     * \return true on success
     */
    bool open(std::string name);

    /*!
     * \brief init Itialize empty database
     * \return true on success.
     */
    bool init(void);

    /*!
     * \brief exec Execute raw SQL statement.
     * Use this fuction to call raw SQL statements.
     * If you need to bind values, use overloaded function @sa exec
     * \param sql_s raw SQL string
     * \return true on success.
     */
    bool exec(const std::string& sql_s) const;

    /*!
     * \brief exec Prepare and execute SQL statement with provided arguments.
     * \param sql_s raw SQL string
     * \param args a list of values to bound
     * \return true on success.
     */
    bool exec(const std::string& sql_s,
              std::initializer_list<std::string> args);

    /*!
     * \brief select Execute raw SQL statement and return the result
     * \param sql_s raw SQL string
     * \return return a map of rows
     */
    rows select(const std::string& sql_s) const;
    rows select(const std::string& sql_s,
                std::initializer_list<std::string> args) const;

    /*!
     * \brief select Execute raw SQL statement and return the result
     * \param sql_s raw SQL string
     * \return return single result
     */
    result select_one(const std::string& sql_s) const;
    result select_one(const std::string& sql_s,
                std::initializer_list<std::string> args) const;

    /*!
     * \brief get_id Get last insert rowid
     * \return rowid
     */
    intptr_t get_id(void) const;

    /*!
     * \brief is_open
     * \return true if db is open
     */
    bool is_open(void) const;

    /*!
     * \brief close database
     * Close database. Call this on app shutdown.
     */
    void close(void) const;

 protected:
     /*! Disable copy */
    DBEngine(DBEngine const&) = delete;
    /*! Disable copy on =*/
    DBEngine& operator=(DBEngine const&) = delete;
    /*! Constructs an DBObject object.*/
    DBEngine();
    /*! Destroys this DBEngine object. */
    ~DBEngine();

    /*! Instance of DBEngine object. */
    static DBEngine* DBEngine_instance;

 private:
    std::string name_;
    sqlite3* db_;
    bool is_open_;
};

}  // namespace lib
}  // namespace q4wine
