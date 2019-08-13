#-------------------------------------------------
#
# Project created by QtCreator 2019-08-10T17:28:11
#
#-------------------------------------------------
QT       += network

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

INSTALLS += target icons qm_files appdesktop startdesktop

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++14
CONFIG += qt warn_on
CONFIG += release
CONFIG += link_pkgconfig
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
    src/mainwindow.cpp \
    src/titlebar.cpp \
    src/contentwidget.cpp \
    src/nowweatherwidget.cpp \
    src/forecastweatherwidget.cpp \
    src/translucentlabel.cpp \
    src/forecastitemwidget.cpp \
    src/tooltip.cpp \
    src/indexitemwidget.cpp \
    src/settingdialog.cpp \
    src/settingtitlebar.cpp \
    src/activebutton.cpp \
    src/searchtitlebar.cpp \
    src/searchinputedit.cpp \
    src/searchview.cpp \
    src/searchmodel.cpp \
    src/searchdelegate.cpp \
    src/locationworker.cpp \
    src/citywidget.cpp \
    src/citylistwidget.cpp \
    src/cityitemwidget.cpp \
    src/spinbox.cpp \
    src/aboutdialog.cpp \
    src/menuaction.cpp \
    src/menuactiongroup.cpp \
    src/global.cpp \
    src/preferences.cpp \
    src/weatherworker.cpp \
    src/networkerrorwidget.cpp \
    src/separator.cpp \
    src/tipwidget.cpp \
    src/texttip.cpp \
    src/tipmodule.cpp \
    src/promptwidget.cpp \
    src/airwidget.cpp \
    src/dbusadaptor.cpp \
    src/automaticlocation.cpp \
    src/maskwidget.cpp

HEADERS  += src/mainwindow.h \
    src/titlebar.h \
    src/contentwidget.h \
    src/nowweatherwidget.h \
    src/forecastweatherwidget.h \
    src/translucentlabel.h \
    src/forecastitemwidget.h \
    src/tooltip.h \
    src/indexitemwidget.h \
    src/settingdialog.h \
    src/settingtitlebar.h \
    src/activebutton.h \
    src/searchtitlebar.h \
    src/searchinputedit.h \
    src/searchview.h \
    src/searchmodel.h \
    src/searchdelegate.h \
    src/data.h \
    src/locationworker.h \
    src/citywidget.h \
    src/citylistwidget.h \
    src/cityitemwidget.h \
    src/spinbox.h \
    src/aboutdialog.h \
    src/menuaction.h \
    src/menuactiongroup.h \
    src/global.h \
    src/preferences.h \
    src/weatherworker.h \
    src/networkerrorwidget.h \
    src/separator.h \
    src/tipwidget.h \
    src/texttip.h \
    src/tipmodule.h \
    src/promptwidget.h \
    src/airwidget.h \
    src/dbusadaptor.h \
    src/automaticlocation.h \
    src/maskwidget.h

RESOURCES += \
    res.qrc

TRANSLATIONS += \
    translation/indicator-china-weather_zh_CN.ts
