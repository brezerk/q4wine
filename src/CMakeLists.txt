cmake_minimum_required(VERSION 2.6)

PROJECT(q4wine)

SET(CMAKE_VERBOSE_MAKEFILE ON)

SET(APP_VERSION "0.120")

if(POLICY CMP0011)
  cmake_policy(SET CMP0011 NEW)
endif(POLICY CMP0011)

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

option(DEBUG "If ON, then build q4wine with icoutils support use it in debug or bug report cases" "OFF")
option(WITH_ICOUTILS "If ON, then build q4wine with icoutils support" "ON")
option(WITH_EMBEDDED_FUSEISO "If ON, then build embedded fuseiso (q4wine-mount) binary" "OFF")
option(WITH_WINEAPPDB "If ON, then build q4wine with wine appdb browser support" "ON")
option(USE_BZIP2 "Use bzip2 to compress man pages" "OFF")
option(USE_GZIP "Use gzip to compress man pages" "OFF")

MESSAGE("\n * Utility for management wine applications and prefixes")
MESSAGE(" * (C) 2008-2010, brezblock core team")
MESSAGE(" * http://brezblock.org.ua/\n")

MESSAGE(STATUS "Q4Wine build enveropment -- \n")

SET(QT_MIN_VERSION "4.4.1")
FIND_PACKAGE(Qt4 REQUIRED)
      IF(QT_FOUND)
            MESSAGE("\n-- Found Qt version : ${QTVERSION}")

            IF(QT_QTSQL_FOUND)
                SET(QT_USE_QTSQL 1)
            ELSE(QT_QTSQL_FOUND)
                MESSAGE(FATAL ERROR "Qt SQL libraries missing.")
            ENDIF(QT_QTSQL_FOUND)

            IF(QT_QTGUI_FOUND)
                SET(QT_USE_QTGUI 1)
            ELSE(QT_QTGUI_FOUND)
                MESSAGE(FATAL ERROR "Qt GUI libraries missing.")
            ENDIF(QT_QTGUI_FOUND)

            IF(QT_QTNETWORK_FOUND)
                SET(QT_USE_QTNETWORK 1)
            ELSE(QT_QTNETWORK_FOUND)
                MESSAGE(FATAL ERROR "Qt NetWork libraries missing.")
            ENDIF(QT_QTNETWORK_FOUND)

            IF(QT_QTXML_FOUND)
                SET(QT_USE_QTXML 1)
            ELSE(QT_QTXML_FOUND)
                MESSAGE(FATAL ERROR "Qt XML libraries missing.")
            ENDIF(QT_QTXML_FOUND)

            IF(QT_QTCORE_FOUND)
                SET(QT_USE_QTCORE 1)
            ELSE(QT_QTCORE_FOUND)
                MESSAGE(FATAL ERROR "Qt Core libraries missing.")
            ENDIF(QT_QTCORE_FOUND)

            INCLUDE(${QT_USE_FILE})
      ELSE(QT_FOUND)
            MESSAGE(FATAL ERROR "Qt4 not found.")
      ENDIF(QT_FOUND)

MESSAGE(STATUS "Build realm is   : ${CMAKE_SYSTEM_NAME} ${CMAKE_HOST_SYSTEM_PROCESSOR}\n")

IF (${CMAKE_SYSTEM_NAME} MATCHES Linux)
    SET(_OS_LINUX_ 1)
    ADD_DEFINITIONS(-Wall -g)
ENDIF (${CMAKE_SYSTEM_NAME} MATCHES Linux)

IF (${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)
      SET(_OS_FREEBSD_ 1)
    ADD_DEFINITIONS(-Wall -g -DKERNEL)
ENDIF (${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)

IF (${CMAKE_SYSTEM_NAME} MATCHES Darwin)
    MESSAGE("")
    MESSAGE(" Warning: Darwin was not tested, and not supported!")
        MESSAGE(" But you can mail test and bugs to us :]")
    MESSAGE("")
          SET(_OS_DARWIN_ 1)
    ADD_DEFINITIONS(-Wall -g -DKERNEL)
ENDIF (${CMAKE_SYSTEM_NAME} MATCHES Darwin)

IF (NOT DEFINED SKIP_RPATH)
    IF (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES x86_64)
        SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib64/q4wine")
    ELSE (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES x86_64)
        SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib/q4wine")
    ENDIF (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES x86_64)
ELSE (NOT DEFINED SKIP_RPATH)
    MESSAGE(STATUS "NOTE: Skip for RPATH build!")
ENDIF (NOT DEFINED SKIP_RPATH)

IF( NOT DESKTOP_ENTRY_PATH )
        SET( DESKTOP_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/share/applications )
ENDIF( NOT DESKTOP_ENTRY_PATH )
IF( NOT PIXMAPS_ENTRY_PATH )
        SET( PIXMAPS_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/share/pixmaps )
ENDIF( NOT PIXMAPS_ENTRY_PATH )
IF( NOT MANPAGE_ENTRY_PATH )
        SET( MANPAGE_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/share/man )
ENDIF( NOT MANPAGE_ENTRY_PATH )

configure_file(${CMAKE_SOURCE_DIR}/config.h.cmake ${CMAKE_SOURCE_DIR}/config.h )

MESSAGE(STATUS "Q4Wine build options --\n")
file(APPEND ${CMAKE_SOURCE_DIR}/config.h "#define APP_ARCH \"${CMAKE_HOST_SYSTEM_PROCESSOR}\"\n" )
file(APPEND ${CMAKE_SOURCE_DIR}/config.h "#define APP_HOST \"${CMAKE_SYSTEM_NAME}\"\n" )

file(APPEND ${CMAKE_SOURCE_DIR}/config.h "#define APP_VERS \"${APP_VERSION}\"\n")
      MESSAGE(STATUS "Version                       : ${APP_VERSION}")
file(APPEND ${CMAKE_SOURCE_DIR}/config.h "#define APP_PREF \"${CMAKE_INSTALL_PREFIX}\"\n" )
    MESSAGE(STATUS "CMAKE_INSTALL_PREFIX          : ${CMAKE_INSTALL_PREFIX}")
    MESSAGE(STATUS "DESKTOP_ENTRY_PATH            : ${DESKTOP_ENTRY_PATH}")
        MESSAGE(STATUS "PIXMAPS_ENTRY_PATH            : ${PIXMAPS_ENTRY_PATH}")
        MESSAGE(STATUS "MANPAGE_ENTRY_PATH            : ${MANPAGE_ENTRY_PATH}\n")
file(APPEND ${CMAKE_SOURCE_DIR}/config.h "\n#endif\n" )

    MESSAGE(STATUS "DEBUG                         : ${DEBUG}")
    MESSAGE(STATUS "WITH_ICOUTILS                 : ${WITH_ICOUTILS}")
    MESSAGE(STATUS "WITH_EMBEDDED_FUSEISO         : ${WITH_EMBEDDED_FUSEISO}")
    MESSAGE(STATUS "WITH_WINEAPPDB                : ${WITH_WINEAPPDB}")
    MESSAGE(STATUS "USE_BZIP2                     : ${USE_BZIP2}")
    MESSAGE(STATUS "USE_GZIP                      : ${USE_GZIP}")

MESSAGE(STATUS "See INSTALL file for details and more info --\n")

find_program( FUSEISO_BIN NAMES fuseiso )
MESSAGE(STATUS "Searching for fuseiso binary --\n")
IF (${FUSEISO_BIN} MATCHES FUSEISO_BIN-NOTFOUND)
    MESSAGE(STATUS "    fuseiso : NOT FOUND!\n")
    MESSAGE(STATUS "    WARNING: To have ability to mount and umount ISO9660 images")
    MESSAGE(STATUS "    WARNING: without root privileges you may wish to install")
    MESSAGE(STATUS "    WARNING: fuseiso package ( http://fuseiso.sourceforge.net/ )\n")
ELSE (${FUSEISO_BIN} MATCHES FUSEISO_BIN-NOTFOUND)
    MESSAGE(STATUS "    fuseiso : ${FUSEISO_BIN}\n")
    IF (${WITH_EMBEDDED_FUSEISO} MATCHES ON )
                MESSAGE(STATUS "    WARNING: You already have fuseiso package installed in you system!")
        MESSAGE(STATUS "    WARNING: It is strongly recommended to disable WITH_EMBEDDED_FUSEISO flag")
        MESSAGE(STATUS "    WARNING: and use native fuseiso functions!\n")
    ENDIF (${WITH_EMBEDDED_FUSEISO} MATCHES ON )
ENDIF (${FUSEISO_BIN} MATCHES FUSEISO_BIN-NOTFOUND)

IF (WITH_ICOUTILS MATCHES ON)
    find_program( WRESTOOL NAMES wrestool )
    find_program( ICOTOOL NAMES icotool )
    MESSAGE(STATUS "Searching for icoutils binaries (WITH_ICOUTILS=ON) --\n")
    IF (${WRESTOOL} MATCHES WRESTOOL-NOTFOUND)
        MESSAGE(STATUS "    wrestool : NOT FOUND!")
        MESSAGE(FATAL_ERROR "Please install icoutils package or disable icoutils support by adding -DWITH_ICOUTILS="OFF" option to cmake")
    ELSE (${WRESTOOL} MATCHES WRESTOOL-NOTFOUND)
        MESSAGE(STATUS "    wrestool : ${WRESTOOL}")
    ENDIF (${WRESTOOL} MATCHES WRESTOOL-NOTFOUND)

    IF (${ICOTOOL} MATCHES ICOTOOL-NOTFOUND)
        MESSAGE(STATUS "    icotool  : NOT FOUND!\n")
        MESSAGE(FATAL_ERROR "Please install icoutils package or disable icoutils support by adding -DWITH_ICOUTILS="OFF" option to cmake")
    ELSE (${ICOTOOL} MATCHES ICOTOOL-NOTFOUND)
        MESSAGE(STATUS "    icotool  : ${ICOTOOL}\n")
    ENDIF (${ICOTOOL} MATCHES ICOTOOL-NOTFOUND)
ENDIF (WITH_ICOUTILS MATCHES ON)

IF (${WITH_EMBEDDED_FUSEISO} MATCHES ON )
        MESSAGE(STATUS "Searching for glib-2.0, fuse, libzip packages (WITH_EMBEDDED_FUSEISO=ON) --\n")
        FIND_PACKAGE(PkgConfig REQUIRED)

    pkg_search_module(GLIB_LIB REQUIRED glib-2.0)
    pkg_search_module(FUSE_LIB REQUIRED fuse)
    pkg_search_module(ZIP_LIB REQUIRED libzip)
    MESSAGE("\n")
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
    ${CMAKE_SOURCE_DIR}/src/plugins
    ${CMAKE_SOURCE_DIR}/src/qtsingleapplication
)

INCLUDE_DIRECTORIES( ${Q4WINE_HEADERS_DIR} ${CMAKE_BINARY_DIR} )

include(i18n/CMakeLists.txt)
add_subdirectory(q4wine-lib)
add_subdirectory(q4wine-cli)

IF (${WITH_EMBEDDED_FUSEISO} MATCHES ON )
    add_subdirectory(fuseiso/src)
ENDIF (${WITH_EMBEDDED_FUSEISO} MATCHES ON )

add_subdirectory(qtsingleapplication)

include(plugins/CMakeLists.txt)

add_subdirectory(q4wine-gui)
add_subdirectory(q4wine-helper)
add_subdirectory(data)
add_subdirectory(theme)
add_subdirectory(icons)
add_subdirectory(man1)

IF (${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)
    add_subdirectory(freebsd)
ENDIF (${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)
