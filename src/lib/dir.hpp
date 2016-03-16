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

/*! \class Dir dir.hpp <q4wine/src/lib/dir.hpp>
 * \brief Describes an virtual directory
 *
 * \author Alexey S. Malakhov <brezerk@gmail.com>
 */
class Dir : public DBObject {
 public:
    /*! Constructs an empty Dir object. */
    Dir();
    /*! Constructs an Dir object.
     *
     * \param name User defined name.
     *
     * \param prefixId The associated row id for WinePrefix object.
     * If it set to 0, then the object is not associated with any
     * database record.
     * See also: #setPrefixId #getPrefixId
     *
     * \param dirId The associated row id for  WineApplication object.
     * If it set to 0, then the object is not associated with any
     * database record.
     * See also: #setDirId #getDirId
     *
     * \param id The associated row id. If it set to 0, then the object
     * is not associated with any database record.
     * See also: DBObject#setId DBObject#getId
    */
    Dir(
            std::string name,
            intptr_t prefixId = 0,
            intptr_t dirId = 0,
            intptr_t id = 0);
    ~Dir();

    /*!
     * \brief Save Object into DB
     * \return True on success
     */
    virtual bool save(void);

    /*!
     * \brief Create Dir object from DB by version.id.
     * \param id Record id.
     * \return Dir object or NULL if not found.
     */
    static Dir* getInstance(intptr_t id);

    void setName(std::string name);
    void setPrefixId(intptr_t prefixId);
    void setDirId(intptr_t dirId);

    const std::string getName(void) const;
    intptr_t getPrefixId(void) const;
    intptr_t getDirId(void) const;

 private:
    /*! Set the table name in q4wine database to lookup object data */
    static const std::string tableName_;
    std::string name_;
    intptr_t prefixId_;
    intptr_t dirId_;
};

}  // namespace lib
}  // namespace q4wine
