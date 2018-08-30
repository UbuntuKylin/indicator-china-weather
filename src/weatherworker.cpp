/*
 * Copyright (C) 2013 ~ 2018 National University of Defense Technology(NUDT) & Tianjin Kylin Ltd.
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

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QEventLoop>

WeatherWorker::WeatherWorker(QObject *parent) :
    QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
{
    connect(m_networkManager, &QNetworkAccessManager::finished, this, [] (QNetworkReply *reply) {
        reply->deleteLater();
    });
}

WeatherWorker::~WeatherWorker()
{

}

bool WeatherWorker::isNetWorkSettingsGood()
{
    //判断网络是否有连接，不一定能上网
    QNetworkConfigurationManager mgr;
    return mgr.isOnline();
}

void WeatherWorker::netWorkOnlineOrNot()
{
    //http://service.ubuntukylin.com:8001/weather/pingnetwork/
    QHostInfo::lookupHost("www.baidu.com", this, SLOT(networkLookedUp(QHostInfo)));
}

void WeatherWorker::networkLookedUp(const QHostInfo &host)
{
    if(host.error() != QHostInfo::NoError) {
        qDebug() << "test network failed, errorCode:" << host.error();
        emit this->nofityNetworkStatus(false);
    }
    else {
        qDebug() << "test network success, the server's ip:" << host.addresses().first().toString();
        emit this->nofityNetworkStatus(true);
    }
}

void WeatherWorker::refreshObserveWeatherData(const QString &cityId)
{
    if (cityId.isEmpty())
        return;

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
    if (cityId.isEmpty())
        return;

    //heweather_forecast_s6
    QString forecastUrl = QString("http://service.ubuntukylin.com:8001/weather/api/2.0/heweather_forecast_s6/%1").arg(cityId);
    QNetworkReply *reply = m_networkManager->get(QNetworkRequest(forecastUrl));
    connect(reply, &QNetworkReply::finished, this, &WeatherWorker::onWeatherForecastReply);
}

void WeatherWorker::requestPingBackWeatherServer()
{
    QNetworkReply *reply = m_networkManager->get(QNetworkRequest(QString("http://service.ubuntukylin.com:8001/weather/pingnetwork/")));
    connect(reply, &QNetworkReply::finished, this, [=] () {
        QNetworkReply *m_reply = qobject_cast<QNetworkReply*>(sender());
        int statusCode = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if(m_reply->error() != QNetworkReply::NoError || statusCode != 200) {//200 is normal status
            qDebug() << "pingback request error:" << m_reply->error() << ", statusCode=" << statusCode;
            return;
        }

        QByteArray ba = m_reply->readAll();
        m_reply->close();
        m_reply->deleteLater();
        QString reply_content = QString::fromUtf8(ba);
        qDebug() << "pingback size: " << ba.size() << reply_content;
    });
}

void WeatherWorker::requestPostHostInfoToWeatherServer(QString hostInfo)
{
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

void WeatherWorker::AccessDedirectUrl(const QString &redirectUrl, WeatherType weatherType)
{
    if (redirectUrl.isEmpty())
        return;

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
    if(reply->error() != QNetworkReply::NoError || statusCode != 200) {//200 is normal status
        qDebug() << "weather request error:" << reply->error() << ", statusCode=" << statusCode;
        if (statusCode == 301 || statusCode == 302) {//redirect
            QVariant redirectionUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
            qDebug() << "redirectionUrl=" << redirectionUrl.toString();
            AccessDedirectUrl(redirectionUrl.toString(), WeatherType::Type_Observe);//AccessDedirectUrl(reply->rawHeader("Location"));
            reply->close();
            reply->deleteLater();
        }
        return;
    }

    QByteArray ba = reply->readAll();
    //QString reply_content = QString::fromUtf8(ba);
    reply->close();
    reply->deleteLater();
    qDebug() << "weather observe size: " << ba.size();

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
    qDebug() << "jsonObject" << jsonObject;

    QJsonObject mainObj = jsonObject.value("KylinWeather").toObject();
    QJsonObject airObj = mainObj.value("air").toObject();
    qDebug() << "aqi=" << airObj.value("aqi").toString();
    qDebug() << "co=" << airObj.value("co").toString();
    qDebug() << "main=" << airObj.value("main").toString();
    qDebug() << "no2=" << airObj.value("no2").toString();
    qDebug() << "o3=" << airObj.value("o3").toString();
    qDebug() << "pm10=" << airObj.value("pm10").toString();
    qDebug() << "pm25=" << airObj.value("pm25").toString();
    qDebug() << "qlty=" << airObj.value("qlty").toString();
    qDebug() << "so2=" << airObj.value("so2").toString();

    QJsonObject weatherObj = mainObj.value("weather").toObject();
    qDebug() << "admin_area=" << weatherObj.value("admin_area").toString();
    qDebug() << "cloud=" << weatherObj.value("cloud").toString();
    qDebug() << "cnty=" << weatherObj.value("cnty").toString();
    qDebug() << "cond_code=" << weatherObj.value("cond_code").toString();
    qDebug() << "cond_txt=" << weatherObj.value("cond_txt").toString();
    qDebug() << "fl=" << weatherObj.value("fl").toString();
    qDebug() << "hum=" << weatherObj.value("hum").toString();
    qDebug() << "id=" << weatherObj.value("id").toString();
    qDebug() << "lat=" << weatherObj.value("lat").toString();
    qDebug() << "lon=" << weatherObj.value("lon").toString();
    qDebug() << "location=" << weatherObj.value("location").toString();
    qDebug() << "parent_city=" << weatherObj.value("parent_city").toString();
    qDebug() << "pcpn=" << weatherObj.value("pcpn").toString();
    qDebug() << "pres=" << weatherObj.value("pres").toString();
    qDebug() << "tmp=" << weatherObj.value("tmp").toString();
    qDebug() << "tz=" << weatherObj.value("tz").toString();
    qDebug() << "update_loc=" << weatherObj.value("update_loc").toString();
    qDebug() << "update_utc=" << weatherObj.value("update_utc").toString();
    qDebug() << "wind_deg=" << weatherObj.value("wind_deg").toString();
    qDebug() << "wind_dir=" << weatherObj.value("wind_dir").toString();
    qDebug() << "wind_sc=" << weatherObj.value("wind_sc").toString();
    qDebug() << "wind_spd=" << weatherObj.value("wind_spd").toString();

    ObserveWeather observeData;
    observeData.id = weatherObj.value("id").toString();
    observeData.city = weatherObj.value("location").toString();

    emit this->observeDataRefreshed(observeData);
}

void WeatherWorker::onWeatherForecastReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(reply->error() != QNetworkReply::NoError || statusCode != 200) {//200 is normal status
        qDebug() << "weather forecast request error:" << reply->error() << ", statusCode=" << statusCode;
        if (statusCode == 301 || statusCode == 302) {//redirect
            QVariant redirectionUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
            qDebug() << "redirectionUrl=" << redirectionUrl.toString();
            AccessDedirectUrl(redirectionUrl.toString(), WeatherType::Type_Forecast);//AccessDedirectUrl(reply->rawHeader("Location"));
            reply->close();
            reply->deleteLater();
        }
        return;
    }

    QByteArray ba = reply->readAll();
    //QString reply_content = QString::fromUtf8(ba);
    reply->close();
    reply->deleteLater();
    qDebug() << "weather forecast size: " << ba.size();

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
    qDebug() << "jsonObject" << jsonObject;

    QJsonObject mainObj = jsonObject.value("KylinWeather").toObject();
    QJsonObject forecastObj = mainObj.value("forecast").toObject();
    qDebug() << "cond_code_d0=" << forecastObj.value("cond_code_d0").toString();
    qDebug() << "cond_code_d1=" << forecastObj.value("cond_code_d1").toString();
    qDebug() << "cond_code_d2=" << forecastObj.value("cond_code_d2").toString();
    qDebug() << "cond_code_n0=" << forecastObj.value("cond_code_n0").toString();
    qDebug() << "cond_code_n1=" << forecastObj.value("cond_code_n1").toString();
    qDebug() << "cond_code_n2=" << forecastObj.value("cond_code_n2").toString();
    qDebug() << "cond_txt_d0=" << forecastObj.value("cond_txt_d0").toString();
    qDebug() << "cond_txt_d1=" << forecastObj.value("cond_txt_d1").toString();
    qDebug() << "cond_txt_d2=" << forecastObj.value("cond_txt_d2").toString();
    qDebug() << "cond_txt_n0=" << forecastObj.value("cond_txt_n0").toString();
    qDebug() << "cond_txt_n1=" << forecastObj.value("cond_txt_n1").toString();
    qDebug() << "cond_txt_n2=" << forecastObj.value("cond_txt_n2").toString();
    qDebug() << "forcast_date0=" << forecastObj.value("forcast_date0").toString();
    qDebug() << "forcast_date1=" << forecastObj.value("forcast_date1").toString();
    qDebug() << "forcast_date2=" << forecastObj.value("forcast_date2").toString();
    qDebug() << "hum0=" << forecastObj.value("hum0").toString();
    qDebug() << "hum1=" << forecastObj.value("hum1").toString();
    qDebug() << "hum2=" << forecastObj.value("hum2").toString();
    qDebug() << "id=" << forecastObj.value("id").toString();
    qDebug() << "location=" << forecastObj.value("location").toString();
    qDebug() << "mr_ms0=" << forecastObj.value("mr_ms0").toString();
    qDebug() << "mr_ms1=" << forecastObj.value("mr_ms1").toString();
    qDebug() << "mr_ms2=" << forecastObj.value("mr_ms2").toString();
    qDebug() << "pcpn0=" << forecastObj.value("pcpn0").toString();
    qDebug() << "pcpn1=" << forecastObj.value("pcpn1").toString();
    qDebug() << "pcpn2=" << forecastObj.value("pcpn2").toString();
    qDebug() << "pop0=" << forecastObj.value("pop0").toString();
    qDebug() << "pop1=" << forecastObj.value("pop1").toString();
    qDebug() << "pop2=" << forecastObj.value("pop2").toString();
    qDebug() << "pres0=" << forecastObj.value("pres0").toString();
    qDebug() << "pres1=" << forecastObj.value("pres1").toString();
    qDebug() << "pres2=" << forecastObj.value("pres2").toString();
    qDebug() << "sr_ss0=" << forecastObj.value("sr_ss0").toString();
    qDebug() << "sr_ss1=" << forecastObj.value("sr_ss1").toString();
    qDebug() << "sr_ss2=" << forecastObj.value("sr_ss2").toString();
    qDebug() << "tmp_max0=" << forecastObj.value("tmp_max0").toString();
    qDebug() << "tmp_max1=" << forecastObj.value("tmp_max1").toString();
    qDebug() << "tmp_max2=" << forecastObj.value("tmp_max2").toString();
    qDebug() << "tmp_min0=" << forecastObj.value("tmp_min0").toString();
    qDebug() << "tmp_min1=" << forecastObj.value("tmp_min1").toString();
    qDebug() << "tmp_min2=" << forecastObj.value("tmp_min2").toString();
    qDebug() << "update_loc=" << forecastObj.value("update_loc").toString();
    qDebug() << "uv_index0=" << forecastObj.value("uv_index0").toString();
    qDebug() << "uv_index1=" << forecastObj.value("uv_index1").toString();
    qDebug() << "uv_index2=" << forecastObj.value("uv_index2").toString();
    qDebug() << "vis0=" << forecastObj.value("vis0").toString();
    qDebug() << "vis1=" << forecastObj.value("vis1").toString();
    qDebug() << "vis2=" << forecastObj.value("vis2").toString();
    qDebug() << "wind_deg0=" << forecastObj.value("wind_deg0").toString();
    qDebug() << "wind_deg1=" << forecastObj.value("wind_deg1").toString();
    qDebug() << "wind_deg2=" << forecastObj.value("wind_deg2").toString();
    qDebug() << "wind_dir0=" << forecastObj.value("wind_dir0").toString();
    qDebug() << "wind_dir1=" << forecastObj.value("wind_dir1").toString();
    qDebug() << "wind_dir2=" << forecastObj.value("wind_dir2").toString();
    qDebug() << "wind_sc0=" << forecastObj.value("wind_sc0").toString();
    qDebug() << "wind_sc1=" << forecastObj.value("wind_sc1").toString();
    qDebug() << "wind_sc2=" << forecastObj.value("wind_sc2").toString();
    qDebug() << "wind_spd0=" << forecastObj.value("wind_spd0").toString();
    qDebug() << "wind_spd1=" << forecastObj.value("wind_spd1").toString();
    qDebug() << "wind_spd2=" << forecastObj.value("wind_spd2").toString();

    QJsonObject lifestyleObj = mainObj.value("lifestyle").toObject();
    qDebug() << "air_brf=" << lifestyleObj.value("air_brf").toString();
    qDebug() << "air_txt=" << lifestyleObj.value("air_txt").toString();
    qDebug() << "comf_brf=" << lifestyleObj.value("comf_brf").toString();
    qDebug() << "comf_txt=" << lifestyleObj.value("comf_txt").toString();
    qDebug() << "cw_brf=" << lifestyleObj.value("cw_brf").toString();
    qDebug() << "cw_txt=" << lifestyleObj.value("cw_txt").toString();
    qDebug() << "drsg_brf=" << lifestyleObj.value("drsg_brf").toString();
    qDebug() << "drsg_txt=" << lifestyleObj.value("drsg_txt").toString();
    qDebug() << "flu_brf=" << lifestyleObj.value("flu_brf").toString();
    qDebug() << "flu_txt=" << lifestyleObj.value("flu_txt").toString();
    qDebug() << "sport_brf=" << lifestyleObj.value("sport_brf").toString();
    qDebug() << "sport_txt=" << lifestyleObj.value("sport_txt").toString();
    qDebug() << "trav_brf=" << lifestyleObj.value("trav_brf").toString();
    qDebug() << "trav_txt=" << lifestyleObj.value("trav_txt").toString();
    qDebug() << "uv_brf=" << lifestyleObj.value("uv_brf").toString();
    qDebug() << "uv_txt=" << lifestyleObj.value("uv_txt").toString();

    /*ForecastWeather forecastData[3];
    for (int i = 0; i < 3; i++) {
        forecastData[i].cond_code_d = "N/A";
        forecastData[i].cond_code_n = "N/A";
        forecastData[i].cond_txt_d = "N/A";
        forecastData[i].cond_txt_n = "N/A";
        forecastData[i].forcast_date = "N/A";
        forecastData[i].hum = "N/A";
        forecastData[i].mr_ms = "N/A";
        forecastData[i].pcpn = "N/A";
        forecastData[i].pop = "N/A";
        forecastData[i].pres = "N/A";
        forecastData[i].sr_ss = "N/A";
        forecastData[i].tmp_max = "N/A";
        forecastData[i].tmp_min = "N/A";
        forecastData[i].uv_index = "N/A";
        forecastData[i].vis = "N/A";
        forecastData[i].wind_deg = "N/A";
        forecastData[i].wind_dir = "N/A";
        forecastData[i].wind_sc = "N/A";
        forecastData[i].wind_spd = "N/A";
    }*/

    QList<ForecastWeather> forecastDatas;

    ForecastWeather data0;
    data0.forcast_date = forecastObj.value("forcast_date0").toString();

    ForecastWeather data1;
    data1.forcast_date = forecastObj.value("forcast_date1").toString();

    ForecastWeather data2;
    data2.forcast_date = forecastObj.value("forcast_date2").toString();

    forecastDatas.append(data0);
    forecastDatas.append(data1);
    forecastDatas.append(data2);

    emit this->forecastDataRefreshed(forecastDatas);
}

void WeatherWorker::onPingBackPostReply()
{
    QNetworkReply *m_reply = qobject_cast<QNetworkReply*>(sender());
    int statusCode = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(m_reply->error() != QNetworkReply::NoError || statusCode != 200) {//200 is normal status
        qDebug() << "post host info request error:" << m_reply->error() << ", statusCode=" << statusCode;
        if (statusCode == 301 || statusCode == 302) {//redirect
            QVariant redirectionUrl = m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
            qDebug() << "pingback redirectionUrl=" << redirectionUrl.toString();
            AccessDedirectUrlWithPost(redirectionUrl.toString());
            m_reply->close();
            m_reply->deleteLater();
        }
        return;
    }

    QByteArray ba = m_reply->readAll();
    m_reply->close();
    m_reply->deleteLater();
    QString reply_content = QString::fromUtf8(ba);
    qDebug() << "return size: " << ba.size() << reply_content;
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
