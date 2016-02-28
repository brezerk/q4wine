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

WineVersion::WineVersion(
            std::string binary,
            std::string loader,
            std::string server,
            std::string libs32,
            std::string libs64,
            uintptr_t id):
    DBObject(tableName_, id),
    binary_(binary),
    loader_(loader),
    server_(server),
    libs32_(libs32),
    libs64_(libs64) {
}

WineVersion::~WineVersion() {
    std::cout << "Destroy Version" << std::endl;
}

/*
std::string WineVersion::getEnvVariables(q4wine::lib::WineArch arch) {
    std::stringList wineStrings;
    if (!server_.isEmpty())
        wineStrings.append(std::string("WINESERVER='%1'").arg(server_));
    if (!loader_.isEmpty())
        wineStrings.append(std::string("WINELOADER='%1'").arg(loader_));
    std::string libs = getLibs(arch);
    if (!libs.isEmpty())
        wineStrings.append(std::string("WINEDLLPATH='%1'").arg(libs));
    return wineStrings.join(" ");
}*/

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
