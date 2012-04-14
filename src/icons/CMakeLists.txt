cmake_minimum_required(VERSION 2.6)

IF( ${CMAKE_SYSTEM_NAME} MATCHES Darwin )
    SET( DESKTOP_PIXMAPS ${CMAKE_SOURCE_DIR}/src/icons/q4wine-128x128.icns )
    SET( DESKTOP_PLIST ${CMAKE_SOURCE_DIR}/src/icons/Info.plist )

    IF( RELEASE )
        install(FILES ${DESKTOP_PIXMAPS} DESTINATION ${PIXMAPS_ENTRY_PATH})
        install(FILES ${DESKTOP_PLIST} DESTINATION ${CMAKE_INSTALL_PREFIX}/${APP_NAME_GLOBAL}.app/Contents)
    ENDIF( RELEASE )
ELSE( ${CMAKE_SYSTEM_NAME} MATCHES Darwin )
    foreach (SIZE 350 256 192 128 96 64 48 32 22 16)
        SET( DESKTOP_PIXMAPS
            ${CMAKE_SOURCE_DIR}/src/icons/q4wine-${SIZE}x${SIZE}.png )
        IF( RELEASE )
            install(FILES ${DESKTOP_PIXMAPS} DESTINATION ${PIXMAPS_ENTRY_PATH}/${SIZE}x${SIZE}/apps RENAME q4wine.png)
        ENDIF( RELEASE )
    endforeach()
ENDIF( ${CMAKE_SYSTEM_NAME} MATCHES Darwin )



