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

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QEventLoop>
#include <QFile>
#include <QApplication>
#include <QStringList>
#include <QUrl>
#include <QVariant>

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
{
    m_networkManager = new QNetworkAccessManager(this);

    connect(this, &WeatherWorker::requestTestNetwork, this, &WeatherWorker::onResponseTestNetwork);
    connect(this, &WeatherWorker::requestPostHostInfoToWeatherServer, this, &WeatherWorker::onPostHostInfoToWeatherServer);
    connect(this, &WeatherWorker::requestGetTheWeatherData, this, &WeatherWorker::onWeatherDataRequest);

    initGsetting();
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

void WeatherWorker::onPostHostInfoToWeatherServer()
{
    QString osInfo = readOsInfo();
    QString hostInfo = QString("%1&version_weather=%2&city=%3").arg(osInfo).arg(qApp->applicationVersion()).arg(m_currentcityname);
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

//利用连接请求网络数据
void WeatherWorker::onWeatherDataRequest(const QString &cityId)
{
    if (cityId.isEmpty()) {
        return;
    }

    QString forecastUrl = QString("http://service.ubuntukylin.com:8001/weather/api/3.0/heweather_data_s6/%1/").arg(cityId);
    QNetworkRequest request;
    request.setUrl(forecastUrl);
    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &WeatherWorker::onWeatherDataReply );
}


//处理返回的网络数据
void WeatherWorker::onWeatherDataReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug()<<"Reply value of getting weather data by URL is: "<<statusCode;

    if (statusCode == 301 || statusCode == 302) {//redirect
        emit responseFailure(statusCode);
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
        qDebug() << "reply error!";
        return;
    }

    QByteArray ba = reply->readAll();
    //QString reply_content = QString::fromUtf8(ba);
    //qDebug() <<reply_content;
    reply->close();
    reply->deleteLater();

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
    if (jsonObject.isEmpty() || jsonObject.size() == 0) {
        qDebug() << "Json object null or empty!";
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
                LifeStyle m_lifestyle;
                m_lifestyle.air_brf = lifestyleObj.value("air_brf").toString();
                m_lifestyle.air_txt = lifestyleObj.value("air_txt").toString();
                m_lifestyle.comf_brf = lifestyleObj.value("comf_brf").toString();
                m_lifestyle.comf_txt = lifestyleObj.value("comf_txt").toString();
                m_lifestyle.cw_brf = lifestyleObj.value("cw_brf").toString();
                m_lifestyle.cw_txt = lifestyleObj.value("cw_txt").toString();
                m_lifestyle.drsg_brf = lifestyleObj.value("drsg_brf").toString();
                m_lifestyle.drsg_txt = lifestyleObj.value("drsg_txt").toString();
                m_lifestyle.flu_brf = lifestyleObj.value("flu_brf").toString();
                m_lifestyle.flu_txt = lifestyleObj.value("flu_txt").toString();
                m_lifestyle.sport_brf = lifestyleObj.value("sport_brf").toString();
                m_lifestyle.sport_txt = lifestyleObj.value("sport_txt").toString();
                m_lifestyle.trav_brf = lifestyleObj.value("trav_brf").toString();
                m_lifestyle.trav_txt = lifestyleObj.value("trav_txt").toString();
                m_lifestyle.uv_brf = lifestyleObj.value("uv_brf").toString();
                m_lifestyle.uv_txt = lifestyleObj.value("uv_txt").toString();

                emit this->requestSetLifeStyle(m_lifestyle);
            }
        }
        if (mainObj.contains("weather")) {
            QJsonObject weatherObj = mainObj.value("weather").toObject();
            if (!weatherObj.isEmpty() && weatherObj.size() > 0) {
                //处理实时天气
                ObserveWeather m_observeweather;

                QString id_msg = weatherObj.value("id").toString();
                if (id_msg != ""){
                    m_observeweather.id = id_msg;
                }

                QString location_msg = weatherObj.value("location").toString();
                if (location_msg != ""){
                    m_observeweather.city = location_msg;
                    m_currentcityname = location_msg;
                }

                QString now_msg = weatherObj.value("now").toString();
                if (now_msg != ""){
                    QStringList strList = now_msg.split(",");
                    QJsonObject m_json;
                    foreach(QString str, strList){
                        if (str != ""){
                            m_json.insert(str.split("=").at(0), str.split("=").at(1));
                        }
                    }

                    m_observeweather.tmp = m_json.value("tmp").toString();
                    m_observeweather.wind_sc = m_json.value("wind_sc").toString();
                    m_observeweather.cond_txt = m_json.value("cond_txt").toString();
                    m_observeweather.vis = m_json.value("vis").toString();
                    m_observeweather.hum = m_json.value("hum").toString();
                    m_observeweather.cond_code = m_json.value("cond_code").toString();
                    m_observeweather.wind_deg = m_json.value("wind_deg").toString();
                    m_observeweather.pcpn = m_json.value("pcpn").toString();
                    m_observeweather.pres = m_json.value("pres").toString();
                    m_observeweather.wind_spd = m_json.value("wind_spd").toString();
                    m_observeweather.wind_dir = m_json.value("wind_dir").toString();
                    m_observeweather.fl = m_json.value("fl").toString();
                    m_observeweather.cloud = m_json.value("cloud").toString();
                    m_observeweather.serveTime =weatherObj.value("update_time").toString();

                    QString weatherNow="";

                    weatherNow.append(m_observeweather.serveTime+",");//时间
                    weatherNow.append(m_observeweather.id+",");//省市编码
                    weatherNow.append(m_observeweather.city+",");//城市名称
                    weatherNow.append(m_observeweather.cond_txt+",");//天气情况
                    weatherNow.append(m_observeweather.hum+"%,");//湿度
                    weatherNow.append(m_observeweather.tmp+"℃,");//温度
                    weatherNow.append(m_observeweather.wind_dir+",");//风向
                    weatherNow.append(m_observeweather.wind_sc+"级,");//风力
                    weatherNow.append(weatherObj.value("admin_area").toString()+",");//省份
                    weatherNow.append(m_observeweather.cond_code+",");
                    setCityWeatherNow(weatherNow);//YYF 写入配置文件供其他组件调用

                    emit this->requestSetObserveWeather(m_observeweather);//用于设置主窗口

                }

                //处理预报天气
                QString forecast_msg = weatherObj.value("forecast").toString();
                if (forecast_msg != ""){
                    QStringList strList = forecast_msg.split(";");
                    foreach(QString strDay, strList){
                        QStringList strListDaySub;
                        QJsonObject m_json;
                        if (strDay != ""){
                            strListDaySub = strDay.split(",");

                            foreach(QString str, strListDaySub){
                                if (str != ""){
                                    m_json.insert(str.split("=").at(0), str.split("=").at(1));
                                }
                            }

                            ForecastWeather m_forecastweather;
                            m_forecastweather.uv_index = m_json.value("uv_index").toString();
                            m_forecastweather.wind_spd = m_json.value("wind_spd").toString();
                            m_forecastweather.sr = m_json.value("sr").toString();
                            m_forecastweather.wind_sc = m_json.value("wind_sc").toString();
                            m_forecastweather.ms = m_json.value("ms").toString();
                            m_forecastweather.cond_txt_d = m_json.value("cond_txt_d").toString();
                            m_forecastweather.vis = m_json.value("vis").toString();
                            m_forecastweather.ss = m_json.value("ss").toString();
                            m_forecastweather.hum = m_json.value("hum").toString();
                            m_forecastweather.cond_txt_n = m_json.value("cond_txt_n").toString();
                            m_forecastweather.pop = m_json.value("pop").toString();
                            m_forecastweather.wind_deg = m_json.value("wind_deg").toString();
                            m_forecastweather.pcpn = m_json.value("pcpn").toString();
                            m_forecastweather.wind_dir = m_json.value("wind_dir").toString();
                            m_forecastweather.cond_code_d = m_json.value("cond_code_d").toString();
                            m_forecastweather.mr = m_json.value("mr").toString();
                            m_forecastweather.date = m_json.value("date").toString();
                            m_forecastweather.tmp_max = m_json.value("tmp_max").toString();
                            m_forecastweather.cond_code_n = m_json.value("cond_code_n").toString();
                            m_forecastweather.pres = m_json.value("pres").toString();
                            m_forecastweather.tmp_min = m_json.value("tmp_min").toString();
                            m_forecastweather.dateTime=m_observeweather.serveTime.split(" ").first();

                            emit this->requestSetForecastWeather(m_forecastweather);
                        }
                    }
                }

            }
        }
    } //end if (jsonObject.contains("KylinWeather"))
}

//获取收藏城市天气数据
void WeatherWorker::onCityWeatherDataRequest()
{
    QString urlPrefix = "http://service.ubuntukylin.com:8001/weather/api/3.0/heweather_simple_s6/?cityids=";
    QStringList cityList = getCityList().split(","); //cityList最后一项为空字符
    for (int i=0; i<cityList.size()-1; i++) {
        if (i == cityList.size()-2) {
            urlPrefix.append(cityList.at(i));
        } else {
            urlPrefix.append(cityList.at(i));
            urlPrefix.append("+");
        }
    }

    QNetworkRequest request;
    request.setUrl(urlPrefix);
    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &WeatherWorker::onCityWeatherDataReply );
}


//处理收藏城市天气数据并发送出去
void WeatherWorker::onCityWeatherDataReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug()<<"Reply value of getting weather data by URL is: "<<statusCode;

    if (statusCode == 301 || statusCode == 302) {//redirect
        return;
    } else if (statusCode == 400) {
        qDebug() << "Weather: Network error (HTTP400/Bad Request)";
        return;
    } else if (statusCode == 403) {
        qDebug() << "Weather: Username or password invalid (permission denied)";
        return;
    } else if (statusCode == 200) {
        // 200 is normal status
    } else {
        return;
    }

    if(reply->error() != QNetworkReply::NoError) {
        qDebug() << "reply error!";
        return;
    }

    QByteArray ba = reply->readAll();
    //QString reply_content = QString::fromUtf8(ba);
    //qDebug() <<reply_content;
    reply->close();
    reply->deleteLater();

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
        qDebug() << "Json object null or empty!";
        return;
    }
    if (jsonObject.contains("KylinWeather")) {
        QJsonObject mainObj = jsonObject.value("KylinWeather").toObject();
        if (mainObj.isEmpty() || mainObj.size() == 0) {
            return;
        }
        if (mainObj.contains("weather")) {
            QString weather_msg = mainObj.value("weather").toString();
            if (weather_msg != "") {
                emit requestSetCityWeather(weather_msg);
            }
        }
    } //end if (jsonObject.contains("KylinWeather"))
}
void WeatherWorker::initGsetting()
{
    if(QGSettings::isSchemaInstalled(CHINAWEATHERDATA))
        m_pWeatherData = new QGSettings(CHINAWEATHERDATA);
    return;
}

QString WeatherWorker::getCityList()
{
    QString str="";
    if (m_pWeatherData != nullptr) {
        QStringList keyList = m_pWeatherData->keys();
        if (keyList.contains("citylist"))
        {
            str = m_pWeatherData->get("citylist").toString();
        }
    }
    return str;
}

void WeatherWorker::setCityWeatherNow(QString str)
{
    m_pWeatherData->set("weather", str);
}
