# i18n cmake lists
cmake_minimum_required(VERSION 2.6)

MESSAGE(STATUS "Process LINGUAS variable --\n")

foreach (LANG ${LINGUAS})
    MESSAGE(STATUS "Add translation for ${LANG}: ${LANG}.ts")
    list(APPEND Q4WINE_I18N_SOURCES ${CMAKE_SOURCE_DIR}/src/i18n/${LANG}.ts)
endforeach()
MESSAGE("\n")
