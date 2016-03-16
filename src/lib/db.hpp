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

/*!
 * \brief A map that represents SQL result.
 * The key is row name, the value is row value.
 * \note In case if SQL return no results, the map will be empty.
 */
typedef std::map<std::string, std::string> result;
typedef std::pair<std::string, std::string> result_p;
/*!
 * \brief An a vector of resuls.
 * \note In case if SQL return no results, the vector will be empty.
 * \sa result
 */
typedef std::vector<result> rows;

/*! \class DBEngine db.hpp <q4wine/src/lib/db.hpp>
 * \brief q4wine's SQLite3 engine warper.
 * This class provides basic common SQL workflow, like making SQL queries,
 * retrieving and inserting data.
 *
 * \author Alexey S. Malakhov <brezerk@gmail.com>
 */
class DBEngine {
 public:
     /**
     * \brief Return instance of DBEngine object.
     * Check current instance. If it is NULL
     * then create a new one.
     * Otherwise return existing one.
     *
     * \return instance of DBEngine.
     */
    static DBEngine* getInstance();

    /*!
     * \brief Open SQLite database.
     * \param name A URI file name. The path component of the URI specifies
     * the disk file that is the SQLite database to be opened. If the path
     * component is omitted, then the database is stored in a temporary
     * file that will be automatically deleted when the database
     * connection closes.
     * \return True on success
     */
    bool open(std::string name);

    /*!
     * \brief Itialize database.
     * Create tables and default bootsrap data on first startup.
     * \return True on success.
     */
    bool init(void);

    /*!
     * \brief Execute raw SQL statement.
     * Use this fuction to call raw SQL statements.
     * If you need to bind values, use overloaded function \sa exec
     * \param sql_s Raw SQL string
     * \return True on success.
     */
    bool exec(const std::string& sql_s) const;

    /*!
     * \brief Prepare and execute SQL statement with provided arguments.
     * \param sql_s Raw SQL string.
     * \param args A list of values to bound.
     * \return True on success.
     */
    bool exec(const std::string& sql_s,
              const std::initializer_list<std::string>& args) const;

    /*!
     * \brief Execute raw SQL statement and return the result.
     * \param sql_s Raw SQL string.
     * \return Return a map of rows \sa rows.
     */
    rows select(const std::string& sql_s) const;

    /*!
     * \brief Prepare and execute SQL statement with provided arguments
     * and return the result.
     * \param sql_s Raw SQL string.
     * \param args A list of values to bound.
     * \return A map of rows \sa rows.
     */
    rows select(const std::string& sql_s,
                const std::initializer_list<std::string>& args) const;

    /*!
     * \brief select Execute raw SQL statement and return the single result
     * \note You don't need to specify SQL LIMIT command, this function will
     * add it by default.
     * \param sql_s Raw SQL string.
     * \return Return single result \sa result.
     */
    result select_one(const std::string& sql_s) const;

    /*!
     * \brief Prepare and execute SQL statement with provided arguments
     * and return the single result.
     * \note You don't need to specify SQL LIMIT command, this function will
     * add it by default.
     * \param sql_s Raw SQL string.
     * \param args A list of values to bound.
     * \return Return single result \sa result.
     */
    result select_one(const std::string& sql_s,
                const std::initializer_list<std::string>& args) const;

    /*!
     * \brief Get last insert rowid.
     * \return Row id
     */
    intptr_t get_id(void) const;

    /*!
     * \brief Check if database already open.
     * \return True if db is open.
     */
    bool is_open(void) const;

    /*!
     * \brief Close database.
     * Call this on app shutdown.
     */
    void close(void) const;

 protected:
     /*! Disable copy */
    DBEngine(DBEngine const&) = delete;
    /*! Disable copy on = */
    DBEngine& operator=(DBEngine const&) = delete;
    /*! Constructs an DBObject object.*/
    DBEngine();
    /*! Destroys this DBEngine object. */
    ~DBEngine();

    /*! Instance of DBEngine object. */
    static DBEngine* DBEngine_instance;

 private:
    /*!
     * \brief Bind arguments to the sqlite prepared query.
     * \param stmt Sqlite prepared query.
     * \param sql_s Raw SQL string. Used for error reporting.
     * \param args A list of values to bound.
     * \return True on success.
     */
    bool bind_args(sqlite3_stmt *stmt,
                   const std::string& sql_s,
                   const std::initializer_list<std::string>& args) const;
    std::string name_;
    sqlite3* db_;
    bool is_open_;
};

}  // namespace lib
}  // namespace q4wine
