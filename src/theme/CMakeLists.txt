cmake_minimum_required(VERSION 2.6)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/theme DESTINATION ${CMAKE_INSTALL_PREFIX}/share/q4wine FILES_MATCHING PATTERN "*.png")
