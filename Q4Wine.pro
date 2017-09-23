#-------------------------------------------------
#
# Project created by QtCreator 2013-03-20T04:30:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Q4Wine
TEMPLATE = app


SOURCES += src/main.cpp\
        src/q4wine-gui/mainwindow.cpp
        

HEADERS  += src/q4wine-gui/mainwindow.h

FORMS    += src/q4wine-gui/uis/MainWindow.ui

FORMS += \
    src/q4wine-gui/uis/Wizard.ui \
    src/q4wine-gui/uis/WineDriveDialog.ui \
    src/q4wine-gui/uis/Run.ui \
    src/q4wine-gui/uis/Process.ui \
    src/q4wine-gui/uis/PrefixSettings.ui \
    src/q4wine-gui/uis/InfoDialog.ui \
    src/q4wine-gui/uis/ImageManager.ui \
    src/q4wine-gui/uis/IconsView.ui \
    src/q4wine-gui/uis/IconSettings.ui \
    src/q4wine-gui/uis/FakeDriveSettings.ui \
    src/q4wine-gui/uis/CommentWidget.ui \
    src/q4wine-gui/uis/AppTestWidget.ui \
    src/q4wine-gui/uis/AppSettings.ui \
    src/q4wine-gui/uis/AppInfoWidget.ui \
    src/q4wine-gui/uis/About.ui

RESOURCES += \
    src/q4wine.qrc \
    src/q4wine-gui/uis/q4wine.qrc

SOURCES += \
    src/plugins/sysmenu.cpp \
    src/plugins/winetricks.cpp

SOURCES += \
    src/core/xmlparser.cpp \
    src/core/registry.cpp \
    src/core/httpcore.cpp

HEADERS += \
    src/core/xmlparser.h \
    src/core/registry.h \
    src/core/httpcore.h \
    src/core/appdbstructs.h

SOURCES += \
    src/darwin/ps.c

HEADERS += \
    src/darwin/ps.h

OTHER_FILES += \
    src/data/zoom-out.png \
    src/data/zoom-in.png \
    src/data/wordpad.png \
    src/data/wizard.png \
    src/data/winetricks-install.png \
    src/data/winemine.png \
    src/data/wineconsole.png \
    src/data/winecfg.png \
    src/data/wineappdb.png \
    src/data/wineappdb-app.png \
    src/data/wine16x16.png \
    src/data/wine.png \
    src/data/view-list-icons.png \
    src/data/view-list-details.png \
    src/data/up.png \
    src/data/uninstaller.png \
    src/data/tools-report-bug.png \
    src/data/terminal.png \
    src/data/taskmgr.png \
    src/data/stop.png \
    src/data/sort-desc.png \
    src/data/sort-create-desc.png \
    src/data/sort-create-asc.png \
    src/data/sort-asc.png \
    src/data/remove.png \
    src/data/reload.png \
    src/data/regedit.png \
    src/data/rebuild.png \
    src/data/q4wine.png \
    src/data/q4wine-128x128.png \
    src/data/prefixc.png \
    src/data/prefix.png \
    src/data/oleview.png \
    src/data/ok.png \
    src/data/notepad.png \
    src/data/love.png \
    src/data/logo.png \
    src/data/list.png \
    src/data/ledorange.png \
    src/data/ledgreen.png \
    src/data/kill.png \
    src/data/iso_manager.png \
    src/data/important.png \
    src/data/import.png \
    src/data/iexplore.png \
    src/data/help.png \
    src/data/go-home.png \
    src/data/folder.png \
    src/data/folder-import.png \
    src/data/folder-downloads.png \
    src/data/folder_open.png \
    src/data/firstc.png \
    src/data/find.png \
    src/data/filter.png \
    src/data/fileopen.png \
    src/data/fakedrive-update.png \
    src/data/fakedrive-new.png \
    src/data/fail.png \
    src/data/export.png \
    src/data/explorer.png \
    src/data/expand.png \
    src/data/exec.png \
    src/data/exec_wine.png \
    src/data/eject.png \
    src/data/drive_menu.png \
    src/data/down.png \
    src/data/down_log.png \
    src/data/date.png \
    src/data/control.png \
    src/data/configure.png \
    src/data/collapse.png \
    src/data/clear-rtl.png \
    src/data/clear-ltr.png \
    src/data/clear-list.png \
    src/data/cdrom_menu.png \
    src/data/bookmark-new.png \
    src/data/blockdevice.png \
    src/data/add.png

OTHER_FILES += \
    src/freebsd/umount.sh \
    src/freebsd/umount_freebsd.sh \
    src/freebsd/mount_image.sh

OTHER_FILES += \
    src/man1/q4wine.1 \
    src/man1/q4wine-helper.1 \
    src/man1/q4wine-cli.1

HEADERS += \
    src/q4wine-cli/q4wine-cli.h

SOURCES += \
    src/q4wine-cli/q4wine-cli.cpp

FORMS += \
    src/plugins/winetricks.ui

HEADERS += \
    src/plugins/winetricks.h \
    src/plugins/sysmenu.h

OTHER_FILES += \
    src/plugins/CMakeLists.txt

SOURCES += \
    src/q4wine-gui/wizard.cpp \
    src/q4wine-gui/winedrivedialog.cpp \
    src/q4wine-gui/run.cpp \
    src/q4wine-gui/progress.cpp \
    src/q4wine-gui/process.cpp \
    src/q4wine-gui/prefixsettings.cpp \
    src/q4wine-gui/infodialog.cpp \
    src/q4wine-gui/imagemanager.cpp \
    src/q4wine-gui/iconsview.cpp \
    src/q4wine-gui/iconsettings.cpp \
    src/q4wine-gui/fakedrivesettings.cpp \
    src/q4wine-gui/appsettings.cpp \
    src/q4wine-gui/about.cpp \
    src/q4wine-gui/versions.cpp

HEADERS += \
    src/q4wine-gui/wizard.h \
    src/q4wine-gui/winedrivedialog.h \
    src/q4wine-gui/run.h \
    src/q4wine-gui/progress.h \
    src/q4wine-gui/process.h \
    src/q4wine-gui/prefixsettings.h \
    src/q4wine-gui/infodialog.h \
    src/q4wine-gui/imagemanager.h \
    src/q4wine-gui/iconsview.h \
    src/q4wine-gui/iconsettings.h \
    src/q4wine-gui/fakedrivesettings.h \
    src/q4wine-gui/appsettings.h \
    src/q4wine-gui/about.h \
    src/q4wine-gui/versions.h

OTHER_FILES += \
    src/q4wine-gui/CMakeLists.txt

SOURCES += \
    src/q4wine-helper/wineobject.cpp \
    src/q4wine-helper/q4wine-helper.cpp

HEADERS += \
    src/q4wine-helper/wineobject.h \
    src/q4wine-helper/q4wine-helper.h

SOURCES += \
    src/q4wine-lib/q4wine-lib.cpp

HEADERS += \
    src/q4wine-lib/q4wine-lib.h

SOURCES += \
    src/qtsingleapplication/qtsingleapplication.cpp \
    src/qtsingleapplication/qtlockedfile.cpp \
    src/qtsingleapplication/qtlockedfile_win.cpp \
    src/qtsingleapplication/qtlockedfile_unix.cpp \
    src/qtsingleapplication/qtlocalpeer.cpp

HEADERS += \
    src/qtsingleapplication/qtsingleapplication.h \
    src/qtsingleapplication/QtSingleApplication \
    src/qtsingleapplication/qtlockedfile.h \
    src/qtsingleapplication/QtLockedFile \
    src/qtsingleapplication/qtlocalpeer.h

OTHER_FILES += \
    src/qtsingleapplication/README.TXT \
    src/qtsingleapplication/LICENSE.GPL3

SOURCES += \
    src/core/database/sysconfig.cpp \
    src/core/database/prefix.cpp \
    src/core/database/logging.cpp \
    src/core/database/last_run_icon.cpp \
    src/core/database/image.cpp \
    src/core/database/icon.cpp \
    src/core/database/dir.cpp \
    src/core/database/db.cpp

HEADERS += \
    src/core/database/sysconfig.h \
    src/core/database/prefix.h \
    src/core/database/logging.h \
    src/core/database/last_run_icon.h \
    src/core/database/image.h \
    src/core/database/icon.h \
    src/core/database/dir.h \
    src/core/database/db.h

OTHER_FILES += \
    src/CMakeLists.txt

OTHER_FILES += \
    src/data/CMakeLists.txt

OTHER_FILES += \
    src/freebsd/CMakeLists.txt

OTHER_FILES += \
    src/i18n/CMakeLists.txt

OTHER_FILES += \
    src/man1/CMakeLists.txt

OTHER_FILES += \
    src/qtsingleapplication/CMakeLists.txt

OTHER_FILES += \
    src/theme/CMakeLists.txt

OTHER_FILES += \
    src/theme/nuvola/data/wizard.png \
    src/theme/nuvola/data/wine.png \
    src/theme/nuvola/data/up.png \
    src/theme/nuvola/data/theme.info \
    src/theme/nuvola/data/stop.png \
    src/theme/nuvola/data/reload.png \
    src/theme/nuvola/data/rebuild.png \
    src/theme/nuvola/data/prefix.png \
    src/theme/nuvola/data/power.png \
    src/theme/nuvola/data/kill.png \
    src/theme/nuvola/data/folder.png \
    src/theme/nuvola/data/folder_open.png \
    src/theme/nuvola/data/exit.png \
    src/theme/nuvola/data/exec.png \
    src/theme/nuvola/data/exec_wine.png \
    src/theme/nuvola/data/edit.png \
    src/theme/nuvola/data/down.png \
    src/theme/nuvola/data/configure.png

SOURCES += \
    src/q4wine-gui/widgets/appdb/linkitemwidget.cpp \
    src/q4wine-gui/widgets/appdb/lineitemwidget.cpp \
    src/q4wine-gui/widgets/appdb/commentwidget.cpp \
    src/q4wine-gui/widgets/appdb/apptestwidget.cpp \
    src/q4wine-gui/widgets/appdb/appinfowidget.cpp \
    src/q4wine-gui/widgets/appdb/appdbwidget.cpp \
    src/q4wine-gui/widgets/appdb/appdbscrollwidget.cpp \
    src/q4wine-gui/widgets/appdb/appdbheaderwidget.cpp

HEADERS += \
    src/q4wine-gui/widgets/appdb/linkitemwidget.h \
    src/q4wine-gui/widgets/appdb/lineitemwidget.h \
    src/q4wine-gui/widgets/appdb/commentwidget.h \
    src/q4wine-gui/widgets/appdb/apptestwidget.h \
    src/q4wine-gui/widgets/appdb/appinfowidget.h \
    src/q4wine-gui/widgets/appdb/appdbwidget.h \
    src/q4wine-gui/widgets/appdb/appdbscrollwidget.h \
    src/q4wine-gui/widgets/appdb/appdbheaderwidget.h

SOURCES += \
    src/q4wine-gui/widgets/wineprocesswidget.cpp \
    src/q4wine-gui/widgets/prefixtreewidget.cpp \
    src/q4wine-gui/widgets/prefixtreetoolbar.cpp \
    src/q4wine-gui/widgets/prefixcontrolwidget.cpp \
    src/q4wine-gui/widgets/prefixconfigwidget.cpp \
    src/q4wine-gui/widgets/loggingwidget.cpp \
    src/q4wine-gui/widgets/iconlistwidget.cpp \
    src/q4wine-gui/widgets/iconlisttoolbar.cpp \
    src/q4wine-gui/widgets/drivelistwidgetitem.cpp

HEADERS += \
    src/q4wine-gui/widgets/wineprocesswidget.h \
    src/q4wine-gui/widgets/prefixtreewidget.h \
    src/q4wine-gui/widgets/prefixtreetoolbar.h \
    src/q4wine-gui/widgets/prefixcontrolwidget.h \
    src/q4wine-gui/widgets/prefixconfigwidget.h \
    src/q4wine-gui/widgets/loggingwidget.h \
    src/q4wine-gui/widgets/iconlistwidget.h \
    src/q4wine-gui/widgets/iconlisttoolbar.h \
    src/q4wine-gui/widgets/drivelistwidgetitem.h
