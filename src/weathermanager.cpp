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

#include "weathermanager.h"
#include "weatherworker.h"
#include "geoipworker.h"

#include <QDebug>
#include <QThread>
#include <QFile>
#include <QTimer>

#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusObjectPath>
#include <QtDBus/QDBusReply>

#include <unistd.h>

namespace {

void quitThread(QThread *thread)
{
    Q_ASSERT(thread);
    if (thread) {
        thread->quit();
        if (!thread->wait(2000)) {
            thread->terminate();
            thread->wait();
        }
    }
}

} // namespace

WeatherManager::WeatherManager(QObject *parent) : QObject(parent)
{
    m_geoipWorker = new GeoIpWorker();
    m_weatherWorker = new WeatherWorker();
    m_geoipThread = new QThread(this);
    m_weatherThread = new QThread(this);
    m_geoipWorker->moveToThread(m_geoipThread);
    m_weatherWorker->moveToThread(m_weatherThread);

    this->initConnections();

    QTimer::singleShot(1, this, [=] {
        m_geoipThread->start();
        m_weatherThread->start();
    });
}

WeatherManager::~WeatherManager()
{
    quitThread(m_geoipThread);
    quitThread(m_weatherThread);
}

void WeatherManager::initConnections()
{
    connect(m_geoipThread, &QThread::finished, m_geoipWorker, &GeoIpWorker::deleteLater);
    connect(m_weatherThread, &QThread::finished, m_weatherWorker, &WeatherWorker::deleteLater);
    connect(m_weatherWorker, &WeatherWorker::nofityNetworkStatus, this, &WeatherManager::nofityNetworkStatus);
    connect(m_geoipWorker, &GeoIpWorker::automaticLocationFinished, this, &WeatherManager::setAutomaticCity);
    connect(m_weatherWorker, &WeatherWorker::responseFailure, this, &WeatherManager::responseFailure);

    connect(m_weatherWorker, SIGNAL(requestSetObserveWeather(ObserveWeather)), this, SIGNAL(requestSetObserveWeather(ObserveWeather)));
    connect(m_weatherWorker, SIGNAL(requestSetForecastWeather(ForecastWeather)), this, SIGNAL(requestSetForecastWeather(ForecastWeather)));
    connect(m_weatherWorker, SIGNAL(requestSetLifeStyle(LifeStyle)), this, SIGNAL(requestSetLifeStyle(LifeStyle)));

    connect(this, SIGNAL(requestShowCollCityWeather()), m_weatherWorker, SLOT(onCityWeatherDataRequest()) );
    connect(m_weatherWorker, &WeatherWorker::requestSetCityWeather, this, &WeatherManager::requestSetCityWeather);

    QDBusConnection::systemBus().connect(QString("org.freedesktop.NetworkManager"),
                                         QString("/org/freedesktop/NetworkManager"),
                                         QString("org.freedesktop.NetworkManager"),
                                         QString("PropertiesChanged"), this, SLOT(onPropertiesChanged(QVariantMap) ) );
}

void WeatherManager::startGetTheWeatherData(QString cityId)
{
    emit m_weatherWorker->requestGetTheWeatherData(cityId);
}

void WeatherManager::startTestNetwork()
{
    emit m_weatherWorker->requestTestNetwork();
}

void WeatherManager::postSystemInfoToServer()
{
    emit m_weatherWorker->requestPostHostInfoToWeatherServer();
}

void WeatherManager::startAutoLocationTask()
{
    emit m_geoipWorker->requestStartWork();
}

void WeatherManager::setAutomaticCity(const QString &cityName)
{
    bool autoSuccess = false;
    CitySettingData info;

    if (cityName.isEmpty()) {
        emit this->requestAutoLocationData(info, false);
        return;
    }

    QFile file(":/data/data/china-city-list.csv");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString line = file.readLine();
        line = line.replace("\n", "");
        while (!line.isEmpty()) {
            QStringList resultList = line.split(",");
            if (resultList.length() < 10) {
                line = file.readLine();
                line = line.replace("\n", "");
                continue;
            }

            QString id = resultList.at(0);
            if (!id.startsWith("CN")) {
                line = file.readLine();
                line = line.replace("\n", "");
                continue;
            }

            if (resultList.at(1).compare(cityName, Qt::CaseInsensitive) == 0 ||
                resultList.at(2).compare(cityName, Qt::CaseInsensitive) == 0 ||
                QString(resultList.at(2) + "市").compare(cityName, Qt::CaseInsensitive) == 0 ||
                QString(resultList.at(2) + "区").compare(cityName, Qt::CaseInsensitive) == 0 ||
                QString(resultList.at(2) + "县").compare(cityName, Qt::CaseInsensitive) == 0) {
                id.remove(0, 2);//remove "CN"
                QString name = resultList.at(2);

                info.active = false;
                info.id = id;
                info.name = name;
                info.icon = ":/res/weather_icons/darkgrey/100.png";

                autoSuccess = true;
                break;
            }

            line = file.readLine();
            line = line.replace("\n", "");
        }
        file.close();
    }

    if (autoSuccess) {
        emit this->requestAutoLocationData(info, true);
    } else {
        emit this->requestAutoLocationData(info, false);
    }
}

int WeatherManager::getTaskBarPos(QString str)
{
    QDBusInterface interface( "com.ukui.panel.desktop",
                              "/",
                              "com.ukui.panel.desktop",
                              QDBusConnection::sessionBus() );

    QDBusReply<int> reply = interface.call("GetPanelPosition", str);
    if (reply.isValid()) {
        return reply;
    } else {
        return 0;
    }
}

int WeatherManager::getTaskBarHeight(QString str)
{
    QDBusInterface interface( "com.ukui.panel.desktop",
                              "/",
                              "com.ukui.panel.desktop",
                              QDBusConnection::sessionBus() );

    QDBusReply<int> reply = interface.call("GetPanelSize", str);
    if (reply.isValid()) {
        return reply;
    } else {
        return 46;
    }
}


void WeatherManager::initConnectionInfo()
{
    QDBusInterface interface( "org.freedesktop.NetworkManager",
                              "/org/freedesktop/NetworkManager",
                              "org.freedesktop.DBus.Properties",
                              QDBusConnection::systemBus() );

    QDBusMessage result = interface.call("Get", "org.freedesktop.NetworkManager", "ActiveConnections");
    QList<QVariant> outArgs = result.arguments();
    QVariant first = outArgs.at(0);
    QDBusVariant dbvFirst = first.value<QDBusVariant>();
    QVariant vFirst = dbvFirst.variant();
    QDBusArgument dbusArgs = vFirst.value<QDBusArgument>();

    QDBusObjectPath objPath;
    dbusArgs.beginArray();
    while (!dbusArgs.atEnd()) {
        dbusArgs >> objPath;
        oldPaths.append(objPath);
        qDebug() <<"debug: *****path is: "<< objPath.path();

        QDBusInterface interface( "org.freedesktop.NetworkManager",
                                  objPath.path(),
                                  "org.freedesktop.DBus.Properties",
                                  QDBusConnection::systemBus() );

        QDBusReply<QVariant> reply = interface.call("Get", "org.freedesktop.NetworkManager.Connection.Active", "Type");
        qDebug()<<"debug: *****connection type is: "<<reply.value().toString();
        oldPathInfo.append(reply.value().toString());
    }
    dbusArgs.endArray();
}

void WeatherManager::onPropertiesChanged(QVariantMap qvm)
{
    for(QString keyStr : qvm.keys()) {
        if (keyStr == "ActiveConnections") {
            const QDBusArgument &dbusArg = qvm.value(keyStr).value<QDBusArgument>();
            QList<QDBusObjectPath> newPaths;
            dbusArg >> newPaths;
            QStringList newPathInfo;
            foreach (QDBusObjectPath objPath, newPaths) {
                //qDebug()<<"dbug: bbbbb  "<<objPath.path();

                QDBusInterface interface( "org.freedesktop.NetworkManager",
                                          objPath.path(),
                                          "org.freedesktop.DBus.Properties",
                                          QDBusConnection::systemBus() );

                QDBusReply<QVariant> reply = interface.call("Get", "org.freedesktop.NetworkManager.Connection.Active", "Type");
                //qDebug()<<"dbug: ccccc "<<reply.value().toString();
                newPathInfo.append(reply.value().toString());
            }

            // 当前的网络连接个数由0个增为1个时，触发天气界面更新
            if (newPaths.size() == 1) {
                if (oldPaths.size() == 0) {
                    QTimer::singleShot(4*1000, this, SLOT(onTimeFinished() ));
                }
            }

            bool isChangeOldPathInfo = true;
            for (int k=0; k<newPathInfo.size(); k++) {
                if (newPathInfo.at(k) == "") {
                    isChangeOldPathInfo = false;
                }
            }
            if (isChangeOldPathInfo) {
                oldPathInfo = newPathInfo;
            }
            oldPaths = newPaths;
        }
    }
}

void WeatherManager::onTimeFinished()
{
    emit newNetworkConnectionCreated();
}
