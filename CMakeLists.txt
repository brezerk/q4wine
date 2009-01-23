cmake_minimum_required(VERSION 2.6)                     

PROJECT(q4wine)

SET(CMAKE_VERBOSE_MAKEFILE ON)

SET( SOURCES
        ${CMAKE_SOURCE_DIR}/src/gui/run.cpp
        ${CMAKE_SOURCE_DIR}/src/main.cpp   
        ${CMAKE_SOURCE_DIR}/src/core/registry.cpp
        ${CMAKE_SOURCE_DIR}/src/core/winebinlauncher.cpp
        ${CMAKE_SOURCE_DIR}/src/gui/about.cpp           
        ${CMAKE_SOURCE_DIR}/src/gui/appsettings.cpp     
        ${CMAKE_SOURCE_DIR}/src/gui/iconsettings.cpp    
        ${CMAKE_SOURCE_DIR}/src/gui/iconsview.cpp       
        ${CMAKE_SOURCE_DIR}/src/gui/mainwindow.cpp      
        ${CMAKE_SOURCE_DIR}/src/gui/prefixsettings.cpp  
        ${CMAKE_SOURCE_DIR}/src/gui/process.cpp         
        ${CMAKE_SOURCE_DIR}/src/gui/wizard.cpp          
        ${CMAKE_SOURCE_DIR}/src/gui/imagemanager.cpp    
        ${CMAKE_SOURCE_DIR}/src/temporary/winetricks.cpp
        ${CMAKE_SOURCE_DIR}/src/core/coremethods.cpp
	${CMAKE_SOURCE_DIR}/src/wis/wisitem.cpp
	#	${CMAKE_SOURCE_DIR}/src/core/commandline.cpp
)                                                       

SET ( HEADERS_DIR
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/gui
        ${CMAKE_SOURCE_DIR}/src/core
        ${CMAKE_SOURCE_DIR}/src/temporary
	${CMAKE_SOURCE_DIR}/src/wis
)                                        

SET ( MOC_HEADEDRS
        ${CMAKE_SOURCE_DIR}/src/core/registry.h
        ${CMAKE_SOURCE_DIR}/src/core/winebinlauncher.h
        ${CMAKE_SOURCE_DIR}/src/gui/appsettings.h     
        ${CMAKE_SOURCE_DIR}/src/gui/iconsettings.h    
        ${CMAKE_SOURCE_DIR}/src/gui/iconsview.h       
        ${CMAKE_SOURCE_DIR}/src/gui/mainwindow.h      
        ${CMAKE_SOURCE_DIR}/src/gui/prefixsettings.h  
        ${CMAKE_SOURCE_DIR}/src/gui/process.h         
        ${CMAKE_SOURCE_DIR}/src/gui/about.h           
        ${CMAKE_SOURCE_DIR}/src/gui/run.h             
        ${CMAKE_SOURCE_DIR}/src/gui/wizard.h          
        ${CMAKE_SOURCE_DIR}/src/gui/imagemanager.h    
        ${CMAKE_SOURCE_DIR}/src/temporary/winetricks.h
        ${CMAKE_SOURCE_DIR}/src/core/coremethods.h
	#${CMAKE_SOURCE_DIR}/src/wis/wisitem.h
	#	${CMAKE_SOURCE_DIR}/src/core/commandline.h
)                                                     

SET ( UIS
        ${CMAKE_SOURCE_DIR}/src/About.ui
        ${CMAKE_SOURCE_DIR}/src/AppSettings.ui
        ${CMAKE_SOURCE_DIR}/src/IconSettings.ui
        ${CMAKE_SOURCE_DIR}/src/IconsView.ui   
        ${CMAKE_SOURCE_DIR}/src/MainWindow.ui  
        ${CMAKE_SOURCE_DIR}/src/PrefixSettings.ui
        ${CMAKE_SOURCE_DIR}/src/Process.ui       
        ${CMAKE_SOURCE_DIR}/src/Run.ui           
        ${CMAKE_SOURCE_DIR}/src/Wizard.ui        
        ${CMAKE_SOURCE_DIR}/src/IconSettings.ui  
        ${CMAKE_SOURCE_DIR}/src/ImageManager.ui  
        ${CMAKE_SOURCE_DIR}/src/temporary/winetricks.ui
)                                                      

SET ( RCCS
        ${CMAKE_SOURCE_DIR}/src/q4wine.qrc
)                                         

SET ( I18N
        ${CMAKE_SOURCE_DIR}/src/i18n/en_us.ts
        ${CMAKE_SOURCE_DIR}/src/i18n/ru_ru.ts
        ${CMAKE_SOURCE_DIR}/src/i18n/uk_ua.ts
)                                            

MESSAGE("")
MESSAGE(STATUS " * Utility for management wine applications and prefixes")
MESSAGE(STATUS " * (C) 2008-2009, brezblock core team")                   
MESSAGE(STATUS " * http://brezblock.org.ua/")                             
MESSAGE("")                                                               

SET(QT_MIN_VERSION "4.4.1")
FIND_PACKAGE(Qt4 REQUIRED) 
        IF(QT_FOUND)       
                MESSAGE(STATUS "Found Qt. Using version: ${QTVERSION}")
                IF(QT_QTSQL_FOUND)                                     
                        SET(QT_USE_QTSQL 1)                            
                ELSE(QT_QTSQL_FOUND)                                   
                        MESSAGE(FATAL ERROR "Qt SQL libraries missing.")
                ENDIF(QT_QTSQL_FOUND)                                   

                IF(QT_QTGUI_FOUND)
                        SET(QT_USE_QTGUI 1)
                ELSE(QT_QTGUI_FOUND)       
                        MESSAGE(FATAL ERROR "Qt GUI libraries missing.")
                ENDIF(QT_QTGUI_FOUND)                                   

                IF(QT_QTNETWORK_FOUND)
                        SET(QT_USE_QTNETWORK 1)
                ELSE(QT_QTNETWORK_FOUND)       
                        MESSAGE(FATAL ERROR "Qt NetWork libraries missing.")
                ENDIF(QT_QTNETWORK_FOUND)                                   

                IF(QT_QTCORE_FOUND)
                        SET(QT_USE_QTCORE 1)
                ELSE(QT_QTCORE_FOUND)       
                        MESSAGE(FATAL ERROR "Qt Core libraries missing.")
                ENDIF(QT_QTCORE_FOUND)                                   

                INCLUDE(${QT_USE_FILE})
        ELSE(QT_FOUND)                 
                MESSAGE(FATAL ERROR "Qt4 not found.")
        ENDIF(QT_FOUND)

SET(APP_VERSION "0.110")

MESSAGE(STATUS "Build realm is: ${CMAKE_SYSTEM_NAME}")

IF (${CMAKE_SYSTEM_NAME} MATCHES Linux)
	SET(_OS_LINUX_ 1)
	ADD_DEFINITIONS(-Wall -g)
ENDIF (${CMAKE_SYSTEM_NAME} MATCHES Linux)

IF (${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)
        SET(_OS_FREEBSD_ 1)
	ADD_DEFINITIONS(-Wall -g -DKERNEL)
ENDIF (${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)

IF (${CMAKE_SYSTEM_NAME} MATCHES Darwin)
        SET(_OS_DARWIN_ 1)
	ADD_DEFINITIONS(-Wall -g -DKERNEL)
ENDIF (${CMAKE_SYSTEM_NAME} MATCHES Darwin)

configure_file(${CMAKE_SOURCE_DIR}/config.h.cmake ${CMAKE_SOURCE_DIR}/config.h )


file(APPEND ${CMAKE_SOURCE_DIR}/config.h "#define APP_VERS \"${APP_VERSION}\"\n")
        MESSAGE(STATUS "q4wine core version: ${APP_VERSION}")
file(APPEND ${CMAKE_SOURCE_DIR}/config.h "#define APP_PREF \"${CMAKE_INSTALL_PREFIX}\"\n" )
        MESSAGE("")
        MESSAGE(STATUS "Install prefix is: ${CMAKE_INSTALL_PREFIX}")
        MESSAGE(STATUS "NOTE: You can set prefix by adding -DCMAKE_INSTALL_PREFIX=/usr/ option to cmake command")
        MESSAGE("")

file(APPEND ${CMAKE_SOURCE_DIR}/config.h "\n#endif\n" )

QT4_WRAP_UI( UI_HEADERS ${UIS})
QT4_WRAP_CPP( MOC_SRCS ${MOC_HEADEDRS} )

QT4_ADD_RESOURCES( RCC_SRCS ${RCCS} )
QT4_ADD_TRANSLATION( QM_I18N ${I18N})

INCLUDE_DIRECTORIES( ${HEADERS_DIR} )
INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR} )

ADD_EXECUTABLE( q4wine ${SOURCES} ${UI_HEADERS} ${MOC_SRCS} ${RCC_SRCS}
${QM_I18N})

IF (${CMAKE_SYSTEM_NAME} MATCHES Linux)
	TARGET_LINK_LIBRARIES( q4wine ${QT_LIBRARIES} )
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES Linux)

IF (${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)
	SET(BSD_LIBS -lkvm)
        TARGET_LINK_LIBRARIES( q4wine ${QT_LIBRARIES} ${BSD_LIBS} )
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)

install(TARGETS q4wine DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/theme DESTINATION
${CMAKE_INSTALL_PREFIX}/share/q4wine)
install(FILES ${QM_I18N} DESTINATION
${CMAKE_INSTALL_PREFIX}/share/q4wine/i18n)
 
