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

WinePrefix::WinePrefix(std::string name,
        std::string path,
        std::string arch,
        WineVersion* version,
        std::string mountPoint,
        std::string virtualDrive,
        std::string execTemplate,
        uintptr_t id):
    DBObject(tableName_, id),
    name_(name),
    path_(path),
    arch_(arch),
    version_(std::shared_ptr<WineVersion>(version)),
    mountPoint_(mountPoint),
    virtualDrive_(virtualDrive),
    execTemplate_(execTemplate) {
}

WinePrefix::~WinePrefix() {
    std::cout << "Destroy WinePrefix" << std::endl;
}

bool WinePrefix::save(void) {
    std::string sql_s;
    if (id_ == 0) {
        sql_s = "INSERT INTO prefix("
                "name, path, arch, version, "
                "mount_point, virtual_drive, "
                "exec_template"
                ") VALUES(?, ?, ?, ?, ?, ?, ?)";
        if (db_->exec(sql_s, {this->getName(),
                          this->getPath(),
                          this->getArch(),
                          std::to_string(this->getVersion()->getId()),
                          this->getMountPoint(),
                          this->getVirtualDrive(),
                          this->getExecutionTemplate()})) {
            this->setId(db_->get_id());
            return true;
        }
    } else {
        sql_s = "UPDATE versions SET name=?, path=?, "
                "arch=?, version=?, mount_point=?, "
                "virtual_drive=?, exec_template=? WHERE id=?";
        return db_->exec(sql_s, {this->getName(),
                          this->getPath(),
                          this->getArch(),
                          std::to_string(this->getVersion()->getId()),
                          this->getMountPoint(),
                          this->getVirtualDrive(),
                          this->getExecutionTemplate(),
                          std::to_string(this->getId())});
    }
    return false;
}

WinePrefix* WinePrefix::getInstance(intptr_t id) {
    q4wine::lib::DBEngine* db =
            q4wine::lib::DBEngine::getInstance();
    std::string sql_s = "SELECT * FROM prefix "
            "WHERE id = ?";
    result res = db->select_one(sql_s, {std::to_string(id)});
    if (!res.empty()) {
        WinePrefix* ret = new WinePrefix(res["name"],
                res["path"],
                res["arch"],
                WineVersion::getInstance(std::stoi(res["version"])),
                res["mount_point"],
                res["virtual_drive"],
                res["exec_template"],
                std::stoi(res["id"]));
        return ret;
    } else {
        return NULL;
    }
}

std::string WinePrefix::getEnvVariables(const WineApplication* wineApp) {
    std::ostringstream env_stream;
    if (wineApp)
        env_stream << wineApp->getEnvVariables();
    env_stream << " WINEPREFIX='" << getPath() << "'";
    if (version_)
        env_stream << version_->getEnvVariables(arch_);
    // wineStrings.append();
    std::string arch = getArch();
    if (!arch.empty())
        env_stream << " WINEARCH='" << arch << "'";

    return env_stream.str();
}

std::string WinePrefix::getExecutionString(const WineApplication* wineApp) {
    std::string tmpl = execTemplate_;
    if (wineApp->getUseTerminal()) {
        tmpl = std::regex_replace(tmpl, std::regex("%CONSOLE_BIN%"),
                                  "/usr/bin/konsole");
        tmpl = std::regex_replace(tmpl, std::regex("%CONSOLE_ARGS%"),
                                  "--noclose -e");
    } else {
        tmpl = std::regex_replace(tmpl, std::regex("%CONSOLE_BIN%"), "");
        tmpl = std::regex_replace(tmpl, std::regex("%CONSOLE_ARGS%"), "");
    }
    tmpl = std::regex_replace(tmpl, std::regex("%ENV_BIN%"),
                              "/usr/bin/env");
    tmpl = std::regex_replace(tmpl, std::regex("%ENV_ARGS%"),
                              getEnvVariables(wineApp));

    // Kept for backward comtibility
    tmpl = std::regex_replace(tmpl, std::regex("%WORK_DIR%"),
                              "");
    tmpl = std::regex_replace(tmpl, std::regex("%SET_NICE%"),
                              wineApp->getNiceCmd());
    tmpl = std::regex_replace(tmpl, std::regex("%WINE_BIN%"),
                              version_->getBinary());
    tmpl = std::regex_replace(tmpl, std::regex("%VIRTUAL_DESKTOP%"),
                              wineApp->getVirtualDesktop());
    std::string program = wineApp->getPath();
    std::string args = wineApp->getArgs();
    std::string extenson = program.substr(program.length()-4);

    if (extenson == ".msi") {
        args = "/i " + program + " " + args;
        program = "msiexec";
    } else if (extenson == ".bat") {
        args = program + " " + args;
        program = "wineconsole";
    }
    tmpl = std::regex_replace(tmpl, std::regex("%PROGRAM_BIN%"),
                              program);
    tmpl = std::regex_replace(tmpl, std::regex("%PROGRAM_ARGS%"),
                              args);

    return tmpl;
}

void WinePrefix::setName(std::string name) {
    name_ = name;
}

void WinePrefix::setPath(std::string path) {
    path_ = path;
}

void WinePrefix::setArch(std::string arch) {
    arch_ = arch;
}

void WinePrefix::setVersion(WineVersion* version) {
    version_ = std::unique_ptr<WineVersion>(version);
}

void WinePrefix::setMountPoint(std::string mountPoint) {
    mountPoint_ = mountPoint;
}

void WinePrefix::setVirtualDrive(std::string virtualDrive) {
    virtualDrive_ = virtualDrive;
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

const std::string WinePrefix::getArch(void) const {
    return arch_;
}

WineVersion* WinePrefix::getVersion(void) const {
    return version_.get();
}

const std::string WinePrefix::getMountPoint(void) const {
    return mountPoint_;
}

const std::string WinePrefix::getVirtualDrive(void) const {
    return virtualDrive_;
}

const std::string WinePrefix::getExecutionTemplate(void) const {
    return execTemplate_;
}

}  // namespace lib
}  // namespace q4wine
