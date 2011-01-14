cmake_minimum_required(VERSION 2.6)

SET( Q4WINE_CLI_SOURCES
    ${CMAKE_SOURCE_DIR}/src/q4wine-cli/q4wine-cli.cpp
    ${CMAKE_SOURCE_DIR}/src/core/database/db.cpp
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
    ${CMAKE_SOURCE_DIR}/src/core/database/prefix.h
    ${CMAKE_SOURCE_DIR}/src/core/database/dir.h
    ${CMAKE_SOURCE_DIR}/src/core/database/icon.h
    ${CMAKE_SOURCE_DIR}/src/core/database/image.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/process.h
)

ADD_EXECUTABLE( q4wine-cli MACOSX_BUNDLE ${Q4WINE_CLI_SOURCES} ${Q4WINE_CLI_HEADERS} ${QM_I18N} ${Q4WINE_LIB_UI_HEADERS} ${Q4WINE_LIB_LIB_MOC_SOURCES} )
add_dependencies( q4wine-cli translations )

IF (${CMAKE_SYSTEM_NAME} MATCHES Linux)
    TARGET_LINK_LIBRARIES( q4wine-cli ${QT_LIBRARIES} q4wine-core)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES Linux)

IF (${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)
    SET(BSD_LIBS -lkvm)
    TARGET_LINK_LIBRARIES( q4wine-cli ${QT_LIBRARIES} ${BSD_LIBS} q4wine-core)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)

IF (${CMAKE_SYSTEM_NAME} MATCHES Darwin)
    TARGET_LINK_LIBRARIES( q4wine-cli ${QT_LIBRARIES} q4wine-core)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES Darwin)

IF( RELEASE )
    install(TARGETS q4wine-cli DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
ENDIF( RELEASE )
