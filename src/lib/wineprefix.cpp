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

#include "src/lib/wineprefix.hpp"
#include <string>

namespace q4wine {
namespace lib {

const std::string WinePrefix::tableName_ = "prefix";

WinePrefix::WinePrefix() : DBObject(tableName_) {
}

WinePrefix::WinePrefix(
        std::string name,
        std::string path,
        WineArch arch,
        WineVersion* version,
        std::string mountPoint,
        std::string virtualDevice,
        std::string execTemplate,
        uint32_t id):
    DBObject(tableName_, id),
    name_(name),
    path_(path),
    arch_(arch),
    version_(std::shared_ptr<WineVersion>(version)),
    mountPoint_(mountPoint),
    virtualDevice_(virtualDevice),
    execTemplate_(execTemplate) {
}

WinePrefix::~WinePrefix() {
    std::cout << "Destroy WinePrefix" << std::endl;
}

/*! FIXME: ok. this is cool. but we need to use execution template
* string instead :)
*/
/*
std::string WinePrefix::getWineEnv() {
    std::stringList wineStrings;
    wineStrings.append(std::string("WINEPREFIX='%1'").arg(getPath()));
    wineStrings.append(version_->getEnvVariables(arch_));
    std::string arch = getArchString();
    if (!arch.isEmpty()) {
        wineStrings.append(std::string("WINEARCH='%1'").arg(arch));
    }
    return wineStrings.join(" ");
}*/

void WinePrefix::setName(std::string name) {
    name_ = name;
}

void WinePrefix::setPath(std::string path) {
    path_ = path;
}

void WinePrefix::setArch(WineArch arch) {
    arch_ = arch;
}

void WinePrefix::setVersion(WineVersion* version) {
    version_ = std::unique_ptr<WineVersion>(version);
}

void WinePrefix::setMountPoint(std::string mountPoint) {
    mountPoint_ = mountPoint;
}

void WinePrefix::setVirtualDevice(std::string virtualDevice) {
    virtualDevice_ = virtualDevice;
}

void WinePrefix::setExecutionTemplate(std::string execTemplate) {
    execTemplate_ = execTemplate;
}

const std::string WinePrefix::getName(void) const {
    return name_;
}

const std::string WinePrefix::getPath(void) const {
    return path_;
}

WineArch WinePrefix::getArch(void) const {
    return arch_;
}

const std::string WinePrefix::getArchString(void) const {
    std::string arch;
    if (arch_ == q4wine::lib::WIN32) {
        arch = "win32";
    } else if (arch_ == q4wine::lib::WIN32) {
        arch = "win64";
    } else {
        arch = std::string();
    }
    return arch;
}

WineVersion* WinePrefix::getVersion(void) const {
    return version_.get();
}

const std::string WinePrefix::getMountPoint(void) const {
    return mountPoint_;
}

const std::string WinePrefix::getVirtualDevice(void) const {
    return virtualDevice_;
}


const std::string WinePrefix::getExecutionTemplate(void) const {
    return execTemplate_;
}

}  // namespace lib
}  // namespace q4wine
