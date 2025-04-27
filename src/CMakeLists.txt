cmake_minimum_required(VERSION 3.28)

PROJECT(q4wine)

SET(CMAKE_VERBOSE_MAKEFILE ON)

SET(APP_VERSION "1.4.2")
SET(APP_NAME_GLOBAL "q4wine")

if(POLICY CMP0011)
    cmake_policy(SET CMP0011 NEW)
endif(POLICY CMP0011)

option(QT6 "If ON, then q4wine will be built with Qt 6.x." "ON")
option(DEBUG "If ON, then q4wine will send debug messages to STDOUT. Use it for development or bug reportint purposes." "OFF")
option(RELEASE "If OFF, then force q4wine to use APP_BUILD as CMAKE_INSTALL_PREFIX. Use it for development purposes." "ON")
option(WITH_ICOUTILS "If ON, then build q4wine with icoutils support." "ON")
option(WITH_SYSTEM_SINGLEAPP "If ON, then use system SingleApplication library instead building one." "OFF")
option(WITH_WINEAPPDB "If ON, then build q4wine with Wine AppDB browser support (OBSOLETE)." "OFF")
option(USE_BZIP2 "Use bzip2 to compress man pages." "OFF")
option(USE_GZIP "Use gzip to compress man pages." "OFF")
option(WITH_DBUS "If ON, then build q4wine with QtDBus notification support." "ON")
set(LINGUAS "" CACHE STRING "af cs de en es fa fr he it ja pl pt_BR uk zh_TW zh_CN")
option(LIBS_ENTRY_PATH "Destination directory for q4wine-core library")

message(STATUS "\n * Utility for Wine applications and prefixes management.")
message(STATUS " * Copyright (C) 2008-2021 by Oleksii S. Malakhov <brezerk@gmail.com>")
message(STATUS " * https://brezblock.org.ua/\n")

message(STATUS "Q4Wine build environment -- \n")

set(QT_MIN_VERSION "6.2.1")
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS LinguistTools)
find_package(Qt6 REQUIRED COMPONENTS Network)
find_package(Qt6 REQUIRED COMPONENTS Sql)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Xml)
find_package(Qt6 REQUIRED COMPONENTS Svg)

if (Qt6Core_FOUND)
    message(STATUS "\n-- Found Qt Core version : ${Qt6Core_VERSION}")
else (Qt6Core_FOUND)
    message(FATAL_ERROR "Qt Core libs not found. Please install Qt ${QT_MIN_VERSION}.")
endif (Qt6Core_FOUND)
if (WITH_DBUS MATCHES ON)
    find_package(Qt6DBus REQUIRED)
    if (NOT Qt6DBus_FOUND)
        message(FATAL_ERROR "Qt DBus libraries missing. Please install Qt DBus ${QT_MIN_VERSION} or set WITH_DBUS to OFF")
    endif (NOT Qt6DBus_FOUND)
endif (WITH_DBUS MATCHES ON)

MESSAGE(STATUS "Build realm is   : ${CMAKE_SYSTEM_NAME} ${CMAKE_HOST_SYSTEM_PROCESSOR}\n")

qt_standard_project_setup()

IF (${CMAKE_SYSTEM_NAME} MATCHES Linux)
    SET(_OS_LINUX_ 1)
    ADD_DEFINITIONS(-Wall -g -std=c++17)
ENDIF (${CMAKE_SYSTEM_NAME} MATCHES Linux)

IF (${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)
    SET(_OS_FREEBSD_ 1)
    ADD_DEFINITIONS(-Wall -g -DKERNEL -std=c++17)
ENDIF (${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)

IF( ${CMAKE_SYSTEM_NAME} MATCHES Darwin )
    link_directories(/usr/lib)
    SET(_OS_DARWIN_ 1)
    ADD_DEFINITIONS(-Wall -g -DKERNEL -std=c++17)
    SET( PROGNAME q4wine )
    SET( MACOSX_BUNDLE_ICON_FILE "q4wine-128x128.icns" )
    SET( MACOSX_BUNDLE_INFO_STRING "Qt utility for Wine applications and prefixes management" )
    SET( MACOSX_BUNDLE_BUNDLE_NAME ${APP_NAME_GLOBAL})
    SET( MACOSX_BUNDLE_SHORT_VERSION_STRING ${APP_VERSION} )
    SET( MACOSX_BUNDLE_VERSION ${APP_VERSION} )
    SET( MACOSX_BUNDLE_LONG_VERSION_STRING Version ${APP_VERSION} )
    SET( MACOSX_BUNDLE_GUI_IDENTIFIER "https://q4wine.brezblock.org.ua/" )
    SET( MACOSX_BUNDLE_COPYRIGH "Copyright (C) 2008-2021 by Oleksii S. Malakhov <brezerk@gmail.com>" )
    SET( DESKTOP_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/${APP_NAME_GLOBAL}.app/Contents/Applications )
    SET( PIXMAPS_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/${APP_NAME_GLOBAL}.app/Contents/Resources )
    SET( DATA_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/${APP_NAME_GLOBAL}.app/Contents/Resources/icons )
    SET( THEMES_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/${APP_NAME_GLOBAL}.app/Contents/Resources)
    SET( MANPAGE_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/${APP_NAME_GLOBAL}.app/Contents/man/man1 )
    SET( LIBS_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/${APP_NAME_GLOBAL}.app/Contents/Frameworks )
    SET( APPS_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/${APP_NAME_GLOBAL}.app/Contents/MacOS )
    SET( L10N_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/${APP_NAME_GLOBAL}.app/Contents/l10n )

    SET( WITH_ICOUTILS OFF )
ELSE( ${CMAKE_SYSTEM_NAME} MATCHES Darwin )
    IF( NOT DATA_ENTRY_PATH )
        SET( DATA_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/share/q4wine/icons )
    ENDIF( NOT DATA_ENTRY_PATH )
    IF( NOT THEMES_ENTRY_PATH )
        SET( THEMES_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/share/q4wine )
    ENDIF( NOT THEMES_ENTRY_PATH )
    IF( I18N_ENTRY_PATH )
        MESSAGE(WARNING "-DI18N_ENTRY_PATH is obsolete. Use -DL10N_ENTRY_PATH instead.")
        SET( L10N_ENTRY_PATH ${I18N_ENTRY_PATH} )
    ENDIF( I18N_ENTRY_PATH )
    IF( NOT L10N_ENTRY_PATH )
        SET( L10N_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/share/q4wine/l10n )
    ENDIF( NOT L10N_ENTRY_PATH )
    IF( NOT APPS_ENTRY_PATH )
        SET( APPS_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/bin )
    ENDIF( NOT APPS_ENTRY_PATH )
    IF( NOT DESKTOP_ENTRY_PATH )
        SET( DESKTOP_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/share/applications )
    ENDIF( NOT DESKTOP_ENTRY_PATH )
    IF( NOT APPDATA_ENTRY_PATH )
        SET( APPDATA_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/share/metainfo )
    ENDIF( NOT APPDATA_ENTRY_PATH )
    IF( NOT PIXMAPS_ENTRY_PATH )
        SET( PIXMAPS_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor )
    ENDIF( NOT PIXMAPS_ENTRY_PATH )
    IF( NOT PIXMAPS_UNITY_ENTRY_PATH )
        SET( PIXMAPS_UNITY_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/share/icons/ubuntu-mono-dark )
    ENDIF( NOT PIXMAPS_UNITY_ENTRY_PATH )
    IF( NOT MANPAGE_ENTRY_PATH )
        SET( MANPAGE_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/share/man )
    ENDIF( NOT MANPAGE_ENTRY_PATH )
    IF( NOT LIBS_ENTRY_PATH )
        IF (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES x86_64)
            SET( LIBS_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/lib64/q4wine)
        ELSEIF (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES aarch64)
            SET( LIBS_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/lib64/q4wine)
        ELSE (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES x86_64)
            SET( LIBS_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/lib/q4wine)
        ENDIF (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES x86_64)
    ENDIF( NOT LIBS_ENTRY_PATH )
ENDIF( ${CMAKE_SYSTEM_NAME} MATCHES Darwin )

# RPATH fixes
# skip the full RPATH for the build tree
#SET(CMAKE_SKIP_BUILD_RPATH  TRUE)
# when building, use the install RPATH already
# (so it doesn't need to relink when installing)
#SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

# the RPATH to be used when installing
# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
#SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

IF( NOT ${CMAKE_SYSTEM_NAME} MATCHES Darwin )
    IF( NOT DEFINED SKIP_RPATH )
        SET(CMAKE_INSTALL_RPATH "${LIBS_ENTRY_PATH}")
    ELSE( NOT DEFINED SKIP_RPATH )
        MESSAGE(STATUS "NOTE: Skip for RPATH build!")
        SET(CMAKE_SKIP_BUILD_RPATH  TRUE)
    ENDIF( NOT DEFINED SKIP_RPATH )
ENDIF( NOT ${CMAKE_SYSTEM_NAME} MATCHES Darwin )

IF( NOT LINGUAS )
    SET ( LINGUAS en af cs de es fa fr he it ja pl pt_BR uk zh_TW zh_CN nl )
ENDIF( NOT LINGUAS )

SET (TS_FILE_DIR "${CMAKE_SOURCE_DIR}/l10n")
SET (TS_FILE_BASE "q4wine")

configure_file(${CMAKE_SOURCE_DIR}/config.h.cmake ${PROJECT_BINARY_DIR}/config.h )

MESSAGE(STATUS "Q4Wine build options --\n")
file(APPEND ${PROJECT_BINARY_DIR}/config.h "#define APP_ARCH \"${CMAKE_HOST_SYSTEM_PROCESSOR}\"\n" )
file(APPEND ${PROJECT_BINARY_DIR}/config.h "#define APP_HOST \"${CMAKE_SYSTEM_NAME}\"\n" )

file(APPEND ${PROJECT_BINARY_DIR}/config.h "#define APP_VERS \"${APP_VERSION}\"\n")
    MESSAGE(STATUS "Version                       : ${APP_VERSION}")
file(APPEND ${PROJECT_BINARY_DIR}/config.h "#define APP_PREF \"${CMAKE_INSTALL_PREFIX}\"\n" )

IF( ${CMAKE_SYSTEM_NAME} MATCHES Darwin )
    file(APPEND ${PROJECT_BINARY_DIR}/config.h "#define _CORELIB_PATH_ \"q4wine.app/Contents/Frameworks/libq4wine-core\"\n" )
ELSE( ${CMAKE_SYSTEM_NAME} MATCHES Darwin )
    file(APPEND ${PROJECT_BINARY_DIR}/config.h "#define _CORELIB_PATH_ \"libq4wine-core\"\n" )
ENDIF( ${CMAKE_SYSTEM_NAME} MATCHES Darwin )

IF( RELEASE )
    MESSAGE(STATUS "CMAKE_INSTALL_PREFIX          : ${CMAKE_INSTALL_PREFIX}")
    MESSAGE(STATUS "DESKTOP_ENTRY_PATH            : ${DESKTOP_ENTRY_PATH}")
    MESSAGE(STATUS "APPDATA_ENTRY_PATH            : ${APPDATA_ENTRY_PATH}")
    MESSAGE(STATUS "PIXMAPS_ENTRY_PATH            : ${PIXMAPS_ENTRY_PATH}")
    MESSAGE(STATUS "PIXMAPS_UNITY_ENTRY_PATH      : ${PIXMAPS_UNITY_ENTRY_PATH}")
    MESSAGE(STATUS "MANPAGE_ENTRY_PATH            : ${MANPAGE_ENTRY_PATH}")
    MESSAGE(STATUS "LIBS_ENTRY_PATH               : ${LIBS_ENTRY_PATH}\n")
ELSE ( RELEASE )
    MESSAGE(STATUS "\n")
    MESSAGE(STATUS "Build Q4Wine in development mode (RELEASE=OFF) --\n")
ENDIF( RELEASE )
file(APPEND ${PROJECT_BINARY_DIR}/config.h "\n#endif\n" )

    MESSAGE(STATUS "DEBUG                         : ${DEBUG}")
    MESSAGE(STATUS "RELEASE                       : ${RELEASE}")
IF( NOT RELEASE )
    file(APPEND ${PROJECT_BINARY_DIR}/config.h "#define APP_BUILD \"${CMAKE_CURRENT_BINARY_DIR}\"\n" )
ENDIF ( NOT RELEASE )
    MESSAGE(STATUS "WITH_ICOUTILS                 : ${WITH_ICOUTILS}")
    MESSAGE(STATUS "WITH_SYSTEM_SINGLEAPP         : ${WITH_SYSTEM_SINGLEAPP}")
    MESSAGE(STATUS "WITH_WINEAPPDB                : ${WITH_WINEAPPDB}")
    MESSAGE(STATUS "USE_BZIP2                     : ${USE_BZIP2}")
    MESSAGE(STATUS "USE_GZIP                      : ${USE_GZIP}")
    MESSAGE(STATUS "WITH_DBUS                     : ${WITH_DBUS}\n")

    MESSAGE(STATUS "LINGUAS                       : ${LINGUAS}\n")

MESSAGE(STATUS "See INSTALL file for details and more info --\n")

IF( NOT ${CMAKE_SYSTEM_NAME} MATCHES Darwin )
    find_program( FUSEISO_BIN NAMES fuseiso )
    MESSAGE(STATUS "Searching for fuseiso binary --\n")
    IF (${FUSEISO_BIN} MATCHES FUSEISO_BIN-NOTFOUND)
        MESSAGE(STATUS "    fuseiso : NOT FOUND!\n")
        MESSAGE(STATUS "    WARNING: To have ability to mount and umount ISO9660 images")
        MESSAGE(STATUS "    WARNING: without root privileges you may wish to install")
        MESSAGE(STATUS "    WARNING: fuseiso package ( http://fuseiso.sourceforge.net/ )\n")
    ELSE (${FUSEISO_BIN} MATCHES FUSEISO_BIN-NOTFOUND)
        MESSAGE(STATUS "    fuseiso : ${FUSEISO_BIN}\n")
    ENDIF (${FUSEISO_BIN} MATCHES FUSEISO_BIN-NOTFOUND)
ENDIF( NOT ${CMAKE_SYSTEM_NAME} MATCHES Darwin )

IF (WITH_ICOUTILS MATCHES ON)
    find_program( WRESTOOL NAMES wrestool )
    find_program( ICOTOOL NAMES icotool )
    MESSAGE(STATUS "Searching for icoutils binaries (WITH_ICOUTILS=ON) --\n")
    IF (${WRESTOOL} MATCHES WRESTOOL-NOTFOUND)
        MESSAGE(STATUS "    wrestool : NOT FOUND!")
        MESSAGE(FATAL_ERROR "Please install icoutils package or disable icoutils support by adding -DWITH_ICOUTILS=\"OFF\" option to cmake")
    ELSE (${WRESTOOL} MATCHES WRESTOOL-NOTFOUND)
        MESSAGE(STATUS "    wrestool : ${WRESTOOL}")
    ENDIF (${WRESTOOL} MATCHES WRESTOOL-NOTFOUND)

    IF (${ICOTOOL} MATCHES ICOTOOL-NOTFOUND)
        MESSAGE(STATUS "    icotool  : NOT FOUND!\n")
        MESSAGE(FATAL_ERROR "Please install icoutils package or disable icoutils support by adding -DWITH_ICOUTILS=\"OFF\" option to cmake")
    ELSE (${ICOTOOL} MATCHES ICOTOOL-NOTFOUND)
        MESSAGE(STATUS "    icotool  : ${ICOTOOL}\n")
    ENDIF (${ICOTOOL} MATCHES ICOTOOL-NOTFOUND)
ENDIF (WITH_ICOUTILS MATCHES ON)

IF (${WITH_EMBEDDED_FUSEISO} MATCHES ON )
    MESSAGE(FATAL_ERROR "    ERROR: Embedded fuseiso is not supported any more, plese remove WITH_EMBEDDED_FUSEISO flag")
    MESSAGE(FATAL_ERROR "    ERROR: and use native fuseiso instead!\n")
    MESSAGE(FATAL_ERROR "    ERROR: fuseiso package available here: https://sourceforge.net/projects/fuseiso/ \n")
ENDIF (${WITH_EMBEDDED_FUSEISO} MATCHES ON )

SET ( Q4WINE_HEADERS_DIR
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/core/database
    ${CMAKE_SOURCE_DIR}/src/q4wine-cli
    ${CMAKE_SOURCE_DIR}/src/q4wine-lib
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/appdb
)

IF (${WITH_SYSTEM_SINGLEAPP} MATCHES ON )
    MESSAGE(STATUS "Searching for SingleApplication package (WITH_SYSTEM_SINGLEAPP=ON) --\n")
    find_package(SingleApplication REQUIRED)
ELSE (${WITH_SYSTEM_SINGLEAPP} MATCHES ON )
    MESSAGE(STATUS "Building embedded third-party SingleApplication package (WITH_SYSTEM_SINGLEAPP=OFF) --\n")
    SET(QAPPLICATION_CLASS QApplication CACHE STRING "Inheritance class for SingleApplication")
    add_subdirectory(third-party/SingleApplication-3.5.2)
    SET ( Q4WINE_HEADERS_DIR
        ${Q4WINE_HEADERS_DIR}
        ${CMAKE_SOURCE_DIR}/src/third-party/SingleApplication-3.5.2
    )
ENDIF (${WITH_SYSTEM_SINGLEAPP} MATCHES ON )

INCLUDE_DIRECTORIES( ${Q4WINE_HEADERS_DIR} ${CMAKE_BINARY_DIR} ${PROJECT_BINARY_DIR} )


add_subdirectory(q4wine-lib)
add_subdirectory(q4wine-cli)
add_subdirectory(q4wine-gui)
add_subdirectory(q4wine-helper)
add_subdirectory(l10n)
add_subdirectory(data)
add_subdirectory(theme)
add_subdirectory(icons)
add_subdirectory(man1)


IF (${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)
    add_subdirectory(freebsd)
ENDIF (${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)

add_custom_target(cppcheck COMMAND cppcheck
    --quiet
    --force
    --inconclusive
    --error-exitcode=254
    -I ${CMAKE_SOURCE_DIR}
    -I ${CMAKE_SOURCE_DIR}/src/q4wine-gui
    -I ${CMAKE_SOURCE_DIR}/src/q4wine-cli
    -I ${CMAKE_SOURCE_DIR}/src/q4wine-helper
    -I ${CMAKE_SOURCE_DIR}/src/q4wine-lib
    -i qrc_q4wine.cpp
    -i ${CMAKE_SOURCE_DIR}/build/
    ${CMAKE_SOURCE_DIR}
    )
