cmake_minimum_required(VERSION 2.6)

SET ( Q4WINE_HEADERS_DIR
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/core/database
    ${CMAKE_SOURCE_DIR}/src/q4wine-cli
    ${CMAKE_SOURCE_DIR}/src/q4wine-lib
    ${CMAKE_SOURCE_DIR}/src/q4wine-helper
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/appdb
    ${CMAKE_SOURCE_DIR}/src/plugins
)

INCLUDE_DIRECTORIES( ${Q4WINE_HEADERS_DIR} ${CMAKE_BINARY_DIR} )

SET( Q4WINE_HELPER_SOURCES
    ${CMAKE_SOURCE_DIR}/src/q4wine-helper/q4wine-helper.cpp
    ${CMAKE_SOURCE_DIR}/src/core/database/db.cpp
    ${CMAKE_SOURCE_DIR}/src/core/database/logging.cpp
    ${CMAKE_SOURCE_DIR}/src/core/database/prefix.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-helper/wineobject.cpp
)

SET( Q4WINE_HELPER_HEADERS
    ${CMAKE_SOURCE_DIR}/src/q4wine-helper/q4wine-helper.h
    ${CMAKE_SOURCE_DIR}/config.h
    ${CMAKE_SOURCE_DIR}/src/core/database/db.h
    ${CMAKE_SOURCE_DIR}/src/core/database/logging.h
    ${CMAKE_SOURCE_DIR}/src/core/database/prefix.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-helper/wineobject.h
)

SET ( Q4WINE_HELPER_MOC_HEADERS
    ${CMAKE_SOURCE_DIR}/src/q4wine-helper/wineobject.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/process.h
)

QT4_WRAP_CPP( Q4WINE_HELPER_LIB_MOC_SOURCES ${Q4WINE_HELPER_MOC_HEADERS} )

ADD_EXECUTABLE( q4wine-helper MACOSX_BUNDLE ${Q4WINE_HELPER_SOURCES}
    ${Q4WINE_HELPER_UI_HEADERS} ${Q4WINE_LIB_UI_HEADERS}
    ${Q4WINE_LIB_LIB_MOC_SOURCES} ${Q4WINE_HELPER_LIB_MOC_SOURCES}
    ${Q4WINE_HELPER_HEADERS} ${QM_I18N} )

add_dependencies( q4wine-helper translations )

IF (${CMAKE_SYSTEM_NAME} MATCHES Linux)
    TARGET_LINK_LIBRARIES( q4wine-helper ${QT_LIBRARIES} q4wine-core)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES Linux)

IF (${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)
    SET(BSD_LIBS -lkvm)
    TARGET_LINK_LIBRARIES( q4wine-helper ${QT_LIBRARIES} ${BSD_LIBS} q4wine-core)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)

IF (${CMAKE_SYSTEM_NAME} MATCHES Darwin)
    TARGET_LINK_LIBRARIES( q4wine-helper ${QT_LIBRARIES} q4wine-core)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES Darwin)

IF( RELEASE )
    install(TARGETS q4wine-helper DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
ENDIF( RELEASE )
