/*
 * Copyright (C) 2018 ~ 2019 kobe24_lixiang@126.com
 *
 * Authors:
 *  lixiang    kobe24_lixiang@126.com
 *  https://github.com/eightplus/xiaoming-weather
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

#include "automaticlocation.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>

#include <GeoIP.h>
#include <GeoIPCity.h>
#ifdef   __cplusplus
extern "C" {
#endif

GEOIP_API unsigned long _GeoIP_lookupaddress(const char *host); //_GeoIP_lookupaddress定义在GeoIP_internal.h中，但libgeoip-dev安装时并没有拷贝GeoIP_internal.h文件

#ifdef   __cplusplus
}
#endif


namespace {

const QString getPublicIpAddrByUrl(const QString &url)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));
    QByteArray ba;
    QEventLoop loop;
    QObject::connect(manager, SIGNAL(finished(QNetworkReply *)), &loop, SLOT(quit()));
    loop.exec();
    ba = reply->readAll();

    QString reply_content = QString::fromUtf8(ba);
    reply->close();
    reply->deleteLater();
    manager->deleteLater();

    if(!reply_content.isEmpty()) {
        QString htmlStr = reply_content.replace(" ", "");
        htmlStr = htmlStr.replace("\r", "");
        htmlStr = htmlStr.replace("\n", "");
        QStringList htmlList = htmlStr.split("<br/>");
        if(htmlList.size() >= 4) {
            QStringList ipList = htmlList.at(4).split("=");
            if (ipList.count() > 1) {
                if (ipList.at(1).contains(",")) {
                    ipList = ipList.at(1).split(",");
                    return ipList.at(0);
                }

                return ipList.at(1);
            }
        }
    }

    return QString("0.0.0.0");
}

const QString getCityFromIPAddr(const QString &ip)
{
    GeoIP *gi = GeoIP_open_type(GEOIP_CITY_EDITION_REV1, GEOIP_STANDARD | GEOIP_SILENCE);
    if (NULL == gi) {
        return QString();
    }

    uint32_t ipnum = _GeoIP_lookupaddress(ip.toStdString().c_str());
    GeoIPRecord *gir = GeoIP_record_by_ipnum(gi, ipnum);//GeoIP_record_by_ipnum_v6
    if (gir) {
        const char *region = GeoIP_region_name_by_code(gir->country_code, gir->region);
        //qDebug() << "country_name=" << gir->country_name << ",region=" << region << ",gir->city=" << gir->city << ",gir->latitude=" << gir->latitude << ",gir->longitude=" << gir->longitude;
        return QString(gir->city);
    }

    return QString();
}

const QString automaicCity()
{
    QString publicIP = getPublicIpAddrByUrl("http://whois.pconline.com.cn/");
    return getCityFromIPAddr(publicIP);
}

}

AutomaticLocation::AutomaticLocation(QObject *parent)
    :QThread(parent)
{

}

void AutomaticLocation::run()
{
    QString cityName = automaicCity();
    emit automaticLocationFinished(cityName);
}
