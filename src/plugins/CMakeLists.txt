cmake_minimum_required(VERSION 2.8.0)

SET( PLUGINS_GUI_SOURCES
    ${CMAKE_SOURCE_DIR}/src/plugins/winetricks.cpp
)

SET ( PLUGINS_GUI_HEADERS
    ${CMAKE_SOURCE_DIR}/src/plugins/winetricks.h
)

IF( NOT ${CMAKE_SYSTEM_NAME} MATCHES Darwin )
    SET ( PLUGINS_GUI_SOURCES
        ${PLUGINS_GUI_SOURCES}
        ${CMAKE_SOURCE_DIR}/src/plugins/sysmenu.cpp
    )
    SET ( PLUGINS_GUI_HEADERS
        ${PLUGINS_GUI_HEADERS}
        ${CMAKE_SOURCE_DIR}/src/plugins/sysmenu.h
    )
ENDIF( NOT ${CMAKE_SYSTEM_NAME} MATCHES Darwin )
