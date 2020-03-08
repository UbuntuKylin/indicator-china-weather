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
#include "preferences.h"
#include "global.h"
using namespace Global;

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QEventLoop>
#include <QFile>
#include <QApplication>
#include <QStringList>

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
    connect(m_networkManager, &QNetworkAccessManager::finished, this, [=] (QNetworkReply *reply) {
        reply->deleteLater();
    });

    connect(this, &WeatherWorker::requestTestNetwork, this, &WeatherWorker::onResponseTestNetwork);
    connect(this, &WeatherWorker::requestPostHostInfoToWeatherServer, this, &WeatherWorker::onPostHostInfoToWeatherServer);
    connect(this, &WeatherWorker::requestRefresheWeatherData, this, &WeatherWorker::onResponseRefresheWeatherData);
}

WeatherWorker::~WeatherWorker()
{
    m_networkManager->deleteLater();
}

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
    if (cityId.isEmpty()) {
        emit responseFailure(0);
        return;
    }

//    /*QString forecastUrl = QString("http://service.ubuntukylin.com:8001/weather/api/3.0/heweather_data_s6/%1").arg(cityId);
//    qDebug() << "forecastUrl=" << forecastUrl;
//    QNetworkAccessManager *manager = new QNetworkAccessManager();
//    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(forecastUrl)));
//    QByteArray responseData;
//    QEventLoop eventLoop;
//    QObject::connect(manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
//    eventLoop.exec();
//    responseData = reply->readAll();
//    reply->deleteLater();
//    manager->deleteLater();
//    qDebug() << "weather observe size: " << responseData.size();*/

    //heweather_observe_s6
    QString forecastUrl = QString("http://service.ubuntukylin.com:8001/weather/api/3.0/heweather_data_s6/%1").arg(cityId);
    QNetworkRequest request;
    request.setUrl(forecastUrl);
    //request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);//Qt5.6 for redirect
    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &WeatherWorker::onWeatherDataReply);
}

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

bool WeatherWorker::AccessDedirectUrl(const QString &redirectUrl)
{
    if (redirectUrl.isEmpty())
        return false;

    QNetworkRequest request;
    QString url;
    url = redirectUrl;
    request.setUrl(QUrl(url));

    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &WeatherWorker::onWeatherDataReply);

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

void WeatherWorker::onWeatherDataReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (statusCode == 301 || statusCode == 302) {//redirect
        bool redirection = false;
        QVariant redirectionUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
        //qDebug() << "Weather: redirectionUrl=" << redirectionUrl.toString();
        redirection = AccessDedirectUrl(redirectionUrl.toString());//AccessDedirectUrl(reply->rawHeader("Location"));
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

        if (mainObj.contains("lifestyle")) {
            QJsonObject lifestyleObj = mainObj.value("lifestyle").toObject();
            if (!lifestyleObj.isEmpty() && lifestyleObj.size() > 0) {
                m_preferences->m_lifestyle.air_brf = lifestyleObj.value("air_brf").toString();
                m_preferences->m_lifestyle.air_txt = lifestyleObj.value("air_txt").toString();
                m_preferences->m_lifestyle.comf_brf = lifestyleObj.value("comf_brf").toString();
                m_preferences->m_lifestyle.comf_txt = lifestyleObj.value("comf_txt").toString();
                m_preferences->m_lifestyle.cw_brf = lifestyleObj.value("cw_brf").toString();
                m_preferences->m_lifestyle.cw_txt = lifestyleObj.value("cw_txt").toString();
                m_preferences->m_lifestyle.drsg_brf = lifestyleObj.value("drsg_brf").toString();
                m_preferences->m_lifestyle.drsg_txt = lifestyleObj.value("drsg_txt").toString();
                m_preferences->m_lifestyle.flu_brf = lifestyleObj.value("flu_brf").toString();
                m_preferences->m_lifestyle.flu_txt = lifestyleObj.value("flu_txt").toString();
                m_preferences->m_lifestyle.sport_brf = lifestyleObj.value("sport_brf").toString();
                m_preferences->m_lifestyle.sport_txt = lifestyleObj.value("sport_txt").toString();
                m_preferences->m_lifestyle.trav_brf = lifestyleObj.value("trav_brf").toString();
                m_preferences->m_lifestyle.trav_txt = lifestyleObj.value("trav_txt").toString();
                m_preferences->m_lifestyle.uv_brf = lifestyleObj.value("uv_brf").toString();
                m_preferences->m_lifestyle.uv_txt = lifestyleObj.value("uv_txt").toString();
            }
        }
        if (mainObj.contains("weather")) {
            QJsonObject weatherObj = mainObj.value("weather").toObject();
            if (!weatherObj.isEmpty() && weatherObj.size() > 0) {
                m_preferences->m_observerWeather.id = weatherObj.value("id").toString();
                m_preferences->m_observerWeather.city = weatherObj.value("location").toString();
                m_preferences->m_observerWeather.updatetime = weatherObj.value("update_time").toString();

                m_preferences->m_aqiAir.id = m_preferences->m_observerWeather.id;

                QString aqi_msg = weatherObj.value("aqi").toString();
                QString forecast_msg = weatherObj.value("forecast").toString();
                int forecast_days = weatherObj.value("forecast_days").toInt();
                QString now_msg = weatherObj.value("now").toString();

//                qDebug() << "===================aqi_msg:" << aqi_msg;
//                qDebug() << "===================now_msg:" << now_msg;
//                qDebug() << "===================forecast_msg:" << forecast_msg;
//                qDebug() << "===================forecast_days:" << forecast_days;

                this->convertWeatherValue(Type_Aqi, aqi_msg, ',');
                this->convertWeatherValue(Type_Observe, now_msg, ',');
                this->convertWeatherValue(Type_Forecast, forecast_msg, ',');

                emit this->requesUiRefreshed();
            }
        }
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

void WeatherWorker::convertWeatherValue(WeatherType weatherType, const QString &line, QChar separator)
{
    switch (weatherType) {
    case WeatherType::Type_Observe:
    {
        if (!line.isEmpty() && line.contains(separator)) {
            QStringList resultList = line.split(separator, QString::SkipEmptyParts);
            foreach (QString result, resultList) {
                if (result.contains(QChar('='))) {
                    QStringList valueList = result.split(QChar('='), QString::SkipEmptyParts);
                    if (valueList.length() == 2) {
                        QString key = valueList.at(0);
                        if (key == "cloud") {
                            m_preferences->m_observerWeather.cloud = valueList.at(1);
                        }
                        else if (key == "cond_code") {
                            m_preferences->m_observerWeather.cond_code = valueList.at(1);
                        }
                        else if (key == "cond_txt") {
                            m_preferences->m_observerWeather.cond_txt = valueList.at(1);
                        }
                        else if (key == "fl") {
                            m_preferences->m_observerWeather.fl = valueList.at(1);
                        }
                        else if (key == "hum") {
                            m_preferences->m_observerWeather.hum = valueList.at(1);
                        }
                        else if (key == "pcpn") {
                            m_preferences->m_observerWeather.pcpn = valueList.at(1);
                        }
                        else if (key == "pres") {
                            m_preferences->m_observerWeather.pres = valueList.at(1);
                        }
                        else if (key == "tmp") {
                            m_preferences->m_observerWeather.tmp = valueList.at(1);
                        }
                        else if (key == "vis") {
                            m_preferences->m_observerWeather.vis = valueList.at(1);
                        }
                        else if (key == "wind_deg") {
                            m_preferences->m_observerWeather.wind_deg = valueList.at(1);
                        }
                        else if (key == "wind_dir") {
                            m_preferences->m_observerWeather.wind_dir = valueList.at(1);
                        }
                        else if (key == "wind_sc") {
                            m_preferences->m_observerWeather.wind_sc = valueList.at(1);
                        }
                        else if (key == "wind_spd") {
                            m_preferences->m_observerWeather.wind_spd = valueList.at(1);
                        }
                    }
                }
            }
        }
        break;
    }
    case WeatherType::Type_Forecast:
    {
        if (!line.isEmpty() && line.contains(separator) &&  line.contains(QChar(';'))) {
            QStringList forecastList = line.split(QChar(';'), QString::SkipEmptyParts);

            //清空配置中原来的天气预报数据
            m_preferences->m_forecastList.clear();

            foreach (QString forecstResult, forecastList) {//forecastList为多天天气预报数据列表，forecstResult为其中某一天的天气预报数据
                ForecastWeather forecast;
                QStringList resultList = forecstResult.split(separator, QString::SkipEmptyParts);
                foreach (QString result, resultList) {
                    if (result.contains(QChar('='))) {
                        QStringList valueList = result.split(QChar('='), QString::SkipEmptyParts);
                        if (valueList.length() == 2) {
                            QString key = valueList.at(0);
                            if (key == "date") {
                                forecast.forcast_date = valueList.at(1);
                            }
                            else if (key == "cond_code_d") {
                                forecast.cond_code_d = valueList.at(1);
                            }
                            else if (key == "cond_code_n") {
                                forecast.cond_code_n = valueList.at(1);
                            }
                            else if (key == "cond_txt_d") {
                                forecast.cond_txt_d = valueList.at(1);
                            }
                            else if (key == "cond_txt_n") {
                                forecast.cond_txt_n = valueList.at(1);
                            }
                            else if (key == "hum") {
                                forecast.hum = valueList.at(1);
                            }
                            else if (key == "mr") {
                                forecast.mr = valueList.at(1);
                            }
                            else if (key == "ms") {
                                forecast.ms = valueList.at(1);
                            }
                            else if (key == "pcpn") {
                                forecast.pcpn = valueList.at(1);
                            }
                            else if (key == "pop") {
                                forecast.pop = valueList.at(1);
                            }
                            else if (key == "pres") {
                                forecast.pres = valueList.at(1);
                            }
                            else if (key == "sr") {
                                forecast.sr = valueList.at(1);
                            }
                            else if (key == "ss") {
                                forecast.ss = valueList.at(1);
                            }
                            else if (key == "tmp_max") {
                                forecast.tmp_max = valueList.at(1);
                            }
                            else if (key == "tmp_min") {
                                forecast.tmp_min = valueList.at(1);
                            }
                            else if (key == "uv_index") {
                                forecast.uv_index = valueList.at(1);
                            }
                            else if (key == "vis") {
                                forecast.vis = valueList.at(1);
                            }
                            else if (key == "wind_deg") {
                                forecast.wind_deg = valueList.at(1);
                            }
                            else if (key == "wind_dir") {
                                forecast.wind_dir = valueList.at(1);
                            }
                            else if (key == "wind_sc") {
                                forecast.wind_sc = valueList.at(1);
                            }
                            else if (key == "wind_spd") {
                                forecast.wind_spd = valueList.at(1);
                            }
                        }
                    }
                }
                m_preferences->m_forecastList.append(forecast);
            }
        }
        break;
    }
    case WeatherType::Type_Aqi:
    {
        if (!line.isEmpty() && line.contains(separator)) {
            QStringList resultList = line.split(separator, QString::SkipEmptyParts);
            foreach (QString result, resultList) {
                if (result.contains(QChar('='))) {
                    QStringList valueList = result.split(QChar('='), QString::SkipEmptyParts);
                    if (valueList.length() == 2) {
                        QString key = valueList.at(0);
                        if (key == "aqi") {
                            m_preferences->m_aqiAir.aqi = valueList.at(1);
                        }
                        else if (key == "qlty") {
                            m_preferences->m_aqiAir.qlty = valueList.at(1);
                        }
                        else if (key == "main") {
                            m_preferences->m_aqiAir.main = valueList.at(1);
                        }
                        else if (key == "pm25") {
                            m_preferences->m_aqiAir.pm25 = valueList.at(1);
                        }
                        else if (key == "pm10") {
                            m_preferences->m_aqiAir.pm10 = valueList.at(1);
                        }
                        else if (key == "no2") {
                            m_preferences->m_aqiAir.no2 = valueList.at(1);
                        }
                        else if (key == "so2") {
                            m_preferences->m_aqiAir.so2 = valueList.at(1);
                        }
                        else if (key == "co") {
                            m_preferences->m_aqiAir.co = valueList.at(1);
                        }
                        else if (key == "o3") {
                            m_preferences->m_aqiAir.o3 = valueList.at(1);
                        }
                    }
                }
            }
            m_preferences->m_observerWeather.air = QString("%1(%2)").arg(m_preferences->m_aqiAir.aqi).arg(m_preferences->m_aqiAir.qlty);
        }
        break;
    }
    default:
        break;
    }
}
