cmake_minimum_required(VERSION 2.6)

SET ( MAN_PAGES
    ${CMAKE_SOURCE_DIR}/src/man1/q4wine.1
    ${CMAKE_SOURCE_DIR}/src/man1/q4wine-cli.1
    ${CMAKE_SOURCE_DIR}/src/man1/q4wine-helper.1
)


MESSAGE(STATUS "Creating man pages --")
foreach ( MAN_PAGE ${MAN_PAGES})
    EXECUTE_PROCESS (
    OUTPUT_FILE ${MAN_PAGE}.gz
    COMMAND "${GZIP_BIN}" -9
    INPUT_FILE ${MAN_PAGE}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/man1/)

    MESSAGE(STATUS "    gzip page ${MAN_PAGE}")
endforeach()

install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/man1 DESTINATION ${CMAKE_INSTALL_PREFIX}/share/man FILES_MATCHING PATTERN "*.1.gz")

