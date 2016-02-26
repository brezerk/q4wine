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

namespace q4wine {
namespace lib {

const QString WineApplication::tableName_ = "icon";

WineApplication::WineApplication() : DBObject(tableName_) {
}

WineApplication::WineApplication(
        QString name,
        QString path,
        QString args,
        QString workDirectory,
        QString iconName,
        QString description,
        QString overrideDlls,
        QString wineDebug,
        QString virtualDesktop,
        QString lang,
        bool useTerminal,
        QString display,
        int32_t priority,
        QString preRunScript,
        QString postRunScript,
        uint32_t prefixId,
        uint32_t dirId,
        uint32_t id):
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

void WineApplication::setName(QString name) {
    name_ = name;
}

void WineApplication::setPath(QString path) {
    path_ = path;
}

void WineApplication::setArgs(QString args) {
    path_ = args;
}

void WineApplication::setWorkDirectory(QString workDirectory) {
    workDirectory_ = workDirectory;
}

void WineApplication::setIconName(QString iconName) {
    iconName_ = iconName;
}

void WineApplication::setDescription(QString description) {
    description_ = description;
}

void WineApplication::setOverrideDlls(QString overrideDlls) {
    overrideDlls_ = overrideDlls;
}

void WineApplication::setWineDebug(QString wineDebug) {
    wineDebug_ = wineDebug;
}

void WineApplication::setVirtualDesktop(QString virtualDesktop) {
    virtualDesktop_ = virtualDesktop;
}

void WineApplication::setUseTerminal(bool useTerminal) {
    useTerminal_ = useTerminal;
}

void WineApplication::setDisplay(QString display) {
    display_ = display;
}

void WineApplication::setPriority(int32_t priority) {
    priority_ = priority;
}

void WineApplication::setPreRunScript(QString preRunScript) {
    preRunScript_ = preRunScript;
}

void WineApplication::setPostRunScript(QString postRunScript) {
    postRunScript_ = postRunScript;
}

void WineApplication::setPrefixId(uint32_t prefixId) {
    prefixId_ = prefixId;
}

void WineApplication::setDirId(uint32_t dirId) {
    dirId_ = dirId;
}

const QString WineApplication::getName(void) const {
    return name_;
}

const QString WineApplication::getPath(void) const {
    return path_;
}

const QString WineApplication::getArgs(void) const {
    return args_;
}

const QString WineApplication::getWorkDirectory(void) const {
    return workDirectory_;
}

const QString WineApplication::getIconName(void) const {
    return iconName_;
}

const QString WineApplication::getDescription(void) const {
    return description_;
}

const QString WineApplication::getOverrideDlls(void) const {
    return overrideDlls_;
}

const QString WineApplication::getWineDebug(void) const {
    return wineDebug_;
}

const QString WineApplication::getVirtualDesktop(void) const {
    return virtualDesktop_;
}

bool WineApplication::getUseTerminal(void) const {
    return useTerminal_;
}

const QString WineApplication::getDisplay(void) const {
    return display_;
}

int32_t WineApplication::getPriority(void) const {
    return priority_;
}

const QString WineApplication::getPreRunScript(void) const {
    return preRunScript_;
}

const QString WineApplication::getPostRunScript(void) const {
    return postRunScript_;
}

uint32_t WineApplication::getPrefixId(void) const {
    return prefixId_;
}

uint32_t WineApplication::getDirId(void) const {
    return dirId_;
}

}  // namespace lib
}  // namespace q4wine
