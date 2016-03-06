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

#include "src/lib/wineversion.hpp"
#include <string>

namespace q4wine {
namespace lib {

const std::string WineVersion::tableName_ = "version";

WineVersion::WineVersion() : DBObject(tableName_) {
}

WineVersion::WineVersion(std::string name,
            std::string binary,
            std::string server,
            std::string loader,
            std::string libs32,
            std::string libs64,
            intptr_t id):
    DBObject(tableName_, id),
    name_(name),
    binary_(binary),
    server_(server),
    loader_(loader),
    libs32_(libs32),
    libs64_(libs64) {
}

WineVersion::~WineVersion() {
    std::cout << "Destroy Version" << std::endl;
}

bool WineVersion::save(void) {
    std::string sql_s;
    if (id_ == 0) {
        sql_s = "INSERT INTO versions("
                "name, binary, server, loader, "
                "libs32, libs64"
                ") VALUES(?, ?, ?, ?, ?, ?)";
        if (db_->exec(sql_s, {this->getName(),
                          this->getBinary(),
                          this->getServer(),
                          this->getLoader(),
                          this->getLibs32(),
                          this->getLibs64()})) {
            this->setId(db_->get_id());
            return true;
        }
    } else {
        sql_s = "UPDATE versions SET name=?, binary=?, "
                "server=?, loader=?, libs32=?, "
                "libs64=? WHERE id=?";
        return db_->exec(sql_s, {this->getName(),
                          this->getBinary(),
                          this->getServer(),
                          this->getLoader(),
                          this->getLibs32(),
                          this->getLibs64(),
                          std::to_string(this->getId())});
    }
    return false;
}

WineVersion* WineVersion::getInstance(intptr_t id) {
    q4wine::lib::DBEngine* db =
            q4wine::lib::DBEngine::getInstance();
    std::string sql_s = "SELECT * FROM versions "
            "WHERE id = ?";
    result res = db->select_one(sql_s, {std::to_string(id)});
    if (!res.empty()) {
        WineVersion* ret = new WineVersion(res["name"],
                res["binary"], res["server"],
                res["loader"], res["libs32"],
                res["libs64"], std::stoi(res["id"]));
        return ret;
    } else {
        return NULL;
    }
}

const std::string WineVersion::getEnvVariables(
        q4wine::lib::WineArch arch) const {
    std::ostringstream env_stream;
    if (!server_.empty())
        env_stream << " WINESERVER='" << server_ << "'";
    if (!loader_.empty())
        env_stream << " WINELOADER='" << loader_ << "'";
    std::string libs = getLibs(arch);
    if (!libs.empty())
        env_stream << " WINEDLLPATH='" << libs << "'";

    return env_stream.str();
}

void WineVersion::setName(std::string name) {
    name_ = name;
}

void WineVersion::setBinary(std::string binary) {
    binary_ = binary;
}

void WineVersion::setLoader(std::string loader) {
    loader_ = loader;
}

void WineVersion::setServer(std::string server) {
    server_ = server;
}

void WineVersion::setLibs32(std::string libs32) {
    libs32_ = libs32;
}

void WineVersion::setLibs64(std::string libs64) {
    libs64_ = libs64;
}

const std::string WineVersion::getName(void) const {
    return name_;
}

const std::string WineVersion::getBinary(void) const {
    return binary_;
}

const std::string WineVersion::getLoader(void) const {
    return loader_;
}

const std::string WineVersion::getServer(void) const {
    return server_;
}

const std::string WineVersion::getLibs32(void) const {
    return libs32_;
}

const std::string WineVersion::getLibs64(void) const {
    return libs64_;
}

const std::string WineVersion::getLibs(q4wine::lib::WineArch arch) const {
    if (arch == q4wine::lib::WIN32) {
        return libs32_;
    } else if (arch == q4wine::lib::WIN64) {
        return libs64_;
    } else {
        return libs32_;
    }
}

}  // namespace lib
}  // namespace q4wine
