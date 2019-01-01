# l10n cmake lists
cmake_minimum_required(VERSION 2.8.0)

MESSAGE(STATUS "Process LINGUAS variable --\n")

foreach (LANG ${LINGUAS})
    MESSAGE(STATUS "Add translation for ${LANG}: ${LANG}.ts")
    list(APPEND TS_L10N ${CMAKE_SOURCE_DIR}/src/l10n/q4wine_${LANG}.ts)
    list(APPEND QM_L10N ${CMAKE_CURRENT_BINARY_DIR}/q4wine_${LANG}.qm)
endforeach ()
MESSAGE("\n")

get_directory_property(Q4WINE_CLI_SOURCES DIRECTORY ../q4wine-cli DEFINITION Q4WINE_CLI_SOURCES)
get_directory_property(Q4WINE_GUI_UI_SOURCES DIRECTORY ../q4wine-gui DEFINITION Q4WINE_GUI_UI_SOURCES)
get_directory_property(Q4WINE_GUI_SOURCES DIRECTORY ../q4wine-gui DEFINITION Q4WINE_GUI_SOURCES)
get_directory_property(Q4WINE_HELPER_SOURCES DIRECTORY ../q4wine-helper DEFINITION Q4WINE_HELPER_SOURCES)
get_directory_property(Q4WINE_LIB_SOURCES DIRECTORY ../q4wine-lib DEFINITION Q4WINE_LIB_SOURCES)

add_custom_target (
    translations
    COMMAND ${Qt5_LUPDATE_EXECUTABLE}
    ${Q4WINE_GUI_SOURCES} ${Q4WINE_GUI_UI_SOURCES}
    ${PLUGINS_GUI_SOURCES} ${PLUGINS_GUI_UI_SOURCES}
    ${Q4WINE_CLI_SOURCES}
    ${Q4WINE_HELPER_SOURCES}
    -ts ${TS_L10N}
    -locations relative
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target (
    translations_build
    ALL
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

foreach (LANG ${LINGUAS})
add_custom_command (
    TARGET translations_build
    PRE_BUILD
    COMMAND ${Qt5_LRELEASE_EXECUTABLE} q4wine_${LANG}.ts -qm ${CMAKE_CURRENT_BINARY_DIR}/q4wine_${LANG}.qm
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endforeach()

IF( RELEASE )
    install(FILES ${QM_L10N} DESTINATION ${L10N_ENTRY_PATH})
ENDIF( RELEASE )
