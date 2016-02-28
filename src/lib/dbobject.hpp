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
#include <string>

#include "src/lib/defines.hpp"

namespace q4wine {
namespace lib {

/*! \class DBObject dbobject.hpp <q4wine/src/lib/dbobject.hpp>
 * \brief Describes an abstract interface of object to q4wine db
 *
 * This class provides common methods like load(), save(),
 * delete() to child objects.
 *
 * It also store associated row id.
 *
 * \author Alexey S. Malakhov <brezerk@gmail.com>
 */
class DBObject {
 public:
    /*! Constructs an DBObject object.
     *
     * \param tableName The q4wine database table name where the object
     * is stored.
     *
     * \param id The associated row id. If it set to 0, then the object
     * is not associated with any database record.
     * See also: #setId #getId
     */
    explicit DBObject(std::string tableName, uintptr_t id = 0);
    /*! Destroys this DBObject object. */
    virtual ~DBObject();

    void setId(uintptr_t id);
    uintptr_t getId(void) const;
 protected:
    std::string tableName_;
    uintptr_t id_;
};

}  // namespace lib
}  // namespace q4wine
