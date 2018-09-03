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

#include "tooltip.h"

#include <QApplication>
#include <QDebug>
#include <QPainter>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsOpacityEffect>

ToolTip::ToolTip(QWidget *parent) :
    QFrame(parent)
    , m_radius(5)
    , m_background(QBrush(QColor(255,255,255,255)))
    , m_borderColor(QColor(224,224,224,130))
{
    this->setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setContentsMargins(0, 0, 0, 0);

    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(5);

    m_frame = new QFrame();
    m_frame->setContentsMargins(0, 0, 0, 0);
    QVBoxLayout *layout = new QVBoxLayout(this->m_frame);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(5);

    m_dateLabel = new QLabel(this);
    m_dWeatherLabel = new QLabel(this);
    m_nWeatherLabel = new QLabel(this);
    m_dIconLabel = new QLabel(this);
    m_nIconLabel = new QLabel(this);
    m_tempLabel = new QLabel(this);
    m_mrmsLabel = new QLabel(this);
    m_srssLabel = new QLabel(this);
    m_humLabel = new QLabel(this);
    m_pcpnLabel = new QLabel(this);
    m_popLabel = new QLabel(this);
    m_presLabel = new QLabel(this);
    m_uvIndexLabel = new QLabel(this);
    m_visLabel = new QLabel(this);
    m_windLabel = new QLabel(this);

    m_dIconLabel->setFixedSize(48, 48);
    m_nIconLabel->setFixedSize(48, 48);

    m_dateLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
    m_dWeatherLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
    m_nWeatherLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
    m_dIconLabel->setStyleSheet("QLabel{border:none;background-color:transparent;}");
    m_nIconLabel->setStyleSheet("QLabel{border:none;background-color:transparent;}");
    m_tempLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
    m_mrmsLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
    m_srssLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
    m_humLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
    m_pcpnLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
    m_popLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
    m_presLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
    m_uvIndexLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
    m_visLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
    m_windLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");

    QVBoxLayout *d_vlayout = new QVBoxLayout;
    d_vlayout->addWidget(m_dIconLabel, 0, Qt::AlignHCenter);
    d_vlayout->addWidget(m_dWeatherLabel, 0, Qt::AlignHCenter);
    QVBoxLayout *n_vlayout = new QVBoxLayout;
    n_vlayout->addWidget(m_nIconLabel, 0, Qt::AlignHCenter);
    n_vlayout->addWidget(m_nWeatherLabel, 0, Qt::AlignHCenter);
    QHBoxLayout *h_layout = new QHBoxLayout;
    h_layout->addLayout(d_vlayout);
    h_layout->addLayout(n_vlayout);

    layout->addWidget(m_dateLabel, 0, Qt::AlignTop | Qt::AlignHCenter);
    layout->addLayout(h_layout);
    layout->addWidget(m_tempLabel, 0, Qt::AlignHCenter);
    layout->addWidget(m_srssLabel, 0, Qt::AlignHCenter);
    layout->addWidget(m_mrmsLabel, 0, Qt::AlignHCenter);
    layout->addWidget(m_humLabel, 0, Qt::AlignHCenter);
    layout->addWidget(m_pcpnLabel, 0, Qt::AlignHCenter);
    layout->addWidget(m_popLabel, 0, Qt::AlignHCenter);
    layout->addWidget(m_presLabel, 0, Qt::AlignHCenter);
    layout->addWidget(m_uvIndexLabel, 0, Qt::AlignHCenter);
    layout->addWidget(m_visLabel, 0, Qt::AlignHCenter);
    layout->addWidget(m_windLabel, 0, Qt::AlignBottom | Qt::AlignHCenter);

    m_layout->addWidget(m_frame, 0, Qt::AlignVCenter);

    adjustSize();
}

ToolTip::~ToolTip()
{

}

void ToolTip::resetData(const ForecastWeather &data, const QString &week)
{
    m_dateLabel->setText(week + data.forcast_date);

    QPixmap pixmap1 = QPixmap(QString(":/res/weather_icons/lightgrey/%1.png").arg(data.cond_code_d));
    pixmap1 = pixmap1.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_dIconLabel->setPixmap(pixmap1);

    QPixmap pixmap2 = QPixmap(QString(":/res/weather_icons/lightgrey/%1.png").arg(data.cond_code_n));
    pixmap2 = pixmap2.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_nIconLabel->setPixmap(pixmap2);

    m_dWeatherLabel->setText(data.cond_txt_d);
    m_nWeatherLabel->setText(data.cond_txt_n);

    m_tempLabel->setText(QString("%1°C~%2°C").arg(data.tmp_min).arg(data.tmp_max));

    if (data.sr_ss.contains(QChar('+'))) {
        QStringList sunList= data.sr_ss.split(QChar('+'));
        m_srssLabel->setText(QString("Sunrise: %1 sunset: %2").arg(sunList.at(0)).arg(sunList.at(1)));
    }
    else {
        m_srssLabel->setText(QString("Sunrise and sunset: %1").arg(data.sr_ss));
    }
    if (data.mr_ms.contains(QChar('+'))) {
        QStringList moonList= data.mr_ms.split(QChar('+'));
        m_mrmsLabel->setText(QString("Moonrise: %1 moonset: %2").arg(moonList.at(0)).arg(moonList.at(1)));
    }
    else {
        m_mrmsLabel->setText(QString("Moonrise and moonset: %1").arg(data.mr_ms));
    }

    m_humLabel->setText(QString("Relative humidity:%1").arg(data.hum));//相对湿度
    m_pcpnLabel->setText(QString("Precipitation:%1 mm").arg(data.pcpn));//降水量
    m_popLabel->setText(QString("Probability of precipitation:%1").arg(data.pop));//降水概率
    m_presLabel->setText(QString("Atmospheric pressure:%1 hPa").arg(data.pres));//大气压强
    m_uvIndexLabel->setText(QString("UV intensity index:%1").arg(data.uv_index));//紫外线强度指数
    m_visLabel->setText(QString("Visibility:%1 km").arg(data.vis));//能见度
    m_windLabel->setText(QString("Wind:%1 %2stage  direction %3  speed %4 km/h").arg(data.wind_dir).arg(data.wind_sc).arg(data.wind_deg).arg(data.wind_spd));
}

void ToolTip::popupTip(QPoint point)
{
    int point_X;
    int point_Y;
    if (point.rx() < this->width()) {
        point_X = point.rx() + 10;
    }
    else {
        point_X = point.rx() - this->width();
    }
    point_Y = point.ry();
    this->move(QPoint(point_X, point_Y));
    QFrame::show();

    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this->m_frame);
    opacityEffect->setOpacity(1);
//    this->m_frame->setGraphicsEffect(opacityEffect);

    QPropertyAnimation *animation = new QPropertyAnimation(opacityEffect, "opacity");
    animation->setDuration(1000);
    animation->setStartValue(0);
    animation->setKeyValueAt(0.25, 1);
    animation->setKeyValueAt(0.5, 1);
    animation->setKeyValueAt(0.75, 1);
    animation->setEndValue(0);
    animation->start();

    connect(animation, &QPropertyAnimation::finished, this, [=] {
//        this->m_frame->setGraphicsEffect(NULL);
//        this->hide();
    });
}

void ToolTip::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);

    QBrush background =  this->m_background;
    QColor borderColor = this->m_borderColor;

    QMarginsF shadowMargins = QMarginsF(2.0, 2.0, 2.0, 2.0);
    QRectF bgRect = QRectF(rect()).marginsRemoved(shadowMargins);
    QPainterPath bgPath;
    bgPath.addRoundedRect(bgRect, this->m_radius, this->m_radius);
    painter.fillPath(bgPath, background);

    QPainterPath borderPath;
    QRectF borderRect = QRectF(rect());
    int borderRadius = this->m_radius;
    QMarginsF borderMargins(0.5, 0.5, 0.5, 0.5);
    borderRadius += 0.5;
    borderRect = borderRect.marginsAdded(borderMargins).marginsRemoved(shadowMargins);
    borderPath.addRoundedRect(borderRect, borderRadius, borderRadius);

    QPen borderPen(borderColor);
    borderPen.setWidthF(1.0);
    painter.strokePath(borderPath, borderPen);
}
