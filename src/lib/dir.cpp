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

#include "src/lib/dir.hpp"
#include <string>

namespace q4wine {
namespace lib {

const std::string Dir::tableName_ = "dir";

Dir::Dir() : DBObject(tableName_) {
}

Dir::Dir(std::string name,
         intptr_t prefixId,
         intptr_t dirId,
         intptr_t id):
    DBObject(tableName_, id),
    name_(name),
    prefixId_(prefixId),
    dirId_(dirId) {
}

Dir::~Dir() {
    std::cout << "Destroy Dir" << std::endl;
}

bool Dir::save(void) {
    std::string sql_s;
    std::string dir_i, prefix_i;

    if (this->getDirId() > 0) {
        dir_i = std::to_string(this->getDirId());
    }

    if (this->getPrefixId() > 0) {
        prefix_i = std::to_string(this->getPrefixId());
    }

    if (id_ == 0) {
        sql_s = "INSERT INTO dir("
                "name, prefix, dir"
                ") VALUES(?, ?, ?)";
        if (db_->exec(sql_s, {this->getName(),
                          prefix_i,
                          dir_i})) {
            this->setId(db_->get_id());
            return true;
        }
    } else {
        sql_s = "UPDATE dir SET name=?, prefix=?, "
                "dir=? WHERE id=?";
        return db_->exec(sql_s, {this->getName(),
                          prefix_i,
                          dir_i,
                          std::to_string(this->getId())});
    }
    return false;
}

Dir* Dir::getInstance(intptr_t id) {
    q4wine::lib::DBEngine* db =
            q4wine::lib::DBEngine::getInstance();
    std::string sql_s = "SELECT * FROM dir "
            "WHERE id = ?";
    result res = db->select_one(sql_s, {std::to_string(id)});
    if (!res.empty()) {
        intptr_t dir_i = 0, prefix_i = 0;
        if (!res["dir"].empty())
            dir_i = std::stoi(res["dir"]);
        if (!res["prefix"].empty())
            prefix_i = std::stoi(res["prefix"]);
        Dir* ret = new Dir(res["name"],
                prefix_i, dir_i,
                std::stoi(res["id"]));
        return ret;
    } else {
        return NULL;
    }
}

void Dir::setName(std::string name) {
    name_ = name;
}

void Dir::setPrefixId(intptr_t prefixId) {
    prefixId_ = prefixId;
}

void Dir::setDirId(intptr_t dirId) {
    dirId_ = dirId;
}

const std::string Dir::getName(void) const {
    return name_;
}

intptr_t Dir::getPrefixId(void) const {
    return prefixId_;
}

intptr_t Dir::getDirId(void) const {
    return dirId_;
}

}  // namespace lib
}  // namespace q4wine
