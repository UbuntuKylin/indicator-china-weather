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

#ifndef WEATHERWORKER_H
#define WEATHERWORKER_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkConfigurationManager>
#include <QHostInfo>

#include "data.h"

class WeatherWorker : public QObject
{
    Q_OBJECT

public:
    explicit WeatherWorker(QObject *parent = 0);
    ~WeatherWorker();

    void AccessDedirectUrlWithPost(const QString &redirectUrl);

public slots:
    void onWeatherDataRequest(const QString &cityId);
    void onWeatherDataReply();

    void onPostHostInfoToWeatherServer();
    void onPingBackPostReply();

    void networkLookedUp(const QHostInfo &host);
    void onResponseTestNetwork();

private:
    QNetworkAccessManager *m_networkManager = nullptr;
    QString m_hostInfoParameters;

    QString m_currentcityname = "";

signals:
    void setLocationData();
    void requestSetObserveWeather(ObserveWeather observeweather);
    void requestSetForecastWeather(ForecastWeather forecastweather);
    void requestSetLifeStyle(LifeStyle lifestyle);

    void requestPostHostInfoToWeatherServer();
    void requestTestNetwork();
    void nofityNetworkStatus(const QString &status);
    void responseFailure(int code);
};

#endif // WEATHERWORKER_H
