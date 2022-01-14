QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/application.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/objects/countrydata.cpp \
    src/proxysetter.cpp

HEADERS += \
    src/CoreProcessControler.h \
    src/Params.h \
    src/application.h \
    src/defs.h \
    src/mainwindow.h \
    src/objects/countrydata.h \
    src/proxysetter.h

FORMS += \
    src/mainwindow.ui

TRANSLATIONS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += src/resources.qrc \
    src/objects/countrydata.qrc

macx: QMAKE_INFO_PLIST = src/res/Info.plist
macx: ICON = src/imgs/Icon.icns


#win32: RC_FILE += src/res/win.rc
win32: RC_ICONS += src/imgs/Icon.ico
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

QMAKE_TARGET_COMPANY = SanJuanTech
QMAKE_TARGET_DESCRIPTION = "A Psiphon very cute and for all. http://github.com/rsanjuan87/PsiphonQt"
QMAKE_TARGET_COPYRIGHT = "GPL v3 AS IS SanJuanTech"

#DISTFILES += \
#    src/res/Info.plist \
#    src/imgs/Icon.icns \
#    src/imgs/Icon.ico \
#    src/imgs/dark.png \
#    src/imgs/light.png \
#    src/imgs/logo.png \
#    src/res/winpopup.qss \
#    src/res/winpopup7.qss \
#    src/res/winpopup8.qss \
#    src/res/win.rc


include(./src/QAutostart/QAutostart.pri)
include(./src/QSingleApp/singleapplication.pri)

CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/outputs/debug
    win32:DESTDIR = $$PWD/outputs/debug/win
    macx:DESTDIR = $$PWD/outputs/debug/mac
    linux:DESTDIR = $$PWD/outputs/debug/linux
} else {
    DESTDIR = $$PWD/outputs/release
    win32:DESTDIR = $$PWD/outputs/release/win
    macx:DESTDIR = $$PWD/outputs/release/mac
    linux:DESTDIR = $$PWD/outputs/release/linux
}


