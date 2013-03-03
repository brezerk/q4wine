cmake_minimum_required(VERSION 2.6)

PROJECT(q4wine)

SET(CMAKE_VERBOSE_MAKEFILE ON)

SET(APP_VERSION "1.0-r2")
SET(APP_NAME_GLOBAL "q4wine")

if(POLICY CMP0011)
    cmake_policy(SET CMP0011 NEW)
endif(POLICY CMP0011)

option(DEBUG "If ON, then q4wine will send debug messages to STDOUT. Use it for development or bug reportint purposes." "OFF")
option(RELEASE "If OFF, then force q4wine to use APP_BUILD as CMAKE_INSTALL_PREFIX. Use it for development purposes." "ON")
option(WITH_ICOUTILS "If ON, then build q4wine with icoutils support." "ON")
option(WITH_SYSTEM_SINGLEAPP "If ON, then use system QtSingleApplication library instead building one." "OFF")
option(WITH_WINEAPPDB "If ON, then build q4wine with Wine AppDB browser support." "ON")
option(USE_BZIP2 "Use bzip2 to compress man pages." "OFF")
option(USE_GZIP "Use gzip to compress man pages." "OFF")
option(WITH_DBUS "If ON, then build q4wine with QtDBus notification support." "ON")
option(LINGUAS "Language list" "en_us;ru_ru;uk_ua;pt_br;es_es;it_it;cs_cz;he_il;pl_pl;de_de;fa_ir")
option(LIBS_ENTRY_PATH "Destination directory for q4wine-core library")

MESSAGE("\n * Utility for Wine applications and prefixes management.")
MESSAGE(" * (C) 2008-2013, Alexey S. Malakhov")
MESSAGE(" * http://brezblock.org.ua/\n")

MESSAGE(STATUS "Q4Wine build environment -- \n")

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
             MESSAGE(FATAL ERROR "Qt Network libraries missing.")
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

         IF(WITH_DBUS)
             IF(QT_QTDBUS_FOUND)
                 SET(QT_USE_QTDBUS 1)
             ELSE(QT_QTDBUS_FOUND)
                 MESSAGE(FATAL ERROR "Qt D-Bus libraries missing.")
             ENDIF(QT_QTDBUS_FOUND)
         ENDIF(WITH_DBUS)

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

IF( ${CMAKE_SYSTEM_NAME} MATCHES Darwin )
    link_directories(/usr/lib)
    SET(_OS_DARWIN_ 1)
    ADD_DEFINITIONS(-Wall -g -DKERNEL)
    SET( PROGNAME q4wine )
    SET( MACOSX_BUNDLE_ICON_FILE "q4wine-128x128.icns" )
    SET( MACOSX_BUNDLE_INFO_STRING "Qt4 utility for Wine applications and prefixes management" )
    SET( MACOSX_BUNDLE_BUNDLE_NAME ${APP_NAME_GLOBAL})
    SET( MACOSX_BUNDLE_SHORT_VERSION_STRING ${APP_VERSION} )
    SET( MACOSX_BUNDLE_VERSION ${APP_VERSION} )
    SET( MACOSX_BUNDLE_LONG_VERSION_STRING Version ${APP_VERSION} )
    SET( MACOSX_BUNDLE_GUI_IDENTIFIER "http://q4wine.brezblock.org.ua/" )
    SET( MACOSX_BUNDLE_COPYRIGH "(C) 2008-2011, Alexey S. Malakhov" )
    SET( DESKTOP_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/${APP_NAME_GLOBAL}.app/Contents/Applications )
    SET( PIXMAPS_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/${APP_NAME_GLOBAL}.app/Contents/Resources )
    SET( DATA_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/${APP_NAME_GLOBAL}.app/Contents/Resources/icons )
    SET( THEMES_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/${APP_NAME_GLOBAL}.app/Contents/Resources)
    SET( MANPAGE_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/${APP_NAME_GLOBAL}.app/Contents/man/man1 )
    SET( LIBS_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/${APP_NAME_GLOBAL}.app/Contents/Frameworks )
    SET( APPS_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/${APP_NAME_GLOBAL}.app/Contents/MacOS )
    SET( I18N_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/${APP_NAME_GLOBAL}.app/Contents/i18n )

    SET( WITH_ICOUTILS OFF )
ELSE( ${CMAKE_SYSTEM_NAME} MATCHES Darwin )
    IF( NOT DATA_ENTRY_PATH )
        SET( DATA_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/share/q4wine/icons )
    ENDIF( NOT DATA_ENTRY_PATH )
    IF( NOT THEMES_ENTRY_PATH )
        SET( THEMES_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/share/q4wine )
    ENDIF( NOT THEMES_ENTRY_PATH )
    IF( NOT I18N_ENTRY_PATH )
        SET( I18N_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/share/q4wine/i18n )
    ENDIF( NOT I18N_ENTRY_PATH )
    IF( NOT APPS_ENTRY_PATH )
        SET( APPS_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/bin )
    ENDIF( NOT APPS_ENTRY_PATH )
    IF( NOT DESKTOP_ENTRY_PATH )
        SET( DESKTOP_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/share/applications )
    ENDIF( NOT DESKTOP_ENTRY_PATH )
    IF( NOT PIXMAPS_ENTRY_PATH )
        SET( PIXMAPS_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor )
    ENDIF( NOT PIXMAPS_ENTRY_PATH )
    IF( NOT MANPAGE_ENTRY_PATH )
        SET( MANPAGE_ENTRY_PATH ${CMAKE_INSTALL_PREFIX}/share/man )
    ENDIF( NOT MANPAGE_ENTRY_PATH )
    IF( NOT LIBS_ENTRY_PATH )
        IF (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES x86_64)
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
    SET ( LINGUAS en_us ru_ru uk_ua pt_br es_es it_it cs_cz he_il pl_pl de_de fa_ir)
ENDIF( NOT LINGUAS )

configure_file(${CMAKE_SOURCE_DIR}/config.h.cmake ${CMAKE_SOURCE_DIR}/config.h )

MESSAGE(STATUS "Q4Wine build options --\n")
file(APPEND ${CMAKE_SOURCE_DIR}/config.h "#define APP_ARCH \"${CMAKE_HOST_SYSTEM_PROCESSOR}\"\n" )
file(APPEND ${CMAKE_SOURCE_DIR}/config.h "#define APP_HOST \"${CMAKE_SYSTEM_NAME}\"\n" )

file(APPEND ${CMAKE_SOURCE_DIR}/config.h "#define APP_VERS \"${APP_VERSION}\"\n")
    MESSAGE(STATUS "Version                       : ${APP_VERSION}")
file(APPEND ${CMAKE_SOURCE_DIR}/config.h "#define APP_PREF \"${CMAKE_INSTALL_PREFIX}\"\n" )

IF( ${CMAKE_SYSTEM_NAME} MATCHES Darwin )
    file(APPEND ${CMAKE_SOURCE_DIR}/config.h "#define _CORELIB_PATH_ \"q4wine.app/Contents/Frameworks/libq4wine-core\"\n" )
ELSE( ${CMAKE_SYSTEM_NAME} MATCHES Darwin )
    file(APPEND ${CMAKE_SOURCE_DIR}/config.h "#define _CORELIB_PATH_ \"libq4wine-core\"\n" )
ENDIF( ${CMAKE_SYSTEM_NAME} MATCHES Darwin )

IF( RELEASE )
    MESSAGE(STATUS "CMAKE_INSTALL_PREFIX          : ${CMAKE_INSTALL_PREFIX}")
    MESSAGE(STATUS "DESKTOP_ENTRY_PATH            : ${DESKTOP_ENTRY_PATH}")
    MESSAGE(STATUS "PIXMAPS_ENTRY_PATH            : ${PIXMAPS_ENTRY_PATH}")
    MESSAGE(STATUS "MANPAGE_ENTRY_PATH            : ${MANPAGE_ENTRY_PATH}")
    MESSAGE(STATUS "LIBS_ENTRY_PATH               : ${LIBS_ENTRY_PATH}\n")
ELSE ( RELEASE )
    MESSAGE("\n")
    MESSAGE(STATUS "Build Q4Wine in development mode (RELEASE=OFF) --\n")
ENDIF( RELEASE )
file(APPEND ${CMAKE_SOURCE_DIR}/config.h "\n#endif\n" )

    MESSAGE(STATUS "DEBUG                         : ${DEBUG}")
    MESSAGE(STATUS "RELEASE                       : ${RELEASE}")
IF( NOT RELEASE )
    file(APPEND ${CMAKE_SOURCE_DIR}/config.h "#define APP_BUILD \"${CMAKE_CURRENT_BINARY_DIR}\"\n" )
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
    MESSAGE(ERROE "    ERROR: Embedded fuseiso is not supported any more, plese remove WITH_EMBEDDED_FUSEISO flag")
    MESSAGE(ERROR "    ERROR: and use native fuseiso instead!\n")
    MESSAGE(ERROR "    ERROR: fuseiso package avalible here: http://fuseiso.sourceforge.net/ \n")
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
)

IF (${WITH_SYSTEM_SINGLEAPP} MATCHES ON )
    MESSAGE(STATUS "Searching for QtSingleApplication package (WITH_SYSTEM_SINGLEAPP=ON) --\n")
    find_path(QTSINGLEAPPLIB_INCLUDE_DIR QtSingleApplication PATH_SUFFIXES QtSolutions)
    find_library(QTSINGLEAPPLIB QtSolutions_SingleApplication-2.6)

    IF (QTSINGLEAPPLIB_INCLUDE_DIR AND QTSINGLEAPPLIB )
        MESSAGE(STATUS "     library : ${QTSINGLEAPPLIB}")
        MESSAGE(STATUS " include dir : ${QTSINGLEAPPLIB_INCLUDE_DIR}")
        SET ( Q4WINE_HEADERS_DIR
            ${Q4WINE_HEADERS_DIR}
            ${QTSINGLEAPPLIB_INCLUDE_DIR}
        )
    ELSE (QTSINGLEAPPLIB_INCLUDE_DIR AND QTSINGLEAPPLIB )
        MESSAGE(STATUS "     library : NOT FOUND!\n")
        MESSAGE(FATAL_ERROR "Please install SingleApplication Library or disable system qtsingleapp support by adding -DWITH_SYSTEM_SINGLEAPP="OFF" option to cmake")
    ENDIF (QTSINGLEAPPLIB_INCLUDE_DIR AND QTSINGLEAPPLIB )
    MESSAGE("\n")
ELSE (${WITH_SYSTEM_SINGLEAPP} MATCHES ON )
    SET ( Q4WINE_HEADERS_DIR
        ${Q4WINE_HEADERS_DIR}
        ${CMAKE_SOURCE_DIR}/src/qtsingleapplication
    )
    add_subdirectory(qtsingleapplication)
    SET (QTSINGLEAPPLIB "qtsingleapp")
ENDIF (${WITH_SYSTEM_SINGLEAPP} MATCHES ON )

INCLUDE_DIRECTORIES( ${Q4WINE_HEADERS_DIR} ${CMAKE_BINARY_DIR} )

add_subdirectory(q4wine-lib)
add_subdirectory(q4wine-cli)

include(plugins/CMakeLists.txt)

add_subdirectory(q4wine-gui)
add_subdirectory(q4wine-helper)
add_subdirectory(data)
add_subdirectory(theme)
add_subdirectory(icons)
add_subdirectory(man1)
add_subdirectory(i18n)

IF (${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)
    add_subdirectory(freebsd)
ENDIF (${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)
