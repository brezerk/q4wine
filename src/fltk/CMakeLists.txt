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
set(Q4WINE_FLTK_HEADERS_DIR
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/fltk
    )

set(Q4WINE_FLTK_SOURCES
    ${CMAKE_SOURCE_DIR}/src/fltk/main.cpp
    )

set(Q4WINE_FLTK_UI_SOURCES
    ${CMAKE_SOURCE_DIR}/src/fltk/ui.fl
    )

fltk_wrap_ui(Ui ${Q4WINE_FLTK_UI_SOURCES})
# add_library(Ui callbacks.cxx ${Q4WINE_FLTK_SOURCES})

add_executable(q4wine-fltk
	${Q4WINE_FLTK_SOURCES}
	${Q4WINE_FLTK_HEADERS}
    )

# cpplint target
add_custom_target (
    cpplint_fltk
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

add_custom_command (
    TARGET cpplint_fltk
    COMMAND ${PYTHON_EXECUTABLE}
            ${CMAKE_SOURCE_DIR}/tools/cpplint.py
            --extensions=hpp,cpp
	    ${Q4WINE_FLTK_SOURCES}
	    ${Q4WINE_FLTK_HEADERS}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

add_dependencies(q4wine-fltk cpplint_qt)
add_dependencies(q4wine-fltk get_version)
#add_dependencies(q4wine-fltk Ui)

include_directories(
    ${Q4WINE_FLTK_HEADERS_DIR}
    ${CMAKE_BINARY_DIR}
    ${FLTK_INCLUDE_DIR}
    )

target_link_libraries(q4wine-fltk
    q4wine
#    Ui
    ${FLTK_LIBRARIES}
    ${OPENGL_LIBRARIES}
    )

# Install targets
if(RELEASE)
    install(TARGETS q4wine-fltk DESTINATION ${BIN_PREFIX})
    install(FILES ${DESKTOP_GUI_ENTRY} DESTINATION ${DESKTOP_ENTRY_PATH})
endif(RELEASE)

# EOF
