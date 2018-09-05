cmake_minimum_required(VERSION 2.8.0)

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

IF ( ${WITH_SYSTEM_SINGLEAPP} MATCHES ON )
    SET ( Q4WINE_HEADERS_DIR
        ${Q4WINE_HEADERS_DIR}
        ${QTSINGLEAPPLIB_INCLUDE_DIR}
    )
ELSE ( ${WITH_SYSTEM_SINGLEAPP} MATCHES ON )
    SET ( Q4WINE_HEADERS_DIR
        ${Q4WINE_HEADERS_DIR}
        ${CMAKE_SOURCE_DIR}/src/qtsingleapplication
    )
ENDIF ( ${WITH_SYSTEM_SINGLEAPP} MATCHES ON )

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
        ${CMAKE_SOURCE_DIR}/config.h
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

IF( QT5 )
QT5_WRAP_UI( Q4WINE_LIB_UI_HEADERS ${Q4WINE_LIB_UI_SOURCES} )
QT5_WRAP_CPP( Q4WINE_LIB_LIB_MOC_SOURCES ${Q4WINE_LIB_MOC_HEADERS} )
ELSE( QT5 )
QT4_WRAP_UI( Q4WINE_LIB_UI_HEADERS ${Q4WINE_LIB_UI_SOURCES} )
QT4_WRAP_CPP( Q4WINE_LIB_LIB_MOC_SOURCES ${Q4WINE_LIB_MOC_HEADERS} )
ENDIF( QT5 )

ADD_LIBRARY( q4wine-core SHARED ${Q4WINE_LIB_SOURCES} ${Q4WINE_LIB_UI_HEADERS} ${Q4WINE_LIB_LIB_MOC_SOURCES} ${Q4WINE_LIB_HEADERS} ${QM_L10N} )

IF( ${CMAKE_SYSTEM_NAME} MATCHES Darwin )
    SET_TARGET_PROPERTIES(q4wine-core PROPERTIES INSTALL_NAME_DIR "@executable_path/../Frameworks")
ENDIF( ${CMAKE_SYSTEM_NAME} MATCHES Darwin )

SET_TARGET_PROPERTIES(q4wine-core PROPERTIES VERSION "0.0.0" OUTPUT_NAME "libq4wine-core")

if(QT5 MATCHES ON)
    target_link_libraries(q4wine-core Qt5::Core Qt5::DBus Qt5::Network Qt5::Sql Qt5::Widgets Qt5::Xml)
else(QT5 MATCHES ON)
target_link_libraries( q4wine-core ${QT_LIBRARIES})
endif(QT5 MATCHES ON)
add_dependencies( q4wine-core translations_build )

IF( RELEASE )
    install( TARGETS q4wine-core LIBRARY DESTINATION ${LIBS_ENTRY_PATH} NAMELINK_SKIP)
ENDIF( RELEASE )
