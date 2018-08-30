#-------------------------------------------------
#
# Project created by QtCreator 2018-08-20T11:02:46
#
#-------------------------------------------------

QT       += network

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

isEqual(QT_MAJOR_VERSION, 5) {
    QT += widgets gui core
}

TARGET = indicator-china-weather
TEMPLATE = app

target.files += indicator-china-wether
target.path = /usr/bin/

icons.files += res/indicator-china-wether.png
icons.path = /usr/share/pixmaps/

desktop.files += indicator-china-wether.desktop
desktop.path = /usr/share/applications/

INSTALLS += target icons desktop

CONFIG += c++14
CONFIG += qt warn_on
CONFIG += release

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
#    src/indexweatherwidget.cpp \
    src/translucentlabel.cpp \
    src/forecastitemwidget.cpp \
    src/tooltip.cpp \
    src/indexitemwidget.cpp \
    src/settingdialog.cpp \
    src/settingtitlebar.cpp \
    src/activebutton.cpp \
    src/searchdialog.cpp \
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
    src/citieslist.cpp \
    src/weatherworker.cpp \
    src/networkerrorwidget.cpp \
    src/separator.cpp

HEADERS  += src/mainwindow.h \
    src/titlebar.h \
    src/contentwidget.h \
    src/nowweatherwidget.h \
    src/forecastweatherwidget.h \
#    src/indexweatherwidget.h \
    src/translucentlabel.h \
    src/forecastitemwidget.h \
    src/tooltip.h \
    src/indexitemwidget.h \
    src/settingdialog.h \
    src/settingtitlebar.h \
    src/activebutton.h \
    src/searchdialog.h \
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
    src/citieslist.h \
    src/weatherworker.h \
    src/networkerrorwidget.h \
    src/separator.h

RESOURCES += \
    res.qrc
