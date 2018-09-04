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

#include "airwidget.h"

#include <QTimer>
#include <QDebug>

AirWidget::AirWidget(QWidget *parent)
    : QFrame(parent)
    , m_layout(new QVBoxLayout(this))
    , m_opacityEffect(new QGraphicsOpacityEffect(this))
    , m_animation(new QPropertyAnimation(this, "pos"))
    , m_timer(new QTimer(this))
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setContentsMargins(20, 2, 20, 2);
//    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setStyleSheet("QFrame{border:1px solid #e0e0e0;border-radius:2px;background-color:rgba(255, 255, 255, 0.8);}");
    this->setFixedSize(200, 150);
    m_layout->setSpacing(1);
    m_layout->setMargin(0);

    m_timer->setSingleShot(true);
    m_timer->setInterval(1000*5);
    connect(m_timer, &QTimer::timeout, this, &AirWidget::animationHide, Qt::QueuedConnection);

    m_opacityEffect->setOpacity(1.0);
    this->setGraphicsEffect(m_opacityEffect);
    m_animation->setEasingCurve(QEasingCurve::InOutCubic);
    m_animation->setDuration(300);

    m_aqiLabel = new QLabel;
    m_qltyLabel = new QLabel;
    m_mainLabel = new QLabel;
    m_pm25Label = new QLabel;
    m_pm10Label = new QLabel;
    m_no2Label = new QLabel;
    m_so2Label = new QLabel;
    m_coLabel = new QLabel;
    m_o3Label = new QLabel;

    m_aqiLabel->setStyleSheet("QLabel{border-radius:3px;background-color:rgba(0,0,0,0.2);color:#808080;font-size:12px;}");
    m_qltyLabel->setStyleSheet("QLabel{border-radius:3px;background-color:rgba(0,0,0,0.2);color:#808080;font-size:12px;}");
    m_mainLabel->setStyleSheet("QLabel{border-radius:3px;background-color:rgba(0,0,0,0.2);color:#808080;font-size:12px;}");
    m_pm25Label->setStyleSheet("QLabel{border-radius:3px;background-color:rgba(0,0,0,0.2);color:#808080;font-size:12px;}");
    m_pm10Label->setStyleSheet("QLabel{border-radius:3px;background-color:rgba(0,0,0,0.2);color:#808080;font-size:12px;}");
    m_no2Label->setStyleSheet("QLabel{border-radius:3px;background-color:rgba(0,0,0,0.2);color:#808080;font-size:12px;}");
    m_so2Label->setStyleSheet("QLabel{border-radius:3px;background-color:rgba(0,0,0,0.2);color:#808080;font-size:12px;}");
    m_coLabel->setStyleSheet("QLabel{border-radius:3px;background-color:rgba(0,0,0,0.2);color:#808080;font-size:12px;}");
    m_o3Label->setStyleSheet("QLabel{border-radius:3px;background-color:rgba(0,0,0,0.2);color:#808080;font-size:12px;}");

    m_layout->addWidget(m_aqiLabel, 0, Qt::AlignTop | Qt::AlignLeft);
    m_layout->addWidget(m_qltyLabel, 0, Qt::AlignLeft);
    m_layout->addWidget(m_mainLabel, 0, Qt::AlignLeft);
    m_layout->addWidget(m_pm25Label, 0, Qt::AlignLeft);
    m_layout->addWidget(m_pm10Label, 0, Qt::AlignLeft);
    m_layout->addWidget(m_no2Label, 0, Qt::AlignLeft);
    m_layout->addWidget(m_so2Label, 0, Qt::AlignLeft);
    m_layout->addWidget(m_coLabel, 0, Qt::AlignLeft);
    m_layout->addWidget(m_o3Label, 0, Qt::AlignLeft);
}

AirWidget::~AirWidget()
{
    m_animation->deleteLater();

    if (m_timer) {
        disconnect(m_timer, SIGNAL(timeout()), this, SLOT(animationHide()));
        if(m_timer->isActive()) {
            m_timer->stop();
        }
        delete m_timer;
        m_timer = nullptr;
    }

    QLayoutItem *child;
    while ((child = m_layout->takeAt(0)) != 0) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }
}

void AirWidget::animationShow()
{
    if (m_animation->state() == QPropertyAnimation::Running)
        return;

    QWidget::show();

    m_animation->setStartValue(QPoint(width(), 0));
    m_animation->setEndValue(QPoint());
    m_animation->start();

    m_timer->start();
}

void AirWidget::animationHide()
{
    if (m_animation->state() == QPropertyAnimation::Running)
        return;

    m_timer->stop();

    m_animation->setStartValue(QPoint());
    m_animation->setEndValue(QPoint(-width(), 0));
    m_animation->start();

    QTimer::singleShot(m_animation->duration(), [=] {
        setAttribute(Qt::WA_TransparentForMouseEvents, false);
        QWidget::hide();
    });
}

void AirWidget::resetData(const Air &data)
{
    m_aqiLabel->setText(QString(tr("aqi:%1")).arg(data.aqi));//空气质量指数
    m_qltyLabel->setText(QString(tr("qlty:%1")).arg(data.qlty));//空气质量
    m_mainLabel->setText(QString(tr("main:%1")).arg(data.main));//主要污染物
    m_pm25Label->setText(QString(tr("pm25:%1")).arg(data.pm25));
    m_pm10Label->setText(QString(tr("pm10:%1")).arg(data.pm10));
    m_no2Label->setText(QString(tr("no2:%1")).arg(data.no2));//二氧化氮
    m_so2Label->setText(QString(tr("so2:%1")).arg(data.so2));//二氧化硫
    m_coLabel->setText(QString(tr("co:%1")).arg(data.co));//一氧化碳
    m_o3Label->setText(QString(tr("o3:%1")).arg(data.o3));//臭氧
}

void AirWidget::mousePressEvent(QMouseEvent *event)
{
    QFrame::mousePressEvent(event);

    this->animationHide();
}
