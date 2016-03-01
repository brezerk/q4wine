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

#include "src/lib/wineapplication.hpp"
#include <string>

namespace q4wine {
namespace lib {

const std::string WineApplication::tableName_ = "icon";

WineApplication::WineApplication() : DBObject(tableName_) {
}

WineApplication::WineApplication(
        std::string name,
        std::string path,
        std::string args,
        std::string workDirectory,
        std::string iconName,
        std::string description,
        std::string overrideDlls,
        std::string wineDebug,
        std::string virtualDesktop,
        std::string lang,
        bool useTerminal,
        std::string display,
        intptr_t priority,
        std::string preRunScript,
        std::string postRunScript,
        uintptr_t prefixId,
        uintptr_t dirId,
        uintptr_t id):
    DBObject(tableName_, id),
    name_(name),
    path_(path),
    args_(args),
    workDirectory_(workDirectory),
    iconName_(iconName),
    description_(description),
    overrideDlls_(overrideDlls),
    wineDebug_(wineDebug),
    virtualDesktop_(virtualDesktop),
    lang_(lang),
    useTerminal_(useTerminal),
    display_(display),
    priority_(priority),
    preRunScript_(preRunScript),
    postRunScript_(postRunScript),
    prefixId_(prefixId),
    dirId_(dirId) {
}

WineApplication::~WineApplication() {
}

const std::string WineApplication::getNiceCmd(void) const {
    std::ostringstream s_cmd;
    if (priority_ != 0) {
        // FIXME: get nice cmd from settings or env
        s_cmd << "/usr/bin/nice " <<  priority_ << " ";
    }
    return s_cmd.str();
}

const std::string WineApplication::getEnvVariables() const {
    std::ostringstream s_env;
    if (!lang_.empty())
        s_env << " LANG='" << lang_ << "'";
    if (!display_.empty())
        s_env << " DISPLAY='" << display_ << "'";
    if (!overrideDlls_.empty())
        s_env << " WINEDLLOVERRIDES='" << overrideDlls_ << "'";
    if (!wineDebug_.empty())
        s_env << " WINEDEBUG='" << wineDebug_ << "'";
    return s_env.str();
}

void WineApplication::setName(std::string name) {
    name_ = name;
}

void WineApplication::setPath(std::string path) {
    path_ = path;
}

void WineApplication::setArgs(std::string args) {
    path_ = args;
}

void WineApplication::setWorkDirectory(std::string workDirectory) {
    workDirectory_ = workDirectory;
}

void WineApplication::setIconName(std::string iconName) {
    iconName_ = iconName;
}

void WineApplication::setDescription(std::string description) {
    description_ = description;
}

void WineApplication::setOverrideDlls(std::string overrideDlls) {
    overrideDlls_ = overrideDlls;
}

void WineApplication::setWineDebug(std::string wineDebug) {
    wineDebug_ = wineDebug;
}

void WineApplication::setVirtualDesktop(std::string virtualDesktop) {
    virtualDesktop_ = virtualDesktop;
}

void WineApplication::setUseTerminal(bool useTerminal) {
    useTerminal_ = useTerminal;
}

void WineApplication::setDisplay(std::string display) {
    display_ = display;
}

void WineApplication::setPriority(intptr_t priority) {
    priority_ = priority;
}

void WineApplication::setPreRunScript(std::string preRunScript) {
    preRunScript_ = preRunScript;
}

void WineApplication::setPostRunScript(std::string postRunScript) {
    postRunScript_ = postRunScript;
}

void WineApplication::setPrefixId(uintptr_t prefixId) {
    prefixId_ = prefixId;
}

void WineApplication::setDirId(uintptr_t dirId) {
    dirId_ = dirId;
}

const std::string WineApplication::getName(void) const {
    return name_;
}

const std::string WineApplication::getPath(void) const {
    return path_;
}

const std::string WineApplication::getArgs(void) const {
    return args_;
}

const std::string WineApplication::getWorkDirectory(void) const {
    return workDirectory_;
}

const std::string WineApplication::getIconName(void) const {
    return iconName_;
}

const std::string WineApplication::getDescription(void) const {
    return description_;
}

const std::string WineApplication::getOverrideDlls(void) const {
    return overrideDlls_;
}

const std::string WineApplication::getWineDebug(void) const {
    return wineDebug_;
}

const std::string WineApplication::getVirtualDesktop(void) const {
    if (!virtualDesktop_.empty()) {
        std::ostringstream str_stream;
        str_stream << "explorer.exe /desktop=";
        std::string tmpl = std::regex_replace(getName(),
                                std::regex("[^a-zA-Z0-9.]"),
                                ".");
        str_stream << tmpl << "," << virtualDesktop_;
        return str_stream.str();
    }

    return virtualDesktop_;
}

bool WineApplication::getUseTerminal(void) const {
    return useTerminal_;
}

const std::string WineApplication::getDisplay(void) const {
    return display_;
}

intptr_t WineApplication::getPriority(void) const {
    return priority_;
}

const std::string WineApplication::getPreRunScript(void) const {
    return preRunScript_;
}

const std::string WineApplication::getPostRunScript(void) const {
    return postRunScript_;
}

uintptr_t WineApplication::getPrefixId(void) const {
    return prefixId_;
}

uintptr_t WineApplication::getDirId(void) const {
    return dirId_;
}

}  // namespace lib
}  // namespace q4wine
