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
#include <algorithm>

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
    if (sqlite3_open(name.c_str(), &db_)) {
        std::cerr << sqlite3_errmsg(db_) << " " << name << std::endl;
        is_open_ = false;
    } else {
        std::cout << "Open database " << name << std::endl;
        is_open_ = true;
        name_ = name;
    }
    return is_open_;
}

intptr_t DBEngine::init(void) {
    std::string sql = "CREATE TABLE test ("
                      "id INTEGER PRIMARY KEY,"
                      "foo REAL,"
                      "bar TEXT"
                      ")";
    return exec(sql);
}

intptr_t DBEngine::exec(const std::string &sql_s) const {
    char *msg_e;
    intptr_t rc = sqlite3_exec(db_, sql_s.c_str(), NULL, NULL, &msg_e);
    if (rc != SQLITE_OK) {
        std::cerr << "Can't execute SQL: '" << sql_s << "'" << std::endl;
        std::cerr << "Error: " << msg_e << std::endl;
        std::cerr << "Error: (" << rc << ") " << msg_e << std::endl;
    }
    sqlite3_free(msg_e);
    return rc;
}

intptr_t DBEngine::exec(const std::string& sql_s,
                    std::initializer_list<std::string> args) {
    sqlite3_stmt *stmt = NULL;
    intptr_t rc = sqlite3_prepare_v2(db_, sql_s.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "Can't prepare statment: '" << sql_s << "'" << std::endl;
        std::cerr << "Error: (" << rc << ") " << sqlite3_errmsg(db_) \
                  << std::endl;
        sqlite3_finalize(stmt);
        return rc;
    }
    intptr_t j = 1;
    for (auto i = args.begin(); i != args.end(); i++, j++) {
        // size_t si = i->length();

        // char* val_s = ;
        /*
        std::copy(i->begin(), i->end(), val_s);
        val_s[i->size()] = '\0';  // don't forget the terminating 0
        */
        rc = sqlite3_bind_text(stmt, j, (*i).data(),
                               (*i).size(), NULL);
        if (rc != SQLITE_OK) {
            std::cerr << "Can't binding value (" << j << ") in: '" \
                      << sql_s << "'" << std::endl;
            std::cerr << "Error: (" << rc << ") " << sqlite3_errmsg(db_) \
                      << std::endl;
            sqlite3_finalize(stmt);
            return rc;
        } else {
            std::cout << "Successfully bound: " << *i << std::endl;
        }
        // delete[] val_s;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Fail to execute query: '" \
                  << sql_s << "'" << std::endl;
        std::cerr << "Error: (" << rc << ") " << sqlite3_errmsg(db_) \
                  << std::endl;
        sqlite3_finalize(stmt);
        return rc;
    } else {
        std::cout << "Done." << std::endl;
    }
    sqlite3_finalize(stmt);
    return rc;
}

rows DBEngine::select(const std::string& sql_s) const {
    rows ret;
    sqlite3_stmt *stmt = NULL;
    intptr_t rc = sqlite3_prepare_v2(db_, sql_s.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "Can't prepare statment: '" << sql_s << "'" << std::endl;
        std::cerr << "Error: (" << rc << ") " << sqlite3_errmsg(db_) \
                  << std::endl;
        sqlite3_finalize(stmt);
        return ret;
    }
    intptr_t i = 0;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {;
        std::cout << "Found row" << std::endl;
        result res;
        for (intptr_t j = 0; j < sqlite3_column_count(stmt); j++) {
            std::cout << "Column " << sqlite3_column_name(stmt, j) \
                      << "(" << j << "): " \
                      << sqlite3_column_text(stmt, j) << std::endl;
            res.insert(result_p(
                           std::string(sqlite3_column_name(stmt, j)),
                           std::string(
                               reinterpret_cast<const char*>(
                                   sqlite3_column_text(stmt, j)))));
        }
        ret.push_back(res);
        i++;
    }
    if (rc != SQLITE_DONE) {
        std::cerr << "Fail to execute query: '" \
                  << sql_s << "'" << std::endl;
        std::cerr << "Error: (" << rc << ") " << sqlite3_errmsg(db_) \
                  << std::endl;
    } else {
        std::cout << "SELECT successfully completed" << std::endl;
    }
    sqlite3_finalize(stmt);
    return ret;
}

result DBEngine::select_one(const std::string& sql_s) const {
    result ret;
    sqlite3_stmt *stmt = NULL;
    std::string sql_lim_s = sql_s + " LIMIT 1";
    intptr_t rc = sqlite3_prepare_v2(db_, sql_lim_s.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "Can't prepare statment: '" << sql_lim_s << "'" \
                  << std::endl;
        std::cerr << "Error: (" << rc << ") " << sqlite3_errmsg(db_) \
                  << std::endl;
        sqlite3_finalize(stmt);
        return ret;
    }
    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {;
        std::cout << "Found row" << std::endl;
        for (intptr_t j = 0; j < sqlite3_column_count(stmt); j++) {
            std::cout << "Column " << sqlite3_column_name(stmt, j) \
                      << "(" << j << "): " \
                      << sqlite3_column_text(stmt, j) << std::endl;
            ret.insert(result_p(
                           std::string(sqlite3_column_name(stmt, j)),
                           std::string(
                               reinterpret_cast<const char*>(
                                   sqlite3_column_text(stmt, j)))));
        }
    }
    if ((rc = sqlite3_step(stmt)) != SQLITE_DONE) {
        std::cerr << "Fail to execute query: '" \
                  << sql_lim_s << "'" << std::endl;
        std::cerr << "Error: (" << rc << ") " << sqlite3_errmsg(db_) \
                  << std::endl;
    } else {
        std::cout << "SELECT successfully completed" << std::endl;
    }
    sqlite3_finalize(stmt);
    return ret;
}

intptr_t DBEngine::get_id() const {
    return intptr_t(sqlite3_last_insert_rowid(db_));
}

DBEngine::~DBEngine() {
    std::cout << "Closing DB" << std::endl;
    sqlite3_close(db_);
}

}  // namespace lib
}  // namespace q4wine
