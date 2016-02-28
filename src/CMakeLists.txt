#=============================================================================
# Copyright (C) 2008-2016 by Alexey S. Malakhov <brezerk@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#=============================================================================

cmake_minimum_required(VERSION 2.8.12)

# Get GIT version strings
add_custom_target (
    get_version
    DEPENDS ${CMAKE_SOURCE_DIR}/src/version.h
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

add_custom_command (
    OUTPUT ${CMAKE_SOURCE_DIR}/src/version.h
    COMMAND ${CMAKE_COMMAND}
    -D SRC=${CMAKE_SOURCE_DIR}/src/version.h.in
    -D DST=${CMAKE_SOURCE_DIR}/src/version.h
    -D RELEASE=${RELEASE}
    -D GIT_EXECUTABLE=${GIT_EXECUTABLE}
    -D APP_VERSION=${APP_VERSION}
    -P ${CMAKE_SOURCE_DIR}/src/version.cmake
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

if(DOC)
    # Doxygen target
    add_custom_target (
        docs ALL
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        )

    add_custom_command (
        TARGET docs
        COMMAND ${DOXYGEN_EXECUTABLE}
        Doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        )
endif(DOC)

# Add project subdirs
add_subdirectory(lib)

if(QT5)
    add_subdirectory(gui)
endif(QT5)

# EOF
