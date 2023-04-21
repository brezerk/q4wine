cmake_minimum_required(VERSION 2.8.0...3.24)

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

QT5_WRAP_CPP( QTSINGLEAPPLICATION-LIB-MOC-SRCS ${QSINGLEAPP_LIB_MOC_HEADERS} )

ADD_LIBRARY( qtsingleapp STATIC ${QSINGLEAPP_LIB_SOURCES} ${QSINGLEAPP_LIB_HEADERS} ${QTSINGLEAPPLICATION-LIB-MOC-SRCS}  )
target_link_libraries(qtsingleapp Qt5::Core Qt5::Network Qt5::Widgets)
