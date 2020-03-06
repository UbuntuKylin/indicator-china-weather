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

#include <QObject>
#include "data.h"

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
    void postSystemInfoToServer();
    void startAutoLocationTask();
    void startTestNetwork();
    void startRefresheWeatherData(const QString &cityId);

private slots:
    void setAutomaticCity(const QString& cityName);

signals:
    void requestAutoLocationData(const CitySettingData & info, bool success);
    void nofityNetworkStatus(const QString &status);
    void responseFailure(int code);
    void requestDiplayServerNotify(const QString &notifyInfo);
    void observeDataRefreshed(const ObserveWeather &data);
    void forecastDataRefreshed(const QList<ForecastWeather> &datas, LifeStyle data);

private:
    GeoIpWorker *m_geoipWorker = nullptr;
    WeatherWorker *m_weatherWorker = nullptr;
    QThread *m_geoipThread = nullptr;
    QThread *m_weatherThread = nullptr;
};

#endif // WEATHER_MANAGER_H
