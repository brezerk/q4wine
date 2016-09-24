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
set(Q4WINE_QT_HEADERS_DIR
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/qt
    )

set(Q4WINE_QT_HEADEDRS
    ${CMAKE_SOURCE_DIR}/src/qt/widgets/desktop/iconlist.hpp
    ${CMAKE_SOURCE_DIR}/src/qt/widgets/desktop/prefixtree.hpp
    ${CMAKE_SOURCE_DIR}/src/qt/widgets/desktop.hpp
    ${CMAKE_SOURCE_DIR}/src/qt/widgets/process/processlist.hpp
    ${CMAKE_SOURCE_DIR}/src/qt/widgets/process.hpp
    ${CMAKE_SOURCE_DIR}/src/qt/widgets/configuration.hpp
    ${CMAKE_SOURCE_DIR}/src/qt/mainwindow.hpp
    )

set(Q4WINE_QT_SOURCES
    ${CMAKE_SOURCE_DIR}/src/qt/widgets/desktop/iconlist.cpp
    ${CMAKE_SOURCE_DIR}/src/qt/widgets/desktop/prefixtree.cpp
    ${CMAKE_SOURCE_DIR}/src/qt/widgets/desktop.cpp
    ${CMAKE_SOURCE_DIR}/src/qt/widgets/process/processlist.cpp
    ${CMAKE_SOURCE_DIR}/src/qt/widgets/process.cpp
    ${CMAKE_SOURCE_DIR}/src/qt/widgets/configuration.cpp
    ${CMAKE_SOURCE_DIR}/src/qt/mainwindow.cpp
    ${CMAKE_SOURCE_DIR}/src/qt/main.cpp
    )

set(Q4WINE_QT_MOC_HEADEDRS
    ${Q4WINE_QT_HEADEDRS}
    )

set(Q4WINE_QT_UI_SOURCES
#    ${CMAKE_SOURCE_DIR}/src/qt/ui/mainwindow.ui
    )

set(Q4WINE_QT_RESOURCE_SOURCES
    ""
    )

# qt5_add_resources(RCC_SRCS ${Q4WINE_RESOURCE_SOURCES})
qt5_wrap_ui(Q4WINE_QT_UI_HEADERS ${Q4WINE_QT_UI_SOURCES})
qt5_wrap_cpp(Q4WINE_QT_MOC_SOURCES ${Q4WINE_QT_MOC_HEADEDRS})

add_executable(q4wine-qt
    ${Q4WINE_QT_SOURCES}
    ${Q4WINE_QT_HEADERS}
    ${Q4WINE_QT_UI_HEADERS}
    ${Q4WINE_QT_MOC_SOURCES}
    ${RCC_SRCS}
    ${QM_I18N}
    )

# cpplint target
add_custom_target (
    cpplint_qt
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

add_custom_command (
    TARGET cpplint_qt
    COMMAND ${PYTHON_EXECUTABLE}
            ${CMAKE_SOURCE_DIR}/tools/cpplint.py
            --extensions=hpp,cpp
            ${Q4WINE_QT_SOURCES}
            ${Q4WINE_QT_HEADERS}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

add_dependencies(q4wine-qt cpplint_qt)
add_dependencies(q4wine-qt get_version)

include_directories(${Q4WINE_QT_HEADERS_DIR} ${CMAKE_BINARY_DIR})
qt5_use_modules(q4wine-qt Core DBus Network Sql Widgets Xml)

target_link_libraries(q4wine-qt q4wine)

# Install targets
if(RELEASE)
    install(TARGETS q4wine-qt DESTINATION ${BIN_PREFIX})
    install(FILES ${DESKTOP_GUI_ENTRY} DESTINATION ${DESKTOP_ENTRY_PATH})
endif(RELEASE)

# EOF
