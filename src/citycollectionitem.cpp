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
    //ui->lbCityName->setText("武汉");

    ui->lbTmp->setStyleSheet("QLabel{border:none;background:transparent;font-size:32px;font-weight:400;color:rgba(255,255,255,1);}");
    //ui->lbTmp->setText("60");

    ui->lbTmpUnit->setStyleSheet("QLabel{border:none;background:transparent;font-size:14px;font-weight:400;color:rgba(255,255,255,1);}");
    //ui->lbTmpUnit->setText("℃");

    ui->lbwea->setStyleSheet("QLabel{border:none;background:transparent;font-size:14px;font-weight:400;color:rgba(255,255,255,1);}");
    //ui->lbwea->setText("晴");

    ui->btnDelete->setStyleSheet("QPushButton{border:0px;background:transparent;background-image:url(:/res/control_icons/city_delete.png);}");
    ui->btnDelete->hide();

    ui->btnAddCity->setStyleSheet("QPushButton{border:0px;background:transparent;}");
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
        if(event->type() == QEvent::HoverEnter) {
            if (this->is_normal_item){
                ui->btnDelete->show();
            }
            return true;
        } else if(event->type() == QEvent::HoverLeave){
            ui->btnDelete->hide();
            return true;
        }
    }

    return QWidget::eventFilter(obj,event);
}

void citycollectionitem::setItemWidgetState(bool isShowNormal, bool isCurrentCity)
{
    if (isCurrentCity) { //如果是当前城市
        ui->lbCityName->show();
        ui->lbTmp->show();
        ui->lbTmpUnit->show();
        ui->lbwea->show();
        ui->lbAddCity->hide();
        ui->btnAddCity->hide();
    } else { //如果不是当前城市
        ui->btnAddCity->show();
        if (isShowNormal) {
            ui->lbCityName->show();
            ui->lbTmp->show();
            ui->lbTmpUnit->show();
            ui->lbwea->show();
            ui->lbAddCity->hide();
        } else {
            ui->lbCityName->hide();
            ui->lbTmp->hide();
            ui->lbTmpUnit->hide();
            ui->lbwea->hide();
            ui->lbAddCity->show();
        }
    }

    this->is_normal_item = isShowNormal;
}

void citycollectionitem::setCityWeather(ObserveWeather observeweather)
{
    ui->lbTmp->setText(observeweather.tmp);
    ui->lbwea->setText(observeweather.cond_txt);
    ui->lbTmpUnit->setText("℃");
    QString weather_code = observeweather.cond_code;
    int code  = weather_code.toInt();
    QString returnStr = convertCodeToBackgroud(code);
    QString picStr = QString("QLabel{background-image:url(%1);}").arg(returnStr);
    ui->lbBackImage->setStyleSheet(picStr);
    ui->lbCityName->setText(observeweather.city);
    this->m_city_id = observeweather.id;
}

void citycollectionitem::setCurrentWeather(QString cityId)
{
    if (!cityId.isEmpty()) {
        onWeatherDataRequest(cityId);
    }
    this->m_city_id = cityId;
}

void citycollectionitem::onWeatherDataRequest(const QString &cityId)
{
    if (cityId.isEmpty()) {
        return;
    }

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
                    QStringList strListSub;
                    foreach(QString str, strList){
                        if (str != ""){
                            strListSub.append(str.split("=").at(1));
                        }
                    }

                    ui->lbTmp->setText(strListSub.at(0));
                    ui->lbwea->setText(strListSub.at(2));
                    ui->lbTmpUnit->setText("℃");
                    QString weather_code = strListSub.at(5);
                    int code  = weather_code.toInt();
                    QString returnStr = convertCodeToBackgroud(code);
                    QString picStr = QString("QLabel{background-image:url(%1);}").arg(returnStr);
                    ui->lbBackImage->setStyleSheet(picStr);

                    //m_observeweather.tmp = strListSub.at(0);
                    //m_observeweather.wind_sc = strListSub.at(1);
                    //m_observeweather.cond_txt = strListSub.at(2);
                    //m_observeweather.vis = strListSub.at(3);
                    //m_observeweather.hum = strListSub.at(4);
                    //m_observeweather.cond_code = strListSub.at(5);
                    //m_observeweather.wind_deg = strListSub.at(6);
                    //m_observeweather.pcpn = strListSub.at(7);
                    //m_observeweather.pres = strListSub.at(8);
                    //m_observeweather.wind_spd = strListSub.at(9);
                    //m_observeweather.wind_dir = strListSub.at(10);
                    //m_observeweather.fl = strListSub.at(11);
                    //m_observeweather.cloud = strListSub.at(12);
                }
            }
        } //end if (mainObj.contains("weather"))
    }
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
