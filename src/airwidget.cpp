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

#include "airwidget.h"
#include "preferences.h"
#include "global.h"
using namespace Global;

#include <QTimer>
#include <QPainter>
#include <QDebug>
#include <QDesktopWidget>
#include <QTimer>
#include <QTimeLine>
#include <QApplication>

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

AirWidget::AirWidget(QWidget *parent)
    : QFrame(parent)
    , m_layout(new QVBoxLayout(this))
    , m_opacityEffect(new QGraphicsOpacityEffect(this))
    , m_animation(new QPropertyAnimation(this, "pos"))
    , m_showTimer(new QTimer(this))
    , m_radius(4)
    , m_borderColor(QColor(0, 0, 0,  255 / 10))
    , m_background(QBrush(QColor(6,54,56,85)))
{
//    setWindowFlags(Qt::ToolTip);
//    setAutoFillBackground(true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setContentsMargins(10, 2, 10, 2);
//    this->setStyleSheet("QFrame{border:none;background-color:rgba(0,0,0,0.9);color:rgb(255,255,255);}");
//    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //this->setStyleSheet("QFrame{border:1px solid #e0e0e0;border-radius:2px;background-color:rgba(255, 255, 255, 0.8);}");
    this->setFixedSize(350 - 100, 150);
    m_layout->setSpacing(1);
    m_layout->setMargin(0);

    m_opacityEffect->setOpacity(1.0);
    this->setGraphicsEffect(m_opacityEffect);
    m_animation->setEasingCurve(QEasingCurve::InOutCubic);
    m_animation->setDuration(500);

    m_aqiLabel = new QLabel;
    m_qltyLabel = new QLabel;
    m_mainLabel = new QLabel;
    m_pm25Label = new QLabel;
    m_pm10Label = new QLabel;
    m_no2Label = new QLabel;
    m_so2Label = new QLabel;
    m_coLabel = new QLabel;
    m_o3Label = new QLabel;

    m_aqiLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:12px;}");
    m_qltyLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:12px;}");
    m_mainLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:12px;}");
    m_pm25Label->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:12px;}");
    m_pm10Label->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:12px;}");
    m_no2Label->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:12px;}");
    m_so2Label->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:12px;}");
    m_coLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:12px;}");
    m_o3Label->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:12px;}");

    m_layout->addWidget(m_aqiLabel, 0, Qt::AlignTop | Qt::AlignLeft);
    m_layout->addWidget(m_qltyLabel, 0, Qt::AlignLeft);
    m_layout->addWidget(m_mainLabel, 0, Qt::AlignLeft);
    m_layout->addWidget(m_pm25Label, 0, Qt::AlignLeft);
    m_layout->addWidget(m_pm10Label, 0, Qt::AlignLeft);
    m_layout->addWidget(m_no2Label, 0, Qt::AlignLeft);
    m_layout->addWidget(m_so2Label, 0, Qt::AlignLeft);
    m_layout->addWidget(m_coLabel, 0, Qt::AlignLeft);
    m_layout->addWidget(m_o3Label, 0, Qt::AlignLeft);

//    QGraphicsDropShadowEffect *bodyShadow = new QGraphicsDropShadowEffect;
//    bodyShadow->setBlurRadius(10.0);
//    bodyShadow->setColor(QColor(0, 0, 0,  255 / 10));
//    bodyShadow->setOffset(0, 2.0);
//    this->setGraphicsEffect(bodyShadow);

    m_showTimer->setSingleShot(true);
    m_showTimer->setTimerType(Qt::VeryCoarseTimer);
    connect(m_showTimer, &QTimer::timeout, this, &AirWidget::animationHide);
}

AirWidget::~AirWidget()
{
    m_animation->deleteLater();

    if (m_showTimer) {
        disconnect(m_showTimer, &QTimer::timeout, this, &AirWidget::animationHide);
//        disconnect(m_showTimer, SIGNAL(timeout()), this, SLOT(animationHide()));
        if(m_showTimer->isActive()) {
            m_showTimer->stop();
        }
        delete m_showTimer;
        m_showTimer = nullptr;
    }

    QLayoutItem *child;
    while ((child = m_layout->takeAt(0)) != 0) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }
}

int AirWidget::getWidth()
{
    int width = 350;
    QWidget *parentWidget = this->parentWidget();

    if (parentWidget ) {
        width = parentWidget->width();
    }

    return width;
}

void AirWidget::animationShow(/*const QString &styleSheet*/)
{
    if (m_animation->state() == QPropertyAnimation::Running)
        return;

//    this->setStyleSheet(styleSheet);

    QWidget::show();

    m_animation->setStartValue(QPoint(335, 0));//QPoint(0, 0)
    m_animation->setEndValue(QPoint(0, 0));
    m_animation->start();
}

void AirWidget::animationHide()
{
    //QDesktopWidget *desktopWidget=qApp->desktop();
    if (m_animation->state() == QPropertyAnimation::Running)
        return;
    m_animation->setStartValue(QPoint(0, 0));
    m_animation->setEndValue(QPoint(335, 0));//QPoint(0, 0)
    m_animation->start();
    QTimer::singleShot(m_animation->duration(), this, [=] {
        setAttribute(Qt::WA_TransparentForMouseEvents, false);
        QWidget::hide();
    });
}

void AirWidget::resetData(const AqiAir &data)
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

void AirWidget::showTooltip(const QPoint &position)
{
    Q_UNUSED(position);

    if (!isVisible()) {
        if (m_preferences->m_observerWeather.cond_code.contains(QChar('n'))) {//#063638
            this->animationShow(/*"QFrame{border:none;background-color:rgba(6,54,56,85%);color:rgb(255,255,255);}"*/);
        }
        else {
            this->animationShow(/*convertCodeToStyleSheet(m_preferences->m_observerWeather.cond_code.toInt())*/);
        }
    }

    m_showTimer->stop();
    m_showTimer->setInterval(3000);
    m_showTimer->start();
}

void AirWidget::mousePressEvent(QMouseEvent *event)
{
    QFrame::mousePressEvent(event);

    this->animationHide();
}

void AirWidget::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);

    m_showTimer->stop();
    m_showTimer->setInterval(300);

    setFocus();
}

void AirWidget::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);

    m_showTimer->start();
}

void AirWidget::focusOutEvent(QFocusEvent *event)
{
    QWidget::focusOutEvent(event);
    m_showTimer->start();
}

void AirWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);

    QBrush background =  this->m_background;
    QColor borderColor = this->m_borderColor;

    //arcTo(rect, 45,315);//即在矩形rect内，起始角度为45，逆时针转315度形成的弧
    //arcTo(rect, 45,-315);//即在矩形rect内，起始角度为45，顺时针转315度形成的弧
    const int triangleOffset = 30;
    const qreal radius = this->m_radius;
    const qreal triangleHeight = 8;
    const qreal triangleWidth = 3;

    QPainterPath path;
    QRect rect = this->rect();
    int cornerSize = radius*2;//调节圆角的大小

    path.moveTo(rect.left() + m_radius, rect.top());
    path.arcTo(rect.left(), rect.top(), cornerSize, cornerSize, 90.0f, 90.0f);

    path.lineTo(rect.left(), rect.bottom() - m_radius);
    path.arcTo(rect.left(), rect.bottom() - cornerSize, cornerSize, cornerSize, 180.0f, 90.0f);

    path.lineTo(rect.right() - triangleWidth - m_radius, rect.bottom());
    path.arcTo(rect.right()- triangleWidth - cornerSize, rect.bottom() - cornerSize, cornerSize, cornerSize, 270.0f, 90.0f);

    path.lineTo(rect.right()- triangleWidth, rect.height() / 2 + triangleHeight/2 + triangleOffset);
    path.lineTo(rect.right(), rect.height() / 2 + triangleOffset);
    path.lineTo(rect.right()- triangleWidth, rect.height() / 2 + triangleOffset - triangleHeight/2);
    path.lineTo(rect.right()- triangleWidth, m_radius);

    path.arcTo(rect.right() - triangleWidth - cornerSize, rect.top(), cornerSize, cornerSize, 0.0f, 90.0f);
    path.lineTo(rect.left() + m_radius, rect.top());

    painter.fillPath(path, background);

    QPen pen(borderColor);
    pen.setWidth(1);
    painter.strokePath(path, pen);
}
