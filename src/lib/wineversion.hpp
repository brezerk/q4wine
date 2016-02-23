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

#pragma once

#include <QString>

namespace q4wine {
namespace lib {

class WineVersion {
 public:
    WineVersion();
    WineVersion(
            QString binary,
            QString loader,
            QString server,
            QString libs32,
            QString libs64);
    ~WineVersion();
    void setBinary(QString binary);
    void setLoader(QString loader);
    void setServer(QString server);
    void setLibs32(QString libs32);
    void setLibs64(QString libs64);

    const QString getBinary(void) const;
    const QString getLoader(void) const;
    const QString getServer(void) const;
    const QString getLibs32(void) const;
    const QString getLibs64(void) const;
    // FIXME: Use global enum WineArch
    const QString getLibs(QString arch) const;

 private:
    QString binary_;
    QString loader_;
    QString server_;
    QString libs32_;
    QString libs64_;
};
}  // namespace lib
}  // namespace q4wine
