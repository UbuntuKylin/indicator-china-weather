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
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkConfigurationManager>
#include <QHostInfo>

#include "data.h"

class WeatherWorker : public QObject
{
    Q_OBJECT

public:
    enum WeatherType
    {
        Type_Observe,
        Type_Forecast,
        Type_Aqi
    };

    explicit WeatherWorker(QObject *parent = 0);
    ~WeatherWorker();

    void requestPingBackWeatherServer();
    bool AccessDedirectUrl(const QString &redirectUrl/*, WeatherType weatherType*/);
    void AccessDedirectUrlWithPost(const QString &redirectUrl);
    QString getErrorCodeDescription(QString errorCode);

    void convertWeatherValue(WeatherType weatherType, const QString &line, QChar separator);

signals:
    void requestTestNetwork();
    void nofityNetworkStatus(const QString &status);
    void requesUiRefreshed();
    void requestPostHostInfoToWeatherServer();
    void responseFailure(int code);
    void requestDiplayServerNotify(const QString &notifyInfo);
    void requestRefresheWeatherData(const QString &cityId);

public slots:
    void onWeatherDataReply();
    void onPingBackPostReply();
    void networkLookedUp(const QHostInfo &host);
    void onPostHostInfoToWeatherServer();
    void onResponseTestNetwork();
    void onResponseRefresheWeatherData(const QString &cityId);

private:
    QNetworkAccessManager *m_networkManager = nullptr;
    QString m_hostInfoParameters;
};

#endif // WEATHERWORKER_H
