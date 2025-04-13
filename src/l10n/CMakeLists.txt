# l10n cmake lists
cmake_minimum_required(VERSION 3.24)

MESSAGE(STATUS "Process LINGUAS variable --\n")

foreach (LANG ${LINGUAS})
    MESSAGE(STATUS "Add translation for ${LANG}: ${LANG}.ts")
    SET (TS_FILES
        ${TS_FILES}
        ${CMAKE_SOURCE_DIR}/src/l10n/q4wine_${LANG}.ts
    )
    SET (QM_L10N
        ${QM_L10N}
        ${CMAKE_SOURCE_DIR}/src/l10n/q4wine_${LANG}.qm
    )
    set(I18N_TRANSLATED_LANGUAGES
        ${I18N_TRANSLATED_LANGUAGES}
        ${LANG}
    )
endforeach ()
MESSAGE("\n")

get_directory_property(Q4WINE_CLI_SOURCES DIRECTORY ../q4wine-cli DEFINITION Q4WINE_CLI_SOURCES)
get_directory_property(Q4WINE_GUI_UI_SOURCES DIRECTORY ../q4wine-gui DEFINITION Q4WINE_GUI_UI_SOURCES)
get_directory_property(Q4WINE_GUI_SOURCES DIRECTORY ../q4wine-gui DEFINITION Q4WINE_GUI_SOURCES)
get_directory_property(Q4WINE_HELPER_SOURCES DIRECTORY ../q4wine-helper DEFINITION Q4WINE_HELPER_SOURCES)
get_directory_property(Q4WINE_LIB_SOURCES DIRECTORY ../q4wine-lib DEFINITION Q4WINE_LIB_SOURCES)

set_source_files_properties(
    ${TS_FILES}
    PROPERTIES OUTPUT_LOCATION "${CMAKE_SOURCE_DIR}/src/l10n"
)

qt_add_translations(q4wine
    TS_FILES ${TS_FILES}
    SOURCES ${Q4WINE_CLI_SOURCES} ${Q4WINE_GUI_UI_SOURCES} ${Q4WINE_GUI_SOURCES} ${Q4WINE_HELPER_SOURCES} ${Q4WINE_LIB_SOURCES} ${PLUGINS_GUI_SOURCES} ${PLUGINS_GUI_UI_SOURCES}
)

IF( RELEASE )
    install(FILES ${QM_L10N} DESTINATION ${L10N_ENTRY_PATH})
ENDIF( RELEASE )
