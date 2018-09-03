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
    , m_radius(4)
    , m_background(QBrush(QColor(255,255,255,255)))
    , m_borderColor(QColor(0, 0, 0, 130))
{
    this->setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setContentsMargins(0, 0, 0, 0);
//    this->resize(100, 180);

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

//    m_dIconLabel->setFixedSize(100, 100);

    QVBoxLayout *d_vlayout = new QVBoxLayout;
    d_vlayout->addWidget(m_dIconLabel);
    d_vlayout->addWidget(m_dWeatherLabel);
    QVBoxLayout *n_vlayout = new QVBoxLayout;
    n_vlayout->addWidget(m_nIconLabel);
    n_vlayout->addWidget(m_nWeatherLabel);
    QHBoxLayout *h_layout = new QHBoxLayout;
    h_layout->addLayout(d_vlayout);
    h_layout->addLayout(n_vlayout);

    layout->addWidget(m_dateLabel, 0, Qt::AlignTop | Qt::AlignHCenter);
    layout->addLayout(h_layout);
    layout->addWidget(m_tempLabel, 0, Qt::AlignHCenter);
    layout->addWidget(m_mrmsLabel, 0, Qt::AlignHCenter);
    layout->addWidget(m_srssLabel, 0, Qt::AlignHCenter);
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

    m_mrmsLabel->setText(data.mr_ms);
    m_srssLabel->setText(data.sr_ss);
    m_humLabel->setText(data.hum);
    m_pcpnLabel->setText(data.pcpn);
    m_popLabel->setText(data.pop);
    m_presLabel->setText(data.pres);
    m_uvIndexLabel->setText(data.uv_index);
    m_visLabel->setText(data.vis);
    m_windLabel->setText(data.wind_deg + data.wind_dir + data.wind_sc + data.wind_spd);
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
    animation->setDuration(2000);
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

void ToolTip::paintEvent(QPaintEvent *)
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
