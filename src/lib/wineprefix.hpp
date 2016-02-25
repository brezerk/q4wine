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

#include <stdint.h>

#include <QString>
#include <QStringList>

#include "src/lib/defines.hpp"
#include "src/lib/dbobject.hpp"
#include "src/lib/wineversion.hpp"

namespace q4wine {
namespace lib {

/*! \class WinePrefix wineprefix.hpp <q4wine/src/lib/wineprefix.hpp>
 * \brief Describes a wine prefix configuration.
 *
 * \par Wine Terms
 *
 * In terms of Wine WINEPREFIX is a directory which holds a virtual
 * windows drive with separate wine settings (drives, virtual desktop,
 * special dlls and so on).
 *
 * \par Q4Wine Terms
 *
 * In terms of Q4Wine WinePrefix is a set of options which describe basic
 * WINEPREFIX configuration.
 *
 * \note It is possible to have multiple WinePrefix pointed to the single
 * directory. In combination with WineVersion: setups is possible to run a
 * number of truly independent wine processes.
 *
 * \author Alexey S. Malakhov <brezerk@gmail.com>
 */
class WinePrefix : public DBObject {
 public:
    /*! Constructs an empty WineConfiguration object. */
    WinePrefix();
    /*! Constructs an WineConfiguration object.
     *
     * \param name is user defined name, should be unique. See also: #setName #getName
     *
     * \param path the name of the directory where Wine will store its data. See also: #setPath #getPath
     *
     * \param arch is the architecture supported by a given Wine prefix. It
     * is set at prefix creation time and cannot be changed afterwards without
     * wiping prefix directory content.
     * See also: #setArch #getArch
     *
     * \param mountPoint an mount point directory where Disk images or
     * CD/DVD drives will be mounted by user request.
     * See also: #setMountPoint #getMountPoint
     *
     * \param virtualDevice an virtual device letter for mount point.
     * See also: #setVirtualDevice #getVirtualDevice
     *
     * \param execTemplate will be used to format command line string to run wine
     * programs.
     * See alse: #setExecutionTemplate #getExecutionTemplate
    */
    WinePrefix(QString name,
            QString path,
            WineArch arch,
            WineVersion version,
            QString mountPoint = QString::null,
            QString virtualDevice = QString::null,
            QString execTemplate = QString::null,
            uint32_t id = 0);
    /*! Destroys this WinePrefix object. */
    ~WinePrefix();

    /*! Constructs wine env variables (WINEPREFIX, WINESERVER, WINELOADER,
     * WINEDLLPATH, WINEARCH) using WinePrefix and WineVersion data.
     *
     * Example:
     * WINEPREFIX='/mnt/ssd/wine/'
     * WINESERVER='/usr/bin/wineserver'
     * WINELOADER='/usr/bin/wine'
     * WINEDLLPATH='/usr/lib64/wine/'
     * WINEARCH='win64'
     *
     * \return QString formatted env variables
     */
    QString getWineEnv();

    void setName(QString name);
    void setPath(QString path);
    void setArch(WineArch arch);
    void setVersion(WineVersion version);
    void setMountPoint(QString mountPoint);
    void setVirtualDevice(QString virtualDevice);
    void setExecutionTemplate(QString execTemplate);
    const QString getName(void) const;
    const QString getPath(void) const;
    WineArch getArch(void) const;
    /*! Return arch string representation */
    const QString getArchString(void) const;
    WineVersion getVersion(void) const;
    const QString getMountPoint(void) const;
    const QString getVirtualDevice(void) const;
    const QString getExecutionTemplate(void) const;

 private:
    /*! Set the table name in q4wine database to lookup object data */
    static const QString tableName_;
    QString name_;
    QString path_;
    WineArch arch_;
    WineVersion version_;
    QString mountPoint_;
    QString virtualDevice_;
    QString execTemplate_;
};

}  // namespace lib
}  // namespace q4wine
