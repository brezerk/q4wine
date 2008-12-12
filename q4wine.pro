######################################################################
# Automatically generated by qmake (2.01a) Fri Jun 13 15:18:34 2008
######################################################################

TEMPLATE = app
TARGET = q4wine

DEPENDPATH += . src src/gui src/core src/i18n
INCLUDEPATH += . src/gui src/core srcs
APP_VERSION = "0.107"

# User notification

isEmpty( PREFIX ) {
		PREFIX=/usr
		warning("==============================================")
		warning(" Note: programm install path is: ")
		warning(" " $$PREFIX)
		warning("+--------------------------------------------+")
		warning(" Set PREFIX option for custom path           ")
		warning(" Like this: qmake PREFIX=/you/own/path       ")
		warning("==============================================")
} else {
		warning("==============================================")
		warning(" Note: programm install path is: ")
		warning(" " $$PREFIX)
		warning("==============================================")
}

# Generating config.h

system(./config.sh $$APP_VERSION $$PREFIX)

# Application source code

HEADERS += config.h \
           ui_Proccess.h \
           src/initdb.h \
           src/core/registry.h \
           src/core/winebinlauncher.h \
           src/gui/about.h \
           src/gui/appsettings.h \
           src/gui/iconsettings.h \
           src/gui/iconsview.h \
           src/gui/mainwindow.h \
           src/gui/prefixsettings.h \
           src/gui/process.h \
           src/gui/run.h \
           src/gui/wizard.h

FORMS += src/About.ui \
         src/AppSettings.ui \
         src/IconSettings.ui \
         src/IconsView.ui \
         src/MainWindow.ui \
         src/PrefixSettings.ui \
         src/Process.ui \
         src/Run.ui \
         src/Wizard.ui

SOURCES += src/gui/run.cpp \
           src/main.cpp \
           src/core/registry.cpp \
           src/core/winebinlauncher.cpp \
           src/gui/about.cpp \
           src/gui/appsettings.cpp \
           src/gui/iconsettings.cpp \
           src/gui/iconsview.cpp \
           src/gui/mainwindow.cpp \
           src/gui/prefixsettings.cpp \
           src/gui/process.cpp \
           src/gui/wizard.cpp

# Application resource 

RESOURCES += src/k4wine.qrc
TRANSLATIONS += src/i18n/en_us.ts \
				src/i18n/ru_ru.ts \
                src/i18n/uk_ua.ts

QT += sql gui core network
CONFIG += build_all warn_on debug

DEFINES += DEBUG

i18n.files = src/i18n/*.qm
i18n.path = $$PREFIX/share/$$TARGET/i18n/
i18n.hint = i18n
INSTALLS += i18n

theme.files = src/theme/*
theme.path = $$PREFIX/share/$$TARGET/themes/
theme.hint = data
INSTALLS += theme

target.path = $$PREFIX/bin/
INSTALLS += target
