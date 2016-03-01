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
        uintptr_t id):
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

std::string WinePrefix::getEnvVariables(const WineApplication* wineApp) {
    std::ostringstream env_stream;
    if (wineApp)
        env_stream << wineApp->getEnvVariables();
    env_stream << " WINEPREFIX='" << getPath() << "'";
    if (version_)
        env_stream << version_->getEnvVariables(arch_);
    // wineStrings.append();
    std::string arch = getArchString();
    if (!arch.empty())
        env_stream << " WINEARCH='" << arch << "'";

    return env_stream.str();
}

std::string WinePrefix::getExecutionString(const WineApplication* wineApp) {
    std::string tmpl = execTemplate_;
    if (wineApp->getUseTerminal()) {
        tmpl = std::regex_replace(tmpl, std::regex("%CONSOLE_BIN%"),
                                  "/usb/bin/konsole");
        tmpl = std::regex_replace(tmpl, std::regex("%CONSOLE_ARGS%"),
                                  "--noclose -e");
    } else {
        tmpl = std::regex_replace(tmpl, std::regex("%CONSOLE_BIN%"), "");
        tmpl = std::regex_replace(tmpl, std::regex("%CONSOLE_ARGS%"), "");
    }
    tmpl = std::regex_replace(tmpl, std::regex("%ENV_BIN%"),
                              "/usb/bin/env");
    tmpl = std::regex_replace(tmpl, std::regex("%ENV_ARGS%"),
                              getEnvVariables(wineApp));
    tmpl = std::regex_replace(tmpl, std::regex("%WORK_DIR%"),
                              wineApp->getWorkDirectory());
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
