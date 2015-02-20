# i18n cmake lists
cmake_minimum_required(VERSION 2.8.0)

MESSAGE(STATUS "Process LINGUAS variable --\n")

foreach (LANG ${LINGUAS})
    MESSAGE(STATUS "Add translation for ${LANG}: ${LANG}.ts")
    list(APPEND TS_I18N ${CMAKE_SOURCE_DIR}/src/i18n/q4wine_${LANG}.ts)
    list(APPEND QM_I18N ${CMAKE_CURRENT_BINARY_DIR}/q4wine_${LANG}.qm)
endforeach ()
MESSAGE("\n")

get_directory_property(Q4WINE_CLI_SOURCES DIRECTORY ../q4wine-cli DEFINITION Q4WINE_CLI_SOURCES)
get_directory_property(Q4WINE_GUI_UI_SOURCES DIRECTORY ../q4wine-gui DEFINITION Q4WINE_GUI_UI_SOURCES)
get_directory_property(Q4WINE_GUI_SOURCES DIRECTORY ../q4wine-gui DEFINITION Q4WINE_GUI_SOURCES)
get_directory_property(Q4WINE_HELPER_SOURCES DIRECTORY ../q4wine-helper DEFINITION Q4WINE_HELPER_SOURCES)
get_directory_property(Q4WINE_LIB_SOURCES DIRECTORY ../q4wine-lib DEFINITION Q4WINE_LIB_SOURCES)

if(QT5 MATCHES ON)
add_custom_target (
    translations
    COMMAND ${Qt5_LUPDATE_EXECUTABLE}
    ${Q4WINE_GUI_SOURCES} ${Q4WINE_GUI_UI_SOURCES}
    ${PLUGINS_GUI_SOURCES} ${PLUGINS_GUI_UI_SOURCES}
    ${Q4WINE_CLI_SOURCES}
    ${Q4WINE_HELPER_SOURCES}
    -ts ${TS_I18N}
    -locations relative
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
else(QT5 MATCHES ON)
add_custom_target (
    translations
    COMMAND ${QT_LUPDATE_EXECUTABLE}
    ${Q4WINE_GUI_SOURCES} ${Q4WINE_GUI_UI_SOURCES}
    ${PLUGINS_GUI_SOURCES} ${PLUGINS_GUI_UI_SOURCES}
    ${Q4WINE_CLI_SOURCES}
    ${Q4WINE_HELPER_SOURCES}
    -ts ${TS_I18N}
    -locations relative
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
endif(QT5 MATCHES ON)

add_custom_target (
    translations_build
    ALL
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

if(QT5 MATCHES ON)
foreach (LANG ${LINGUAS})
add_custom_command (
    TARGET translations_build
    PRE_BUILD
    COMMAND ${Qt5_LRELEASE_EXECUTABLE} q4wine_${LANG}.ts -qm ${CMAKE_CURRENT_BINARY_DIR}/q4wine_${LANG}.qm
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endforeach()
else(QT5 MATCHES ON)
foreach (LANG ${LINGUAS})
add_custom_command (
    TARGET translations_build
    PRE_BUILD
    COMMAND ${QT_LRELEASE_EXECUTABLE} q4wine_${LANG}.ts -qm ${CMAKE_CURRENT_BINARY_DIR}/q4wine_${LANG}.qm
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endforeach()
endif(QT5 MATCHES ON)

IF( RELEASE )
    install(FILES ${QM_I18N} DESTINATION ${I18N_ENTRY_PATH})
ENDIF( RELEASE )
