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

#ifndef CONTENTWIDGET_H
#define CONTENTWIDGET_H

#include <QFrame>

#include "data.h"

class QVBoxLayout;
class NowWeatherWidget;
class ForecastWeatherWidget;
class NetworkErrorWidget;
//class IndexWeatherWidget;

class ContentWidget : public QFrame
{
    Q_OBJECT

public:
    explicit ContentWidget(QWidget *parent = 0);
    ~ContentWidget();

    void setNetworkErrorPages();

    void refreshObserveUI(const ObserveWeather &data);
    void refreshForecastUI(const ForecastWeather &data, int index);
    void refreshLifestyleUI(const LifeStyle &data);

    void setDayStyleSheets();
    void setNightStyleSheets();

    void showServerNotifyInfo(const QString &info);

signals:
    void requestRetryWeather();

private:
    QVBoxLayout *m_layout = nullptr;

    NowWeatherWidget *m_nowWidget = nullptr;
    ForecastWeatherWidget *m_forecastWidget = nullptr;
    NetworkErrorWidget *m_networkErrorWidget = nullptr;
//    IndexWeatherWidget *m_indexWidget = nullptr;
};

#endif // CONTENTWIDGET_H
