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

#include "geoipworker.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QRegularExpression>
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

const char UbuntuUrl[] = "http://geoip.ubuntu.com/lookup";
const char PconlineUrl[] = "http://whois.pconline.com.cn/";

const void getIpAndCityByUbuntu(const QString &url, QString &ip, QString &city)
{
    const QString ipPattern = "<Ip>([^<]+)";
    const QString cityPattern = "<City>([^<]+)";

    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));

    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    //QObject::connect(manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    //QObject::connect(manager, SIGNAL(finished(QNetworkReply *)), &loop, SLOT(quit()));
    loop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray ba = reply->readAll();

        QString reply_content = QString::fromUtf8(ba);
        reply->close();
        reply->deleteLater();
        manager->deleteLater();

        QRegularExpression ipRegular(ipPattern, QRegularExpression::MultilineOption);
        QRegularExpressionMatch match = ipRegular.match(reply_content);
        if (match.hasMatch()) {
            ip = match.captured(1);
        }
        else {
            ip = QString("0.0.0.0");
        }

        QRegularExpression cityRegular(cityPattern, QRegularExpression::MultilineOption);
        match = cityRegular.match(reply_content);
        if (match.hasMatch()) {
            city = match.captured(1);
        }
        else {
            city = QString();
        }
    }
}

const QString getIpByPconline(const QString &url)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    //QObject::connect(manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    //QObject::connect(manager, SIGNAL(finished(QNetworkReply *)), &loop, SLOT(quit()));
    loop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray ba = reply->readAll();
        QString reply_content = QString::fromUtf8(ba);
        reply->close();
        reply->deleteLater();
        manager->deleteLater();

        if (!reply_content.isEmpty()) {
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
    }

    return QString("0.0.0.0");
}

const QString getCityFromIpByAmap(const QString &ip)
{
    // https://lbs.amap.com/api/webservice/guide/api/ipconfig/

    //amap json
    //http://restapi.amap.com/v3/ip?key=44a80558982f0c3031fae15aa8711a92&ip=218.76.23.26

    //amap xml
    //https://restapi.amap.com/v3/ip?ip=218.76.23.26&output=xml&key=44a80558982f0c3031fae15aa8711a92

    QString url = QString("http://restapi.amap.com/v3/ip?key=44a80558982f0c3031fae15aa8711a92&ip=%1").arg(ip);
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    //QObject::connect(manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    //QObject::connect(manager, SIGNAL(finished(QNetworkReply *)), &loop, SLOT(quit()));
    loop.exec();

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(reply->error() != QNetworkReply::NoError || statusCode != 200) {//200 is normal status
        reply->close();
        reply->deleteLater();
        manager->deleteLater();
        return QString();
    }

    QByteArray ba = reply->readAll();
    //QString reply_content = QString::fromUtf8(ba);
    reply->close();
    reply->deleteLater();
    manager->deleteLater();

    QJsonParseError err;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(ba, &err);
    if (err.error != QJsonParseError::NoError) {// Json type error
        return QString();
    }
    if (jsonDocument.isNull() || jsonDocument.isEmpty()) {
        return QString();
    }

    QJsonObject jsonObject = jsonDocument.object();
    if (jsonObject.isEmpty() || jsonObject.size() == 0) {
        return QString();
    }

    if (jsonObject.contains("city")) {
        return jsonObject.value("city").toString();
    }

    return QString();
}

const QString getCityFromIpByTaobao(const QString &ip)
{
    QString url = QString("http://ip.taobao.com/service/getIpInfo.php?ip=%1").arg(ip);
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    //QObject::connect(manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    //QObject::connect(manager, SIGNAL(finished(QNetworkReply *)), &loop, SLOT(quit()));
    loop.exec();

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(reply->error() != QNetworkReply::NoError || statusCode != 200) {//200 is normal status
        reply->close();
        reply->deleteLater();
        manager->deleteLater();
        return QString();
    }

    QByteArray ba = reply->readAll();
    reply->close();
    reply->deleteLater();
    manager->deleteLater();

    QJsonParseError err;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(ba, &err);
    if (err.error != QJsonParseError::NoError) {// Json type error
        return QString();
    }
    if (jsonDocument.isNull() || jsonDocument.isEmpty()) {
        return QString();
    }

    QJsonObject jsonObject = jsonDocument.object();
    if (jsonObject.isEmpty() || jsonObject.size() == 0) {
        return QString();
    }

    if (jsonObject.contains("data")) {
        QJsonObject dataObj = jsonObject.value("data").toObject();
        if (dataObj.isEmpty() || dataObj.size() == 0) {
            return QString();
        }
        if (dataObj.contains("city")) {
            return dataObj.value("city").toString();
        }
    }

    return QString();
}

const QString getCityFromIPAddr(const QString &ip)
{
    int charset = GEOIP_CHARSET_UTF8;
//    charset = GEOIP_CHARSET_ISO_8859_1;

    GeoIP *gi = GeoIP_open_type(GEOIP_CITY_EDITION_REV1, GEOIP_STANDARD | GEOIP_SILENCE);
    if (NULL == gi) {
        return QString();
    }
//    gi->charset = charset;

    uint32_t ipnum = _GeoIP_lookupaddress(ip.toStdString().c_str());
    if (ipnum == 0) {
        printf("%s: can't resolve ip ( %s )\n", GeoIPDBDescription[GEOIP_CITY_EDITION_REV1], ip.toStdString().c_str());
        return QString();
    }
    GeoIPRecord *gir = GeoIP_record_by_ipnum(gi, ipnum);
    if (gir) {
        //const char *region = GeoIP_region_name_by_code(gir->country_code, gir->region);
        //qDebug() << "country_name=" << gir->country_name << ",region=" << region << ",gir->city=" << gir->city << ",gir->latitude=" << gir->latitude << ",gir->longitude=" << gir->longitude;
        QString cityName = QString(gir->city);
        GeoIPRecord_delete(gir);
        GeoIP_delete(gi);
        return cityName;
    }

    GeoIP_delete(gi);

    return QString();
}

const QString automaicCity()
{
    QString ip;
    QString city;

    getIpAndCityByUbuntu(UbuntuUrl, ip, city);
    if (ip.isEmpty() || ip == "0.0.0.0") {
        ip = getIpByPconline(PconlineUrl);
    }

//    qDebug() << "ip:" << ip;

    if (city.isEmpty()) {
        city = getCityFromIPAddr(ip);//根据ip从geoip库定位城市
        if (city.isEmpty()) {
            city = getCityFromIpByAmap(ip);//根据ip从高德API定位城市，该方式使用高德key，访问次数有限
        }
        if (city.isEmpty()) {
            city = getCityFromIpByTaobao(ip);//根据ip从淘宝service定位城市，该方式访问速度慢，可能访问失败
        }
    }

//    qDebug() << "city:" << city;

    return city;
}

} // namespace


GeoIpWorker::GeoIpWorker(QObject* parent) : QObject(parent)
{
    //m_networkManager = new QNetworkAccessManager(this);
    //connect(m_networkManager, &QNetworkAccessManager::finished, this, &GeoIpWorker::onReplyFinished);
    connect(this, &GeoIpWorker::requestStartWork, this, &GeoIpWorker::doWork);
}

void GeoIpWorker::doWork()
{
    QString cityName = automaicCity();
    emit automaticLocationFinished(cityName);
}

/*void GeoIpWorker::onReplyFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {

    }
    reply->deleteLater();
}*/
