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

#ifndef NOW_WEATHER_WIDGET_H
#define NOW_WEATHER_WIDGET_H

#include <QFrame>
#include <QTimer>

#include "weatherworker.h"

class QLabel;
class TranslucentLabel;
class TipWidget;
class TextTip;
class TipModule;

class NowWeatherWidget : public QFrame
{
    Q_OBJECT

public:
    explicit NowWeatherWidget(WeatherWorker *weatherWorker, QFrame *parent = 0);
    ~NowWeatherWidget();

    void setWeatherIcon(const QString &iconPath);
    void displayTip();
    void refreshData(const ObserveWeather &data);

private:
    QLabel *m_tempLabel = nullptr;
    QLabel *m_humidityLabel = nullptr;
    QLabel *m_humidityValueLabel = nullptr;
    QLabel *m_windLabel = nullptr;
    QLabel *m_windPowerLabel = nullptr;
    QLabel *m_weatherIcon = nullptr;
    TranslucentLabel *m_aqiLabel = nullptr;
    TranslucentLabel *m_temperatureLabel = nullptr;
    TipWidget *m_tipWidget = nullptr;
    WeatherWorker *m_weatherWorker = nullptr;
    QTimer *m_tipTimer = nullptr;
    TextTip *m_tip = nullptr;
    TipModule *m_tipModule = nullptr;
};

#endif // NOW_WEATHER_WIDGET_H
