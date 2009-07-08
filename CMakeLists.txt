cmake_minimum_required(VERSION 2.6)

PROJECT(q4wine)

SET(CMAKE_VERBOSE_MAKEFILE ON)

SET(APP_VERSION "0.113-dev")

SET( DESKTOP_ENTRY
	${CMAKE_SOURCE_DIR}/q4wine.desktop
)

SET( DESKTOP_PIXMAPS
	${CMAKE_SOURCE_DIR}/src/icons/q4wine.png
	${CMAKE_SOURCE_DIR}/src/data/eject.png
	${CMAKE_SOURCE_DIR}/src/data/explorer.png
	${CMAKE_SOURCE_DIR}/src/data/exec_wine.png
	${CMAKE_SOURCE_DIR}/src/data/notepad.png
	${CMAKE_SOURCE_DIR}/src/data/regedit.png
	${CMAKE_SOURCE_DIR}/src/data/uninstaller.png
	${CMAKE_SOURCE_DIR}/src/data/winecfg.png
	${CMAKE_SOURCE_DIR}/src/data/wineconsole.png
)

SET( SOURCES
	${CMAKE_SOURCE_DIR}/src/gui/run.cpp
	${CMAKE_SOURCE_DIR}/src/main.cpp
	${CMAKE_SOURCE_DIR}/src/core/database/db.cpp
	${CMAKE_SOURCE_DIR}/src/core/database/prefix.cpp
	${CMAKE_SOURCE_DIR}/src/core/database/dir.cpp
	${CMAKE_SOURCE_DIR}/src/core/database/icon.cpp
	${CMAKE_SOURCE_DIR}/src/core/database/image.cpp
	${CMAKE_SOURCE_DIR}/src/core/registry.cpp
     ${CMAKE_SOURCE_DIR}/src/core/winebinlauncher.cpp
	${CMAKE_SOURCE_DIR}/src/gui/about.cpp
	${CMAKE_SOURCE_DIR}/src/gui/appsettings.cpp
	${CMAKE_SOURCE_DIR}/src/gui/iconsettings.cpp
	${CMAKE_SOURCE_DIR}/src/gui/iconsview.cpp
	${CMAKE_SOURCE_DIR}/src/gui/mainwindow.cpp
	${CMAKE_SOURCE_DIR}/src/gui/prefixsettings.cpp
	${CMAKE_SOURCE_DIR}/src/gui/process.cpp
	${CMAKE_SOURCE_DIR}/src/gui/wizard.cpp
	${CMAKE_SOURCE_DIR}/src/gui/imagemanager.cpp
#	${CMAKE_SOURCE_DIR}/src/wis/wisitem.cpp
	${CMAKE_SOURCE_DIR}/src/gui/draglistwidget.cpp
)

SET ( HEADERS_DIR
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/src/gui
	${CMAKE_SOURCE_DIR}/src/core
	${CMAKE_SOURCE_DIR}/src/temporary
	${CMAKE_SOURCE_DIR}/src/wis
)


SET ( HEADERS
	${CMAKE_SOURCE_DIR}/src/gui/run.h
	${CMAKE_SOURCE_DIR}/src/core/registry.h
	${CMAKE_SOURCE_DIR}/src/core/database/db.h
	${CMAKE_SOURCE_DIR}/src/core/database/prefix.h
	${CMAKE_SOURCE_DIR}/src/core/database/dir.h
	${CMAKE_SOURCE_DIR}/src/core/database/icon.h
	${CMAKE_SOURCE_DIR}/src/core/database/image.h
	${CMAKE_SOURCE_DIR}/src/core/winebinlauncher.h
	${CMAKE_SOURCE_DIR}/src/gui/about.h
	${CMAKE_SOURCE_DIR}/src/gui/appsettings.h
	${CMAKE_SOURCE_DIR}/src/gui/iconsettings.h
	${CMAKE_SOURCE_DIR}/src/gui/iconsview.h
	${CMAKE_SOURCE_DIR}/src/gui/mainwindow.h
	${CMAKE_SOURCE_DIR}/src/gui/prefixsettings.h
	${CMAKE_SOURCE_DIR}/src/gui/process.h
	${CMAKE_SOURCE_DIR}/src/gui/wizard.h
	${CMAKE_SOURCE_DIR}/src/gui/imagemanager.h
	${CMAKE_SOURCE_DIR}/src/wis/wisitem.h
	${CMAKE_SOURCE_DIR}/src/core/database/initdb.h
	${CMAKE_SOURCE_DIR}/config.h
	${CMAKE_SOURCE_DIR}/src/gui/draglistwidget.h
)

SET ( MOC_HEADEDRS
	${CMAKE_SOURCE_DIR}/src/core/registry.h
	${CMAKE_SOURCE_DIR}/src/core/winebinlauncher.h
	${CMAKE_SOURCE_DIR}/src/gui/appsettings.h
	${CMAKE_SOURCE_DIR}/src/gui/iconsettings.h
	${CMAKE_SOURCE_DIR}/src/gui/iconsview.h
	${CMAKE_SOURCE_DIR}/src/gui/mainwindow.h
	${CMAKE_SOURCE_DIR}/src/gui/prefixsettings.h
	${CMAKE_SOURCE_DIR}/src/gui/process.h
	${CMAKE_SOURCE_DIR}/src/gui/about.h
	${CMAKE_SOURCE_DIR}/src/gui/run.h
	${CMAKE_SOURCE_DIR}/src/gui/wizard.h
	${CMAKE_SOURCE_DIR}/src/gui/imagemanager.h
	#${CMAKE_SOURCE_DIR}/src/wis/wisitem.h
	${CMAKE_SOURCE_DIR}/src/gui/draglistwidget.h
)

SET ( UIS
	${CMAKE_SOURCE_DIR}/src/uis/About.ui
	${CMAKE_SOURCE_DIR}/src/uis/AppSettings.ui
	${CMAKE_SOURCE_DIR}/src/uis/IconSettings.ui
	${CMAKE_SOURCE_DIR}/src/uis/IconsView.ui
	${CMAKE_SOURCE_DIR}/src/uis/MainWindow.ui
	${CMAKE_SOURCE_DIR}/src/uis/PrefixSettings.ui
	${CMAKE_SOURCE_DIR}/src/uis/Process.ui
	${CMAKE_SOURCE_DIR}/src/uis/Run.ui
	${CMAKE_SOURCE_DIR}/src/uis/Wizard.ui
	${CMAKE_SOURCE_DIR}/src/uis/IconSettings.ui
	${CMAKE_SOURCE_DIR}/src/uis/ImageManager.ui
)

IF (${WITH_WINETOOLS} MATCHES ON )
	SET( SOURCES
		${SOURCES}
		${CMAKE_SOURCE_DIR}/src/temporary/winetricks.cpp
		${CMAKE_SOURCE_DIR}/src/temporary/winetrickshelp.cpp
	)

	SET ( HEADERS
		${HEADERS}
		${CMAKE_SOURCE_DIR}/src/temporary/winetricks.h
		${CMAKE_SOURCE_DIR}/src/temporary/winetrickshelp.h
	)

	SET ( MOC_HEADEDRS
		${MOC_HEADEDRS}
		${CMAKE_SOURCE_DIR}/src/temporary/winetricks.h
		${CMAKE_SOURCE_DIR}/src/temporary/winetrickshelp.h
	)

	SET ( UIS
		${UIS}
		${CMAKE_SOURCE_DIR}/src/temporary/winetricks.ui
		${CMAKE_SOURCE_DIR}/src/temporary/winetrickshelp.ui

	)
ENDIF (${WITH_WINETOOLS} MATCHES ON )

SET ( LIB_MOC_HEADEDRS
	${CMAKE_SOURCE_DIR}/src/gui/process.h
)

SET ( LIB_UIS
	${CMAKE_SOURCE_DIR}/src/uis/Process.ui
)


SET( Q4WINE-LIB-SOURCES
		${CMAKE_SOURCE_DIR}/src/q4wine-lib/main.cpp
		${CMAKE_SOURCE_DIR}/src/core/database/db.cpp
		${CMAKE_SOURCE_DIR}/src/core/database/prefix.cpp
		${CMAKE_SOURCE_DIR}/src/core/database/dir.cpp
		${CMAKE_SOURCE_DIR}/src/core/database/icon.cpp
		${CMAKE_SOURCE_DIR}/src/core/database/image.cpp
)

SET( Q4WINE-LIB-HEADERS
		${CMAKE_SOURCE_DIR}/src/q4wine-lib/main.h
		${CMAKE_SOURCE_DIR}/config.h
		${CMAKE_SOURCE_DIR}/src/core/database/db.h
		${CMAKE_SOURCE_DIR}/src/core/database/prefix.h
		${CMAKE_SOURCE_DIR}/src/core/database/dir.h
		${CMAKE_SOURCE_DIR}/src/core/database/icon.h
		${CMAKE_SOURCE_DIR}/src/core/database/image.h
)

SET( Q4WINE-CLI-SOURCES
		${CMAKE_SOURCE_DIR}/src/q4wine-cli/main.cpp
		${CMAKE_SOURCE_DIR}/src/core/database/db.cpp
		${CMAKE_SOURCE_DIR}/src/core/database/prefix.cpp
		${CMAKE_SOURCE_DIR}/src/core/database/dir.cpp
		${CMAKE_SOURCE_DIR}/src/core/database/icon.cpp
		${CMAKE_SOURCE_DIR}/src/core/database/image.cpp
		${CMAKE_SOURCE_DIR}/src/gui/process.cpp
)

SET( Q4WINE-CLI-HEADERS
		${CMAKE_SOURCE_DIR}/src/q4wine-lib/main.h
		${CMAKE_SOURCE_DIR}/src/q4wine-cli/main.h
		${CMAKE_SOURCE_DIR}/config.h
		${CMAKE_SOURCE_DIR}/src/core/database/db.h
		${CMAKE_SOURCE_DIR}/src/core/database/prefix.h
		${CMAKE_SOURCE_DIR}/src/core/database/dir.h
		${CMAKE_SOURCE_DIR}/src/core/database/icon.h
		${CMAKE_SOURCE_DIR}/src/core/database/image.h
		${CMAKE_SOURCE_DIR}/src/gui/process.h
)

SET ( RCCS
	${CMAKE_SOURCE_DIR}/src/q4wine.qrc
)

SET ( I18N
	${CMAKE_SOURCE_DIR}/src/i18n/en_us.ts
	${CMAKE_SOURCE_DIR}/src/i18n/ru_ru.ts
	${CMAKE_SOURCE_DIR}/src/i18n/uk_ua.ts
	${CMAKE_SOURCE_DIR}/src/i18n/pt_br.ts
	${CMAKE_SOURCE_DIR}/src/i18n/es_es.ts
)

MESSAGE("")
MESSAGE(STATUS " * Utility for management wine applications and prefixes")
MESSAGE(STATUS " * (C) 2008-2009, brezblock core team")
MESSAGE(STATUS " * http://brezblock.org.ua/")
MESSAGE("")

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

IF (NOT DEFINED WITH_ICOTOOLS)
	SET(WITH_ICOTOOLS "ON")
ENDIF (NOT DEFINED WITH_ICOTOOLS)

IF (NOT DEFINED WITH_WINETOOLS)
	SET(WITH_WINETOOLS "OFF")
ENDIF (NOT DEFINED WITH_WINETOOLS)

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

QT4_WRAP_UI( UI_HEADERS ${UIS})
QT4_WRAP_CPP( MOC_SRCS ${MOC_HEADEDRS} )

QT4_WRAP_UI( LIB_UI_HEADERS ${LIB_UIS})
QT4_WRAP_CPP( LIB_MOC_SRCS ${LIB_MOC_HEADEDRS} )

QT4_ADD_RESOURCES( RCC_SRCS ${RCCS} )
QT4_ADD_TRANSLATION( QM_I18N ${I18N})

INCLUDE_DIRECTORIES( ${HEADERS_DIR} )
INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR} )

ADD_LIBRARY( q4wine-core SHARED ${Q4WINE-LIB-SOURCES} ${LIB_UI_HEADERS} ${LIB_MOC_SRCS} ${Q4WINE-LIB-HEADERS} ${QM_I18N} )

ADD_EXECUTABLE( q4wine-cli ${Q4WINE-CLI-SOURCES} ${Q4WINE-CLI-HEADERS} ${QM_I18N} ${LIB_UI_HEADERS} ${LIB_MOC_SRCS})

ADD_EXECUTABLE( q4wine ${SOURCES} ${HEADERS} ${UI_HEADERS} ${MOC_SRCS} ${RCC_SRCS} ${QM_I18N} )

IF (${CMAKE_SYSTEM_NAME} MATCHES Linux)
	TARGET_LINK_LIBRARIES( q4wine ${QT_LIBRARIES} q4wine-core)
	TARGET_LINK_LIBRARIES( q4wine-cli ${QT_LIBRARIES} q4wine-core)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES Linux)

IF (${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)
	SET(BSD_LIBS -lkvm)
	TARGET_LINK_LIBRARIES( q4wine ${QT_LIBRARIES} ${BSD_LIBS} q4wine-core)
	TARGET_LINK_LIBRARIES( q4wine-cli ${QT_LIBRARIES} ${BSD_LIBS} q4wine-core)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)

install(TARGETS q4wine-cli DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(TARGETS q4wine DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(TARGETS q4wine-core DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/theme DESTINATION ${CMAKE_INSTALL_PREFIX}/share/q4wine)
install(FILES ${QM_I18N} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/q4wine/i18n)
install(FILES ${DESKTOP_ENTRY} DESTINATION ${DESKTOP_ENTRY_PATH})
install(FILES ${DESKTOP_PIXMAPS} DESTINATION ${PIXMAPS_ENTRY_PATH})
