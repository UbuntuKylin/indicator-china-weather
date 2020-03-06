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

#ifndef GEOIP_WORKER_H
#define GEOIP_WORKER_H

#include <QObject>

class QNetworkAccessManager;
class QNetworkReply;

class GeoIpWorker : public QObject
{
    Q_OBJECT
public:
    explicit GeoIpWorker(QObject *parent = nullptr);

private slots:
    void doWork();
    //void onReplyFinished(QNetworkReply *reply);

signals:
    void requestStartWork();
    void automaticLocationFinished(const QString &cityName);

private:
    //QNetworkAccessManager *m_networkManager = nullptr;
};

#endif // GEOIP_WORKER_H
