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

#include "settingtitlebar.h"
#include "activebutton.h"

#include <QApplication>
#include <QDebug>
#include <QTimer>
#include <QPainter>

SettingTitleBar::SettingTitleBar(QWidget *parent) : QWidget(parent)
  , m_borderColor(QColor(255, 255, 255, 153))
  , m_borderRadius(1)
  , m_borderWidth(1)
  , m_bgBrush(QBrush(QColor("#1374e8")))
  , m_timer(new QTimer(this))
{
    this->initWidgets();

    m_timer->setSingleShot(true);
    m_timer->setInterval(1000*5);
    connect(m_timer, &QTimer::timeout, this, [=] {
        m_timer->stop();
        m_tipLabel->setVisible(false);
    }, Qt::QueuedConnection);
}

SettingTitleBar::~SettingTitleBar()
{
    QLayoutItem *child;
    while ((child = m_layout->takeAt(0)) != 0) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }
}

void SettingTitleBar::initLeftContent()
{
    QWidget *w = new QWidget;
    m_lLayout = new QHBoxLayout(w);
    m_lLayout->setContentsMargins(0, 0, 0, 0);
    m_lLayout->setSpacing(0);
    m_toplayout->addWidget(w, 1, Qt::AlignLeft);
}

void SettingTitleBar::initMiddleContent()
{
    QWidget *w = new QWidget;
    w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_mLayout = new QHBoxLayout(w);
    m_mLayout->setContentsMargins(0, 0, 0, 0);
    m_mLayout->setSpacing(5);
    m_toplayout->addWidget(w);

    QLabel *m_iconLabel = new QLabel;
    m_iconLabel->setContentsMargins(0, 0, 0, 0);
    QPixmap iconPixmap = QPixmap(QIcon::fromTheme("indicator-china-weather", QIcon(":/res/indicator-china-weather_min.png")).pixmap(QSize(16, 16)));
    m_iconLabel->setPixmap(iconPixmap);
    m_iconLabel->setFixedSize(iconPixmap.size());

    QLabel *m_titleLabel = new QLabel;
    m_titleLabel->setStyleSheet("QLabel{font-size:12px;color: rgb(255,255,255);background-color:transparent;}");
    m_titleLabel->setText(tr("Kylin Weather - Setting"));

    m_mLayout->addWidget(m_iconLabel);
    m_mLayout->addWidget(m_titleLabel);
}

void SettingTitleBar::initRightContent()
{
    QWidget *w = new QWidget;
    m_rLayout = new QHBoxLayout(w);
    m_rLayout->setContentsMargins(0, 0, 5, 0);
    m_rLayout->setSpacing(0);
    m_toplayout->addWidget(w, 1, Qt::AlignRight);

    m_closeBtn = new QPushButton(this);
    m_closeBtn->setFocusPolicy(Qt::NoFocus);
    m_closeBtn->setFixedSize(32,32);
    m_closeBtn->setStyleSheet("QPushButton{background-image:url(':/res/close_normal_btn.png');border:0px;}QPushButton:hover{background:url(':/res/close_hover_btn.png');}QPushButton:pressed{background:url(':/res/close_pressed_btn.png');}");

    m_rLayout->addWidget(m_closeBtn);

    connect(m_closeBtn, &QPushButton::clicked, this, &SettingTitleBar::requestCloseDialog);
}

void SettingTitleBar::initBottomContent()
{
    QWidget *w = new QWidget;
    m_bLayout = new QHBoxLayout(w);
    m_bLayout->setContentsMargins(10, 0, 20, 0);
    m_bLayout->setSpacing(10);
    m_layout->addWidget(w);

    ActiveButton *locationBtn = new ActiveButton(tr("Location Setting"));
    locationBtn->setActive(true);
    ActiveButton *systemBtn = new ActiveButton(tr("System Setting"));

    m_tipLabel = new QLabel(this);
    m_tipLabel->setFixedHeight(28);
    m_tipLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_tipLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#fff222;font-size:12px;}");
    m_tipLabel->setVisible(false);

    m_bLayout->addWidget(locationBtn);
    m_bLayout->addWidget(systemBtn);
    m_bLayout->addStretch();
    m_bLayout->addWidget(m_tipLabel);

    connect(locationBtn, &ActiveButton::btnClicked, this, [=] {
        locationBtn->setActive(true);
        systemBtn->setActive(false);
        emit this->requestSwitchPage(true);
    });
    connect(systemBtn, &ActiveButton::btnClicked, this, [=] {
        systemBtn->setActive(true);
        locationBtn->setActive(false);
        emit this->requestSwitchPage(false);
    });
}

void SettingTitleBar::initWidgets()
{
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    this->setLayout(m_layout);

    m_toplayout = new QHBoxLayout;
    m_toplayout->setContentsMargins(0, 0, 0, 0);
    m_toplayout->setSpacing(0);

    m_layout->addLayout(m_toplayout);

    initLeftContent();
    initMiddleContent();
    initRightContent();
    initBottomContent();
}


void SettingTitleBar::showWarnInfo(const QString &info)
{
    m_tipLabel->setText(info);
    m_tipLabel->setVisible(true);
    m_timer->start();
}

void SettingTitleBar::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

//    QPainterPath outBorderPath;
//    outBorderPath.addRoundedRect(this->rect(), m_borderRadius, m_borderRadius);
//    QPen pen(m_borderColor, m_borderWidth);
//    painter.setPen(pen);
//    painter.drawPath(outBorderPath);

//    QRect borderRect;
//    borderRect.setRect(this->rect().x() + m_borderWidth, this->rect().y() + m_borderWidth, this->rect().width() - m_borderWidth * 2, this->rect().height() - m_borderWidth * 2);
//    QPainterPath inBorderPath;
//    inBorderPath.addRoundedRect(borderRect, m_borderRadius, m_borderRadius);
//    painter.setClipPath(inBorderPath);

    painter.fillRect(0, 0, width(), height(), m_bgBrush);
}
