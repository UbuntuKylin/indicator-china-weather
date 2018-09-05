/*
 * Copyright (C) 2013 ~ 2018 National University of Defense Technology(NUDT) & Tianjin Kylin Ltd.
 *
 * Authors:
 *  Kobe Lee    lixiang@kylinos.cn/kobe24_lixiang@126.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("kylin");
    a.setApplicationName("Kylin Weather (indication-china-weather)");
    a.setApplicationVersion("3.0.0");
    a.setQuitOnLastWindowClosed(false);//Avoid that after hiding mainwindow, close the sub window would cause the program exit

    QString locale = QLocale::system().name();
    QTranslator translator;
    if(locale == "zh_CN") {
        if(!translator.load("indicator-china-weather_" + locale + ".qm",
                            ":/qm/translation/"))
            qDebug() << "Load translation file："<< "indicator-china-weather_" + locale + ".qm" << " failed!";
        else
            a.installTranslator(&translator);
    }

//    QTranslator qtTranslator;
//    qtTranslator.load("qt_" + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
//    a.installTranslator(&qtTranslator);

    MainWindow w;
    w.show();

    return a.exec();
}
