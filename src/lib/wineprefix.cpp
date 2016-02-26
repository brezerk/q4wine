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

namespace q4wine {
namespace lib {

const QString WinePrefix::tableName_ = "prefix";

WinePrefix::WinePrefix() : DBObject(tableName_) {
}

WinePrefix::WinePrefix(
        QString name,
        QString path,
        WineArch arch,
        WineVersion version,
        QString mountPoint,
        QString virtualDevice,
        QString execTemplate,
        uint32_t id):
    DBObject(tableName_, id),
    name_(name),
    path_(path),
    arch_(arch),
    version_(version),
    mountPoint_(mountPoint),
    virtualDevice_(virtualDevice),
    execTemplate_(execTemplate) {
}

WinePrefix::~WinePrefix() {
}

/*! FIXME: ok. this is cool. but we need to use execution template
* string instead :)
*/
QString WinePrefix::getWineEnv() {
    QStringList wineStrings;
    wineStrings.append(QString("WINEPREFIX='%1'").arg(getPath()));
    wineStrings.append(version_.getEnvVariables(arch_));
    QString arch = getArchString();
    if (!arch.isEmpty()) {
        wineStrings.append(QString("WINEARCH='%1'").arg(arch));
    }
    return wineStrings.join(" ");
}

void WinePrefix::setName(QString name) {
    name_ = name;
}

void WinePrefix::setPath(QString path) {
    path_ = path;
}

void WinePrefix::setArch(WineArch arch) {
    arch_ = arch;
}

void WinePrefix::setVersion(WineVersion version) {
    version_ = version;
}

void WinePrefix::setMountPoint(QString mountPoint) {
    mountPoint_ = mountPoint;
}

void WinePrefix::setVirtualDevice(QString virtualDevice) {
    virtualDevice_ = virtualDevice;
}

void WinePrefix::setExecutionTemplate(QString execTemplate) {
    execTemplate_ = execTemplate;
}

const QString WinePrefix::getName(void) const {
    return name_;
}

const QString WinePrefix::getPath(void) const {
    return path_;
}

WineArch WinePrefix::getArch(void) const {
    return arch_;
}

const QString WinePrefix::getArchString(void) const {
    QString arch;
    if (arch_ == q4wine::lib::WIN32) {
        arch = "win32";
    } else if (arch_ == q4wine::lib::WIN32) {
        arch = "win64";
    } else {
        arch = QString::null;
    }
    return arch;
}

WineVersion WinePrefix::getVersion(void) const {
    return version_;
}

const QString WinePrefix::getMountPoint(void) const {
    return mountPoint_;
}

const QString WinePrefix::getVirtualDevice(void) const {
    return virtualDevice_;
}


const QString WinePrefix::getExecutionTemplate(void) const {
    return execTemplate_;
}

}  // namespace lib
}  // namespace q4wine
