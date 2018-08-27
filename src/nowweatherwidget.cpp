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

#include <QVBoxLayout>
#include <QEvent>
#include <QDebug>

NowWeatherWidget::NowWeatherWidget(WeatherWorker *weatherWorker, QFrame *parent) :
    QFrame(parent)
    , m_weatherWorker(weatherWorker)
{
    this->setFixedSize(355, 180);
//    this->setStyleSheet("QLabel{border-radius: 0px; color:rgb(250, 250, 250); background-color:argb(250, 205, 120, 130);}");

    //-----------------------------
    m_tempLabel = new QLabel(this);
    m_tempLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff; font-size:65px;}");
    m_tempLabel->setGeometry(10, 10, 130, 100);
    m_tempLabel->setAlignment(Qt::AlignRight);
//    QFont font;
//    font.setPointSize(65);
//    const QFontMetrics fm(font);
    QString temp = "-33";
    m_tempLabel->setText(temp);
//    m_tempLabel->move(10, 0);
//    qDebug() << fm.width(temp) << fm.height();

    QLabel *tempUnit = new QLabel(this);
    tempUnit->setAlignment(Qt::AlignLeft);
    tempUnit->setGeometry(140, 10, 40, 30);
    tempUnit->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:20px;}");
    tempUnit->setText("°C");


    //-----------------------------
    QLabel *sdIcon = new QLabel(this);
    sdIcon->setGeometry(m_tempLabel->x(), m_tempLabel->y() + m_tempLabel->height() - 20, 20, 20);
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
    m_humidityValueLabel->setText("44%");

    //-----------------------------
    QLabel *windIcon = new QLabel(this);
    windIcon->setGeometry(m_humidityLabel->x() + 60, m_humidityLabel->y(), 20, 20);
    windIcon->setStyleSheet("QLabel{border:none;background-color:transparent;}");
    windIcon->setPixmap(QPixmap(":/res/current_wind.png"));

    m_windLabel = new QLabel(this);
    m_windLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:12x;}");
    m_windLabel->setGeometry(windIcon->x() + windIcon->width(), windIcon->y(), 70, 20);
    m_windLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_windLabel->setText(tr("西南风"));

    m_windPowerLabel = new QLabel(this);
    m_windPowerLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:12x;}");
    m_windPowerLabel->setGeometry(m_windLabel->x(), m_windLabel->y() + m_windLabel->height(), 40, 20);
    m_windPowerLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_windPowerLabel->setText("五级");

    m_weatherIcon = new QLabel(this);
    m_weatherIcon->setGeometry(this->width() - 100 - 10, 0, 100, 100);
    m_weatherIcon->setStyleSheet("QLabel{border:none;background-color:transparent;}");


    this->setWeatherIcon(":/res/weather_icons/lightgrey/100.png");


    m_aqiLabel = new TranslucentLabel(this);
    m_aqiLabel->setLabelIcon(":/res/aqi.png");
    m_aqiLabel->setLabelText("28 优");
    m_aqiLabel->move(m_weatherIcon->x() + 10, m_weatherIcon->y() + m_weatherIcon->height());

    m_temperatureLabel = new TranslucentLabel(this);
    m_temperatureLabel->setLabelIcon(":/res/temp.png");
    m_temperatureLabel->setLabelText("高温");
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
}

NowWeatherWidget::~NowWeatherWidget()
{
}

void NowWeatherWidget::setWeatherIcon(const QString &iconPath)
{
    m_weatherIcon->setPixmap(QPixmap(iconPath));
}
