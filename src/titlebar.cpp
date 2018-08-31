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

#include "titlebar.h"

#include <QApplication>
#include <QDebug>

TitleBar::TitleBar(QWidget *parent) : QWidget(parent)
{
    this->setFixedHeight(32);

    this->initWidgets();
}

TitleBar::~TitleBar()
{
    QLayoutItem *child;
    while ((child = m_layout->takeAt(0)) != 0) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }
}

void TitleBar::initLeftContent()
{
    m_leftWidget = new QWidget;
    m_leftWidget->setFixedSize(69, 22);
    m_lLayout = new QHBoxLayout(m_leftWidget);
    m_lLayout->setContentsMargins(5, 0, 0, 0);
    m_lLayout->setSpacing(5);
    m_layout->addWidget(m_leftWidget, 1, Qt::AlignLeft | Qt::AlignTop);

    QPushButton *city_button = new QPushButton;
    city_button->setFocusPolicy(Qt::NoFocus);
    city_button->setStyleSheet("QPushButton{background:transparent;border:none;}");
    city_button->setFixedSize(11, 11);
    city_button->setIconSize(QSize(11, 11));
    city_button->setIcon(QIcon(":/res/set_city.png"));

    m_cityLabel = new QLabel;
    m_cityLabel->setText("长沙");//for test
    m_cityLabel->setStyleSheet("QLabel{font-size:12px;color:#ffffff;background:transparent;background-color:transparent;}");

    QLabel *locationLabel = new QLabel;
    locationLabel->setStyleSheet("QLabel{background:transparent;}");
    locationLabel->setFixedSize(8, 11);
    locationLabel->setPixmap(QPixmap(":/res/location.png"));

    m_lLayout->addStretch();
    m_lLayout->addWidget(city_button);
    m_lLayout->addWidget(m_cityLabel);
    m_lLayout->addWidget(locationLabel);
    m_lLayout->addStretch();

    connect(city_button, &QPushButton::clicked, this, &TitleBar::requestShowSettingDialog);
}

void TitleBar::initRightContent()
{
    QWidget *w = new QWidget;
    m_rLayout = new QHBoxLayout(w);
    m_rLayout->setContentsMargins(0, 0, 5, 0);
    m_rLayout->setSpacing(0);
    m_layout->addWidget(w, 1, Qt::AlignRight);
    m_minBtn = new QPushButton(this);
    m_closeBtn = new QPushButton(this);

    m_minBtn->setFocusPolicy(Qt::NoFocus);
    m_closeBtn->setFocusPolicy(Qt::NoFocus);

    m_minBtn->setFixedSize(32,32);
    m_closeBtn->setFixedSize(32,32);

    m_minBtn->setStyleSheet("QPushButton{background-image:url(':/res/min_normal_btn.png');border:0px;}QPushButton:hover{background:url(':/res/min_hover_btn.png');}QPushButton:pressed{background:url(':/res/min_pressed_btn.png');}");
    m_closeBtn->setStyleSheet("QPushButton{background-image:url(':/res/close_normal_btn.png');border:0px;}QPushButton:hover{background:url(':/res/close_hover_btn.png');}QPushButton:pressed{background:url(':/res/close_pressed_btn.png');}");

    m_rLayout->addWidget(m_minBtn);
    m_rLayout->addWidget(m_closeBtn);

    connect(m_minBtn, &QPushButton::clicked, this, [=] {
        if (parentWidget() && parentWidget()->parentWidget()) {
//            parentWidget()->parentWidget()->showMinimized();
            parentWidget()->parentWidget()->hide();
        }
    });

    connect(m_closeBtn, &QPushButton::clicked, this, [=] {
//        qApp->quit();
        if (parentWidget() && parentWidget()->parentWidget()) {
//            parentWidget()->parentWidget()->showMinimized();
            parentWidget()->parentWidget()->hide();
        }
    });
}

void TitleBar::initWidgets()
{
    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(2, 2, 0, 0);
    m_layout->setSpacing(0);
    this->setLayout(m_layout);

    initLeftContent();
    initRightContent();
}

void TitleBar::setCityName(const QString &city)
{
    m_cityLabel->setText(city);
}

void TitleBar::setDayStyleSheets()
{
    m_leftWidget->setStyleSheet("QWidget{background-image:none;}");
}

void TitleBar::setNightStyleSheets()
{
    m_leftWidget->setStyleSheet("QWidget{background-image:url(':/res/location_bg_hover.png');}");
}
