cmake_minimum_required(VERSION 3.24)

SET( DESKTOP_GUI_ENTRY
    ${CMAKE_SOURCE_DIR}/q4wine.desktop
)

SET( APPDATA_ENTRY
    ${CMAKE_SOURCE_DIR}/pkg/flatpak/ua.org.brezblock.q4wine.appdata.xml
)

SET ( Q4WINE_RESOURCE_SOURCES
    ${CMAKE_SOURCE_DIR}/src/q4wine.qrc
)

SET( Q4WINE_GUI_SOURCES
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/run.cpp
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/core/database/db.cpp
    ${CMAKE_SOURCE_DIR}/src/core/database/table.cpp
    ${CMAKE_SOURCE_DIR}/src/core/database/prefix.cpp
    ${CMAKE_SOURCE_DIR}/src/core/database/versions.cpp
    ${CMAKE_SOURCE_DIR}/src/core/database/dir.cpp
    ${CMAKE_SOURCE_DIR}/src/core/database/icon.cpp
    ${CMAKE_SOURCE_DIR}/src/core/database/last_run_icon.cpp
    ${CMAKE_SOURCE_DIR}/src/core/database/image.cpp
    ${CMAKE_SOURCE_DIR}/src/core/database/logging.cpp
    ${CMAKE_SOURCE_DIR}/src/core/database/versions.cpp
    ${CMAKE_SOURCE_DIR}/src/core/database/sysconfig.cpp
    ${CMAKE_SOURCE_DIR}/src/core/registry.cpp
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
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/versions.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/iconlistwidget.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/iconlisttoolbar.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/prefixtreewidget.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/prefixtreetoolbar.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/wineprocesswidget.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/prefixcontrolwidget.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/drivelistwidgetitem.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/loggingwidget.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/prefixconfigwidget.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/winetricks.cpp
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/fakedrivesettings.cpp
)

SET ( Q4WINE_GUI_HEADERS
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/run.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/authors.h
    ${CMAKE_SOURCE_DIR}/src/core/registry.h
    ${CMAKE_SOURCE_DIR}/src/core/database/db.h
    ${CMAKE_SOURCE_DIR}/src/core/database/table.h
    ${CMAKE_SOURCE_DIR}/src/core/database/versions.h
    ${CMAKE_SOURCE_DIR}/src/core/database/prefix.h
    ${CMAKE_SOURCE_DIR}/src/core/database/dir.h
    ${CMAKE_SOURCE_DIR}/src/core/database/icon.h
    ${CMAKE_SOURCE_DIR}/src/core/database/last_run_icon.h
    ${CMAKE_SOURCE_DIR}/src/core/database/image.h
    ${CMAKE_SOURCE_DIR}/src/core/database/logging.h
    ${CMAKE_SOURCE_DIR}/src/core/database/versions.h
    ${CMAKE_SOURCE_DIR}/src/core/database/sysconfig.h
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
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/versions.h
    ${PROJECT_BINARY_DIR}/config.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/iconlistwidget.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/iconlisttoolbar.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/prefixtreewidget.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/prefixtreetoolbar.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/wineprocesswidget.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/prefixcontrolwidget.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/drivelistwidgetitem.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/loggingwidget.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/prefixconfigwidget.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/winetricks.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/fakedrivesettings.h
)

SET ( Q4WINE_GUI_MOC_HEADEDRS
    ${CMAKE_SOURCE_DIR}/src/core/registry.h
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
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/versions.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/iconlistwidget.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/iconlisttoolbar.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/prefixtreewidget.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/prefixtreetoolbar.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/wineprocesswidget.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/prefixcontrolwidget.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/drivelistwidgetitem.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/loggingwidget.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/prefixconfigwidget.h
    ${CMAKE_SOURCE_DIR}/src/core/database/db.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/fakedrivesettings.h
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/widgets/winetricks.h
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
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/VersionManager.ui
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/Winetricks.ui
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
        ${CMAKE_SOURCE_DIR}/src/core/xmlparser.cpp
        ${CMAKE_SOURCE_DIR}/src/core/httpcore.cpp
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
        ${CMAKE_SOURCE_DIR}/src/core/xmlparser.h
        ${CMAKE_SOURCE_DIR}/src/core/httpcore.h
        ${CMAKE_SOURCE_DIR}/src/core/appdbstructs.h
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
        ${CMAKE_SOURCE_DIR}/src/core/xmlparser.h
        ${CMAKE_SOURCE_DIR}/src/core/httpcore.h
    )

    SET ( Q4WINE_GUI_UI_SOURCES
        ${Q4WINE_GUI_UI_SOURCES}
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/AppInfoWidget.ui
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/AppTestWidget.ui
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis/CommentWidget.ui
    )
ENDIF (${WITH_WINEAPPDB} MATCHES ON )

IF( NOT ${CMAKE_SYSTEM_NAME} MATCHES Darwin )
    SET ( Q4WINE_GUI_SOURCES
        ${Q4WINE_GUI_SOURCES}
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/sysmenu.cpp
    )
    SET ( Q4WINE_GUI_HEADERS
        ${Q4WINE_GUI_HEADERS}
        ${CMAKE_SOURCE_DIR}/src/q4wine-gui/sysmenu.h
    )
ENDIF( NOT ${CMAKE_SYSTEM_NAME} MATCHES Darwin )

add_custom_target (
    authors
    COMMAND ${CMAKE_SOURCE_DIR}/tools/gen_authors.py --generate
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

QT6_ADD_RESOURCES( RCC_SRCS ${Q4WINE_RESOURCE_SOURCES} )

SET ( CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_SOURCE_DIR}/src/q4wine-gui/uis
)

QT6_WRAP_UI( Q4WINE_GUI_UI_HEADERS ${Q4WINE_GUI_UI_SOURCES} )
QT6_WRAP_CPP( Q4WINE_GUI_MOC_SOURCES ${Q4WINE_GUI_MOC_HEADEDRS} )
ADD_EXECUTABLE( q4wine ${Q4WINE_GUI_SOURCES} ${Q4WINE_GUI_HEADERS} ${Q4WINE_GUI_UI_HEADERS} ${Q4WINE_GUI_MOC_SOURCES} ${RCC_SRCS} ${QM_L10N} )

IF (${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)
    SET(BSD_LIBS -lkvm)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)

target_link_libraries(q4wine PRIVATE q4wine-core Qt6::Core Qt6::DBus Qt6::Network Qt6::Sql Qt6::Widgets Qt6::Xml SingleApplication::SingleApplication ${BSD_LIBS})

IF( RELEASE )
    #install(TARGETS q4wine RUNTIME DESTINATION ${APPS_ENTRY_PATH} BUNDLE DESTINATION ${CMAKE_INSTALL_PREFIX})
    install(TARGETS q4wine DESTINATION ${APPS_ENTRY_PATH})
    install(FILES ${DESKTOP_GUI_ENTRY} DESTINATION ${DESKTOP_ENTRY_PATH})
    install(FILES ${APPDATA_ENTRY} DESTINATION ${APPDATA_ENTRY_PATH})
    set(CPACK_BINARY_DRAGNDROP ON)
ENDIF( RELEASE )
