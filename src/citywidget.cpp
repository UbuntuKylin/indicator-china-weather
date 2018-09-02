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

#include "citywidget.h"
#include "citylistwidget.h"
#include "cityitemwidget.h"

#include <QEvent>
#include <QScrollBar>
#include <QDebug>

CityWidget::CityWidget(QWidget *parent)
    : QWidget(parent)
    , m_cityListWidget(new CityListWidget)
    , m_addBtn(new QPushButton(this))
{
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setStyleSheet("QWidget{border:none; background-color:#ffffff;}");

    m_scrollArea = new QScrollArea;
    m_scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameStyle(QFrame::NoFrame);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{margin:0px 0px 0px 0px;background-color:rgb(255,255,255,100);border:0px;width:10px;}" \
                                             "QScrollBar::sub-line:vertical{subcontrol-origin:margin;border:1px solid red;height:13px;}" \
                                             "QScrollBar::up-arrow:vertical{subcontrol-origin:margin;background-color:blue;height:13px;}" \
                                             "QScrollBar::sub-page:vertical{background-color:#EEEDF0;}" \
                                             "QScrollBar::handle:vertical{background-color:#D1D0D2;width:10px;}" \
                                             "QScrollBar::handle:vertical:hover{background-color:#14ACF5;width:10px;}" \
                                             "QScrollBar::handle:vertical:pressed{background-color:#0B95D7;width:10px;}" \
                                             "QScrollBar::add-page:vertical{background-color:#EEEDF0;}" \
                                             "QScrollBar::down-arrow:vertical{background-color:yellow;}" \
                                             "QScrollBar::add-line:vertical{subcontrol-origin:margin;border:1px solid green;height:18px;}");

    m_addBtn->setFocusPolicy(Qt::NoFocus);
    m_addBtn->setStyleSheet("QPushButton{font-size:12px;color:#808080;border:none;background:transparent;text-align:left;}QPushButton:hover{background-color:#f5fbff;}");//margin-left:18px;border:1px solid rgba(0, 0, 0, 0.08);
    m_addBtn->setFixedWidth(this->width());
    m_addBtn->setText("Add City");
    m_addBtn->setIconSize(QSize(24, 24));
    m_addBtn->setIcon(QIcon(":/res/add.png"));

    m_layout = new QVBoxLayout;
    m_layout->addWidget(m_scrollArea/*, 1, Qt::AlignCenter*/);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    this->setLayout(m_layout);

    m_widget = new QWidget;
    //m_widget->setStyleSheet("QWidget{background-color: #0d8700;}");//test widget's area with background color
    m_widget->setFixedWidth(m_scrollArea->width());
    m_scrollArea->setWidget(m_widget);
    QVBoxLayout *v_layout = new QVBoxLayout(m_widget);
    v_layout->setContentsMargins(0,0,0,0);
    v_layout->setSpacing(10);
    v_layout->addWidget(m_cityListWidget);
    v_layout->addWidget(m_addBtn);
    v_layout->addStretch();

    connect(m_addBtn, &QPushButton::clicked, this, &CityWidget::requestAddCity);

    m_dataList.clear();
    this->loadCityItems();//for test data
}

CityWidget::~CityWidget()
{
    QList<CityItemWidget *> items = findChildren<CityItemWidget*>();
    for (CityItemWidget *item : items) {
        m_cityListWidget->removeItem(item);
        item->deleteLater();
    }

    QLayoutItem *child;
    while ((child = m_layout->takeAt(0)) != 0) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }

    if (m_addBtn) {
        delete m_addBtn;
        m_addBtn = nullptr;
    }
    if (m_cityListWidget) {
        delete m_cityListWidget;
        m_cityListWidget = nullptr;
    }
    if (m_widget) {
        delete m_widget;
        m_widget = nullptr;
    }
    if (m_scrollArea) {
        delete m_scrollArea;
        m_scrollArea = nullptr;
    }
}


bool CityWidget::event(QEvent *event)
{
    if (event->type() == QEvent::LayoutRequest) {;
        m_widget->setFixedHeight(m_layout->sizeHint().height());
    }

    return QWidget::event(event);
}

void CityWidget::loadCityItems()
{
    //test data;
    CitySettingData info;
    info.active = true;
    info.id = "101010100";
    info.name = "北京";
    info.temperature = "33";
    info.icon = ":/res/weather_icons/lightgrey/100.png";
    CitySettingData info1;
    info1.active = false;
    info1.id = "101020100";
    info1.name = "上海";
    info1.temperature = "34";
    info1.icon = ":/res/weather_icons/lightgrey/100.png";
    CitySettingData info2;
    info2.active = false;
    info2.id = "101250101";
    info2.name = "长沙";
    info2.temperature = "35";
    info2.icon = ":/res/weather_icons/lightgrey/100.png";

    addCityItem(info);
    addCityItem(info1);
    addCityItem(info2);
}

void CityWidget::addCityItem(const CitySettingData &info)
{
    if (m_dataList.count() > 10) {
        qDebug() << "Max len is 10";
        return;
    }

    for (CitySettingData line : m_dataList) {
        if (info.id == line.id) {
            qDebug() << "The city had exist!";
            return;
        }
    }
    m_dataList.append(info);

    CityItemWidget *item = new CityItemWidget(info);
    m_cityListWidget->appendItem(item);
//    connect(item, SIGNAL(enter()), this, SLOT(onMouseEnter()));
    connect(item, &CityItemWidget::requestDeleteCity, this, [=] (const QString id) {
        if (this->m_dataList.count() == 1) {
            qDebug() << "At least there must be a city!!!";
            return;
        }
        for (CitySettingData line : m_dataList) {
            if (line.id == id) {
                //delete this city
                this->removeCityItemById(id);
                emit this->requestRemoveCityFromMenu(id);

                // If the deleted city is the default city
                if (line.active) {
                    // Reset default city and Access the city's weather
                    //TODO
                    emit this->requestSetDefaultCity();

//                    item->setItemAction(true);
                }
            }
        }
    });

    connect(item, &CityItemWidget::requestRefreshDefaultCity, this, [=] (const QString id) {
        for (CitySettingData line : m_dataList) {
            if (line.id == id) {
            }
            QList<CityItemWidget *> items = findChildren<CityItemWidget*>();
            for (CityItemWidget *item : items) {
//                item->setItemAction(true);
                //TODO
            }
        }
    });
}

void CityWidget::removeCityItemById(const QString &id)
{
    QList<CityItemWidget *> items = findChildren<CityItemWidget*>();
    for (CityItemWidget *item : items) {
        if (item->getCityId() == id) {
            m_cityListWidget->removeItem(item);
            item->deleteLater();
            break;
        }
    }
}

/*void CityWidget::onMouseEnter()
{
    QList<CityItemWidget *> items = findChildren<CityItemWidget*>();
    for (CityItemWidget *item : items) {
        if (item->getCityId() == ((CityItemWidget*) sender())->getCityId()) {
            item->setItemHovered();
        } else {
            item->unsetItemHovered();
        }
    }
}*/
