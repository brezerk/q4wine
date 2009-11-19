cmake_minimum_required(VERSION 2.6)

PROJECT(q4wine)

SET(CMAKE_VERBOSE_MAKEFILE ON)
SET(APP_VERSION "0.114-r2")

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

SET( DESKTOP_ENTRY
	${CMAKE_SOURCE_DIR}/q4wine.desktop
)

SET( DESKTOP_PIXMAPS
	${CMAKE_SOURCE_DIR}/src/icons/q4wine.png
)

SET( ICON_PIXMAPS
	${CMAKE_SOURCE_DIR}/src/data/eject.png
	${CMAKE_SOURCE_DIR}/src/data/explorer.png
	${CMAKE_SOURCE_DIR}/src/data/exec_wine.png
	${CMAKE_SOURCE_DIR}/src/data/notepad.png
	${CMAKE_SOURCE_DIR}/src/data/regedit.png
	${CMAKE_SOURCE_DIR}/src/data/uninstaller.png
	${CMAKE_SOURCE_DIR}/src/data/winecfg.png
	${CMAKE_SOURCE_DIR}/src/data/wineconsole.png
)

SET( Q4WINE_GUI_SOURCES
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/run.cpp
	${CMAKE_SOURCE_DIR}/src/main.cpp
	${CMAKE_SOURCE_DIR}/src/core/database/db.cpp
	${CMAKE_SOURCE_DIR}/src/core/database/prefix.cpp
	${CMAKE_SOURCE_DIR}/src/core/database/dir.cpp
	${CMAKE_SOURCE_DIR}/src/core/database/icon.cpp
	${CMAKE_SOURCE_DIR}/src/core/database/last_run_icon.cpp
	${CMAKE_SOURCE_DIR}/src/core/database/image.cpp
	${CMAKE_SOURCE_DIR}/src/core/registry.cpp
	${CMAKE_SOURCE_DIR}/src/core/winebinlauncher.cpp
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/about.cpp
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/appsettings.cpp
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/iconsettings.cpp
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/iconsview.cpp
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/mainwindow.cpp
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/prefixsettings.cpp
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/process.cpp
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/wizard.cpp
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/imagemanager.cpp
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/winedrivedialog.cpp
#	${CMAKE_SOURCE_DIR}/src/wis/wisitem.cpp
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/draglistwidget.cpp
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/appdbsearchwidget.cpp
)

SET ( Q4WINE_HEADERS_DIR
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/src/q4wine-gui
	${CMAKE_SOURCE_DIR}/src/core
	${CMAKE_SOURCE_DIR}/src/temporary
	${CMAKE_SOURCE_DIR}/src/wis
	${CMAKE_SOURCE_DIR}/src/qtsingleapplication/
)


SET ( Q4WINE_GUI_HEADERS
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/run.h
	${CMAKE_SOURCE_DIR}/src/core/registry.h
	${CMAKE_SOURCE_DIR}/src/core/database/db.h
	${CMAKE_SOURCE_DIR}/src/core/database/prefix.h
	${CMAKE_SOURCE_DIR}/src/core/database/dir.h
	${CMAKE_SOURCE_DIR}/src/core/database/icon.h
	${CMAKE_SOURCE_DIR}/src/core/database/last_run_icon.h
	${CMAKE_SOURCE_DIR}/src/core/database/image.h
	${CMAKE_SOURCE_DIR}/src/core/winebinlauncher.h
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/about.h
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/appsettings.h
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/iconsettings.h
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/iconsview.h
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/mainwindow.h
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/prefixsettings.h
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/process.h
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/wizard.h
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/imagemanager.h
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/winedrivedialog.h
	${CMAKE_SOURCE_DIR}/src/wis/wisitem.h
	${CMAKE_SOURCE_DIR}/src/core/database/initdb.h
	${CMAKE_SOURCE_DIR}/config.h
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/draglistwidget.h
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/appdbsearchwidget.h
)

SET ( Q4WINE_GUI_MOC_HEADEDRS
	${CMAKE_SOURCE_DIR}/src/core/registry.h
	${CMAKE_SOURCE_DIR}/src/core/winebinlauncher.h
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/appsettings.h
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/iconsettings.h
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/iconsview.h
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/mainwindow.h
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/prefixsettings.h
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/process.h
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/about.h
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/run.h
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/wizard.h
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/imagemanager.h
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/winedrivedialog.h
	#${CMAKE_SOURCE_DIR}/src/wis/wisitem.h
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/draglistwidget.h
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/appdbsearchwidget.h
)

SET ( Q4WINE_GUI_UI_SOURCES
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/About.ui
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/AppSettings.ui
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/IconSettings.ui
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/IconsView.ui
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/MainWindow.ui
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/PrefixSettings.ui
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/Process.ui
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/Run.ui
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/Wizard.ui
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/IconSettings.ui
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/ImageManager.ui
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/WineDriveDialog.ui
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/AppDBSearchWidget.ui
)

IF (${WITH_WINETRIKS} MATCHES ON )
	SET( Q4WINE_GUI_SOURCES
		${Q4WINE_GUI_SOURCES}
		${CMAKE_SOURCE_DIR}/src/temporary/winetricks.cpp
	)

	SET ( Q4WINE_GUI_HEADERS
		${Q4WINE_GUI_HEADERS}
		${CMAKE_SOURCE_DIR}/src/temporary/winetricks.h
	)

	SET ( Q4WINE_GUI_MOC_HEADEDRS
		${Q4WINE_GUI_MOC_HEADEDRS}
		${CMAKE_SOURCE_DIR}/src/temporary/winetricks.h
	)

	SET ( Q4WINE_GUI_UI_SOURCES
		${Q4WINE_GUI_UI_SOURCES}
		${CMAKE_SOURCE_DIR}/src/temporary/winetricks.ui
	)
ENDIF (${WITH_WINETRIKS} MATCHES ON )

SET( Q4WINE_LIB_SOURCES
		${CMAKE_SOURCE_DIR}/src/q4wine-lib/main.cpp
		${CMAKE_SOURCE_DIR}/src/core/database/db.cpp
		${CMAKE_SOURCE_DIR}/src/core/database/prefix.cpp
		${CMAKE_SOURCE_DIR}/src/core/database/dir.cpp
		${CMAKE_SOURCE_DIR}/src/core/database/icon.cpp
		${CMAKE_SOURCE_DIR}/src/core/database/image.cpp
)

SET( Q4WINE_LIB_HEADERS
		${CMAKE_SOURCE_DIR}/src/q4wine-lib/main.h
		${CMAKE_SOURCE_DIR}/config.h
		${CMAKE_SOURCE_DIR}/src/core/database/db.h
		${CMAKE_SOURCE_DIR}/src/core/database/prefix.h
		${CMAKE_SOURCE_DIR}/src/core/database/dir.h
		${CMAKE_SOURCE_DIR}/src/core/database/icon.h
		${CMAKE_SOURCE_DIR}/src/core/database/image.h
)

SET ( Q4WINE_LIB_MOC_HEADERS
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/process.h
)

SET ( Q4WINE_LIB_UI_SOURCES
	${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/Process.ui
)


SET( QSINGLEAPP_LIB_SOURCES
	${CMAKE_SOURCE_DIR}/src/qtsingleapplication/qtsingleapplication.cpp
	${CMAKE_SOURCE_DIR}/src/qtsingleapplication/qtlocalpeer.cpp
)

SET( QSINGLEAPP_LIB_HEADERS
	${CMAKE_SOURCE_DIR}/src/qtsingleapplication/qtsingleapplication.h
	${CMAKE_SOURCE_DIR}/src/qtsingleapplication/qtlocalpeer.h
)

SET( QSINGLEAPP_LIB_MOC_HEADERS
	${CMAKE_SOURCE_DIR}/src/qtsingleapplication/qtsingleapplication.h
	${CMAKE_SOURCE_DIR}/src/qtsingleapplication/qtlocalpeer.h
)

SET( Q4WINE_CLI_SOURCES
		${CMAKE_SOURCE_DIR}/src/q4wine-cli/main.cpp
		${CMAKE_SOURCE_DIR}/src/core/database/db.cpp
		${CMAKE_SOURCE_DIR}/src/core/database/prefix.cpp
		${CMAKE_SOURCE_DIR}/src/core/database/dir.cpp
		${CMAKE_SOURCE_DIR}/src/core/database/icon.cpp
		${CMAKE_SOURCE_DIR}/src/core/database/image.cpp
		${CMAKE_SOURCE_DIR}/src/q4wine-gui/process.cpp
)

SET( Q4WINE_CLI_HEADERS
		${CMAKE_SOURCE_DIR}/src/q4wine-lib/main.h
		${CMAKE_SOURCE_DIR}/src/q4wine-cli/main.h
		${CMAKE_SOURCE_DIR}/config.h
		${CMAKE_SOURCE_DIR}/src/core/database/db.h
		${CMAKE_SOURCE_DIR}/src/core/database/prefix.h
		${CMAKE_SOURCE_DIR}/src/core/database/dir.h
		${CMAKE_SOURCE_DIR}/src/core/database/icon.h
		${CMAKE_SOURCE_DIR}/src/core/database/image.h
		${CMAKE_SOURCE_DIR}/src/q4wine-gui/process.h
)

SET ( Q4WINE_RESOURCE_SOURCES
	${CMAKE_SOURCE_DIR}/src/q4wine.qrc
)

SET ( Q4WINE_I18N_SOURCES
	${CMAKE_SOURCE_DIR}/src/i18n/en_us.ts
	${CMAKE_SOURCE_DIR}/src/i18n/ru_ru.ts
	${CMAKE_SOURCE_DIR}/src/i18n/uk_ua.ts
	${CMAKE_SOURCE_DIR}/src/i18n/pt_br.ts
	${CMAKE_SOURCE_DIR}/src/i18n/es_es.ts
	${CMAKE_SOURCE_DIR}/src/i18n/it_it.ts
)

SET ( Q4WINE_FUSEISO_SOURCES
	${CMAKE_SOURCE_DIR}/src/fuseiso/src/fuseiso.c
	${CMAKE_SOURCE_DIR}/src/fuseiso/src/isofs.c
)

SET ( Q4WINE_FUSEISO_HEADERS
	${CMAKE_SOURCE_DIR}/src/fuseiso/src/linux/iso_fs.h
	${CMAKE_SOURCE_DIR}/src/fuseiso/src/linux/rock.h
	${CMAKE_SOURCE_DIR}/src/fuseiso/src/isofs.h
)

MESSAGE("")
MESSAGE(STATUS " * Utility for management wine applications and prefixes")
MESSAGE(STATUS " * (C) 2008-2009, brezblock core team")
MESSAGE(STATUS " * http://brezblock.org.ua/")
MESSAGE("")

IF (${WITH_EMBEDDED_FUSEISO} MATCHES ON )
	FIND_PACKAGE(PkgConfig REQUIRED)

	pkg_search_module(GLIB_LIB REQUIRED glib-2.0)
	pkg_search_module(FUSE_LIB REQUIRED fuse)
	pkg_search_module(ZIP_LIB REQUIRED libzip)
ENDIF (${WITH_EMBEDDED_FUSEISO} MATCHES ON )


SET(QT_MIN_VERSION "4.4.1")
FIND_PACKAGE(Qt4 REQUIRED)
	  IF(QT_FOUND)
			MESSAGE(STATUS "Found Qt. Using version: ${QTVERSION}")
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

			IF(QT_QTCORE_FOUND)
				SET(QT_USE_QTCORE 1)
			ELSE(QT_QTCORE_FOUND)
				MESSAGE(FATAL ERROR "Qt Core libraries missing.")
			ENDIF(QT_QTCORE_FOUND)

			INCLUDE(${QT_USE_FILE})
	  ELSE(QT_FOUND)
			MESSAGE(FATAL ERROR "Qt4 not found.")
	  ENDIF(QT_FOUND)

MESSAGE(STATUS "Build realm is: ${CMAKE_SYSTEM_NAME}")

IF (${CMAKE_SYSTEM_NAME} MATCHES Linux)
	SET(_OS_LINUX_ 1)
	ADD_DEFINITIONS(-Wall -g)
	IF( NOT DESKTOP_ENTRY_PATH )
		SET( DESKTOP_ENTRY_PATH /usr/share/applications/ )
	ENDIF( NOT DESKTOP_ENTRY_PATH )
	IF( NOT PIXMAPS_ENTRY_PATH )
		SET( PIXMAPS_ENTRY_PATH /usr/share/pixmaps/ )
	ENDIF( NOT PIXMAPS_ENTRY_PATH )
ENDIF (${CMAKE_SYSTEM_NAME} MATCHES Linux)

IF (${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)
	  SET(_OS_FREEBSD_ 1)
	ADD_DEFINITIONS(-Wall -g -DKERNEL)
	IF( NOT DESKTOP_ENTRY_PATH )
		SET( DESKTOP_ENTRY_PATH /usr/local/share/applications/ )
	ENDIF( NOT DESKTOP_ENTRY_PATH )
	IF( NOT PIXMAPS_ENTRY_PATH )
		SET( PIXMAPS_ENTRY_PATH /usr/local/share/pixmaps/ )
	ENDIF( NOT PIXMAPS_ENTRY_PATH )
ENDIF (${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)

IF (${CMAKE_SYSTEM_NAME} MATCHES Darwin)
	MESSAGE("")
	MESSAGE(" Warning: Darwin was not tested, and not supported!")
	MESSAGE(" You can mail test nad bugs to us")
	MESSAGE("")
	  SET(_OS_DARWIN_ 1)
	ADD_DEFINITIONS(-Wall -g -DKERNEL)
ENDIF (${CMAKE_SYSTEM_NAME} MATCHES Darwin)

IF (NOT DEFINED WITH_ICOUTILS)
	SET(WITH_ICOUTILS "ON")

	find_program( WRESTOOL NAMES wrestool )
	find_program( ICOTOOL NAMES icotool )
	MESSAGE(STATUS "Searching for icoutils binaries...")
	IF (${WRESTOOL} MATCHES WRESTOOL-NOTFOUND)
		MESSAGE(STATUS "wrestool: NOT FOUND!")
		MESSAGE(FATAL_ERROR "Please install icoutils package or disable icoutils support by adding -DWITH_ICOUTILS="OFF" option to cmake")
	ELSE (${WRESTOOL} MATCHES WRESTOOL-NOTFOUND)
		MESSAGE(STATUS "wrestool: ${WRESTOOL}")
	ENDIF (${WRESTOOL} MATCHES WRESTOOL-NOTFOUND)

	IF (${ICOTOOL} MATCHES ICOTOOL-NOTFOUND)
		MESSAGE(STATUS "icotool: NOT FOUND!")
		MESSAGE(FATAL_ERROR "Please install icoutils package or disable icoutils support by adding -DWITH_ICOUTILS="OFF" option to cmake")
	ELSE (${ICOTOOL} MATCHES ICOTOOL-NOTFOUND)
		MESSAGE(STATUS "icotool: ${ICOTOOL}")
	ENDIF (${ICOTOOL} MATCHES ICOTOOL-NOTFOUND)

ENDIF (NOT DEFINED WITH_ICOUTILS)

IF (NOT DEFINED WITH_WINETRIKS)
	SET(WITH_WINETRIKS "OFF")
ENDIF (NOT DEFINED WITH_WINETRIKS)

configure_file(${CMAKE_SOURCE_DIR}/config.h.cmake ${CMAKE_SOURCE_DIR}/config.h )

file(APPEND ${CMAKE_SOURCE_DIR}/config.h "#define APP_VERS \"${APP_VERSION}\"\n")
	  MESSAGE(STATUS "q4wine core version: ${APP_VERSION}")
file(APPEND ${CMAKE_SOURCE_DIR}/config.h "#define APP_PREF \"${CMAKE_INSTALL_PREFIX}\"\n" )
	  MESSAGE("")
	  MESSAGE(STATUS "Install prefix is: ${CMAKE_INSTALL_PREFIX}")
	  MESSAGE(STATUS "NOTE: You can set prefix by adding -DCMAKE_INSTALL_PREFIX=/usr/ option to cmake command")
	  MESSAGE("")
	  MESSAGE(STATUS "Desktop items install path is: ${DESKTOP_ENTRY_PATH}")
	  MESSAGE(STATUS "NOTE: You can set install path by adding -DDESKTOP_ENTRY_PATH=/usr/share/application option to cmake command")
	  MESSAGE("")
	  MESSAGE(STATUS "Install prefix is: ${PIXMAPS_ENTRY_PATH}")
	  MESSAGE(STATUS "NOTE: You can set install path by adding -DPIXMAPS_ENTRY_PATH=/usr/share/pixmaps option to cmake command")
	  MESSAGE("")
	  MESSAGE(STATUS "NOTE: For more configuration options see INSTALL text file!")
	  MESSAGE("")
file(APPEND ${CMAKE_SOURCE_DIR}/config.h "\n#endif\n" )

QT4_WRAP_UI( Q4WINE_GUI_UI_HEADERS ${Q4WINE_GUI_UI_SOURCES})
QT4_WRAP_CPP( Q4WINE_GUI_MOC_SOURCES ${Q4WINE_GUI_MOC_HEADEDRS} )

QT4_WRAP_UI( Q4WINE_LIB_UI_HEADERS ${Q4WINE_LIB_UI_SOURCES})
QT4_WRAP_CPP( Q4WINE_LIB_LIB_MOC_SOURCES ${Q4WINE_LIB_MOC_HEADERS} )

QT4_WRAP_CPP( QTSINGLEAPPLICATION-LIB-MOC-SRCS ${QSINGLEAPP_LIB_MOC_HEADERS} )

QT4_ADD_RESOURCES( RCC_SRCS ${Q4WINE_RESOURCE_SOURCES} )
QT4_ADD_TRANSLATION( QM_I18N ${Q4WINE_I18N_SOURCES})

INCLUDE_DIRECTORIES( ${Q4WINE_HEADERS_DIR} ${CMAKE_BINARY_DIR} )

IF (${WITH_EMBEDDED_FUSEISO} MATCHES ON )
	INCLUDE_DIRECTORIES( ${GLIB_LIB_INCLUDE_DIRS} ${FUSE_LIB_INCLUDE_DIRS} )
ENDIF (${WITH_EMBEDDED_FUSEISO} MATCHES ON )

IF (NOT DEFINED SKIP_RPATH)
	IF (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES x86_64)
		SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib64/q4wine")
	ELSE (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES x86_64)
		SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib/q4wine")
	ENDIF (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES x86_64)
ELSE (NOT DEFINED SKIP_RPATH)
	MESSAGE(STATUS "NOTE: Skip for RPATH build!")
ENDIF (NOT DEFINED SKIP_RPATH)

ADD_LIBRARY( q4wine-core SHARED ${Q4WINE_LIB_SOURCES} ${Q4WINE_LIB_UI_HEADERS} ${Q4WINE_LIB_LIB_MOC_SOURCES} ${Q4WINE_LIB_HEADERS} ${QM_I18N} )
ADD_LIBRARY( qtsingleapp STATIC ${QSINGLEAPP_LIB_SOURCES} ${QSINGLEAPP_LIB_HEADERS} ${QTSINGLEAPPLICATION-LIB-MOC-SRCS}  )

ADD_EXECUTABLE( q4wine-cli ${Q4WINE_CLI_SOURCES} ${Q4WINE_CLI_HEADERS} ${QM_I18N} ${Q4WINE_LIB_UI_HEADERS} ${Q4WINE_LIB_LIB_MOC_SOURCES})
ADD_EXECUTABLE( q4wine ${Q4WINE_GUI_SOURCES} ${Q4WINE_GUI_HEADERS} ${Q4WINE_GUI_UI_HEADERS} ${Q4WINE_GUI_MOC_SOURCES} ${RCC_SRCS} ${QM_I18N} )


IF (${WITH_EMBEDDED_FUSEISO} MATCHES ON )
	ADD_EXECUTABLE( q4wine-mount ${Q4WINE_FUSEISO_SOURCES} ${Q4WINE_FUSEISO_HEADERS} )
	SET_TARGET_PROPERTIES( q4wine-mount PROPERTIES COMPILE_FLAGS ${FUSE_LIB_CFLAGS} COMPILE_FLAGS ${GLIB_LIB_CFLAGS} )
	TARGET_LINK_LIBRARIES( q4wine-mount ${FUSE_LIB_LDFLAGS} ${GLIB_LIB_LDFLAGS} ${ZIP_LIB_LDFLAGS})
ENDIF (${WITH_EMBEDDED_FUSEISO} MATCHES ON )

IF (${CMAKE_SYSTEM_NAME} MATCHES Linux)
	TARGET_LINK_LIBRARIES( q4wine ${QT_LIBRARIES} q4wine-core qtsingleapp)
	TARGET_LINK_LIBRARIES( q4wine-cli ${QT_LIBRARIES} q4wine-core)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES Linux)

IF (${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)
	SET(BSD_LIBS -lkvm)
	TARGET_LINK_LIBRARIES( q4wine ${QT_LIBRARIES} ${BSD_LIBS} q4wine-core qtsingleapp)
	TARGET_LINK_LIBRARIES( q4wine-cli ${QT_LIBRARIES} ${BSD_LIBS} q4wine-core)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)

install(TARGETS q4wine-cli DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(TARGETS q4wine DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

IF (${WITH_EMBEDDED_FUSEISO} MATCHES ON )
	install(TARGETS q4wine-mount DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
ENDIF (${WITH_EMBEDDED_FUSEISO} MATCHES ON )

IF (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES x86_64)
	install(TARGETS q4wine-core LIBRARY DESTINATION	${CMAKE_INSTALL_PREFIX}/lib64/q4wine)
ELSE (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES x86_64)
	install(TARGETS q4wine-core LIBRARY DESTINATION	${CMAKE_INSTALL_PREFIX}/lib/q4wine)
ENDIF (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES x86_64)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/theme DESTINATION ${CMAKE_INSTALL_PREFIX}/share/q4wine)

IF (${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)
install(FILES ${CMAKE_SOURCE_DIR}/src/freebsd/mount_image.sh DESTINATION ${CMAKE_INSTALL_PREFIX}/share/q4wine/scripts PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE)
install(FILES ${CMAKE_SOURCE_DIR}/src/freebsd/umount.sh DESTINATION ${CMAKE_INSTALL_PREFIX}/share/q4wine/scripts PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)

install(FILES ${ICON_PIXMAPS} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/q4wine/icons)
install(FILES ${QM_I18N} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/q4wine/i18n)
install(FILES ${DESKTOP_ENTRY} DESTINATION ${DESKTOP_ENTRY_PATH})
install(FILES ${DESKTOP_PIXMAPS} DESTINATION ${PIXMAPS_ENTRY_PATH})
