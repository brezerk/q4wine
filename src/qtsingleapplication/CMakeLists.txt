cmake_minimum_required(VERSION 2.8.0)

SET( QSINGLEAPP_LIB_SOURCES
    ${CMAKE_SOURCE_DIR}/src/qtsingleapplication/qtsingleapplication.cpp
    ${CMAKE_SOURCE_DIR}/src/qtsingleapplication/qtlocalpeer.cpp
)

SET( QSINGLEAPP_LIB_HEADERS
    ${CMAKE_SOURCE_DIR}/src/qtsingleapplication/qtsingleapplication.h
    ${CMAKE_SOURCE_DIR}/src/qtsingleapplication/qtlocalpeer.h
)

SET( QSINGLEAPP_LIB_MOC_HEADERS
    ${CMAKE_SOURCE_DIR}/src/qtsingleapplication/qtsingleapplication.h
    ${CMAKE_SOURCE_DIR}/src/qtsingleapplication/qtlocalpeer.h
)

IF( QT5 MATCHES ON )
QT5_WRAP_CPP( QTSINGLEAPPLICATION-LIB-MOC-SRCS ${QSINGLEAPP_LIB_MOC_HEADERS} )
ELSE( QT5 MATCHES ON )
QT4_WRAP_CPP( QTSINGLEAPPLICATION-LIB-MOC-SRCS ${QSINGLEAPP_LIB_MOC_HEADERS} )
ENDIF( QT5 MATCHES ON )

ADD_LIBRARY( qtsingleapp STATIC ${QSINGLEAPP_LIB_SOURCES} ${QSINGLEAPP_LIB_HEADERS} ${QTSINGLEAPPLICATION-LIB-MOC-SRCS}  )
if(QT5 MATCHES ON)
    target_link_libraries(qtsingleapp Qt5::Core Qt5::Network Qt5::Widgets)
endif(QT5 MATCHES ON)