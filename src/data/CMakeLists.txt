cmake_minimum_required(VERSION 2.8.0...3.24)

SET( ICON_PIXMAPS
    ${CMAKE_SOURCE_DIR}/src/data/exec_wine.png
)

SET( ICON_SCALABLE
    ${CMAKE_SOURCE_DIR}/src/data/wine/cdrom.svg
    ${CMAKE_SOURCE_DIR}/src/data/wine/winefile.svg
    ${CMAKE_SOURCE_DIR}/src/data/wine/iexplore.svg
    ${CMAKE_SOURCE_DIR}/src/data/wine/oic_winlogo.svg
    ${CMAKE_SOURCE_DIR}/src/data/wine/control.svg
    ${CMAKE_SOURCE_DIR}/src/data/wine/taskmgr.svg
    ${CMAKE_SOURCE_DIR}/src/data/wine/notepad.svg
    ${CMAKE_SOURCE_DIR}/src/data/wine/regedit.svg
    ${CMAKE_SOURCE_DIR}/src/data/wine/trash_file.svg
    ${CMAKE_SOURCE_DIR}/src/data/wine/winecfg.svg
    ${CMAKE_SOURCE_DIR}/src/data/wine/winemine.svg
    ${CMAKE_SOURCE_DIR}/src/data/wine/wordpad.svg
    ${CMAKE_SOURCE_DIR}/src/data/wine/wcmd.svg
)

IF( RELEASE )
    install(FILES ${ICON_PIXMAPS} DESTINATION ${DATA_ENTRY_PATH})
    install(FILES ${ICON_SCALABLE} DESTINATION ${DATA_ENTRY_PATH})
ENDIF( RELEASE )

