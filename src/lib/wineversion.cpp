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

namespace q4wine {
namespace lib {

const QString WineVersion::tableName_ = "version";

WineVersion::WineVersion() : DBObject(tableName_) {
}

WineVersion::WineVersion(
            QString binary,
            QString loader,
            QString server,
            QString libs32,
            QString libs64,
            uint32_t id):
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

QString WineVersion::getEnvVariables(q4wine::lib::WineArch arch) {
    QStringList wineStrings;
    if (!server_.isEmpty())
        wineStrings.append(QString("WINESERVER='%1'").arg(server_));
    if (!loader_.isEmpty())
        wineStrings.append(QString("WINELOADER='%1'").arg(loader_));
    QString libs = getLibs(arch);
    if (!libs.isEmpty())
        wineStrings.append(QString("WINEDLLPATH='%1'").arg(libs));
    return wineStrings.join(" ");
}

void WineVersion::setBinary(QString binary) {
    binary_ = binary;
}

void WineVersion::setLoader(QString loader) {
    loader_ = loader;
}

void WineVersion::setServer(QString server) {
    server_ = server;
}

void WineVersion::setLibs32(QString libs32) {
    libs32_ = libs32;
}

void WineVersion::setLibs64(QString libs64) {
    libs64_ = libs64;
}

const QString WineVersion::getBinary(void) const {
    return binary_;
}

const QString WineVersion::getLoader(void) const {
    return loader_;
}

const QString WineVersion::getServer(void) const {
    return server_;
}

const QString WineVersion::getLibs32(void) const {
    return libs32_;
}

const QString WineVersion::getLibs64(void) const {
    return libs64_;
}

const QString WineVersion::getLibs(q4wine::lib::WineArch arch) const {
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
