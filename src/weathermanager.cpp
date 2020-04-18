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
#include "data.h"

#include <QDebug>
#include <QThread>
#include <QFile>
#include <QTimer>

WeatherManager::WeatherManager(QObject *parent) : QObject(parent)
{
    m_geoipWorker = new GeoIpWorker();
    m_weatherWorker = new WeatherWorker();

    connect(m_weatherWorker, &WeatherWorker::nofityNetworkStatus, this, &WeatherManager::nofityNetworkStatus);
    connect(m_geoipWorker, &GeoIpWorker::automaticLocationFinished, this, &WeatherManager::setAutomaticCity);
}

WeatherManager::~WeatherManager()
{

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
