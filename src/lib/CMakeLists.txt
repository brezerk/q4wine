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
set(LIB_VERSION_MAJOR 1)
set(LIB_VERSION_MINOR 0)
set(LIB_VERSION_PATCH 0)
set(LIB_API_VERSION ${LIBQTWINE_VERSION_MAJOR})
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
    ${CMAKE_SOURCE_DIR}/src/lib/defines.hpp
    ${CMAKE_SOURCE_DIR}/src/lib/wineprefix.hpp
    ${CMAKE_SOURCE_DIR}/src/lib/wineversion.hpp
    )

set(Q4WINE_LIB_SOURCES
    ${CMAKE_SOURCE_DIR}/src/lib/wineprefix.cpp
    ${CMAKE_SOURCE_DIR}/src/lib/wineversion.cpp
    )

# Q4Wine lib tests
set(Q4WINE_LIB_TEST_SOURCE
    ${CMAKE_SOURCE_DIR}/src/lib/test/test_q4winelib.cpp
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
            ${Q4WINE_LIB_TEST_SOURCE}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

add_executable(test_libq4wine ${Q4WINE_LIB_TEST_SOURCE} ${Q4WINE_LIB_SOURCES} ${Q4WINE_LIB_HEADERS})
add_dependencies(test_libq4wine cpplint_lib)
qt5_use_modules(test_libq4wine Core Test)
add_test(libq4wine test_libq4wine)

add_library(libq4wine SHARED ${Q4WINE_LIB_SOURCES} ${Q4WINE_LIB_HEADERS})
add_dependencies(libq4wine cpplint_lib)

set_target_properties(libq4wine
    PROPERTIES
    VERSION ${LIBQTWINE_VERSION}
    SOVERSION ${LIBQTWINE_API_VERSION}
    OUTPUT_NAME libq4wine
    )

qt5_use_modules(libq4wine Core Network Sql)

# Install targets
if(RELEASE)
    install(TARGETS libq4wine DESTINATION ${LIB_PREFIX})
endif(RELEASE)

# EOF
