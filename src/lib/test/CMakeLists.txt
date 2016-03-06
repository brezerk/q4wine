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

# Q4Wine lib tests
set(Q4WINE_LIB_TEST_DBENGINE
    ${CMAKE_SOURCE_DIR}/src/lib/test/test_dbengine.cpp
    )

set(Q4WINE_LIB_TEST_WINEOBJECTS
    ${CMAKE_SOURCE_DIR}/src/lib/test/test_wineobjects.cpp
    )

include_directories(${Q4WINE_LIB_HEADERS_DIR} ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR})

# cpplint target
add_custom_target (
    cpplint_lib_test
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

add_custom_command (
    TARGET cpplint_lib_test
    COMMAND ${PYTHON_EXECUTABLE}
            ${CMAKE_SOURCE_DIR}/tools/cpplint.py
            --extensions=hpp,cpp
            ${Q4WINE_LIB_TEST_DBENGINE}
            ${Q4WINE_LIB_TEST_WINEOBJECTS}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

add_executable(test_wineobjects
    ${Q4WINE_LIB_TEST_WINEOBJECTS}
    ${Q4WINE_LIB_SOURCES}
    ${Q4WINE_LIB_HEADERS}
    )
add_dependencies(test_wineobjects cpplint_lib_test)
target_link_libraries(test_wineobjects ${Boost_LIBRARIES} ${SQLITE3_LIBRARIES})
add_test(wineobjects test_wineobjects)

add_executable(test_dbengine
    ${Q4WINE_LIB_TEST_DBENGINE}
    ${Q4WINE_LIB_SOURCES}
    ${Q4WINE_LIB_HEADERS}
    )
add_dependencies(test_dbengine cpplint_lib_test)
target_link_libraries(test_dbengine ${Boost_LIBRARIES} ${SQLITE3_LIBRARIES})
add_test(dbengine test_dbengine)

# EOF
