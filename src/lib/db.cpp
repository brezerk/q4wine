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
    /*
     * FIXME: We need 1.x -> 2.x migration shema.
     *
     * 2.x changes:
     *
     *    versions.CONSTRAINT
     *    versions.wine_exec      -> binary
     *    versions.wine_server    -> server
     *    versions.wine_loader    -> loader
     *    versions.wine_dllpath32 -> libs32
     *    versions.wine_dllpath64 -> libs64
     *
     *    prefix.CONSTRAINT
     *    prefix.version          -> FOREIGN KEY version.id
     *    prefix.cdrom_mount      -> mount_point
     *    prefix.mountpoint_windrive -> virtual_drive
     *    prefix.run_string       -> exec_template
     */

    std::string sql = "PRAGMA foreign_keys = ON;";
    if (exec(sql) != SQLITE_OK)
        return false;

    sql = "CREATE TABLE IF NOT EXISTS versions "
          "(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
          "name TEXT, binary TEXT, "
          "server TEXT, loader TEXT, "
          "libs32 TEXT, libs64 TEXT, "
          "CONSTRAINT name_unique UNIQUE (name));";
    if (exec(sql) != SQLITE_OK)
        return false;

    sql = "CREATE TABLE IF NOT EXISTS prefix "
          "(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
          "name TEXT, path TEXT, arch TEXT, "
          "version INTEGER, "
          "mount_point TEXT, virtual_drive TEXT, "
          "exec_template TEXT, "
          "FOREIGN KEY(version) REFERENCES versions(id), "
          "CONSTRAINT name_unique UNIQUE (name));";
    if (exec(sql) != SQLITE_OK)
        return false;
    return true;
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

    intptr_t i = 1;
    for (auto j = args.begin(); j != args.end(); j++, i++) {
        rc = sqlite3_bind_text(stmt, i, (*j).data(),
                               (*j).size(), NULL);
        if (rc != SQLITE_OK) {
            std::cerr << "Can't binding value (" << i << ") in: '" \
                      << sql_s << "'" << std::endl;
            std::cerr << "Error: (" << rc << ") " << sqlite3_errmsg(db_) \
                      << std::endl;
            sqlite3_finalize(stmt);
            return rc;
        }
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Fail to execute query: '" \
                  << sql_s << "'" << std::endl;
        std::cerr << "Error: (" << rc << ") " << sqlite3_errmsg(db_) \
                  << std::endl;
        sqlite3_finalize(stmt);
        return rc;
    }
    sqlite3_finalize(stmt);
    return rc;
}

rows DBEngine::select(const std::string& sql_s) const {
    return select(sql_s, {});
}

rows DBEngine::select(const std::string& sql_s,
                      std::initializer_list<std::string> args) const {
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

    intptr_t i = 1;
    for (auto j = args.begin(); j != args.end(); j++, i++) {
        rc = sqlite3_bind_text(stmt, i, (*j).data(),
                               (*j).size(), NULL);
        if (rc != SQLITE_OK) {
            std::cerr << "Can't binding value (" << i << ") in: '" \
                      << sql_s << "'" << std::endl;
            std::cerr << "Error: (" << rc << ") " << sqlite3_errmsg(db_) \
                      << std::endl;
            sqlite3_finalize(stmt);
            return ret;
        }
    }

    i = 0;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {;
        result res;
        for (intptr_t j = 0; j < sqlite3_column_count(stmt); j++) {
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
    }
    sqlite3_finalize(stmt);
    return ret;
}

result DBEngine::select_one(const std::string& sql_s) const {
    return select_one(sql_s, {});
}

result DBEngine::select_one(const std::string& sql_s,
                            std::initializer_list<std::string> args) const {
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

    intptr_t i = 1;
    for (auto j = args.begin(); j != args.end(); j++, i++) {
        rc = sqlite3_bind_text(stmt, i, (*j).data(),
                               (*j).size(), NULL);
        if (rc != SQLITE_OK) {
            std::cerr << "Can't binding value (" << i << ") in: '" \
                      << sql_s << "'" << std::endl;
            std::cerr << "Error: (" << rc << ") " << sqlite3_errmsg(db_) \
                      << std::endl;
            sqlite3_finalize(stmt);
            return ret;
        }
    }

    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {;
        for (intptr_t j = 0; j < sqlite3_column_count(stmt); j++) {
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
    }
    sqlite3_finalize(stmt);
    return ret;
}

intptr_t DBEngine::get_id(void) const {
    return intptr_t(sqlite3_last_insert_rowid(db_));
}

DBEngine::~DBEngine() {
    close();
}

bool DBEngine::is_open(void) const {
    return is_open_;
}

void DBEngine::close(void) const {
    std::cout << "Closing DB" << std::endl;
    sqlite3_close(db_);
}

}  // namespace lib
}  // namespace q4wine
