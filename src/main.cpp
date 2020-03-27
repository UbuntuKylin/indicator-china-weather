/*
 * Copyright (C) 2013 ~ 2020 National University of Defense Technology(NUDT) & Tianjin Kylin Ltd.
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
#include <QObject>
#include <QDir>

#include <signal.h>

int main(int argc, char *argv[])
{
    signal(SIGINT, [](int) { QApplication::quit(); });// 设置退出信号

    QApplication a(argc, argv);
    a.setOrganizationName("kylin");
    a.setApplicationName("Kylin Weather (indicator-china-weather)");
    a.setApplicationVersion("3.1.0");
    a.setQuitOnLastWindowClosed(false);//Avoid that after hiding mainwindow, close the sub window would cause the program exit

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QTranslator app_trans;
    QTranslator qt_trans;
    QString locale = QLocale::system().name();

    QString trans_path;
    if (QDir("/usr/share/indicator-china-weather/translations").exists()) {
        trans_path = "/usr/share/indicator-china-weather/translations";
    }
    else {
        trans_path = qApp->applicationDirPath() + "/translations";
    }

    QString qt_trans_path;
    qt_trans_path = QLibraryInfo::location(QLibraryInfo::TranslationsPath);// /usr/share/qt5/translations

    if (locale == "zh_CN") {
        if(!app_trans.load("indicator-china-weather_" + locale + ".qm", trans_path))
            qDebug() << "Load translation file："<< "indicator-china-weather_" + locale + ".qm from" << trans_path << "failed!";
        else
            a.installTranslator(&app_trans);

        if(!qt_trans.load("qt_" + locale + ".qm", qt_trans_path))
            qDebug() << "Load translation file："<< "qt_" + locale + ".qm from" << qt_trans_path << "failed!";
        else
            a.installTranslator(&qt_trans);
    }

    MainWindow w;
    w.show();

    return a.exec();
}
