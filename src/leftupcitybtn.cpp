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

#include "leftupcitybtn.h"
#include "citycollectionwidget.h"

LeftUpCityBtn::LeftUpCityBtn(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(76, 20);
    this->setCursor(QCursor(Qt::PointingHandCursor));

    m_layout = new QHBoxLayout(this);

    m_addCityBtn = new QPushButton(this);
    m_addCityBtn->setFocusPolicy(Qt::NoFocus);
    m_addCityBtn->setFixedSize(20, 20);
    m_addCityBtn->setStyleSheet("QPushButton{border:0px;background:transparent;background-image:url(':/res/control_icons/add_normal_btn.png');}");
    // m_addCityBtn->setCursor(QCursor(Qt::PointingHandCursor));
    // m_iconLabel->setScaledContents(true);//自动缩放,显示图像大小自动调整为Qlabel大小

    m_addCityLabel = new QLabel(this);
    m_addCityLabel->setFixedSize(48, 16);
    m_addCityLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:16px;}");

    m_layout->addWidget(m_addCityBtn);
    // m_layout->addItem(new QSpacerItem(8,8,QSizePolicy::Fixed));
    m_layout->setSpacing(8);
    m_layout->addWidget(m_addCityLabel);

    setLayout(m_layout);
    m_layout->setContentsMargins(0, 0, 0, 0);

    m_addCityLabel->setText("北京");
    setStyleSheet("QWidget{background-color:rgba(255,0,0,1);}");
}

void LeftUpCityBtn::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);

    m_addCityBtn->setStyleSheet("QPushButton{border:0px;background:transparent;background-image:url(':/res/control_icons/add_hover_btn.png');}");

    this->m_mouseHover = true;
}

void LeftUpCityBtn::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);

    m_addCityBtn->setStyleSheet("QPushButton{border:0px;background:transparent;background-image:url(':/res/control_icons/add_normal_btn.png');}");

    this->m_mouseHover = false;
}

void LeftUpCityBtn::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    m_addCityBtn->setStyleSheet("QPushButton{border:0px;background:transparent;background-image:url(':/res/control_icons/add_pressed_btn.png');}");

    CityCollectionWidget *m_citycollectionwidget = new CityCollectionWidget();
    m_citycollectionwidget->show();
}

void LeftUpCityBtn::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
}
