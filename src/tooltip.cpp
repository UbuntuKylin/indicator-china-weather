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
    , _radius(4)
    , _background(QBrush(QColor(255,255,255,255)))
    , _borderColor(QColor(0, 0, 0, 130))
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

    m_weekLabel = new QLabel(this);
    m_dateLabel = new QLabel(this);
    m_weatherLabel = new QLabel(this);
    m_iconLabel = new QLabel(this);
    m_tempLabel = new QLabel(this);

    m_weekLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
    m_dateLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
    m_weatherLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
    m_iconLabel->setStyleSheet("QLabel{border:none;background-color:transparent;}");
    m_tempLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");

    m_iconLabel->setFixedSize(100, 100);

    layout->addWidget(m_weekLabel, 0, Qt::AlignTop | Qt::AlignHCenter);
    layout->addWidget(m_dateLabel, 0, Qt::AlignHCenter);
    layout->addWidget(m_weatherLabel, 0, Qt::AlignHCenter);
    layout->addWidget(m_iconLabel, 0, Qt::AlignHCenter);
    layout->addWidget(m_tempLabel, 0, Qt::AlignBottom | Qt::AlignHCenter);

    m_layout->addWidget(m_frame, 0, Qt::AlignVCenter);

    adjustSize();
}

void ToolTip::setBackground(QBrush background)
{
    this->_background = background;
}

void ToolTip::setRadius(int radius)
{
    this->_radius = radius;
}

void ToolTip::setBorderColor(QColor borderColor)
{
    this->_borderColor = borderColor;
}

void ToolTip::resetData()
{
    //TODO
    m_weekLabel->setText("今天");
    m_dateLabel->setText("20180821");
    m_weatherLabel->setText("晴");
    m_iconLabel->setPixmap(QPixmap(":/res/weather_icons/lightgrey/100.png"));
    m_tempLabel->setText("28°C-36°C");
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

    QBrush background =  this->_background;
    QColor borderColor = this->_borderColor;

    QMarginsF shadowMargins = QMarginsF(2.0, 2.0, 2.0, 2.0);
    QRectF bgRect = QRectF(rect()).marginsRemoved(shadowMargins);
    QPainterPath bgPath;
    bgPath.addRoundedRect(bgRect, this->_radius, this->_radius);
    painter.fillPath(bgPath, background);

    QPainterPath borderPath;
    QRectF borderRect = QRectF(rect());
    int borderRadius = this->_radius;
    QMarginsF borderMargins(0.5, 0.5, 0.5, 0.5);
    borderRadius += 0.5;
    borderRect = borderRect.marginsAdded(borderMargins).marginsRemoved(shadowMargins);
    borderPath.addRoundedRect(borderRect, borderRadius, borderRadius);

    QPen borderPen(borderColor);
    borderPen.setWidthF(1.0);
    painter.strokePath(borderPath, borderPen);
}
