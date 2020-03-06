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
#include "geoipworker.h"
#include "weatherworker.h"
#include "data.h"
#include "preferences.h"
#include "global.h"
using namespace Global;

#include <QDebug>
#include <QThread>
#include <QFile>
#include <QTimer>

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
    , m_geoipWorker(new GeoIpWorker())
    , m_weatherWorker(new WeatherWorker())
    , m_geoipThread(new QThread(this))
    , m_weatherThread(new QThread(this))
{
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
    /*if (m_geoipWorker) {
        m_geoipWorker->deleteLater();
    }
    if (m_weatherWorker) {
        m_weatherWorker->deleteLater();
    }*/
    quitThread(m_geoipThread);
    quitThread(m_weatherThread);
}

void WeatherManager::initConnections()
{
    connect(m_geoipThread, &QThread::finished, m_geoipWorker, &GeoIpWorker::deleteLater);
    connect(m_weatherThread, &QThread::finished, m_weatherWorker, &WeatherWorker::deleteLater);
    connect(m_geoipWorker, &GeoIpWorker::automaticLocationFinished, this, &WeatherManager::setAutomaticCity);
    connect(m_weatherWorker, &WeatherWorker::nofityNetworkStatus, this, &WeatherManager::nofityNetworkStatus);
    connect(m_weatherWorker, &WeatherWorker::responseFailure, this, &WeatherManager::responseFailure);
    connect(m_weatherWorker, &WeatherWorker::requestDiplayServerNotify, this, &WeatherManager::requestDiplayServerNotify);
    connect(m_weatherWorker, &WeatherWorker::observeDataRefreshed, this, &WeatherManager::observeDataRefreshed);
    connect(m_weatherWorker, &WeatherWorker::forecastDataRefreshed, this, &WeatherManager::forecastDataRefreshed);
}

void WeatherManager::postSystemInfoToServer()
{
    emit m_weatherWorker->requestPostHostInfoToWeatherServer();
}

void WeatherManager::startAutoLocationTask()
{
    emit m_geoipWorker->requestStartWork();
}

void WeatherManager::startTestNetwork()
{
    emit m_weatherWorker->requestTestNetwork();
}

void WeatherManager::startRefresheWeatherData(const QString &cityId)
{
    emit m_weatherWorker->requestRefresheWeatherData(cityId);
}

void WeatherManager::setAutomaticCity(const QString &cityName)
{
    bool autoSuccess = false;
    CitySettingData info;

    if (cityName.isEmpty()) {
        emit this->requestAutoLocationData(info, false);
        return;
    }
    //CN101250101,changsha,长沙,CN,China,中国,hunan,湖南,changsha,长沙,28.19409,112.98228,"430101,430100,430000",
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


                if (m_preferences->isCitiesCountOverMax()) {
                    if (m_preferences->isCityIdExist(id)) {
                        //从已有列表中将自动定位的城市设置为默认城市
                        m_preferences->setCurrentCityIdAndName(name);
                    }
                    else {
                        break;
                    }
                }
                else {
                    if (m_preferences->isCityIdExist(id)) {
                        m_preferences->setCurrentCityIdAndName(name);
                    }
                    else {
                        City city;
                        city.id = id;
                        city.name = name;
                        m_preferences->setCurrentCityIdAndName(name);
                        m_preferences->addCityInfoToPref(city);
                        m_preferences->save();
                    }
                }

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
    }
    else {
        emit this->requestAutoLocationData(info, false);
    }
}
