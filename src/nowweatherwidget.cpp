/*
 * Copyright (C) 2013 ~ 2019 National University of Defense Technology(NUDT) & Tianjin Kylin Ltd.
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
#include "texttip.h"
#include "tipmodule.h"
#include "airwidget.h"

#include <QVBoxLayout>
#include <QEvent>
#include <QDebug>

#include "preferences.h"
#include "global.h"
using namespace Global;

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

//it must be consistent with the function named convertCodeToBackgroud() which in MainWindow
inline QString convertCodeToStyleSheet(int code)
{
    if (code == 100 || code == 900) {//#ee613f
        return "QFrame{border:none;background-color:rgba(238,97,63,85%);color:rgb(255,255,255);}";
    }
    else if (code <= 103 && code >= 101) {//#0c69c3
        return "QFrame{border:none;background-color:rgba(12,105,195,85%);color:rgb(255,255,255);}";
    }
    else if (code == 104 || code == 901) {//#404b5b
        return "QFrame{border:none;background-color:rgba(64,75,91,85%);color:rgb(255,255,255);}";
    }
    else if (code <= 204 && code >= 200) {//#ee613f
        return "QFrame{border:none;background-color:rgba(238,97,63,85%);color:rgb(255,255,255);}";
    }
    else if (code <= 213 && code >= 205) {//#404b5b
        return "QFrame{border:none;background-color:rgba(64,75,91,85%);color:rgb(255,255,255);}";
    }
    else if (code <= 399 && code >= 300) {//#5336bf
        return "QFrame{border:none;background-color:rgba(83,54,191,85%);color:rgb(255,255,255);}";
    }
    else if (code <= 499 && code >= 400) {//#4f88b3
        return "QFrame{border:none;background-color:rgba(79,136,179,85%);color:rgb(255,255,255);}";
    }
    else if (code <= 502 && code >= 500) {//#312e33
        return "QFrame{border:none;background-color:rgba(49,46,51,85%);color:rgb(255,255,255);}";
    }
    else if (code <= 508 && code >= 503) {//#98733f
        return "QFrame{border:none;background-color:rgba(152,115,63,85%);color:rgb(255,255,255);}";
    }
    else if (code <= 515 && code >= 509) {//#312e33
        return "QFrame{border:none;background-color:rgba(49,46,51,85%);color:rgb(255,255,255);}";
    }
    else {//#ee613f
        return "QFrame{border:none;background-color:rgba(238,97,63,85%);color:rgb(255,255,255);}";
    }
}

NowWeatherWidget::NowWeatherWidget(QFrame *parent) :
    QFrame(parent)
    , m_tipTimer(new QTimer(this))
//    , m_tipModule(new TipModule)
//    , m_tip(new TextTip(QString(), this))
{
    this->setFixedSize(355, 180);

    //m_tip->setFixedSize(100, 32);
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
    m_tempLabel->setGeometry(10, 14, 100, 70);
    m_tempLabel->setAlignment(Qt::AlignCenter);
//    QFont font;
//    font.setPointSize(65);
//    const QFontMetrics fm(font);
//    m_tempLabel->move(10, 0);
//    qDebug() << fm.width(temp) << fm.height();

    QLabel *tempUnit = new QLabel(this);
    tempUnit->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    tempUnit->setGeometry(m_tempLabel->x() + m_tempLabel->width(), m_tempLabel->y(), 40, 28);
    tempUnit->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:20px;}");
    tempUnit->setText("°C");

    m_weatherLabel = new QLabel(this);
    m_weatherLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff; font-size:20px;}");
    m_weatherLabel->setGeometry(tempUnit->x(), m_tempLabel->height() - 28/2, 130, 28);
    m_weatherLabel->setAlignment(Qt::AlignLeft | Qt::AlignBottom);

    //-----------------------------
    QLabel *sdIcon = new QLabel(this);
    sdIcon->setGeometry(m_tempLabel->x(), m_tempLabel->y() + m_tempLabel->height() + 5, 20, 20);
    sdIcon->setStyleSheet("QLabel{border:none;background-color:transparent;}");
    sdIcon->setPixmap(QPixmap(":/res/current_sd.png"));

    QLabel *m_humidityLabel = new QLabel(this);
    m_humidityLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:12x;}");

    m_humidityLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_humidityLabel->setText(tr("Humidity"));
    QFont font = m_humidityLabel->font();
    const QFontMetrics fm(font);
    m_humidityLabel->setGeometry(sdIcon->x() + sdIcon->width(), sdIcon->y(), fm.width(m_humidityLabel->text()), 20);

    m_humidityValueLabel = new QLabel(this);
    m_humidityValueLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:12x;}");
    m_humidityValueLabel->setGeometry(m_humidityLabel->x(), m_humidityLabel->y() + m_humidityLabel->height() + 5, 40, 20);
    m_humidityValueLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

    //-----------------------------
    QLabel *windIcon = new QLabel(this);
    windIcon->setGeometry(m_humidityLabel->x() + m_humidityLabel->width() + 15, m_humidityLabel->y(), 20, 20);
    windIcon->setStyleSheet("QLabel{border:none;background-color:transparent;}");
    windIcon->setPixmap(QPixmap(":/res/current_wind.png"));

    m_windLabel = new QLabel(this);
    m_windLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:12x;}");
    m_windLabel->setGeometry(windIcon->x() + windIcon->width(), windIcon->y(), 70, 20);
    m_windLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

    m_windPowerLabel = new QLabel(this);
    m_windPowerLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:12x;}");
    m_windPowerLabel->setGeometry(m_windLabel->x(), m_humidityValueLabel->y(), 90, 20);
    m_windPowerLabel->setAlignment(Qt::AlignBottom | Qt::AlignLeft);

    m_weatherIcon = new QLabel(this);
    m_weatherIcon->setGeometry(this->width() - 64 - 20, m_tempLabel->y(), 64, 64);
    m_weatherIcon->setStyleSheet("QLabel{border:none;background-color:transparent;}");
    //m_weatherIcon->setProperty("TextTipWidget", QVariant::fromValue<QWidget *>(m_tip));
    //m_weatherIcon->installEventFilter(m_tipModule);

    m_ariWidget = new AirWidget(this);
    m_ariWidget->raise();
    m_ariWidget->setVisible(false);

    m_aqiLabel = new TranslucentLabel(true, this);
    m_aqiLabel->setLabelIcon(":/res/aqi.png");
    m_aqiLabel->setToolTip(tr("Click to see details of air quality"));
    m_aqiLabel->move(m_weatherIcon->x() - 10, m_windLabel->y());
    connect(m_aqiLabel, &TranslucentLabel::clicked, this, [=] {
        if (m_ariWidget->isVisible()) {
            m_ariWidget->animationHide();
        }
        else {
            if (m_preferences->air.id != m_preferences->m_currentCityId) {
                return;
            }
            if (m_preferences->air.aqi.contains("Unknown") && m_preferences->air.qlty.contains("Unknown")) {
                return;
            }
            m_ariWidget->resetData(m_preferences->air);
            if (m_preferences->weather.cond_code.contains(QChar('n'))) {//#063638
                m_ariWidget->animationShow("QFrame{border:none;background-color:rgba(6,54,56,85%);color:rgb(255,255,255);}");
            }
            else {
                m_ariWidget->animationShow(convertCodeToStyleSheet(m_preferences->weather.cond_code.toInt()));
            }
        }
    });

    m_temperatureLabel = new TranslucentLabel(false, this);
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
    //this->displayTip(tr("Update failure"));
}

NowWeatherWidget::~NowWeatherWidget()
{
    delete m_tipTimer;
    //delete m_tipModule;
    delete m_ariWidget;
}

void NowWeatherWidget::displayTip(const QString &info)
{
    if (m_tipWidget->isVisible())
        m_tipWidget->setVisible(false);
    if (m_tipTimer->isActive())
        m_tipTimer->stop();

    m_tipWidget->setLabelIcon(":/res/update_warn.png");
    m_tipWidget->setLabelText(info);
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
    //m_tip->resetTipText(data.cond_txt);
    m_weatherLabel->setText(data.cond_txt);

    m_tempLabel->setText(data.tmp);
    if (data.hum.isEmpty()) {
        m_humidityValueLabel->setText("-");
    }
    else {
        m_humidityValueLabel->setText(data.hum + "%");
    }
    m_windLabel->setText(data.wind_dir);
    int wind_sc = data.wind_sc.toInt();
    if (wind_sc == 0) {
        m_windPowerLabel->setText(tr("Static wind"));//静风
    }
    else {
        m_windPowerLabel->setText(tr("%1stage wind").arg(data.wind_sc));//级风
    }
    this->setWeatherIcon(QString(":/res/weather_icons/white/%1.png").arg(data.cond_code));
    if (data.air.isEmpty() || data.air.contains("Unknown")) {
        m_aqiLabel->setLabelText(QString(tr("Unknown")));
    }
    else {
        m_aqiLabel->setLabelText(data.air);
    }
    m_temperatureLabel->setLabelText(convertTemperatureToString(data.tmp.toInt()));
}

void NowWeatherWidget::mousePressEvent(QMouseEvent *event)
{
    if (m_ariWidget->isVisible()) {
        m_ariWidget->animationHide();
    }

    QFrame::mousePressEvent(event);
}
