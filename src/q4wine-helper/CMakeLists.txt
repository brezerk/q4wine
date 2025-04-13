cmake_minimum_required(VERSION 3.24)

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
    ${CMAKE_SOURCE_DIR}/src/core/database/table.cpp
    ${CMAKE_SOURCE_DIR}/src/core/database/versions.cpp
    ${CMAKE_SOURCE_DIR}/src/core/database/logging.cpp
    ${CMAKE_SOURCE_DIR}/src/core/database/prefix.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-helper/wineobject.cpp
)

SET( Q4WINE_HELPER_HEADERS
    ${CMAKE_SOURCE_DIR}/src/core/database/db.h
    ${CMAKE_SOURCE_DIR}/src/core/database/table.h
    ${CMAKE_SOURCE_DIR}/src/core/database/versions.h
    ${CMAKE_SOURCE_DIR}/src/core/database/logging.h
    ${CMAKE_SOURCE_DIR}/src/core/database/prefix.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-helper/wineobject.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-helper/q4wine-helper.h
    ${PROJECT_BINARY_DIR}/config.h
)

SET ( Q4WINE_HELPER_MOC_HEADERS
    ${CMAKE_SOURCE_DIR}/src/q4wine-helper/wineobject.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/process.h
)

SET ( CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis
)

QT6_WRAP_CPP( Q4WINE_HELPER_LIB_MOC_SOURCES ${Q4WINE_HELPER_MOC_HEADERS} )

ADD_EXECUTABLE( q4wine-helper ${Q4WINE_HELPER_SOURCES}
    ${Q4WINE_HELPER_UI_HEADERS} ${Q4WINE_LIB_UI_HEADERS}
    ${Q4WINE_LIB_LIB_MOC_SOURCES} ${Q4WINE_HELPER_LIB_MOC_SOURCES}
    ${Q4WINE_HELPER_HEADERS} ${QM_L10N} )

IF (${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)
    SET(BSD_LIBS -lkvm)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)

target_link_libraries(q4wine-helper PRIVATE q4wine-core Qt6::Core Qt6::DBus Qt6::Network Qt6::Sql Qt6::Widgets Qt6::Xml )

IF( RELEASE )
    install(TARGETS q4wine-helper DESTINATION ${APPS_ENTRY_PATH})
ENDIF( RELEASE )
