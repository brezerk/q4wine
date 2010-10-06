cmake_minimum_required(VERSION 2.6) 

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

SET( Q4WINE_LIB_SOURCES
        ${CMAKE_SOURCE_DIR}/src/q4wine-lib/q4wine-lib.cpp
        ${CMAKE_SOURCE_DIR}/src/core/database/db.cpp
        ${CMAKE_SOURCE_DIR}/src/core/database/prefix.cpp
        ${CMAKE_SOURCE_DIR}/src/core/database/dir.cpp
        ${CMAKE_SOURCE_DIR}/src/core/database/icon.cpp
        ${CMAKE_SOURCE_DIR}/src/core/database/image.cpp
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/process.cpp
)

SET( Q4WINE_LIB_HEADERS
        ${CMAKE_SOURCE_DIR}/src/q4wine-lib/q4wine-lib.h
        ${CMAKE_SOURCE_DIR}/config.h
        ${CMAKE_SOURCE_DIR}/src/core/database/db.h
        ${CMAKE_SOURCE_DIR}/src/core/database/prefix.h
        ${CMAKE_SOURCE_DIR}/src/core/database/dir.h
        ${CMAKE_SOURCE_DIR}/src/core/database/icon.h
        ${CMAKE_SOURCE_DIR}/src/core/database/image.h
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/process.h
)

SET ( Q4WINE_LIB_MOC_HEADERS
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/process.h
    ${CMAKE_SOURCE_DIR}/src/core/database/db.h
)

SET ( Q4WINE_LIB_UI_SOURCES
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/Process.ui
)

QT4_WRAP_UI( Q4WINE_LIB_UI_HEADERS ${Q4WINE_LIB_UI_SOURCES} )
QT4_WRAP_CPP( Q4WINE_LIB_LIB_MOC_SOURCES ${Q4WINE_LIB_MOC_HEADERS} )

ADD_LIBRARY( q4wine-core SHARED ${Q4WINE_LIB_SOURCES} ${Q4WINE_LIB_UI_HEADERS} ${Q4WINE_LIB_LIB_MOC_SOURCES} ${Q4WINE_LIB_HEADERS} ${QM_I18N} )

install(TARGETS q4wine-core LIBRARY DESTINATION ${LIBS_ENTRY_PATH})
