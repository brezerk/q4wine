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

project(q4wine)
set(APP_VERSION "2.0")

# Enable tests
enable_testing()

# Execute moc if needed
set(CMAKE_AUTOMOC ON)

# Modules path
set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_SOURCE_DIR}/CMakeModules
    )

# Include modules
include(CheckCXXCompilerFlag)

# Build options
option(RELEASE		"Disable debug. Use it for user-ready buils."		"OFF")
option(LANGUAGES	"Compile localization files for specified languages."	""   )
option(DYNAMIC_LINKING	"Use dynamic linking. Or build static binaries."	"ON" )

# Banner
if(RELEASE)
    message("\n * Utility for Wine applications and prefixes management.")
    message(" * Copyright (C) 2008-2016 by Alexey S. Malakhov <brezerk@gmail.com>")
    message(" * http://brezblock.org.ua/\n")
endif(RELEASE)

message(STATUS "${PROJECT_NAME} build environment --\n")
message(STATUS "Build realm is: ${CMAKE_SYSTEM_NAME} ${CMAKE_HOST_SYSTEM_PROCESSOR}\n")

# OS Check Placeholders. Will be used for BINDIST
if(${CMAKE_SYSTEM_NAME} MATCHES Linux)
    set(_OS_LINUX_ 1)
endif(${CMAKE_SYSTEM_NAME} MATCHES Linux)

if(${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)
    set(_OS_FREEBSD_ 1)
endif(${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)

if(${CMAKE_SYSTEM_NAME} MATCHES Darwin)
    set(_OS_DARWIN_ 1)
endif(${CMAKE_SYSTEM_NAME} MATCHES Darwin)

if(${CMAKE_SYSTEM_NAME} MATCHES Windows)
    set(_OS_WINDOWS_ 1)
endif(${CMAKE_SYSTEM_NAME} MATCHES Windows)

# Dynamic linking WIP
if(NOT DYNAMIC_LINKING)
    message(AUTHOR_WARNING
        "-DDYNAMIC_LINKING=OFF is set. This is NOT supported yet. "
        "Work in progress. Use it for your own risk."
        )
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a;.dll.a")
    set(BUILD_SHARED_LIBRARIES OFF)
    check_cxx_compiler_flag (-static HAVE_STATIC_FLAG)
    if(HAVE_STATIC_FLAG)
        set(CMAKE_EXE_LINKER_FLAGS
            "${CMAKE_EXE_LINKER_FLAGS}
            -static"
            )
    endif(HAVE_STATIC_FLAG)
    # Workaround for cmake link library guesser
    set(CMAKE_EXE_LINK_DYNAMIC_C_FLAGS)       # remove -Wl,-Bdynamic
    set(CMAKE_EXE_LINK_DYNAMIC_CXX_FLAGS)
    set(CMAKE_SHARED_LIBRARY_C_FLAGS)         # remove -fPIC
    set(CMAKE_SHARED_LIBRARY_CXX_FLAGS)
    set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)    # remove -rdynamic
    set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)
else(NOT DYNAMIC_LINKING)
    if(MINGW)
        # Avoid depending on MinGW runtime DLLs
        set(CMAKE_EXE_LINKER_FLAGS
            "${CMAKE_EXE_LINKER_FLAGS}
            -static-libgcc
            -static-libstdc++"
            )
    endif(MINGW)
endif(NOT DYNAMIC_LINKING)

# Sysem specific actions
if(_OS_LINUX_ OR _OS_FREEBSD_)
    if(NOT DATA_PREFIX)
        set(DATA_PREFIX ${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME})
    endif(NOT DATA_PREFIX)
    if(NOT BIN_PREFIX)
        set(BIN_PREFIX ${CMAKE_INSTALL_PREFIX}/bin)
    endif(NOT BIN_PREFIX)
    if(NOT DESKTOP_ENTRY_PATH)
        set(DESKTOP_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/share/applications)
    endif(NOT DESKTOP_ENTRY_PATH)
    if(NOT PIXMAPS_ENTRY_PATH)
        set(PIXMAPS_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor)
    endif(NOT PIXMAPS_ENTRY_PATH)
    if(NOT MANPAGE_ENTRY_PATH)
        set(MANPAGE_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/share/man)
    endif(NOT MANPAGE_ENTRY_PATH)
endif(_OS_LINUX_ OR _OS_FREEBSD_)

if(_OS_WINDOWS_)
    if(NOT DATA_PREFIX)
        set(DATA_PREFIX ${CMAKE_INSTALL_PREFIX})
    endif(NOT DATA_PREFIX)
    if(NOT BIN_PREFIX)
        set(BIN_PREFIX ${CMAKE_INSTALL_PREFIX})
    endif(NOT BIN_PREFIX)
endif(_OS_WINDOWS_)

if(NOT LANGUAGES)
    set(LANGUAGES ru uk en)
endif(NOT LANGUAGES)

message(STATUS "${PROJECT_NAME} build options --\n")

# Set build types and display info
if(RELEASE)
    set(CMAKE_BUILD_TYPE RELEASE)
    add_definitions(-DRELEASE)
    message(STATUS "CMAKE_INSTALL_PREFIX     : ${CMAKE_INSTALL_PREFIX}")
    message(STATUS "BIN_PREFIX               : ${BIN_PREFIX}")
    message(STATUS "DATA_PREFIX              : ${DATA_PREFIX}")
    message(STATUS "DESKTOP_ENTRY_PATH       : ${DESKTOP_ENTRY_PATH}")
    message(STATUS "PIXMAPS_ENTRY_PATH       : ${PIXMAPS_ENTRY_PATH}")
    message(STATUS "PIXMAPS_UNITY_ENTRY_PATH : ${PIXMAPS_UNITY_ENTRY_PATH}")
    message(STATUS "MANPAGE_ENTRY_PATH       : ${MANPAGE_ENTRY_PATH}\n")
else(RELEASE)
    set(CMAKE_BUILD_TYPE DEBUG)
    #Set error tolerance to zero
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Wall")
    #Some extra
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wshadow")
    #And more extra
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wstrict-aliasing -Wstrict-overflow")
    #And even more
    check_cxx_compiler_flag (-pedantic HAVE_PEDANTIC_FLAG)
    if(HAVE_PEDANTIC_FLAG)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
    endif(HAVE_PEDANTIC_FLAG)
    set(CMAKE_VERBOSE_MAKEFILE ON)
    message(STATUS
        "Build ${PROJECT_NAME} in development mode (RELEASE=OFF) --\n")

    if(GIT_BINARY)
        set(GIT_EXECUTABLE ${GIT_BINARY})
    else(GIT_BINARY)
        find_package(Git)
        if(NOT GIT_FOUND)
            message(AUTHOR_WARNING
                "Git binary not found. Build version will "
                "be set to NULL. Install Git package or "
                "use -DGIT_BINARY to set path to git binary."
                )
        endif(NOT GIT_FOUND)
    endif(GIT_BINARY)
    message(STATUS "GIT_BINARY              : ${GIT_EXECUTABLE}")
    if(PYTHON_BINARY)
        set(PYTHON_EXECUTABLE ${GIT_PYTHON})
    else(PYTHON_BINARY)
        find_package(PythonInterp REQUIRED)
        if(NOT PYTHONINTERP_FOUND)
            message(ERROR
                "Python binary not found. Install Python package or "
                "use -DPYTHON_BINARY to set path to Python binary."
                )
        endif(NOT PYTHONINTERP_FOUND )
    endif(PYTHON_BINARY)
    message(STATUS "PYTHON_BINARY           : ${PYTHON_EXECUTABLE}")
endif(RELEASE)

# We gona use c++11 standart
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -MMD")

message(STATUS "CMAKE_CXX_FLAGS         : ${CMAKE_CXX_FLAGS}\n")
message(STATUS "DYNAMIC_LINKING         : ${DYNAMIC_LINKING}")
message(STATUS "RELEASE                 : ${RELEASE}\n")

message(STATUS "LANGUAGES               : ${LANGUAGES}\n")

message(STATUS "See INSTALL file for details and more info --\n")

# Force out-of-source build
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR
        "ERROR: This project requires an out of source build.\n"
        "Remove the file 'CMakeCache.txt' found in this "
        "directory before continuing, create a separate "
        "build directory and run 'cmake [options] <srcs> "
        "from there.\n"
        "See INSTALL file for details and more info\n"
        )
endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

# Search for required libraries
set(QT_MIN_VERSION "5.2.1")

find_package(Qt5Core REQUIRED)
find_package(Qt5LinguistTools REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5Test REQUIRED)

if(Qt5Core_FOUND)
    message(STATUS "Fount Qt Core version   : ${Qt5Core_VERSION}")
else(Qt5Core_FOUND)
    message(FATAL_ERROR
        "Qt Core libs not found. Please install Qt ${QT_MIN_VERSION}."
        )
endif(Qt5Core_FOUND)

# Add project subdirs
add_subdirectory(src)

# Unintall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY
    )

add_custom_target(uninstall
    "${CMAKE_COMMAND}"
    -P
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    )

# EOF
