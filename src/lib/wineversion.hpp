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

#include <iostream>
#include <memory>
#include <string>
#include <sstream>

#include "src/lib/defines.hpp"
#include "src/lib/dbobject.hpp"

namespace q4wine {
namespace lib {

/*! \class WineVersion wineversion.hpp <q4wine/src/lib/wineversion.hpp>
 * \brief Describes a wine version configuration.
 *
 * \par Wine Terms
 *
 * Wine allows user to set WINESERVER, WINELOADER and WINEDLLPATH env
 * variables to use different binaries. This allows user to have multiple
 * wine instances installed on the system.
 *
 * \par Q4Wine Terms
 *
 * \note It is possible to have multiple WinePrefix pointed to the single
 * directory. In combination with WineVersion: setups is possible to run a
 * number of truly independent wine processes.
 *
 * \author Alexey S. Malakhov <brezerk@gmail.com>
 */
class WineVersion : public DBObject {
 public:
    /*! Constructs an empty WineVersion object. */
    WineVersion();
    /*! Constructs an WineVersion object.
     *
     * \param name User defined name.
     *
     * \param binary The general wine binary. Defaults to /usr/bin/wine if not
     * set and if this doesn't exist we will then look for a file named "wine"
     * in the path and in a few other likely locations.
     * See also: #setBinary #getBinary
     *
     * \param loader Specifies the path and name of the wine binary to use to
     * launch new Windows processes. If not set, Wine will try to load
     * /usr/bin/wine, and if this doesn't exist it will then look for a file
     * named "wine" in the path and in a few other likely locations.
     * See also: #setLoader #getLoader
     *
     * \param server Specifies the path and name of the wineserver binary.
     * If not set, Wine will try to load /usr/bin/wineserver, and if this
     * doesn't exist it will then look for a file named "wineserver" in the
     * path and in a few other likely locations.
     * See also: #setServer #getServer
     *
     * \param libs32 Specifies the path(s) in which to search for builtin
     * dlls and Winelib applications. This is a list of directories separated
     * by ":". In addition to any directory specified in WINEDLLPATH, Wine
     * will also look in /usr/lib32/wine.
     * See also: #setLibs32 #getLibs32 #getLibs
     *
     * \param libs64 Specifies the path(s) in which to search for builtin
     * dlls and Winelib applications. This is a list of directories separated
     * by ":". In addition to any directory specified in WINEDLLPATH, Wine
     * will also look in /usr/lib64/wine.
     * See also: #setLibs64 #getLibs64 #getLibs
     *
     * \param id The associated row id. If it set to 0, then the object
     * is not associated with any database record.
     * See also: DBObject#setId DBObject#getId
    */
    WineVersion(
            std::string name,
            std::string binary,
            std::string server = std::string(),
            std::string loader = std::string(),
            std::string libs32 = std::string(),
            std::string libs64 = std::string(),
            intptr_t id = 0);
    ~WineVersion();

    /*!
     * \brief save Object into DB
     * \return true on success
     */
    virtual bool save(void);

    /*!
     * \brief getInstance Create WineVersion object from DB by version.id
     * \param id Record id
     * \return WineVersion object or NULL of not found.
     */
    static WineVersion* getInstance(intptr_t id);

    /*! Constructs env variables (WINESERVER, WINELOADER, WINEDLLPATH) from
     * WineVersion data.
     *
     * Example:
     * WINESERVER='/usr/bin/wineserver'
     * WINELOADER='/usr/bin/wine'
     * WINEDLLPATH='/usr/lib64/wine/'
     *
     * \param arch Desired Wine Arch \see WinePrefix
     *
     * \return Formatted env variables
    */
    const std::string getEnvVariables(const std::string arch) const;

    void setName(std::string name);
    void setBinary(std::string binary);
    void setServer(std::string server);
    void setLoader(std::string loader);
    void setLibs32(std::string libs32);
    void setLibs64(std::string libs64);

    const std::string getName(void) const;
    const std::string getBinary(void) const;
    const std::string getServer(void) const;
    const std::string getLoader(void) const;
    const std::string getLibs32(void) const;
    const std::string getLibs64(void) const;
    /*! Return libs configured for provided version */
    const std::string getLibs(const std::string arch) const;

 private:
    /*! Set the table name in q4wine database to lookup object data */
    static const std::string tableName_;
    std::string name_;
    std::string binary_;
    std::string server_;
    std::string loader_;
    std::string libs32_;
    std::string libs64_;
};

}  // namespace lib
}  // namespace q4wine
