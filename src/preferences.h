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

#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QString>
#include <QStringList>

class CitiesList;

class Preferences {
public:
    Preferences();
    ~Preferences();

    void reset();

    void save();
    void load();

    QString currentCity;
    QString currentCityId;

    struct Weather_settings {
        QString weatherStatus;
        QString weatherIcon;
        QString temperature;
        QString sd;
        QString wind;
        QString aqi;
    } weather;

    struct Forecast_settings {
        QString day1_weatherStatus;
        QString day1_weatherIcon;
        QString day1_temperature;
        QString day1_tempRange;
        QString sd;
        QString wind;
        QString aqi;
    } forecast;

    CitiesList *m_cityList;
};

#endif // PREFERENCES_H
