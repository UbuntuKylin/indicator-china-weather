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

#include "citycollectionitem.h"
#include "ui_citycollectionitem.h"

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

citycollectionitem::citycollectionitem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::citycollectionitem)
{
    ui->setupUi(this);
    this->setStyleSheet("QWidget{background:transparent;border:none;}");

    ui->lbBackImage->setStyleSheet("QWidget{background:rgba(61,107,229,1);border:none;}");

    ui->lbCityName->setStyleSheet("QLabel{border:none;background:transparent;font-size:14px;font-weight:400;color:rgba(255,255,255,1);}");

    ui->lbTmp->setStyleSheet("QLabel{border:none;background:transparent;font-size:32px;font-weight:400;color:rgba(255,255,255,1);}");

    ui->lbTmpUnit->setStyleSheet("QLabel{border:none;background:transparent;font-size:14px;font-weight:400;color:rgba(255,255,255,1);}");

    ui->lbwea->setStyleSheet("QLabel{border:none;background:transparent;font-size:14px;font-weight:400;color:rgba(255,255,255,1);}");

    ui->btnDelete->setStyleSheet("QPushButton{border:0px;background:transparent;background-image:url(:/res/control_icons/city_delete.png);}");
    ui->btnDelete->setFocusPolicy(Qt::NoFocus);
    ui->btnDelete->hide();

    ui->btnAddCity->setStyleSheet("QPushButton{border:0px;background:transparent;}");
    ui->btnAddCity->setFocusPolicy(Qt::NoFocus);
    ui->btnAddCity->hide();

    ui->lbAddCity->setStyleSheet("QLabel{border:0px;background:transparent;background-image:url(:/res/control_icons/city_add.png);}");
    ui->lbAddCity->hide();

    this->setAttribute(Qt::WA_Hover,true);
    this->installEventFilter(this);

    m_networkManager = new QNetworkAccessManager(this);
}

citycollectionitem::~citycollectionitem()
{
    delete ui;
}

bool citycollectionitem::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == this){
        if(event->type() == QEvent::HoverEnter) { //show close button when hover enter city item
            if (this->is_normal_item) { //if item is not add city item
                if (this->is_curr_city) { //if item is not current city item, mean can not delete current city
                    ui->btnDelete->hide();
                } else {
                    ui->btnDelete->show();
                }
            }
            return true;
        } else if(event->type() == QEvent::HoverLeave){ //hide close button when hover leave city item
            ui->btnDelete->hide();
            return true;
        }
    }

    return QWidget::eventFilter(obj,event);
}

void citycollectionitem::setItemWidgetState(bool isShowNormal, bool isCurrentCity, int collCityNum)
{
    if (isCurrentCity) { //set current city item style
        ui->lbCityName->show();
        ui->lbTmp->show();
        ui->lbTmpUnit->show();
        ui->lbwea->show();
        ui->lbAddCity->hide();
        ui->btnAddCity->hide();
    } else {
        ui->btnAddCity->show();
        if (isShowNormal) { //set collected city item style
            ui->lbCityName->show();
            ui->lbTmp->show();
            ui->lbTmpUnit->show();
            ui->lbwea->show();
            ui->lbAddCity->hide();
        } else { //set add city item style
            ui->lbCityName->hide();
            ui->lbTmp->hide();
            ui->lbTmpUnit->hide();
            ui->lbwea->hide();
            ui->lbAddCity->show();
        }
    }

    this->m_collcitynum = collCityNum;
    this->is_normal_item = isShowNormal;
    this->is_curr_city = isCurrentCity;
}

void citycollectionitem::setCityWeather(ObserveWeather observeweather)
{
    ui->lbTmp->setText(observeweather.tmp); //set current temperature
    ui->lbwea->setText(observeweather.cond_txt); //set current wrather describe
    ui->lbTmpUnit->setText("℃"); //set temperature unit
    //
   if(observeweather.tmp.toInt() > -1 && observeweather.tmp.toInt() < 10){
       ui->lbTmp->setGeometry(18,56,18,28);
       ui->lbTmpUnit->move(39,56);
       ui->lbwea->move(62,72);
     }else if(observeweather.tmp.toInt() < -9){
       ui->lbTmp->setGeometry(18,56,57,28);
       ui->lbTmpUnit->move(71,56);
       ui->lbwea->move(91,72);
   }
   else{
       ui->lbTmp->setGeometry(18,56,39,28);
       ui->lbTmpUnit->move(60,56);
       ui->lbwea->move(80,72);
   }
   //
    QString weather_code = observeweather.cond_code;
    int code  = weather_code.toInt();
    QString returnStr = convertCodeToBackgroud(code);
    QString picStr = QString("QLabel{background-image:url(%1);}").arg(returnStr);
    ui->lbBackImage->setStyleSheet(picStr); //add background image 
    ui->lbCityName->setText(observeweather.city); //add city name 
    this->m_city_id = observeweather.id;
}

void citycollectionitem::setCurrentWeather(QString cityId)
{
    this->m_city_id = cityId;

    if (!cityId.isEmpty()) {
        QThread *mThread = new QThread();
        this->moveToThread(mThread);
        connect(mThread, SIGNAL(finished()), mThread, SLOT(deleteLater()));
        connect(mThread, SIGNAL(started()), this, SLOT(onThreadStart()));
        connect(this, SIGNAL(requestGetWeatherData(QString)), this, SLOT(onWeatherDataRequest(QString)));
        connect(this, SIGNAL(mThreadFinish()), mThread, SLOT(quit()));
        mThread->start();

        //onWeatherDataRequest(cityId);
    }
}

void citycollectionitem::onThreadStart()
{
    emit requestGetWeatherData(this->m_city_id);
}

void citycollectionitem::onWeatherDataRequest(const QString &cityId)
{
    if (cityId.isEmpty()) { return; }

    QString forecastUrl = QString("http://service.ubuntukylin.com:8001/weather/api/3.0/heweather_data_s6/%1/").arg(cityId);
    QNetworkRequest request;
    request.setUrl(forecastUrl);
    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &citycollectionitem::onWeatherDataReply );
}

void citycollectionitem::onWeatherDataReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug()<<"Reply value of getting weather data by URL is: "<<statusCode;

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
            QJsonObject weatherObj = mainObj.value("weather").toObject();
            if (!weatherObj.isEmpty() && weatherObj.size() > 0) {
                //设置收藏城市实时天气
                QString location_msg = weatherObj.value("location").toString();
                if (location_msg != ""){
                    ui->lbCityName->setText(location_msg);
                }

                QString now_msg = weatherObj.value("now").toString();
                if (now_msg != ""){
                    QStringList strList = now_msg.split(",");
                    QJsonObject m_json;
                    foreach(QString strKey, strList){
                        if (strKey != ""){
                            m_json.insert(strKey.split("=").at(0), strKey.split("=").at(1));
                        }
                    }

                    ui->lbTmp->setText(m_json.value("tmp").toString());
                    ui->lbwea->setText(m_json.value("cond_txt").toString());
                    ui->lbTmpUnit->setText("℃");
                    QString weather_code = m_json.value("cond_code").toString();
                    int code  = weather_code.toInt();
                    QString returnStr = convertCodeToBackgroud(code);
                    QString picStr = QString("QLabel{background-image:url(%1);}").arg(returnStr);
                    ui->lbBackImage->setStyleSheet(picStr);
                }
            }
        } //end if (mainObj.contains("weather"))
    } //end if (jsonObject.contains("KylinWeather"))
    emit this->mThreadFinish();
}

QString citycollectionitem::convertCodeToBackgroud(int code)
{
    if (code == 100 || code == 900) {
        QTime current_time = QTime::currentTime();
        int hour = current_time.hour();
        if (hour>=6 && hour<= 18){
            return ":/res/image/weather-clear.png";
        } else {
            return ":/res/image/weather-clear-night.png";
        }
    }
    else if (code <= 103 && code >= 101) {
        return ":/res/image/weather-few-clouds.png";
    }
    else if (code == 104 || code == 901) {
        return ":/res/image/weather-overcast.png";
    }
    else if (code <= 204 && code >= 200) {
        return ":/res/image/weather-clear.png";
    }
    else if (code <= 213 && code >= 205) {
        return ":/res/image/weather-overcast.png";
    }
    else if (code <= 399 && code >= 300) {
        return ":/res/image/weather-rain.png";
    }
    else if (code <= 499 && code >= 400) {
        return ":/res/image/weather-snow.png";
    }
    else if (code <= 502 && code >= 500) {
        return ":/res/image/weather-fog.png";
    }
    else if (code <= 508 && code >= 503) {
        return ":/res/image/weather-sandstorm.png";
    }
    else if (code <= 515 && code >= 509) {
        return ":/res/image/weather-fog.png";
    }
    else {
        QTime current_time = QTime::currentTime();
        int hour = current_time.hour();
        if (hour>=6 && hour<= 18){
            return ":/res/image/weather-clear.png";
        } else {
            return ":/res/image/weather-clear-night.png";
        }
    }
}

void citycollectionitem::on_btnAddCity_clicked()
{
    if (this->is_normal_item) {
        //将该收藏城市设置为当前城市
        emit changeCurrentCity(this->m_city_id);
    } else {
        //显示添加收藏城市界面
        emit showCityAddWiget();
    }
}

void citycollectionitem::on_btnDelete_clicked()
{
    emit requestDeleteCity(this->m_city_id);
}
