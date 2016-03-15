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
set(Q4WINE_CLI_HEADERS_DIR
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/cli
    )

set(Q4WINE_CLI_SOURCES
    ${CMAKE_SOURCE_DIR}/src/cli/main.cpp
    )

add_executable(q4wine-cli
    ${Q4WINE_CLI_SOURCES}
    ${Q4WINE_CLI_HEADERS}
    )

# cpplint target
add_custom_target (
    cpplint_cli
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

add_custom_command (
    TARGET cpplint_cli
    COMMAND ${PYTHON_EXECUTABLE}
            ${CMAKE_SOURCE_DIR}/tools/cpplint.py
            --extensions=hpp,cpp
	    ${Q4WINE_CLI_SOURCES}
	    ${Q4WINE_CLI_HEADERS}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

add_dependencies(q4wine-cli cpplint_cli)
add_dependencies(q4wine-cli get_version)

include_directories(${Q4WINE_CLI_HEADERS_DIR} ${CMAKE_BINARY_DIR})

target_link_libraries(q4wine-cli q4wine)

# Install targets
if(RELEASE)
    install(TARGETS q4wine-cli DESTINATION ${BIN_PREFIX})
    install(FILES ${DESKTOP_GUI_ENTRY} DESTINATION ${DESKTOP_ENTRY_PATH})
endif(RELEASE)

# EOF
