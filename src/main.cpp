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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("kylin");
    a.setApplicationName("Kylin Weather (indication-china-weather)");
    a.setApplicationVersion("1.0.0");
    a.setQuitOnLastWindowClosed(false);//Avoid that after hiding mainwindow, close the sub window would cause the program exit

    MainWindow w;
    w.show();

    //http://service.ubuntukylin.com:8001/weather/api/1.0/observe/101250101/
    //service.ubuntukylin.com:8001/weather/api/1.0/heweather_forecast/101250101/

    //pingback
    //http://service.ubuntukylin.com:8001/weather/pingnetwork/
    //service.ubuntukylin.com:8001/weather/pingbackmain?distro=ubuntu&version_os=16.04&version_weather=1.0&city=长沙

    //post
//    QNetworkRequest request;
//    request.setUrl(QUrl("http://localhost:8888/login"));
//    QByteArray postData;
//    postData.append("myname=lk&mypwd=33");
//    QNetworkReply* reply = m_accessManager->post(request,postData);


//    QUrl url("http://localhost:8080/WebRoot/servlet/LoginServlet");
//    QByteArray append("username=admin&password=123456");
//    QNetworkReply* reply = nam->post(QNetworkRequest(url), append);


    //get
//    QNetworkRequest request;
//    request.setUrl(QUrl("http://dict.baidu.com/s?wd=name"));
//    m_accessManager->get(request);

    return a.exec();
}
