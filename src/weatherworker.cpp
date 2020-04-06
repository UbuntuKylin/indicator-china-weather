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

WeatherWorker::WeatherWorker(QObject *parent) :
    QObject(parent)
{
    m_networkManager = new QNetworkAccessManager(this);
}

WeatherWorker::~WeatherWorker()
{
    m_networkManager->deleteLater();
}

void WeatherWorker::onWeatherDataRequest(const QString &cityId)
{
    qDebug()<<"cityId: "<<cityId;
    if (cityId.isEmpty()) {
        return;
    }

    QString forecastUrl = QString("http://service.ubuntukylin.com:8001/weather/api/3.0/heweather_data_s6/%1/").arg(cityId);
    QNetworkRequest request;
    request.setUrl(forecastUrl);
    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &WeatherWorker::onWeatherDataReply );
}

void WeatherWorker::onWeatherDataReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug()<<"value of statusCode is: "<<statusCode;

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
                QString now_msg = weatherObj.value("now").toString();
                if (now_msg != ""){
                    QStringList strList = now_msg.split(",");
                    QStringList strListSub;
                    foreach(QString str, strList){
                        if (str != ""){
                            strListSub.append(str.split("=").at(1));
                        }
                    }

                    ObserveWeather m_observeweather;
                    m_observeweather.tmp = strListSub.at(0);
                    m_observeweather.wind_sc = strListSub.at(1);
                    m_observeweather.cond_txt = strListSub.at(2);
                    m_observeweather.vis = strListSub.at(3);
                    m_observeweather.hum = strListSub.at(4);
                    m_observeweather.cond_code = strListSub.at(5);
                    m_observeweather.wind_deg = strListSub.at(6);
                    m_observeweather.pcpn = strListSub.at(7);
                    m_observeweather.pres = strListSub.at(8);
                    m_observeweather.wind_spd = strListSub.at(9);
                    m_observeweather.wind_dir = strListSub.at(10);
                    m_observeweather.fl = strListSub.at(11);
                    m_observeweather.cloud = strListSub.at(12);

                    emit this->requestSetObserveWeather(m_observeweather);//用于设置主窗口

                }

                //处理预报天气
                QString forecast_msg = weatherObj.value("forecast").toString();
                if (forecast_msg != ""){
                    QStringList strList = forecast_msg.split(";");
                    foreach(QString strDay, strList){
                        QStringList strListDaySub;
                        QStringList strListSub;
                        if (strDay != ""){
                            strListDaySub = strDay.split(",");

                            foreach(QString str, strListDaySub){
                                if (str != ""){
                                    strListSub.append(str.split("=").at(1));
                                }
                            }

                            ForecastWeather m_forecastweather;
                            m_forecastweather.uv_index = strListSub.at(0);
                            m_forecastweather.wind_spd = strListSub.at(1);
                            m_forecastweather.sr = strListSub.at(2);
                            m_forecastweather.wind_sc = strListSub.at(3);
                            m_forecastweather.ms = strListSub.at(4);
                            m_forecastweather.cond_txt_d = strListSub.at(5);
                            m_forecastweather.vis = strListSub.at(6);
                            m_forecastweather.ss = strListSub.at(7);
                            m_forecastweather.hum = strListSub.at(8);
                            m_forecastweather.cond_txt_n = strListSub.at(9);
                            m_forecastweather.pop = strListSub.at(10);
                            m_forecastweather.wind_deg = strListSub.at(11);
                            m_forecastweather.pcpn = strListSub.at(12);
                            m_forecastweather.wind_dir = strListSub.at(13);
                            m_forecastweather.cond_code_d = strListSub.at(14);
                            m_forecastweather.mr = strListSub.at(15);
                            m_forecastweather.date = strListSub.at(16);
                            m_forecastweather.tmp_max = strListSub.at(17);
                            m_forecastweather.cond_code_n = strListSub.at(18);
                            m_forecastweather.pres = strListSub.at(19);
                            m_forecastweather.tmp_min = strListSub.at(20);

                            emit this->requestSetForecastWeather(m_forecastweather);
                        }
                    }
                }

            }
        }
    }
}
