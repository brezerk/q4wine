# i18n cmake lists
cmake_minimum_required(VERSION 2.6)

MESSAGE(STATUS "Process LANGUAGES variable --\n")

foreach (LANGUAGE ${LANGUAGES})
    MESSAGE(STATUS "Add translation for ${LANGUAGE}: ${LANGUAGE}.ts")
    list(APPEND Q4WINE_I18N_SOURCES ${CMAKE_SOURCE_DIR}/src/i18n/${LANGUAGE}.ts)
endforeach()



    MESSAGE("\n")
