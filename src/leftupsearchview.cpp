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

#include "leftupsearchview.h"
#include "weatherworker.h"

#include <QScrollBar>

LeftUpSearchView::LeftUpSearchView(QWidget *parent)
    : QListView(parent)
{
    this->setMouseTracking(true);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setStyleSheet("QListView{margin:0px;padding:0px;border:none;border-radius:4px;outline:none;background-color:rgba(255,255,255,0.25);}"
                        "QListView::item:selected:!active{background-color:rgba(255,255,255,0.25);}"
                        "QListView::item:selected:active{background-color:rgba(255,255,255,0.25);}"
                        "QListView::item:hover{background-color:rgba(255,255,255,0.25);}");
    this->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{margin:0px 0px 0px 0px;background-color:transparent;border:0px;width:1px;}" \
                                             "QScrollBar::sub-line:vertical{subcontrol-origin:margin;border:none;height:6px;}" \
                                             "QScrollBar::up-arrow:vertical{subcontrol-origin:margin;background-color:transparent;height:6px;}" \
                                             "QScrollBar::sub-page:vertical{background-color:transparent;}" \
                                             "QScrollBar::handle:vertical{background-color:transparent;width:1px;}" \
                                             "QScrollBar::handle:vertical:hover{background-color:transparent;width:1px;}" \
                                             "QScrollBar::handle:vertical:pressed{background-color:transparent;width:1px;}" \
                                             "QScrollBar::add-page:vertical{background-color:transparent;}" \
                                             "QScrollBar::down-arrow:vertical{background-color:transparent;}" \
                                             "QScrollBar::add-line:vertical{subcontrol-origin:margin;border:none;height:6px;}");

    m_weatherWorker = new WeatherWorker(this);

    connect(m_weatherWorker, SIGNAL(requestSetObserveWeather(ObserveWeather)), this, SIGNAL(requestSetObserveWeather(ObserveWeather)));
    connect(m_weatherWorker, SIGNAL(requestSetForecastWeather(ForecastWeather)), this, SIGNAL(requestSetForecastWeather(ForecastWeather)));
    connect(m_weatherWorker, SIGNAL(requestSetLifeStyle(LifeStyle)), this, SIGNAL(requestSetLifeStyle(LifeStyle)));
    connect(m_weatherWorker, SIGNAL(responseFailure(int)), this, SIGNAL(responseFailure(int)));
}

LeftUpSearchView::~LeftUpSearchView()
{

}

void LeftUpSearchView::mouseReleaseEvent(QMouseEvent *e)
{
    QModelIndexList sourceIndexList = this->selectionModel()->selectedIndexes();

    foreach (QModelIndex sourceIndex, sourceIndexList){
        QVariant variant = sourceIndex.data(Qt::UserRole);
        ItemData data = variant.value<ItemData>();
        // qDebug() << "Index : " << sourceIndex.row();
        // emit requestSetCityName(data.cityName);
        m_weatherWorker->onWeatherDataRequest(data.cityId); //获取选取城市的天气数据
    }
}

void LeftUpSearchView::requestWeatherData(QString cityId){
    m_weatherWorker->onWeatherDataRequest(cityId);
}
