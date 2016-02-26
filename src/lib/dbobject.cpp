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

#include "src/lib/dbobject.hpp"

namespace q4wine {
namespace lib {

DBObject::DBObject(
        QString tableName,
        uint32_t id)
    :
      tableName_(tableName),
      id_(id) {
}

DBObject::~DBObject() {
}

void DBObject::setId(uint32_t id) {
    id_ = id;
}

uint32_t DBObject::getId(void) const {
    return id_;
}

}  // namespace lib
}  // namespace q4wine
