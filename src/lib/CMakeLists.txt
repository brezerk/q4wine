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

# Set the library versions
set(LIB_VERSION_MAJOR 2)
set(LIB_VERSION_MINOR 0)
set(LIB_VERSION_PATCH 0)
set(LIB_API_VERSION ${LIB_VERSION_MAJOR})
set(LIB_VERSION
    ${LIB_VERSION_MAJOR}.${LIB_VERSION_MINOR}.${LIB_VERSION_PATCH}
    )

# Q4Wine lib sources
set(Q4WINE_LIB_HEADERS_DIR
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/lib
    )

set(Q4WINE_LIB_HEADERS
    ${CMAKE_SOURCE_DIR}/src/lib/db.hpp
    ${CMAKE_SOURCE_DIR}/src/lib/defines.hpp
    ${CMAKE_SOURCE_DIR}/src/lib/dbobject.hpp
    ${CMAKE_SOURCE_DIR}/src/lib/dir.hpp
    ${CMAKE_SOURCE_DIR}/src/lib/wineprefix.hpp
    ${CMAKE_SOURCE_DIR}/src/lib/wineversion.hpp
    ${CMAKE_SOURCE_DIR}/src/lib/wineapplication.hpp
    )

set(Q4WINE_LIB_SOURCES
    ${CMAKE_SOURCE_DIR}/src/lib/db.cpp
    ${CMAKE_SOURCE_DIR}/src/lib/dbobject.cpp
    ${CMAKE_SOURCE_DIR}/src/lib/dir.cpp
    ${CMAKE_SOURCE_DIR}/src/lib/wineprefix.cpp
    ${CMAKE_SOURCE_DIR}/src/lib/wineversion.cpp
    ${CMAKE_SOURCE_DIR}/src/lib/wineapplication.cpp
    )

include_directories(${Q4WINE_LIB_HEADERS_DIR} ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR})

# cpplint target
add_custom_target (
    cpplint_lib
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

add_custom_command (
    TARGET cpplint_lib
    COMMAND ${PYTHON_EXECUTABLE}
            ${CMAKE_SOURCE_DIR}/tools/cpplint.py
            --extensions=hpp,cpp
            ${Q4WINE_LIB_SOURCES}
            ${Q4WINE_LIB_HEADERS}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

add_library(q4wine SHARED ${Q4WINE_LIB_SOURCES} ${Q4WINE_LIB_HEADERS})
add_dependencies(q4wine cpplint_lib)

if (NOT RELEASE)
    add_subdirectory(test)
endif (NOT RELEASE)

set_target_properties(q4wine
    PROPERTIES
    VERSION ${LIB_VERSION}
    SOVERSION ${LIB_API_VERSION}
    )

target_link_libraries(q4wine
    ${SQLITE3_LIBRARIES}
    )

# Install targets
if(RELEASE)
    install(TARGETS q4wine DESTINATION ${LIB_PREFIX})
    #FIXME: Install headers too
endif(RELEASE)

# EOF
