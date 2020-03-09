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

#include "citywidget.h"
#include "citylistwidget.h"
#include "cityitemwidget.h"

#include <QEvent>
#include <QScrollBar>
#include <QTimer>
#include <QDebug>

#include "preferences.h"
#include "global.h"
using namespace Global;

CityWidget::CityWidget(QWidget *parent)
    : QWidget(parent)
    , m_cityListWidget(new CityListWidget)
    , m_addBtn(new QPushButton(this))
//    , m_timer(new QTimer(this))
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
    m_addBtn->setCursor(QCursor(Qt::PointingHandCursor));
    m_addBtn->setStyleSheet("QPushButton{padding-left:10px;font-size:12px;color:#808080;border:none;background:transparent;text-align:left;}QPushButton:hover{background-color:#f5fbff;}");//margin-left:18px;border:1px solid rgba(0, 0, 0, 0.08);
    m_addBtn->setFixedWidth(this->width());
    m_addBtn->setText(tr("Add City"));
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
    //m_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_scrollArea->setWidget(m_widget);
    QVBoxLayout *v_layout = new QVBoxLayout(m_widget);
    v_layout->setContentsMargins(0,0,0,0);
    v_layout->setSpacing(10);
    v_layout->addWidget(m_cityListWidget);
    v_layout->addWidget(m_addBtn);
    v_layout->addStretch();

    connect(m_addBtn, &QPushButton::clicked, this, &CityWidget::requestAddCity);

    m_dataList.clear();
    this->loadCityItems();

//    m_timer->setSingleShot(false);
//    m_timer->setInterval(30);
//    connect(m_timer, &QTimer::timeout, this, &CityWidget::refreshListWeatherStatus, Qt::QueuedConnection);
}

CityWidget::~CityWidget()
{
//    if (m_timer) {
//        disconnect(m_timer, SIGNAL(timeout()), this, SLOT(refreshListWeatherStatus()));
//        if(m_timer->isActive()) {
//            m_timer->stop();
//        }
//        delete m_timer;
//        m_timer = nullptr;
//    }

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
    if (event->type() == QEvent::LayoutRequest) {
        m_widget->setFixedHeight(m_layout->sizeHint().height());
    }

    return QWidget::event(event);
}

void CityWidget::loadCityItems()
{
    for (int i = 0; i < m_preferences->m_cities.size(); ++i) {
        CitySettingData info;
        info.active = (m_preferences->m_currentCityId == m_preferences->m_cities.at(i).id) ? true : false;
        info.id = m_preferences->m_cities.at(i).id;
        info.name = m_preferences->m_cities.at(i).name;
        info.temperature = (m_preferences->m_currentCityId == m_preferences->m_cities.at(i).id) ? m_preferences->m_observerWeather.tmp + "°C" : "";

        if (m_preferences->m_observerWeather.cond_code.isEmpty()) {
            info.icon = ":/res/weather_icons/darkgrey/999.png";
        }
        else {
            info.icon = (m_preferences->m_currentCityId == m_preferences->m_cities.at(i).id) ? QString(":/res/weather_icons/darkgrey/%1.png").arg(m_preferences->m_observerWeather.cond_code) : ":/res/weather_icons/darkgrey/999.png";

        }
        addCityItem(info);
    }
}

void CityWidget::addCityItem(const CitySettingData &info)
{
    if (m_preferences->isCitiesCountOverMax()) {
        emit responseCityError(QString(tr("Only 10 cities can be added at most!")));//最多只能添加10个城市
        return;
    }

    for (CitySettingData line : m_dataList) {
        if (info.id == line.id) {
            emit responseCityError(QString(tr("The city already exists!")));//该城市已存在
            return;
        }
    }

    m_dataList.append(info);

    CityItemWidget *item = new CityItemWidget(info);
    m_cityListWidget->appendItem(item);
//    connect(item, SIGNAL(enter()), this, SLOT(onMouseEnter()));
    connect(item, &CityItemWidget::requestDeleteCity, this, [=] (const QString id) {
        if (m_preferences->citiesCount() == 1) {
            emit responseCityError(QString(tr("At least one city needs to be preserved!")));//至少需要保留一个城市
            return;
        }

        for (CitySettingData line : m_dataList) {
            if (line.id == id) {
                //delete this city
                int pos = m_dataList.indexOf(line);
                if (pos != -1) {
                    m_dataList.removeAt(pos);
                }
                this->removeCityItemById(id);

                //如果删除的城市是默认城市，则需要设置新的默认城市
                m_preferences->removeCityInfoFromPref(id, line.active);//remove and update m_cityList, m_cities, m_currentCity and m_currentCityId
                this->refreshCityList(m_preferences->m_currentCityId);
                emit this->requestRefreshCityMenu(line.active);
            }
        }
    });

    //根据城市id设置该城市为默认选中的当前城市
    connect(item, &CityItemWidget::requestSetDefaultCityById, this, [=] (const QString id) {
        QList<CityItemWidget *> cityItems = findChildren<CityItemWidget*>();
        for (CityItemWidget *cityItem : cityItems) {
            if (cityItem->getCityId() == id) {
                cityItem->setItemAction(true);

                QList<CitySettingData>::iterator iter = m_dataList.begin();
                for (; iter != m_dataList.end(); iter++) {
                    if (iter->id == id)
                        iter->setActive(true);
                    else
                        iter->setActive(false);
                }

                emit this->requestRefreshWeatherById(id);
            }
            else {
                cityItem->setItemAction(false);
            }
//            m_timer->start();
        }
    });
//    m_timer->start();
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
//    m_timer->start();
    this->refreshListWeatherStatus();
}

void CityWidget::refreshListWeatherStatus()
{
    QList<CityItemWidget *> items = findChildren<CityItemWidget*>();
    for (CityItemWidget *item : items) {
        if (item->getCityId() == m_preferences->m_currentCityId) {
            QString icon;
            QString temp;
            if (m_preferences->m_observerWeather.cond_code.isEmpty()) {
                temp = "";
            }
            else {
                temp = m_preferences->m_observerWeather.tmp + "°C";
            }
            if (m_preferences->m_observerWeather.cond_code.isEmpty()) {
                icon = ":/res/weather_icons/darkgrey/999.png";
            }
            else {
                icon = QString(":/res/weather_icons/darkgrey/%1.png").arg(m_preferences->m_observerWeather.cond_code);
            }
            item->setItemWeather(temp, icon);
        }
        else {
            item->setItemWeather("", ":/res/weather_icons/darkgrey/999.png");
        }
    }
}

void CityWidget::refreshCityList(const QString &id)
{
    QList<CityItemWidget *> cityItems = findChildren<CityItemWidget*>();
    for (CityItemWidget *cityItem : cityItems) {
        if (cityItem->getCityId() == id)
            cityItem->setItemAction(true);
        else
            cityItem->setItemAction(false);
    }
    //m_timer->start();

    this->refreshListWeatherStatus();
}

void CityWidget::showEvent(QShowEvent *event)
{
    this->refreshListWeatherStatus();
    QWidget::showEvent(event);
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
