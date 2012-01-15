cmake_minimum_required(VERSION 2.6)

SET( DESKTOP_GUI_ENTRY
    ${CMAKE_SOURCE_DIR}/q4wine.desktop
)

SET ( Q4WINE_RESOURCE_SOURCES
    ${CMAKE_SOURCE_DIR}/src/q4wine.qrc
)

SET( Q4WINE_GUI_SOURCES
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/run.cpp
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/core/database/db.cpp
    ${CMAKE_SOURCE_DIR}/src/core/database/prefix.cpp
    ${CMAKE_SOURCE_DIR}/src/core/database/dir.cpp
    ${CMAKE_SOURCE_DIR}/src/core/database/icon.cpp
    ${CMAKE_SOURCE_DIR}/src/core/database/last_run_icon.cpp
    ${CMAKE_SOURCE_DIR}/src/core/database/image.cpp
    ${CMAKE_SOURCE_DIR}/src/core/database/logging.cpp
    ${CMAKE_SOURCE_DIR}/src/core/registry.cpp
    ${CMAKE_SOURCE_DIR}/src/core/xmlparser.cpp
    ${CMAKE_SOURCE_DIR}/src/core/httpcore.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/about.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/appsettings.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/iconsettings.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/iconsview.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/mainwindow.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/prefixsettings.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/process.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/progress.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/wizard.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/imagemanager.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/winedrivedialog.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/iconlistwidget.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/iconlisttoolbar.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/prefixtreewidget.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/prefixtreetoolbar.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/wineprocesswidget.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/prefixcontrolwidget.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/drivelistwidgetitem.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/loggingwidget.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/fakedrivesettings.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/infodialog.cpp
)

SET ( Q4WINE_GUI_HEADERS
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/run.h
    ${CMAKE_SOURCE_DIR}/src/core/registry.h
    ${CMAKE_SOURCE_DIR}/src/core/database/db.h
    ${CMAKE_SOURCE_DIR}/src/core/database/prefix.h
    ${CMAKE_SOURCE_DIR}/src/core/database/dir.h
    ${CMAKE_SOURCE_DIR}/src/core/database/icon.h
    ${CMAKE_SOURCE_DIR}/src/core/database/last_run_icon.h
    ${CMAKE_SOURCE_DIR}/src/core/database/image.h
    ${CMAKE_SOURCE_DIR}/src/core/database/logging.h
    ${CMAKE_SOURCE_DIR}/src/core/appdbstructs.h
    ${CMAKE_SOURCE_DIR}/src/core/xmlparser.h
    ${CMAKE_SOURCE_DIR}/src/core/httpcore.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/about.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/appsettings.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/iconsettings.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/iconsview.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/mainwindow.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/prefixsettings.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/process.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/progress.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/wizard.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/imagemanager.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/winedrivedialog.h
    ${CMAKE_SOURCE_DIR}/config.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/iconlistwidget.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/iconlisttoolbar.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/prefixtreewidget.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/prefixtreetoolbar.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/wineprocesswidget.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/prefixcontrolwidget.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/drivelistwidgetitem.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/loggingwidget.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/fakedrivesettings.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/infodialog.h
)

SET ( Q4WINE_GUI_MOC_HEADEDRS
    ${CMAKE_SOURCE_DIR}/src/core/registry.h
    ${CMAKE_SOURCE_DIR}/src/core/xmlparser.h
    ${CMAKE_SOURCE_DIR}/src/core/httpcore.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/appsettings.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/iconsettings.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/iconsview.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/mainwindow.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/prefixsettings.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/process.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/progress.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/about.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/run.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/wizard.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/imagemanager.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/winedrivedialog.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/iconlistwidget.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/iconlisttoolbar.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/prefixtreewidget.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/prefixtreetoolbar.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/wineprocesswidget.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/prefixcontrolwidget.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/drivelistwidgetitem.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/loggingwidget.h
    ${CMAKE_SOURCE_DIR}/src/core/database/db.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/fakedrivesettings.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/infodialog.h
)

SET ( Q4WINE_GUI_UI_SOURCES
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/About.ui
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/AppSettings.ui
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/IconSettings.ui
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/IconsView.ui
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/MainWindow.ui
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/PrefixSettings.ui
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/FakeDriveSettings.ui
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/Process.ui
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/Run.ui
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/Wizard.ui
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/IconSettings.ui
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/ImageManager.ui
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/WineDriveDialog.ui
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/InfoDialog.ui
)

IF (${WITH_WINEAPPDB} MATCHES ON )
    SET( Q4WINE_GUI_SOURCES
        ${Q4WINE_GUI_SOURCES}
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/appdb/appdbwidget.cpp
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/appdb/linkitemwidget.cpp
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/appdb/appdbscrollwidget.cpp
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/appdb/appdbheaderwidget.cpp
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/appdb/appinfowidget.cpp
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/appdb/lineitemwidget.cpp
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/appdb/apptestwidget.cpp
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/appdb/commentwidget.cpp
    )

    SET ( Q4WINE_GUI_HEADERS
        ${Q4WINE_GUI_HEADERS}
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/appdb/appdbwidget.h
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/appdb/linkitemwidget.h
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/appdb/appdbscrollwidget.h
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/appdb/appdbheaderwidget.h
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/appdb/appinfowidget.h
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/appdb/lineitemwidget.h
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/appdb/apptestwidget.h
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/appdb/commentwidget.h
    )

    SET ( Q4WINE_GUI_MOC_HEADEDRS
        ${Q4WINE_GUI_MOC_HEADEDRS}
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/appdb/appdbwidget.h
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/appdb/linkitemwidget.h
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/appdb/appdbscrollwidget.h
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/appdb/appdbheaderwidget.h
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/appdb/appinfowidget.h
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/appdb/lineitemwidget.h
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/appdb/apptestwidget.h
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/appdb/commentwidget.h
    )

    SET ( Q4WINE_GUI_UI_SOURCES
        ${Q4WINE_GUI_UI_SOURCES}
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/AppInfoWidget.ui
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/AppTestWidget.ui
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/CommentWidget.ui
    )
ENDIF (${WITH_WINEAPPDB} MATCHES ON )

QT4_ADD_RESOURCES( RCC_SRCS ${Q4WINE_RESOURCE_SOURCES} )

QT4_WRAP_UI( Q4WINE_GUI_UI_HEADERS ${Q4WINE_GUI_UI_SOURCES} ${PLUGINS_GUI_UI_SOURCES} )
QT4_WRAP_CPP( Q4WINE_GUI_MOC_SOURCES ${Q4WINE_GUI_MOC_HEADEDRS} ${PLUGINS_GUI_MOC_HEADEDRS} )
ADD_EXECUTABLE( q4wine ${Q4WINE_GUI_SOURCES} ${Q4WINE_GUI_HEADERS} ${Q4WINE_GUI_UI_HEADERS} ${Q4WINE_GUI_MOC_SOURCES} ${RCC_SRCS} ${QM_I18N} ${PLUGINS_GUI_SOURCES} ${PLUGINS_GUI_HEADERS} ${PLUGINS_GUI_UI_HEADERS} ${PLUGINS_GUI_MOC_SOURCES})
add_dependencies( q4wine translations_build )

IF (${CMAKE_SYSTEM_NAME} MATCHES Linux)
    TARGET_LINK_LIBRARIES( q4wine  ${QT_LIBRARIES} q4wine-core ${QTSINGLEAPPLIB})
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES Linux)

IF (${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)
    SET(BSD_LIBS -lkvm)
    TARGET_LINK_LIBRARIES( q4wine ${QT_LIBRARIES} ${BSD_LIBS} q4wine-core ${QTSINGLEAPPLIB})
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)

IF (${CMAKE_SYSTEM_NAME} MATCHES Darwin)
    TARGET_LINK_LIBRARIES( q4wine ${QT_LIBRARIES} q4wine-core ${QTSINGLEAPPLIB})
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES Darwin)

IF( RELEASE )
    #install(TARGETS q4wine RUNTIME DESTINATION ${APPS_ENTRY_PATH} BUNDLE DESTINATION ${CMAKE_INSTALL_PREFIX})
    install(TARGETS q4wine DESTINATION ${APPS_ENTRY_PATH})
    install(FILES ${DESKTOP_GUI_ENTRY} DESTINATION ${DESKTOP_ENTRY_PATH})
    set(CPACK_BINARY_DRAGNDROP ON)
ENDIF( RELEASE )
