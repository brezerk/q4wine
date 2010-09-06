cmake_minimum_required(VERSION 2.6)

SET ( MAN_PAGES
    ${CMAKE_SOURCE_DIR}/src/man1/q4wine.1
    ${CMAKE_SOURCE_DIR}/src/man1/q4wine-cli.1
    ${CMAKE_SOURCE_DIR}/src/man1/q4wine-helper.1
)

MESSAGE(STATUS "Process man pages --")

IF (USE_GZIP MATCHES ON)
   SET ( ARC gzip )
   SET ( ARC_SUFF gz )
ENDIF (USE_GZIP MATCHES ON)

IF (USE_BZIP2 MATCHES ON)
   SET ( ARC bzip2 )
   SET ( ARC_SUFF bz2 )
ENDIF (USE_BZIP2 MATCHES ON)

IF (ARC)
    find_program( ARC_BIN NAMES ${ARC} )
    MESSAGE(STATUS "Searching for ${ARC} binary --\n")
    IF (${ARC_BIN} MATCHES ARC_BIN-NOTFOUND)
         MESSAGE(STATUS "    ${ARC} : NOT FOUND!\n")
         MESSAGE(FATAL_ERROR "Please install ${ARC} package!")
    ELSE (${ARC_BIN} MATCHES ARC_BIN-NOTFOUND)
         MESSAGE(STATUS "    ${ARC} : ${ARC_BIN}\n")
    ENDIF (${ARC_BIN} MATCHES ARC_BIN-NOTFOUND)

     foreach ( MAN_PAGE ${MAN_PAGES})
         EXECUTE_PROCESS (
         OUTPUT_FILE ${MAN_PAGE}.${ARC_SUFF}
         COMMAND "${ARC_BIN}" -9
         INPUT_FILE ${MAN_PAGE}
         WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/man1/)

         MESSAGE(STATUS "    ${ARC} page ${MAN_PAGE}")
    endforeach()

    install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/man1 DESTINATION
    ${MANPAGE_ENTRY_PATH} FILES_MATCHING PATTERN "*.1.${ARC_SUFF}"
    PATTERN "CMakeFiles" EXCLUDE)

 ELSE (ARC)
    MESSAGE(STATUS "Compresser flags (USE_BZIP2 or USE_GZIP) not set --\n")
    MESSAGE(STATUS "Installing not compressed man pages --\n")

    install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/man1 DESTINATION
    ${MANPAGE_ENTRY_PATH} FILES_MATCHING PATTERN "*.1"
    PATTERN "CMakeFiles" EXCLUDE)
 ENDIF (ARC)
