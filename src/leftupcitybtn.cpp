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
    this->setFixedSize(108, 20);
    this->setCursor(QCursor(Qt::PointingHandCursor));

    m_layout = new QHBoxLayout(this);

    m_addCityBtn = new QLabel(this);
    //m_addCityBtn->setFocusPolicy(Qt::NoFocus);
    m_addCityBtn->setFixedSize(20, 20);
    m_addCityBtn->setStyleSheet("QLabel{border:0px;background:transparent;background-image:url(':/res/control_icons/add_normal_btn.png');}");
    // m_addCityBtn->setCursor(QCursor(Qt::PointingHandCursor));
    // m_iconLabel->setScaledContents(true);//自动缩放,显示图像大小自动调整为Qlabel大小

    m_addCityLabel = new QLabel(this);
    m_addCityLabel->setFixedSize(80, 18);
    m_addCityLabel->setAlignment(Qt::AlignVCenter);
    m_addCityLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:16px;}");

    m_layout->addWidget(m_addCityBtn);
    // m_layout->addItem(new QSpacerItem(8,8,QSizePolicy::Fixed));
    m_layout->setSpacing(8);
    m_layout->addWidget(m_addCityLabel);

    setLayout(m_layout);
    m_layout->setContentsMargins(1, 1, 1, 1);

    m_addCityLabel->setText("N/A");
    setStyleSheet("QWidget{background-color:rgba(255,0,0,1);}");

    connect(this, &LeftUpCityBtn::requestSetCityName, this, [=] (QString cityName) {
        m_addCityLabel->setText(cityName);

    });
}

void LeftUpCityBtn::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);

    m_addCityBtn->setStyleSheet("QLabel{border:0px;background:transparent;background-image:url(':/res/control_icons/add_hover_btn.png');}");

    this->m_mouseHover = true;
}

void LeftUpCityBtn::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);

    m_addCityBtn->setStyleSheet("QLabel{border:0px;background:transparent;background-image:url(':/res/control_icons/add_normal_btn.png');}");

    this->m_mouseHover = false;
}

void LeftUpCityBtn::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    m_addCityBtn->setStyleSheet("QLabel{border:0px;background:transparent;background-image:url(':/res/control_icons/add_pressed_btn.png');}");

    if (!is_open_city_collect_widget) {
        m_citycollectionwidget = new CityCollectionWidget();
        //接收来自收藏城市窗口发来的信号，再发送一个信号到主窗口
        connect(m_citycollectionwidget, &CityCollectionWidget::sendCurrentCityId, this, &LeftUpCityBtn::sendCurrentCityId);

        //给收藏城市窗口发送信号进行更新
        connect(this,&LeftUpCityBtn::updatecity,m_citycollectionwidget,&CityCollectionWidget::updatecity);

        //requestShowCollCityWeather信号会一直传到weathermanager.cpp，用于获取收藏列表中城市的实时天气
        connect(m_citycollectionwidget, &CityCollectionWidget::requestShowCollCityWeather, this, &LeftUpCityBtn::requestShowCollCityWeather);

        //接收来自主窗口的天气数据，再发送到收藏城市列表窗口
        connect(this, SIGNAL(requestSetCityWeather(QString)), m_citycollectionwidget, SLOT(onRequestSetCityWeather(QString)));

        //收到关闭窗口的消息
        connect(m_citycollectionwidget, &CityCollectionWidget::requestChangeWidgetState, this, [=] () {
            is_open_city_collect_widget = false;
            m_citycollectionwidget->deleteLater(); //销毁窗口
        });
        m_citycollectionwidget->show();

        emit requestShowCollCityWeather(); //发送该信号，显示收藏城市窗口各城市的事实天气

        is_open_city_collect_widget = true; //为真时无法打开新的收藏窗口
    }else{
        if(nullptr != m_citycollectionwidget){
            m_citycollectionwidget->show();
            m_citycollectionwidget->activateWindow();
            return;
        }
    }
}

void LeftUpCityBtn::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
}
