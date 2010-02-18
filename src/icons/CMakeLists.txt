cmake_minimum_required(VERSION 2.6)

SET( DESKTOP_PIXMAPS
    ${CMAKE_SOURCE_DIR}/src/icons/q4wine.png
)

install(FILES ${DESKTOP_PIXMAPS} DESTINATION ${PIXMAPS_ENTRY_PATH})

