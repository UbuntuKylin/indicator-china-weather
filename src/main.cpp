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
#include "dbusadaptor.h"

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QObject>
#include <QDir>
#include <signal.h>
#include <X11/Xlib.h>

#include <QtSingleApplication>

bool onlyOne(QtSingleApplication &a)
{
    if (a.isRunning()) {
        a.sendMessage(QApplication::arguments().length() > 1 ? QApplication::arguments().at(1) : a.applicationFilePath());
        qDebug() << "Can't lock single file, indicator-china-weather is already running!";
        return true;
    }
    return false;
}

void setAttribute(QtSingleApplication &a)
{
    signal(SIGINT, [](int) { QApplication::quit(); });// 设置退出信号
    //自适应高清屏幕
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QIcon::setThemeName("ukui-icon-theme-default");

    a.setOrganizationName("kylin");
    a.setApplicationName("Kylin Weather (indicator-china-weather)");
    a.setApplicationVersion("3.1.0");
    a.setQuitOnLastWindowClosed(false);//Avoid that after hiding mainwindow, close the sub window would cause the program exit
}

void showThis(MainWindow &w)
{
    //读取开机启动服务列表，判断是否开机启动
    QString homepath="/.config/autostart/indicator-china-weather.desktop";
    QFile file(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+homepath);
    if(file.exists())
        w.handleIconClickedSub(); //显示在屏幕中央
}

void responseCommand(QtSingleApplication &a)
{
    //提供DBus接口，添加show参数
    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::translate("main", "KylinWeather"));
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption swOption(QStringLiteral("show"),QCoreApplication::translate("main", "show indicator-china-weather test"));

    parser.addOptions({swOption});
    parser.process(a);

    if(parser.isSet(swOption) || !QDBusConnection::sessionBus().registerService("com.kylin.weather"))
    {
        if(!a.isRunning())return;
            QDBusInterface *interface = new QDBusInterface("com.kylin.weather",
                                                           "/com/kylin/weather",
                                                           "com.kylin.weather",
                                                           QDBusConnection::sessionBus(),
                                                           NULL);

            interface->call(QStringLiteral("showMainWindow"));
    }
}

int main(int argc, char *argv[])
{
    QString id = QString("indicator-china-weather-"+QLatin1String(getenv("DISPLAY")));
    QtSingleApplication a(id, argc, argv);
    responseCommand(a);//响应外部DBus命令
    if(onlyOne(a))return 0;
    setAttribute(a);//设置属性

    //翻译（YYF 经自测封装到函数里会导致程序部分翻译文件失效）
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
    showThis(w);//读取开机启动服务列表，判断是否开机启动

    //建立DBus服务（YYF 经自测封装到函数里会导致程序在响应DBus信号时异常结束）
    DbusAdaptor adaptor(&w);
    Q_UNUSED(adaptor);
    auto connection = QDBusConnection::sessionBus();
    qDebug()<<"建立DBus服务成功： "<< (connection.registerService("com.kylin.weather")&&connection.registerObject("/com/kylin/weather", &w));

    return a.exec();
}
