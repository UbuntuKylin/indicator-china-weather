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


#include "citylistwidget.h"
#include "cityitemwidget.h"
#include "data.h"

#include <QListWidgetItem>
#include <QScrollBar>
#include <QApplication>
#include <QDebug>

CityListWidget::CityListWidget(QWidget *parent) : QListWidget(parent)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setFixedWidth(parent->width() - 2);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_dataList.clear();

    connect(this, &CityListWidget::itemDoubleClicked, this, [=] (QListWidgetItem *item) {
        CityItemWidget *fileItem = static_cast<CityItemWidget *>(itemWidget(item));
        qDebug() << fileItem->getCityId();
    });

    this->loadCityItems();

}

CityListWidget::~CityListWidget()
{
    this->clearUI();
}

void CityListWidget::resetData()
{

}

void CityListWidget::clearUI()
{
    for(int i = 0; i < this->count(); i++) {
        QListWidgetItem *listItem = item(i);
        CityItemWidget *item = static_cast<CityItemWidget *>(itemWidget(listItem));
        if (item) {
            item->deleteLater();
        }
    }
    this->clear();
}

void CityListWidget::removeItemById(const QString &id)
{
    QList<CityItemWidget *> items = findChildren<CityItemWidget*>();
    for (CityItemWidget *item : items) {
        if (item->getCityId() == id) {
            this->removeItemWidget(item->getCityItem());
            item->deleteLater();
            break;
        }
    }
}

void CityListWidget::onRefreshUI()
{
    this->clearUI();
    this->loadCityItems();
}

void CityListWidget::loadCityItems()
{
    //test data;
    CitySettingData info;
    info.active = true;
    info.id = "101010100";
    info.name = "北京";
    info.temperature = "33";
    info.icon = ":/res/weather_icons/lightgrey/100.png";
    loadItem(info);

    CitySettingData info1;
    info1.active = false;
    info1.id = "101020100";
    info1.name = "上海";
    info1.temperature = "34";
    info1.icon = ":/res/weather_icons/lightgrey/100.png";
    loadItem(info1);

    CitySettingData info2;
    info2.active = false;
    info2.id = "101250101";
    info2.name = "长沙";
    info2.temperature = "35";
    info2.icon = ":/res/weather_icons/lightgrey/100.png";
    loadItem(info2);

    m_dataList.append(info);
    m_dataList.append(info1);
    m_dataList.append(info2);

    this->verticalScrollBar()->setValue(0);
}

void CityListWidget::loadItem(const CitySettingData &info)
{
    if (m_dataList.count() > 10) {
        qDebug() << "Max len is 10";
        return;
    }

    for (CitySettingData line : m_dataList) {
        if (info.id == line.id) {
            return;
        }
    }
    CityItemWidget *item = new CityItemWidget(info);
    connect(item, SIGNAL(enter()), this, SLOT(onMouseEnter()));
    connect(item, &CityItemWidget::requestDeleteCity, this, [=] (const QString id) {
        if (this->m_dataList.count() == 1) {
            qDebug() << "At least there must be a city!!!";
            return;
        }
        for (CitySettingData line : m_dataList) {
            if (line.id == id) {
                //delete this city
                this->removeItemById(id);

                // If the deleted city is the default city
                if (line.active) {
                    // Reset default city and Access the city's weather
                    //TODO
                }
            }
        }
    });
    this->addItem(item->getCityItem());
    setItemWidget(item->getCityItem(), item);
}

void CityListWidget::onMouseEnter()
{
    for(int i = 0; i < this->count(); i++) {
        QListWidgetItem *listItem = item(i);
        CityItemWidget *item = static_cast<CityItemWidget *>(itemWidget(listItem));
        if (item->getCityId() == ((CityItemWidget*) sender())->getCityId()) {
            item->setItemHovered();
        } else {
            item->unsetItemHovered();
        }
    }
}
