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

#ifndef WEATHER_MANAGER_H
#define WEATHER_MANAGER_H

#include "data.h"

#include <QObject>
#include <QDBusObjectPath>

class QThread;
class CitySettingData;
class GeoIpWorker;
class WeatherWorker;

class WeatherManager : public QObject
{
    Q_OBJECT
public:
    explicit WeatherManager(QObject *parent = nullptr);
    ~WeatherManager();

    void initConnections();
    void initConnectionInfo();

    void startGetTheWeatherData(QString cityId);
    void startTestNetwork();
    void postSystemInfoToServer();
    void startAutoLocationTask();

    int getTaskBarPos(QString str);
    int getTaskBarHeight(QString str);

private:
    WeatherWorker *m_weatherWorker = nullptr;
    GeoIpWorker *m_geoipWorker = nullptr;
    QThread *m_geoipThread = nullptr;
    QThread *m_weatherThread = nullptr;

    QStringList oldPathInfo;
    QList<QDBusObjectPath> oldPaths;

private slots:
    void setAutomaticCity(const QString& cityName);
    void onPropertiesChanged(QVariantMap qvm);
    void onTimeFinished();

signals:
    void requestShowCollCityWeather();

    void requestAutoLocationData(const CitySettingData & info, bool success);
    void nofityNetworkStatus(const QString &status);
    void responseFailure(int code);
    void requestSetCityWeather(QString weather_data);

    void requestSetObserveWeather(ObserveWeather observerdata);
    void requestSetForecastWeather(ForecastWeather forecastweather);
    void requestSetLifeStyle(LifeStyle lifestyle);

    void newNetworkConnectionCreated();
};

#endif // WEATHER_MANAGER_H
