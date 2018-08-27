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

#include "preferences.h"
#include "citieslist.h"

#include <QSettings>

#include "global.h"
using namespace Global;

Preferences::Preferences()
{
    m_cityList = new CitiesList;

    reset();
    load();
}

Preferences::~Preferences()
{
    save();
    delete m_cityList;
}

void Preferences::reset()
{
    weather.weatherStatus = "";
    weather.weatherIcon = "";
    weather.temperature = "";
    weather.sd = "";
    weather.wind = "";
    weather.aqi = "";

    m_cityList->clear();
}

void Preferences::save()
{
    QSettings *set = m_settings;

    set->beginGroup("City");
    set->setValue("current_city", currentCity);
    set->setValue("current_city_id", currentCityId);
    set->setValue("city_list", m_cityList->getCitiesList());
    set->endGroup();

    set->beginGroup("Weather");
    set->setValue("status", weather.weatherStatus);
    set->setValue("icon", weather.weatherIcon);
    set->setValue("temperature", weather.temperature);
    set->setValue("sd", weather.sd);
    set->setValue("wind", weather.wind);
    set->setValue("aqi", weather.aqi);
    set->endGroup();

//    set->beginGroup("Forecast");
//    set->endGroup();

    set->sync();
}

void Preferences::load()
{
    QSettings *set = m_settings;

    set->beginGroup("City");
    currentCity = set->value("current_city", currentCity).toString();
    currentCityId = set->value("current_city_id", currentCityId).toString();
    m_cityList->loadCityesToStringList(set->value("city_list", m_cityList->getCitiesList()).toStringList());
    set->endGroup();

    set->beginGroup("Weather");
    weather.weatherStatus = set->value("status", weather.weatherStatus).toString();
    weather.weatherIcon = set->value("icon", weather.weatherIcon).toString();
    weather.temperature = set->value("temperature", weather.temperature).toString();
    weather.sd = set->value("sd", weather.sd).toString();
    weather.wind = set->value("wind", weather.wind).toString();
    weather.aqi = set->value("aqi", weather.aqi).toString();
    set->endGroup();
}
