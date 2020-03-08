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

#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QString>
#include <QStringList>

#include "data.h"

class Preferences {
public:
    Preferences();
    ~Preferences();

    void save();
    void load();

    //void addCityToStringList(QString name);
    //void removeCityFromStringList(QString name);
    //void loadCityesToStringList(QStringList list);
    QStringList getCitiesList();
    //QString cityName(int index);
    int citiesCount();
    void setCurrentCityIdAndName(const QString &cityName/*int index*/);
    void resetCurrentCityNameById(const QString &id);

    void addCityInfoToPref(City city);
    void removeCityInfoFromPref(const QString &id, bool isActive);
    void reloadCityList();
    bool isCityIdExist(const QString &id);
    bool isCitiesCountOverMax();
    void setDefaultCity();

    QString m_currentCity;
    QString m_currentCityId;
    int m_updateFrequency;
    int m_opacity;
    bool m_serverNotify;

    ObserveWeather m_observerWeather;
    LifeStyle m_lifestyle;
    AqiAir m_aqiAir;

    QList<ForecastWeather> m_forecastList;

    int m_maxCityItems;
    QStringList m_cityList;
    //QStringList m_cityIdList;
    QList<City> m_cities;
};

#endif // PREFERENCES_H
