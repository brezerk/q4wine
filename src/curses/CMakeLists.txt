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

# Q4Wine gui sources
set(Q4WINE_CURSES_HEADERS_DIR
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/curses
    )

set(Q4WINE_CURSES_SOURCES
    ${CMAKE_SOURCE_DIR}/src/curses/main.cpp
    )

add_executable(q4wine-curses
    ${Q4WINE_CURSES_SOURCES}
    ${Q4WINE_CURSES_HEADERS}
    )

# cpplint target
add_custom_target (
    cpplint_curses
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

add_custom_command (
    TARGET cpplint_curses
    COMMAND ${PYTHON_EXECUTABLE}
            ${CMAKE_SOURCE_DIR}/tools/cpplint.py
            --extensions=hpp,cpp
            ${Q4WINE_CURSES_SOURCES}
            ${Q4WINE_CURSES_HEADERS}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

add_dependencies(q4wine-curses cpplint_curses)
add_dependencies(q4wine-curses get_version)

include_directories(${Q4WINE_CURSES_HEADERS_DIR} ${CMAKE_BINARY_DIR} ${CURSES_INCLUDE_DIR})

target_link_libraries(q4wine-curses q4wine ${CURSES_LIBRARIES})

# Install targets
if(RELEASE)
    install(TARGETS q4wine-curses DESTINATION ${BIN_PREFIX})
    install(FILES ${DESKTOP_GUI_ENTRY} DESTINATION ${DESKTOP_ENTRY_PATH})
endif(RELEASE)

# EOF
