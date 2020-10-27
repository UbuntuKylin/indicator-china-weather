#-------------------------------------------------
#
# Project created by QtCreator 2019-08-10T17:28:11
#
#-------------------------------------------------
QT       += network
include(src/qtsingleapplication/qtsingleapplication.pri)
isEqual(QT_MAJOR_VERSION, 5) {
    QT += widgets gui core dbus
}

TARGET = indicator-china-weather
TEMPLATE = app

target.source  += $$TARGET
target.path = /usr/bin

!system($$PWD/translations/generate_translations_pm.sh): error("Failed to generate pm")
qm_files.files = translations/*.qm
qm_files.path = /usr/share/indicator-china-weather/translations/

icons.files += res/indicator-china-weather.png
icons.path = /usr/share/pixmaps/

appdesktop.files += indicator-china-weather.desktop
appdesktop.path = /usr/share/applications/

startdesktop.files += indicator-china-weather.desktop
startdesktop.path = /etc/xdg/autostart/

schemes.files += data/org.china-weather-data.gschema.xml
schemes.path = /usr/share/glib-2.0/schemas/
    
INSTALLS += target icons qm_files appdesktop startdesktop schemes

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

LIBS += -lX11

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++14
CONFIG += qt warn_on
CONFIG += release
CONFIG += link_pkgconfig
PKGCONFIG += gsettings-qt
#PKGCONFIG += geoip

LIBS += -lGeoIP

QMAKE_CPPFLAGS *= $(shell dpkg-buildflags --get CPPFLAGS)
QMAKE_CFLAGS   *= $(shell dpkg-buildflags --get CFLAGS)
QMAKE_CXXFLAGS *= $(shell dpkg-buildflags --get CXXFLAGS)
QMAKE_LFLAGS   *= $(shell dpkg-buildflags --get LDFLAGS)

unix {
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
}

SOURCES += src/main.cpp \
    src/cityaddhotview.cpp \
    src/cityaddsearchbox.cpp \
    src/cityaddsearchdelegate.cpp \
    src/cityaddsearchview.cpp \
    src/cityaddwidget.cpp \
    src/citycollectionitem.cpp \
    src/citycollectionwidget.cpp \
    src/darkcityaddsearchdelegate.cpp \
    src/dbusadaptor.cpp \
    src/geoipworker.cpp \
    src/hotcity.cpp \
    src/informationwidget.cpp \
    src/leftupcitybtn.cpp \
    src/leftupsearchbox.cpp \
    src/leftupsearchdelegate.cpp \
    src/leftupsearchview.cpp \
    src/locationworker.cpp \
    src/mainwindow.cpp \
    src/promptwidget.cpp \
    src/weathermanager.cpp \
    src/weatherworker.cpp

HEADERS  += src/mainwindow.h \
    src/cityaddhotview.h \
    src/cityaddsearchbox.h \
    src/cityaddsearchdelegate.h \
    src/cityaddsearchview.h \
    src/cityaddwidget.h \
    src/citycollectionitem.h \
    src/citycollectionwidget.h \
    src/darkcityaddsearchdelegate.h \
    src/data.h \
    src/dbusadaptor.h \
    src/geoipworker.h \
    src/hotcity.h \
    src/informationwidget.h \
    src/leftupcitybtn.h \
    src/leftupsearchbox.h \
    src/leftupsearchdelegate.h \
    src/leftupsearchview.h \
    src/locationworker.h \
    src/promptwidget.h \
    src/weathermanager.h \
    src/weatherworker.h


RESOURCES += \
    res.qrc

TRANSLATIONS += \
    translations/indicator-china-weather_zh_CN.ts \
    translations/indicator-china-weather_bo.ts

FORMS += \
    src/cityaddwidget.ui \
    src/citycollectionitem.ui \
    src/citycollectionwidget.ui \
    src/informationwidget.ui \
    src/mainwindow.ui

DISTFILES +=
