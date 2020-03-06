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

#include "weatherworker.h"
//#include "automaticlocation.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QEventLoop>
#include <QFile>
#include <QApplication>

#include "preferences.h"
#include "global.h"
using namespace Global;

inline QString readOsInfo()
{
    QString idParse = "DISTRIB_ID=";
    QString releaseParse = "DISTRIB_RELEASE=";
    QString osId;
    QString osRelease;

    QFile file("/etc/lsb-release");
    if (!file.open(QFile::ReadOnly)) {
        qCritical() << QString("open lsb-release file failed");
        return QString("distro=ukylin&version_os=18.04");
    }

    QByteArray content = file.readAll();
    file.close();
    QTextStream stream(&content, QIODevice::ReadOnly);
    while (!stream.atEnd()) {
        QString line = stream.readLine();
        if (line.startsWith(idParse)) {
            osId = line.remove(0, idParse.size());
        }
        else if (line.startsWith(releaseParse)) {
            osRelease = line.remove(0, releaseParse.size());
        }
    }

    return QString("distro=%1&version_os=%2").arg(osId).arg(osRelease);
}

WeatherWorker::WeatherWorker(QObject *parent) :
    QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
{
    connect(m_networkManager, &QNetworkAccessManager::finished, this, [] (QNetworkReply *reply) {
        reply->deleteLater();
    });

    connect(this, &WeatherWorker::requestTestNetwork, this, &WeatherWorker::onResponseTestNetwork);
    connect(this, &WeatherWorker::requestPostHostInfoToWeatherServer, this, &WeatherWorker::onPostHostInfoToWeatherServer);
    connect(this, &WeatherWorker::requestRefresheWeatherData, this, &WeatherWorker::onResponseRefresheWeatherData);





//    m_automatic = new AutomaticLocation(this);
//    connect(m_automatic, &AutomaticLocation::automaticLocationFinished, this, &WeatherWorker::setAutomaticCity);
}

WeatherWorker::~WeatherWorker()
{
    m_networkManager->deleteLater();
}

//void WeatherWorker::startAutoLocationTask()
//{
//    m_automatic->start();
//}

void WeatherWorker::onResponseTestNetwork()
{
    QNetworkConfigurationManager mgr;
    if (mgr.isOnline()) {//判断网络是否有连接，不一定能上网，如果连接了，则开始检查互联网是否可以ping通
        //http://service.ubuntukylin.com:8001/weather/pingnetwork/
        QHostInfo::lookupHost("www.baidu.com", this, SLOT(networkLookedUp(QHostInfo)));
    }
    else {
        emit nofityNetworkStatus("Fail");//物理网线未连接
    }
}

void WeatherWorker::onResponseRefresheWeatherData(const QString &cityId)
{
    refreshObserveWeatherData(m_preferences->m_currentCityId);
    refreshForecastWeatherData(m_preferences->m_currentCityId);
}

//bool WeatherWorker::isNetWorkSettingsGood()
//{
//    //判断网络是否有连接，不一定能上网
//    QNetworkConfigurationManager mgr;
//    return mgr.isOnline();
//}

//void WeatherWorker::netWorkOnlineOrNot()
//{
//    //http://service.ubuntukylin.com:8001/weather/pingnetwork/
//    QHostInfo::lookupHost("www.baidu.com", this, SLOT(networkLookedUp(QHostInfo)));
//}

void WeatherWorker::networkLookedUp(const QHostInfo &host)
{
    if(host.error() != QHostInfo::NoError) {
        //qDebug() << "test network failed, errorCode:" << host.error();
        emit this->nofityNetworkStatus(host.errorString());
    }
    else {
        //qDebug() << "test network success, the server's ip:" << host.addresses().first().toString();
        emit this->nofityNetworkStatus("OK");
    }
}

void WeatherWorker::refreshObserveWeatherData(const QString &cityId)
{
    if (cityId.isEmpty()) {
        emit responseFailure(0);
        return;
    }

    /*QString forecastUrl = QString("http://service.ubuntukylin.com:8001/weather/api/1.0/observe/%1").arg(cityId);
    qDebug() << "forecastUrl=" << forecastUrl;
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(forecastUrl)));
    QByteArray responseData;
    QEventLoop eventLoop;
    QObject::connect(manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    responseData = reply->readAll();
    reply->deleteLater();
    manager->deleteLater();
    qDebug() << "weather observe size: " << responseData.size();*/

    //heweather_observe_s6
    QString forecastUrl = QString("http://service.ubuntukylin.com:8001/weather/api/2.0/heweather_observe_s6/%1").arg(cityId);
    QNetworkRequest request;
    request.setUrl(forecastUrl);
    //request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);//Qt5.6 for redirect
    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &WeatherWorker::onWeatherObserveReply);
}

void WeatherWorker::refreshForecastWeatherData(const QString &cityId)
{
    if (cityId.isEmpty()) {
        emit responseFailure(0);
        return;
    }

    //heweather_forecast_s6
    QString forecastUrl = QString("http://service.ubuntukylin.com:8001/weather/api/2.0/heweather_forecast_s6/%1").arg(cityId);
    QNetworkReply *reply = m_networkManager->get(QNetworkRequest(forecastUrl));
    connect(reply, &QNetworkReply::finished, this, &WeatherWorker::onWeatherForecastReply);
}

void WeatherWorker::requestPingBackWeatherServer()
{
    QNetworkReply *reply = m_networkManager->get(QNetworkRequest(QString("http://service.ubuntukylin.com:8001/weather/pinginformation/")));
    connect(reply, &QNetworkReply::finished, this, [=] () {
        QNetworkReply *m_reply = qobject_cast<QNetworkReply*>(sender());
        int statusCode = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if(m_reply->error() != QNetworkReply::NoError || statusCode != 200) {//200 is normal status
            qDebug() << "pingback request error:" << m_reply->error() << ", statusCode=" << statusCode;
            emit responseFailure(statusCode);
            return;
        }

        QByteArray ba = m_reply->readAll();
        m_reply->close();
        m_reply->deleteLater();

        QJsonParseError err;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(ba, &err);
        if (err.error != QJsonParseError::NoError) {// Json type error
            qDebug() << "Json type error";
            return;
        }
        if (jsonDocument.isNull() || jsonDocument.isEmpty()) {
            qDebug() << "Json null or empty!";
            return;
        }

        QJsonObject jsonObject = jsonDocument.object();
        if (jsonObject.isEmpty() || jsonObject.size() == 0) {
            return;
        }
        if (jsonObject.contains("info")) {
            QString notifyInfo = jsonObject.value("info").toString();
            if (!notifyInfo.isEmpty() && !notifyInfo.isNull()) {
                emit requestDiplayServerNotify(notifyInfo);
            }
        }
    });
}

void WeatherWorker::onPostHostInfoToWeatherServer()
{
    QString osInfo = readOsInfo();
    QString hostInfo = QString("%1&version_weather=%2&city=%3").arg(osInfo).arg(qApp->applicationVersion()).arg(m_preferences->m_currentCity);
    this->m_hostInfoParameters = hostInfo;

    QByteArray parameters = hostInfo.toUtf8();
    QNetworkRequest request;
    request.setUrl(QUrl("http://service.ubuntukylin.com:8001/weather/pingbackmain"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::ContentLengthHeader, parameters.length());
    //QUrl url("http://service.ubuntukylin.com:8001/weather/pingbackmain");
    QNetworkReply *reply = m_networkManager->post(request, parameters);//QNetworkReply *reply = m_networkManager->post(QNetworkRequest(url), parameters);
    connect(reply, &QNetworkReply::finished, this, &WeatherWorker::onPingBackPostReply);
}

bool WeatherWorker::AccessDedirectUrl(const QString &redirectUrl, WeatherType weatherType)
{
    if (redirectUrl.isEmpty())
        return false;

    QNetworkRequest request;
    QString url;
    url = redirectUrl;
    request.setUrl(QUrl(url));

    QNetworkReply *reply = m_networkManager->get(request);

    switch (weatherType) {
    case WeatherType::Type_Observe:
        connect(reply, &QNetworkReply::finished, this, &WeatherWorker::onWeatherObserveReply);
        break;
    case WeatherType::Type_Forecast:
        connect(reply, &QNetworkReply::finished, this, &WeatherWorker::onWeatherForecastReply);
        break;
    default:
        break;
    }

    return true;
}

void WeatherWorker::AccessDedirectUrlWithPost(const QString &redirectUrl)
{
    if (redirectUrl.isEmpty())
        return;

    QNetworkRequest request;
    QString url;
    url = redirectUrl;
    QByteArray parameters = this->m_hostInfoParameters.toUtf8();
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::ContentLengthHeader, parameters.length());
    QNetworkReply *reply = m_networkManager->post(request, parameters);
    connect(reply, &QNetworkReply::finished, this, &WeatherWorker::onPingBackPostReply);
}

void WeatherWorker::onWeatherObserveReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (statusCode == 301 || statusCode == 302) {//redirect
        bool redirection = false;
        QVariant redirectionUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
        //qDebug() << "Weather: redirectionUrl=" << redirectionUrl.toString();
        redirection = AccessDedirectUrl(redirectionUrl.toString(), WeatherType::Type_Observe);//AccessDedirectUrl(reply->rawHeader("Location"));
        reply->close();
        reply->deleteLater();
        if (!redirection) {
            emit responseFailure(statusCode);
        }
        return;
    }
    else if (statusCode == 400) {
        qDebug() << "Weather: Network error (HTTP400/Bad Request)";
        emit responseFailure(statusCode);
        return;
    }
    else if (statusCode == 403) {
        qDebug() << "Weather: Username or password invalid (permission denied)";
        emit responseFailure(statusCode);
        return;
    }
    else if (statusCode == 200) {
        // 200 is normal status
    }
    else {
        emit responseFailure(statusCode);
        return;
    }

    if(reply->error() != QNetworkReply::NoError) {
        //qDebug() << "weather request error:" << reply->error() << ", statusCode=" << statusCode;
        emit responseFailure(statusCode);
        return;
    }

/*
    bool redirection = false;
    if(reply->error() != QNetworkReply::NoError || statusCode != 200) {//200 is normal status
        qDebug() << "weather request error:" << reply->error() << ", statusCode=" << statusCode;
        if (statusCode == 301 || statusCode == 302) {//redirect
            QVariant redirectionUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
            qDebug() << "redirectionUrl=" << redirectionUrl.toString();
            redirection = AccessDedirectUrl(redirectionUrl.toString(), WeatherType::Type_Observe);//AccessDedirectUrl(reply->rawHeader("Location"));
            reply->close();
            reply->deleteLater();
        }
        if (!redirection) {
            emit responseFailure(statusCode);
        }

        return;
    }
    */

    QByteArray ba = reply->readAll();
    //QString reply_content = QString::fromUtf8(ba);
    reply->close();
    reply->deleteLater();
    //qDebug() << "weather observe size: " << ba.size();

    QJsonParseError err;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(ba, &err);
    if (err.error != QJsonParseError::NoError) {// Json type error
        qDebug() << "Json type error";
        emit responseFailure(0);
        return;
    }
    if (jsonDocument.isNull() || jsonDocument.isEmpty()) {
        qDebug() << "Json null or empty!";
        emit responseFailure(0);
        return;
    }

    QJsonObject jsonObject = jsonDocument.object();
    //qDebug() << "jsonObject" << jsonObject;
    if (jsonObject.isEmpty() || jsonObject.size() == 0) {
        emit responseFailure(0);
        return;
    }
    if (jsonObject.contains("KylinWeather")) {
        QJsonObject mainObj = jsonObject.value("KylinWeather").toObject();
        if (mainObj.isEmpty() || mainObj.size() == 0) {
            emit responseFailure(0);
            return;
        }

        if (mainObj.contains("air")) {
            QJsonObject airObj = mainObj.value("air").toObject();
            if (!airObj.isEmpty() && airObj.size() > 0) {
                m_preferences->air.aqi = airObj.value("aqi").toString();
                m_preferences->air.qlty = airObj.value("qlty").toString();
                m_preferences->air.main = airObj.value("main").toString();
                m_preferences->air.pm25 = airObj.value("pm25").toString();
                m_preferences->air.pm10 = airObj.value("pm10").toString();
                m_preferences->air.no2 = airObj.value("no2").toString();
                m_preferences->air.so2 = airObj.value("so2").toString();
                m_preferences->air.co = airObj.value("co").toString();
                m_preferences->air.o3 = airObj.value("o3").toString();

                m_preferences->weather.air = QString("%1(%2)").arg(airObj.value("aqi").toString()).arg(airObj.value("qlty").toString());
            }
        }
        if (mainObj.contains("weather")) {
            QJsonObject weatherObj = mainObj.value("weather").toObject();
            if (!weatherObj.isEmpty() && weatherObj.size() > 0) {
                m_preferences->air.id = weatherObj.value("id").toString();//如果有weather，则给id赋值

                m_preferences->weather.id = weatherObj.value("id").toString();
                m_preferences->weather.city = weatherObj.value("location").toString();
                m_preferences->weather.updatetime = weatherObj.value("update_loc").toString();
                m_preferences->weather.cloud = weatherObj.value("cloud").toString();
                m_preferences->weather.cond_code = weatherObj.value("cond_code").toString();
                m_preferences->weather.cond_txt = weatherObj.value("cond_txt").toString();
                m_preferences->weather.fl = weatherObj.value("fl").toString();
                m_preferences->weather.hum = weatherObj.value("hum").toString();
                m_preferences->weather.pcpn = weatherObj.value("pcpn").toString();
                m_preferences->weather.pres = weatherObj.value("pres").toString();
                m_preferences->weather.tmp = weatherObj.value("tmp").toString();
                m_preferences->weather.vis = weatherObj.value("vis").toString();
                m_preferences->weather.wind_deg = weatherObj.value("wind_deg").toString();
                m_preferences->weather.wind_dir = weatherObj.value("wind_dir").toString();
                m_preferences->weather.wind_sc = weatherObj.value("wind_sc").toString();
                m_preferences->weather.wind_spd = weatherObj.value("wind_spd").toString();
            }
        }

        emit this->observeDataRefreshed(m_preferences->weather);
    }
}

void WeatherWorker::onWeatherForecastReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (statusCode == 301 || statusCode == 302) {//redirect
        bool redirection = false;
        QVariant redirectionUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
        //qDebug() << "Forecast: redirectionUrl=" << redirectionUrl.toString();
        redirection = AccessDedirectUrl(redirectionUrl.toString(), WeatherType::Type_Forecast);//AccessDedirectUrl(reply->rawHeader("Location"));
        reply->close();
        reply->deleteLater();
        if (!redirection) {
            emit responseFailure(statusCode);
        }
        return;
    }
    else if (statusCode == 400) {
        qDebug() << "Forecast: Network error (HTTP400/Bad Request)";
        emit responseFailure(statusCode);
        return;
    }
    else if (statusCode == 403) {
        qDebug() << "Forecast: Username or password invalid (permission denied)";
        emit responseFailure(statusCode);
        return;
    }
    else if (statusCode == 200) {
        // 200 is normal status
    }
    else {
        emit responseFailure(statusCode);
        return;
    }

    if(reply->error() != QNetworkReply::NoError) {
        //qDebug() << "weather forecast request error:" << reply->error() << ", statusCode=" << statusCode;
        emit responseFailure(statusCode);
        return;
    }

    /*
    bool redirection = false;
    if(reply->error() != QNetworkReply::NoError || statusCode != 200) {//200 is normal status
        qDebug() << "weather forecast request error:" << reply->error() << ", statusCode=" << statusCode;
        if (statusCode == 301 || statusCode == 302) {//redirect
            QVariant redirectionUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
            qDebug() << "redirectionUrl=" << redirectionUrl.toString();
            redirection = AccessDedirectUrl(redirectionUrl.toString(), WeatherType::Type_Forecast);//AccessDedirectUrl(reply->rawHeader("Location"));
            reply->close();
            reply->deleteLater();
        }
        if (!redirection) {
            emit responseFailure(statusCode);
        }
        return;
    }
    */

    QByteArray ba = reply->readAll();
    //QString reply_content = QString::fromUtf8(ba);
    reply->close();
    reply->deleteLater();
    //qDebug() << "weather forecast size: " << ba.size();

    QJsonParseError err;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(ba, &err);
    if (err.error != QJsonParseError::NoError) {// Json type error
        qDebug() << "Json type error";
        emit responseFailure(0);
        return;
    }
    if (jsonDocument.isNull() || jsonDocument.isEmpty()) {
        qDebug() << "Json null or empty!";
        emit responseFailure(0);
        return;
    }

    QJsonObject jsonObject = jsonDocument.object();
    //qDebug() << "jsonObject" << jsonObject;
    if (jsonObject.isEmpty() || jsonObject.size() == 0) {
        emit responseFailure(0);
        return;
    }
    if (jsonObject.contains("KylinWeather")) {
        QJsonObject mainObj = jsonObject.value("KylinWeather").toObject();
        if (mainObj.isEmpty() || mainObj.size() == 0) {
            emit responseFailure(0);
            return;
        }

        QList<ForecastWeather> forecastDatas;

        if (mainObj.contains("forecast")) {
            QJsonObject forecastObj = mainObj.value("forecast").toObject();
            if (!forecastObj.isEmpty() && forecastObj.size() > 0) {
                m_preferences->forecast0.forcast_date = forecastObj.value("forcast_date0").toString();
                m_preferences->forecast0.cond_code_d = forecastObj.value("cond_code_d0").toString();
                m_preferences->forecast0.cond_code_n = forecastObj.value("cond_code_n0").toString();
                m_preferences->forecast0.cond_txt_d = forecastObj.value("cond_txt_d0").toString();
                m_preferences->forecast0.cond_txt_n = forecastObj.value("cond_txt_n0").toString();
                m_preferences->forecast0.hum = forecastObj.value("hum0").toString();
                m_preferences->forecast0.mr_ms = forecastObj.value("mr_ms0").toString();
                m_preferences->forecast0.pcpn = forecastObj.value("pcpn0").toString();
                m_preferences->forecast0.pop = forecastObj.value("pop0").toString();
                m_preferences->forecast0.pres = forecastObj.value("pres0").toString();
                m_preferences->forecast0.sr_ss = forecastObj.value("sr_ss0").toString();
                m_preferences->forecast0.tmp_max = forecastObj.value("tmp_max0").toString();
                m_preferences->forecast0.tmp_min = forecastObj.value("tmp_min0").toString();
                m_preferences->forecast0.uv_index = forecastObj.value("uv_index0").toString();
                m_preferences->forecast0.vis = forecastObj.value("vis0").toString();
                m_preferences->forecast0.wind_deg = forecastObj.value("wind_deg0").toString();
                m_preferences->forecast0.wind_dir = forecastObj.value("wind_dir0").toString();
                m_preferences->forecast0.wind_sc = forecastObj.value("wind_sc0").toString();
                m_preferences->forecast0.wind_spd = forecastObj.value("wind_spd0").toString();

                m_preferences->forecast1.forcast_date = forecastObj.value("forcast_date1").toString();
                m_preferences->forecast1.cond_code_d = forecastObj.value("cond_code_d1").toString();
                m_preferences->forecast1.cond_code_n = forecastObj.value("cond_code_n1").toString();
                m_preferences->forecast1.cond_txt_d = forecastObj.value("cond_txt_d1").toString();
                m_preferences->forecast1.cond_txt_n = forecastObj.value("cond_txt_n1").toString();
                m_preferences->forecast1.hum = forecastObj.value("hum1").toString();
                m_preferences->forecast1.mr_ms = forecastObj.value("mr_ms1").toString();
                m_preferences->forecast1.pcpn = forecastObj.value("pcpn1").toString();
                m_preferences->forecast1.pop = forecastObj.value("pop1").toString();
                m_preferences->forecast1.pres = forecastObj.value("pres1").toString();
                m_preferences->forecast1.sr_ss = forecastObj.value("sr_ss1").toString();
                m_preferences->forecast1.tmp_max = forecastObj.value("tmp_max1").toString();
                m_preferences->forecast1.tmp_min = forecastObj.value("tmp_min1").toString();
                m_preferences->forecast1.uv_index = forecastObj.value("uv_index1").toString();
                m_preferences->forecast1.vis = forecastObj.value("vis1").toString();
                m_preferences->forecast1.wind_deg = forecastObj.value("wind_deg1").toString();
                m_preferences->forecast1.wind_dir = forecastObj.value("wind_dir1").toString();
                m_preferences->forecast1.wind_sc = forecastObj.value("wind_sc1").toString();
                m_preferences->forecast1.wind_spd = forecastObj.value("wind_spd1").toString();

                m_preferences->forecast2.forcast_date = forecastObj.value("forcast_date2").toString();
                m_preferences->forecast2.cond_code_d = forecastObj.value("cond_code_d2").toString();
                m_preferences->forecast2.cond_code_n = forecastObj.value("cond_code_n2").toString();
                m_preferences->forecast2.cond_txt_d = forecastObj.value("cond_txt_d2").toString();
                m_preferences->forecast2.cond_txt_n = forecastObj.value("cond_txt_n2").toString();
                m_preferences->forecast2.hum = forecastObj.value("hum2").toString();
                m_preferences->forecast2.mr_ms = forecastObj.value("mr_ms2").toString();
                m_preferences->forecast2.pcpn = forecastObj.value("pcpn2").toString();
                m_preferences->forecast2.pop = forecastObj.value("pop2").toString();
                m_preferences->forecast2.pres = forecastObj.value("pres2").toString();
                m_preferences->forecast2.sr_ss = forecastObj.value("sr_ss2").toString();
                m_preferences->forecast2.tmp_max = forecastObj.value("tmp_max2").toString();
                m_preferences->forecast2.tmp_min = forecastObj.value("tmp_min2").toString();
                m_preferences->forecast2.uv_index = forecastObj.value("uv_index2").toString();
                m_preferences->forecast2.vis = forecastObj.value("vis2").toString();
                m_preferences->forecast2.wind_deg = forecastObj.value("wind_deg2").toString();
                m_preferences->forecast2.wind_dir = forecastObj.value("wind_dir2").toString();
                m_preferences->forecast2.wind_sc = forecastObj.value("wind_sc2").toString();
                m_preferences->forecast2.wind_spd = forecastObj.value("wind_spd2").toString();


                forecastDatas.append(m_preferences->forecast0);
                forecastDatas.append(m_preferences->forecast1);
                forecastDatas.append(m_preferences->forecast2);
            }
        }

        if (mainObj.contains("lifestyle")) {
            QJsonObject lifestyleObj = mainObj.value("lifestyle").toObject();
            if (!lifestyleObj.isEmpty() && lifestyleObj.size() > 0) {
                m_preferences->lifestyle.air_brf = lifestyleObj.value("air_brf").toString();
                m_preferences->lifestyle.air_txt = lifestyleObj.value("air_txt").toString();
                m_preferences->lifestyle.comf_brf = lifestyleObj.value("comf_brf").toString();
                m_preferences->lifestyle.comf_txt = lifestyleObj.value("comf_txt").toString();
                m_preferences->lifestyle.cw_brf = lifestyleObj.value("cw_brf").toString();
                m_preferences->lifestyle.cw_txt = lifestyleObj.value("cw_txt").toString();
                m_preferences->lifestyle.drsg_brf = lifestyleObj.value("drsg_brf").toString();
                m_preferences->lifestyle.drsg_txt = lifestyleObj.value("drsg_txt").toString();
                m_preferences->lifestyle.flu_brf = lifestyleObj.value("flu_brf").toString();
                m_preferences->lifestyle.flu_txt = lifestyleObj.value("flu_txt").toString();
                m_preferences->lifestyle.sport_brf = lifestyleObj.value("sport_brf").toString();
                m_preferences->lifestyle.sport_txt = lifestyleObj.value("sport_txt").toString();
                m_preferences->lifestyle.trav_brf = lifestyleObj.value("trav_brf").toString();
                m_preferences->lifestyle.trav_txt = lifestyleObj.value("trav_txt").toString();
                m_preferences->lifestyle.uv_brf = lifestyleObj.value("uv_brf").toString();
                m_preferences->lifestyle.uv_txt = lifestyleObj.value("uv_txt").toString();
            }
        }

        emit this->forecastDataRefreshed(forecastDatas, m_preferences->lifestyle);
    }
}

void WeatherWorker::onPingBackPostReply()
{
    QNetworkReply *m_reply = qobject_cast<QNetworkReply*>(sender());
    int statusCode = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(m_reply->error() != QNetworkReply::NoError || statusCode != 200) {//200 is normal status
        //qDebug() << "post host info request error:" << m_reply->error() << ", statusCode=" << statusCode;
        if (statusCode == 301 || statusCode == 302) {//redirect
            QVariant redirectionUrl = m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
            //qDebug() << "pingback redirectionUrl=" << redirectionUrl.toString();
            AccessDedirectUrlWithPost(redirectionUrl.toString());
            m_reply->close();
            m_reply->deleteLater();
        }
        return;
    }

    //QByteArray ba = m_reply->readAll();
    m_reply->close();
    m_reply->deleteLater();
    //QString reply_content = QString::fromUtf8(ba);
    //qDebug() << "return size: " << ba.size() << reply_content;
}

/*  http://www.heweather.com/documents/status-code  */
QString WeatherWorker::getErrorCodeDescription(QString errorCode)
{
    if ("ok" == errorCode) {
        return "数据正常";
    }
    else if ("invalid key" == errorCode) {
        return "错误的key，请检查你的key是否输入以及是否输入有误";
    }
    else if ("unknown location" == errorCode) {
        return "未知或错误城市/地区";
    }
    else if ("no data for this location" == errorCode) {
        return "该城市/地区没有你所请求的数据";
    }
    else if ("no more requests" == errorCode) {
        return "超过访问次数，需要等到当月最后一天24点（免费用户为当天24点）后进行访问次数的重置或升级你的访问量";
    }
    else if ("param invalid" == errorCode) {
        return "参数错误，请检查你传递的参数是否正确";
    }
    else if ("too fast" == errorCode) {//http://www.heweather.com/documents/qpm
        return "超过限定的QPM，请参考QPM说明";
    }
    else if ("dead" == errorCode) {//http://www.heweather.com/contact
        return "无响应或超时，接口服务异常请联系我们";
    }
    else if ("permission denied" == errorCode) {
        return "无访问权限，你没有购买你所访问的这部分服务";
    }
    else if ("sign error" == errorCode) {//http://www.heweather.com/documents/api/s6/sercet-authorization
        return "签名错误，请参考签名算法";
    }
    else {
        return tr("Unknown");
    }
}


//void WeatherWorker::setAutomaticCity(const QString &cityName)
//{
//    bool autoSuccess = false;
//    CitySettingData info;

//    if (cityName.isEmpty()) {
//        emit this->requestAutoLocationData(info, false);
//        return;
//    }
//    //CN101250101,changsha,长沙,CN,China,中国,hunan,湖南,changsha,长沙,28.19409,112.98228,"430101,430100,430000",
//    QFile file(":/data/data/china-city-list.csv");
//    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        QString line = file.readLine();
//        line = line.replace("\n", "");
//        while (!line.isEmpty()) {
//            QStringList resultList = line.split(",");
//            if (resultList.length() < 10) {
//                line = file.readLine();
//                line = line.replace("\n", "");
//                continue;
//            }

//            QString id = resultList.at(0);
//            if (!id.startsWith("CN")) {
//                line = file.readLine();
//                line = line.replace("\n", "");
//                continue;
//            }

//            if (resultList.at(1).compare(cityName, Qt::CaseInsensitive) == 0 ||
//                resultList.at(2).compare(cityName, Qt::CaseInsensitive) == 0 ||
//                QString(resultList.at(2) + "市").compare(cityName, Qt::CaseInsensitive) == 0 ||
//                QString(resultList.at(2) + "区").compare(cityName, Qt::CaseInsensitive) == 0 ||
//                QString(resultList.at(2) + "县").compare(cityName, Qt::CaseInsensitive) == 0) {
//                id.remove(0, 2);//remove "CN"
//                QString name = resultList.at(2);

//                if (m_preferences->isCitiesCountOverMax()) {
//                    if (m_preferences->isCityIdExist(id)) {
//                        //从已有列表中将自动定位的城市设置为默认城市
//                        m_preferences->setCurrentCityIdAndName(name);
//                    }
//                    else {
//                        break;
//                    }
//                }
//                else {
//                    if (m_preferences->isCityIdExist(id)) {
//                        m_preferences->setCurrentCityIdAndName(name);
//                    }
//                    else {
//                        City city;
//                        city.id = id;
//                        city.name = name;
//                        m_preferences->setCurrentCityIdAndName(name);
//                        m_preferences->addCityInfoToPref(city);
//                        m_preferences->save();
//                    }
//                }

//                info.active = false;
//                info.id = id;
//                info.name = name;
//                info.icon = ":/res/weather_icons/darkgrey/100.png";

//                autoSuccess = true;
//                break;
//            }

//            line = file.readLine();
//            line = line.replace("\n", "");
//        }
//        file.close();
//    }

//    if (autoSuccess) {
//        emit this->requestAutoLocationData(info, true);
//    }
//    else {
//        emit this->requestAutoLocationData(info, false);
//    }
//}
