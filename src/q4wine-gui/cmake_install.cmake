# Install script for directory: /home/brezerk/develop/q4wine/src/q4wine-gui

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "0")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/q4wine/i18n" TYPE FILE FILES
    "/home/brezerk/develop/q4wine/src/q4wine-gui/en_us.qm"
    "/home/brezerk/develop/q4wine/src/q4wine-gui/ru_ru.qm"
    "/home/brezerk/develop/q4wine/src/q4wine-gui/uk_ua.qm"
    "/home/brezerk/develop/q4wine/src/q4wine-gui/pt_br.qm"
    "/home/brezerk/develop/q4wine/src/q4wine-gui/es_es.qm"
    "/home/brezerk/develop/q4wine/src/q4wine-gui/it_it.qm"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF(EXISTS "$ENV{DESTDIR}/usr/local/bin/q4wine")
    FILE(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/bin/q4wine"
         RPATH "/usr/local/lib/q4wine")
  ENDIF(EXISTS "$ENV{DESTDIR}/usr/local/bin/q4wine")
  FILE(INSTALL DESTINATION "/usr/local/bin" TYPE EXECUTABLE FILES "/home/brezerk/develop/q4wine/src/q4wine-gui/q4wine")
  IF(EXISTS "$ENV{DESTDIR}/usr/local/bin/q4wine")
    FILE(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/usr/local/bin/q4wine"
         OLD_RPATH "/usr/lib/qt4:/home/brezerk/develop/q4wine/src/q4wine-lib:"
         NEW_RPATH "/usr/local/lib/q4wine")
    IF(CMAKE_INSTALL_DO_STRIP)
      EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/usr/local/bin/q4wine")
    ENDIF(CMAKE_INSTALL_DO_STRIP)
  ENDIF(EXISTS "$ENV{DESTDIR}/usr/local/bin/q4wine")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/applications" TYPE FILE FILES "/home/brezerk/develop/q4wine/q4wine.desktop")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

