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

#include "nowweatherwidget.h"
#include "translucentlabel.h"
#include "tipwidget.h"

#include <QVBoxLayout>
#include <QEvent>
#include <QDebug>

inline QString convertTemperatureToString(int temp)
{
    if (temp <= -40) {
        return QString(QObject::tr("Extremely cold"));//极寒
    }
    else if (temp > -40 && temp <= -35) {
        return QString(QObject::tr("Cold and cold"));//奇寒
    }
    else if (temp > -35 && temp <= -30) {
        return QString(QObject::tr("Severe cold"));//酷寒
    }
    else if (temp > -30 && temp <= -20) {
        return QString(QObject::tr("Bitter cold"));//严寒
    }
    else if (temp > -20 && temp <= -15) {
        return QString(QObject::tr("Deep cold"));//深寒
    }
    else if (temp > -15 && temp <= -10) {
        return QString(QObject::tr("Very cold"));//大寒
    }
    else if (temp > -10 && temp <= -5) {
        return QString(QObject::tr("Lesser cold"));//小寒
    }
    else if (temp > -5 && temp <= 0) {
        return QString(QObject::tr("Mildly cold"));//轻寒
    }
    else if (temp > 0 && temp <= 5) {
        return QString(QObject::tr("Slightly cold"));//微寒
    }
    else if (temp > 5 && temp <= 10) {
        return QString(QObject::tr("Cold"));//凉
    }
    else if (temp > 10 && temp <= 14) {
        return QString(QObject::tr("Warm and cool"));//温凉
    }
    else if (temp > 14 && temp <= 18) {
        return QString(QObject::tr("Mild"));//温和
    }
    else if (temp > 18 && temp <= 22) {
        return QString(QObject::tr("Warm"));//温暖
    }
    else if (temp > 22 && temp <= 25) {
        return QString(QObject::tr("Hot"));//热
    }
    else if (temp > 25 && temp <= 30) {
        return QString(QObject::tr("Scorching hot"));//炎热
    }
    else if (temp > 30 && temp <= 35) {
        return QString(QObject::tr("Boiling hot"));//酷热
    }
    else if (temp > 35 && temp <= 39) {
        return QString(QObject::tr("Excessive heat"));//奇热
    }
    else {//>=40
        return QString(QObject::tr("Extremely hot"));//极热
    }
}

NowWeatherWidget::NowWeatherWidget(WeatherWorker *weatherWorker, QFrame *parent) :
    QFrame(parent)
    , m_weatherWorker(weatherWorker)
    , m_tipTimer(new QTimer(this))
{
    this->setFixedSize(355, 180);
//    this->setStyleSheet("QLabel{border-radius: 0px; color:rgb(250, 250, 250); background-color:rgba(0,0,0,0.2)}");

    //test background color
    /*this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(Qt::gray));
    this->setPalette(palette);*/

    m_tipWidget = new TipWidget(this);
    m_tipWidget->setGeometry(10, 0, this->width(), 22);
    m_tipWidget->setVisible(false);

    //-----------------------------
    m_tempLabel = new QLabel(this);
    m_tempLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff; font-size:65px;}");
    m_tempLabel->setGeometry(10, 14, 130, 80);
    m_tempLabel->setAlignment(Qt::AlignRight | Qt::AlignTop);
//    QFont font;
//    font.setPointSize(65);
//    const QFontMetrics fm(font);
//    m_tempLabel->move(10, 0);
//    qDebug() << fm.width(temp) << fm.height();

    QLabel *tempUnit = new QLabel(this);
    tempUnit->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    tempUnit->setGeometry(140, m_tempLabel->y(), 40, 30);
    tempUnit->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:20px;}");
    tempUnit->setText("°C");

    //-----------------------------
    QLabel *sdIcon = new QLabel(this);
    sdIcon->setGeometry(m_tempLabel->x(), m_tempLabel->y() + m_tempLabel->height() /*- 20*/, 20, 20);
    sdIcon->setStyleSheet("QLabel{border:none;background-color:transparent;}");
    sdIcon->setPixmap(QPixmap(":/res/current_sd.png"));

    m_humidityLabel = new QLabel(this);
    m_humidityLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:12x;}");
    m_humidityLabel->setGeometry(sdIcon->x() + sdIcon->width(), sdIcon->y(), 40, 20);
    m_humidityLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_humidityLabel->setText(tr("湿度"));

    m_humidityValueLabel = new QLabel(this);
    m_humidityValueLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:12x;}");
    m_humidityValueLabel->setGeometry(m_humidityLabel->x(), m_humidityLabel->y() + m_humidityLabel->height(), 40, 20);
    m_humidityValueLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    //-----------------------------
    QLabel *windIcon = new QLabel(this);
    windIcon->setGeometry(m_humidityLabel->x() + 60, m_humidityLabel->y(), 20, 20);
    windIcon->setStyleSheet("QLabel{border:none;background-color:transparent;}");
    windIcon->setPixmap(QPixmap(":/res/current_wind.png"));

    m_windLabel = new QLabel(this);
    m_windLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:12x;}");
    m_windLabel->setGeometry(windIcon->x() + windIcon->width(), windIcon->y(), 70, 20);
    m_windLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    m_windPowerLabel = new QLabel(this);
    m_windPowerLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:12x;}");
    m_windPowerLabel->setGeometry(m_windLabel->x(), m_windLabel->y() + m_windLabel->height(), 90, 20);
    m_windPowerLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    m_weatherIcon = new QLabel(this);
    m_weatherIcon->setGeometry(this->width() - 64 - 20, m_tempLabel->y(), 64, 64);
    m_weatherIcon->setStyleSheet("QLabel{border:none;background-color:transparent;}");

    m_aqiLabel = new TranslucentLabel(this);
    m_aqiLabel->setLabelIcon(":/res/aqi.png");
    m_aqiLabel->move(m_weatherIcon->x() - 10, m_windLabel->y());

    m_temperatureLabel = new TranslucentLabel(this);
    m_temperatureLabel->setLabelIcon(":/res/temp.png");
    m_temperatureLabel->move(m_aqiLabel->x(), m_aqiLabel->y() + m_aqiLabel->height() + 8);

//    QFont ft;
//    QFontMetrics fm(ft);
//    QString elided_text = fm.elidedText(name, Qt::ElideRight, 260);
//    name_label->setText(elided_text);
//    elided_text = fm.elidedText(desc, Qt::ElideRight, 260);
//    description_label->setText(elided_text);
//    if(elided_text.endsWith("…"))
//        description_label->setToolTip(desc);

    //	QFont &text_font = const_cast<QFont &>(font());
    //	text_font.setWeight(QFont::Bold);


    connect(m_tipTimer, &QTimer::timeout, this, [=] {
        m_tipTimer->stop();
        m_tipWidget->setVisible(false);
    });
    //this->displayTip();
}

NowWeatherWidget::~NowWeatherWidget()
{
    delete m_tipTimer;
}

void NowWeatherWidget::displayTip()
{
    if (m_tipWidget->isVisible())
        m_tipWidget->setVisible(false);
    if (m_tipTimer->isActive())
        m_tipTimer->stop();

    m_tipWidget->setLabelIcon(":/res/update_warn.png");
    m_tipWidget->setLabelText(tr("Update failure"));
    m_tipWidget->setVisible(true);
    m_tipTimer->start(8000);
}

void NowWeatherWidget::setWeatherIcon(const QString &iconPath)
{
    QPixmap pixmap = QPixmap(iconPath);
    pixmap = pixmap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_weatherIcon->setPixmap(pixmap);
}

void NowWeatherWidget::refreshData(const ObserveWeather &data)
{
    m_tempLabel->setText(data.tmp);
    m_humidityValueLabel->setText(data.hum);
    m_windLabel->setText(data.wind_dir);
    int wind_sc = data.wind_sc.toInt();
    if (wind_sc == 0) {
        m_windPowerLabel->setText(tr("Static wind"));//静风
    }
    else {
        m_windPowerLabel->setText(tr("%1stage wind").arg(data.wind_sc));//级风
    }
    this->setWeatherIcon(QString(":/res/weather_icons/white/%1.png").arg(data.cond_code));
    m_aqiLabel->setLabelText(data.air);
    m_temperatureLabel->setLabelText(convertTemperatureToString(data.tmp.toInt()));
}
