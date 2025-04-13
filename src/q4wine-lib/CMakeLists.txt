cmake_minimum_required(VERSION 3.24)

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

INCLUDE_DIRECTORIES( ${Q4WINE_HEADERS_DIR} ${CMAKE_BINARY_DIR} )

SET( Q4WINE_LIB_SOURCES
        ${CMAKE_SOURCE_DIR}/src/q4wine-lib/q4wine-lib.cpp
        ${CMAKE_SOURCE_DIR}/src/core/database/db.cpp
        ${CMAKE_SOURCE_DIR}/src/core/database/table.cpp
        ${CMAKE_SOURCE_DIR}/src/core/database/prefix.cpp
        ${CMAKE_SOURCE_DIR}/src/core/database/dir.cpp
        ${CMAKE_SOURCE_DIR}/src/core/database/icon.cpp
        ${CMAKE_SOURCE_DIR}/src/core/database/image.cpp
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/process.cpp
)

SET( Q4WINE_LIB_HEADERS
        ${CMAKE_SOURCE_DIR}/src/q4wine-lib/q4wine-lib.h
        ${PROJECT_BINARY_DIR}/config.h
        ${CMAKE_SOURCE_DIR}/src/core/database/db.h
        ${CMAKE_SOURCE_DIR}/src/core/database/table.h
        ${CMAKE_SOURCE_DIR}/src/core/database/prefix.h
        ${CMAKE_SOURCE_DIR}/src/core/database/dir.h
        ${CMAKE_SOURCE_DIR}/src/core/database/icon.h
        ${CMAKE_SOURCE_DIR}/src/core/database/image.h
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/process.h
)

IF( ${CMAKE_SYSTEM_NAME} MATCHES Darwin )
    SET( Q4WINE_LIB_SOURCES
            ${Q4WINE_LIB_SOURCES}
            ${CMAKE_SOURCE_DIR}/src/darwin/ps.c
    )
    SET( Q4WINE_LIB_HEADERS
            ${Q4WINE_LIB_HEADERS}
            ${CMAKE_SOURCE_DIR}/src/darwin/ps.h
    )
ENDIF( ${CMAKE_SYSTEM_NAME} MATCHES Darwin )

SET ( Q4WINE_LIB_MOC_HEADERS
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/process.h
        ${CMAKE_SOURCE_DIR}/src/core/database/db.h
)

SET ( Q4WINE_LIB_UI_SOURCES
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/Process.ui
)

SET ( CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis
)

QT_WRAP_UI( Q4WINE_LIB_UI_HEADERS ${Q4WINE_LIB_UI_SOURCES} )
QT_WRAP_CPP( Q4WINE_LIB_LIB_MOC_SOURCES ${Q4WINE_LIB_MOC_HEADERS} )

ADD_LIBRARY( q4wine-core SHARED
        ${Q4WINE_LIB_SOURCES}
        ${Q4WINE_LIB_UI_HEADERS}
        ${Q4WINE_LIB_LIB_MOC_SOURCES}
        ${Q4WINE_LIB_HEADERS}
        ${QM_L10N}
)

IF( ${CMAKE_SYSTEM_NAME} MATCHES Darwin )
    SET_TARGET_PROPERTIES(q4wine-core PROPERTIES INSTALL_NAME_DIR "@executable_path/../Frameworks")
ENDIF( ${CMAKE_SYSTEM_NAME} MATCHES Darwin )

TARGET_LINK_LIBRARIES(q4wine-core PRIVATE Qt6::Core Qt6::DBus Qt6::Network Qt6::Sql Qt6::Widgets Qt6::Xml)

IF( RELEASE )
    INSTALL( TARGETS q4wine-core
             RUNTIME DESTINATION ${LIBS_ENTRY_PATH}
             LIBRARY DESTINATION ${LIBS_ENTRY_PATH}
             NAMELINK_SKIP)
ENDIF( RELEASE )
