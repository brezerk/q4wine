cmake_minimum_required(VERSION 2.8.0)

SET( Q4WINE_CLI_SOURCES
    ${CMAKE_SOURCE_DIR}/src/q4wine-cli/q4wine-cli.cpp
    ${CMAKE_SOURCE_DIR}/src/core/database/db.cpp
    ${CMAKE_SOURCE_DIR}/src/core/database/table.cpp
    ${CMAKE_SOURCE_DIR}/src/core/database/versions.cpp
    ${CMAKE_SOURCE_DIR}/src/core/database/prefix.cpp
    ${CMAKE_SOURCE_DIR}/src/core/database/dir.cpp
    ${CMAKE_SOURCE_DIR}/src/core/database/icon.cpp
    ${CMAKE_SOURCE_DIR}/src/core/database/image.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/process.cpp
)

SET( Q4WINE_CLI_HEADERS
    ${CMAKE_SOURCE_DIR}/src/q4wine-lib/q4wine-lib.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-cli/q4wine-cli.h
    ${CMAKE_SOURCE_DIR}/config.h
    ${CMAKE_SOURCE_DIR}/src/core/database/db.h
    ${CMAKE_SOURCE_DIR}/src/core/database/table.h
    ${CMAKE_SOURCE_DIR}/src/core/database/versions.h
    ${CMAKE_SOURCE_DIR}/src/core/database/prefix.h
    ${CMAKE_SOURCE_DIR}/src/core/database/dir.h
    ${CMAKE_SOURCE_DIR}/src/core/database/icon.h
    ${CMAKE_SOURCE_DIR}/src/core/database/image.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/process.h
)

ADD_EXECUTABLE( q4wine-cli ${Q4WINE_CLI_SOURCES} ${Q4WINE_CLI_HEADERS} ${QM_L10N} ${Q4WINE_LIB_UI_HEADERS} ${Q4WINE_LIB_LIB_MOC_SOURCES} )
add_dependencies( q4wine-cli translations_build )

IF (${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)
    SET(BSD_LIBS -lkvm)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)

if(QT5 MATCHES ON)
    target_link_libraries(q4wine-cli Qt5::Core Qt5::DBus Qt5::Network Qt5::Sql Qt5::Widgets Qt5::Xml)
    target_link_libraries(q4wine-cli q4wine-core ${BSD_LIBS})
else(QT5 MATCHES ON)
TARGET_LINK_LIBRARIES( q4wine-cli ${QT_LIBRARIES} q4wine-core ${BSD_LIBS})
endif(QT5 MATCHES ON)

IF( RELEASE )
    install(TARGETS q4wine-cli DESTINATION ${APPS_ENTRY_PATH})
ENDIF( RELEASE )
