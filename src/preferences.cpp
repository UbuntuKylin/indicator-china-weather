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

#include "preferences.h"

#include <QSettings>
#include <QDebug>

#include "global.h"
using namespace Global;

Preferences::Preferences()
{
    m_cityList.clear();
    //m_cityIdList.clear();
    m_forecastList.clear();
    m_cities.clear();
    m_maxCityItems = 10;
    m_updateFrequency = 30;
    m_opacity = 100;
    m_serverNotify = true;

    load();
}

Preferences::~Preferences()
{
    save();
}

void Preferences::save()
{
    QSettings *set = m_settings;

    set->beginGroup("City");
    set->setValue("current_city_id", m_currentCityId);
    set->setValue("current_city", m_currentCity);
    //set->setValue("city_list2", this->getCitiesList());

    set->remove("city_list");
    set->beginWriteArray("city_list");
    for (int i = 0; i < m_cities.size(); ++i) {
        set->setArrayIndex(i);
        set->setValue("id", m_cities.at(i).id);
        set->setValue("name", m_cities.at(i).name);
        //qDebug() << m_cities.at(i).id << m_cities.at(i).name;
    }
    set->endArray();
    set->setValue("update_frequency", m_updateFrequency);
    set->setValue("opacity", m_opacity);
    set->endGroup();

    set->beginGroup("Weather");
    set->setValue("updatetime", m_observerWeather.updatetime);
    set->setValue("air", m_observerWeather.air);
    set->setValue("cloud", m_observerWeather.cloud);
    set->setValue("cond_code", m_observerWeather.cond_code);
    set->setValue("cond_txt", m_observerWeather.cond_txt);
    set->setValue("fl", m_observerWeather.fl);
    set->setValue("hum", m_observerWeather.hum);
    set->setValue("pcpn", m_observerWeather.pcpn);
    set->setValue("pres", m_observerWeather.pres);
    set->setValue("tmp", m_observerWeather.tmp);
    set->setValue("vis", m_observerWeather.vis);
    set->setValue("wind_deg", m_observerWeather.wind_deg);
    set->setValue("wind_dir", m_observerWeather.wind_dir);
    set->setValue("wind_sc", m_observerWeather.wind_sc);
    set->setValue("wind_spd", m_observerWeather.wind_spd);
    set->endGroup();



//    [Forecast]
//    cond_code_d=306, 305, 101
//    cond_code_n=306, 101, 101
//    cond_txt_d=中雨, 小雨, 多云
//    cond_txt_n=中雨, 多云, 多云
//    forcast_date=2020-03-08, 2020-03-09, 2020-03-10
//    hum=95, 84, 78
//    mr_ms=16:58+05:44, 18:07+06:30, 19:16+07:13
//    pcpn=0.0, 3.1, 0.0
//    pop=24, 68, 4
//    pres=999, 1013, 1012
//    sr_ss=06:43+18:33, 06:42+18:34, 06:41+18:34
//    tmp_max=16, 16, 17
//    tmp_min=13, 9, 8
//    uv_index=2, 2, 12
//    vis=13, 24, 25
//    wind_deg=353, 6, 352
//    wind_dir=北风, 北风, 北风
//    wind_sc=1-2, 3-4, 1-2
//    wind_spd=11, 22, 8


    QString cond_code_d;
    QString cond_code_n;
    QString cond_txt_d;
    QString cond_txt_n;
    QString forcast_date;
    QString hum;
//    QString mr_ms;
    QString mr;
    QString ms;
    QString pcpn;
    QString pop;
    QString pres;
//    QString sr_ss;
    QString sr;
    QString ss;
    QString tmp_max;
    QString tmp_min;
    QString uv_index;
    QString vis;
    QString wind_deg;
    QString wind_dir;
    QString wind_sc;
    QString wind_spd;
    set->beginGroup("Forecast");
    int forecastCount = m_forecastList.size();
    for (int i = 0; i < forecastCount; ++i) {
        forcast_date += m_forecastList.at(i).forcast_date;
        cond_code_d += m_forecastList.at(i).cond_code_d;
        cond_txt_d += m_forecastList.at(i).cond_txt_d;
        cond_code_n += m_forecastList.at(i).cond_code_n;
        cond_txt_n += m_forecastList.at(i).cond_txt_n;
        hum += m_forecastList.at(i).hum;
        mr += m_forecastList.at(i).mr;
        ms += m_forecastList.at(i).ms;
        pcpn += m_forecastList.at(i).pcpn;
        pop += m_forecastList.at(i).pop;
        pres += m_forecastList.at(i).pres;
        sr += m_forecastList.at(i).sr;
        ss += m_forecastList.at(i).ss;
        tmp_max += m_forecastList.at(i).tmp_max;
        tmp_min += m_forecastList.at(i).tmp_min;
        uv_index += m_forecastList.at(i).uv_index;
        vis += m_forecastList.at(i).vis;
        wind_deg += m_forecastList.at(i).wind_deg;
        wind_dir += m_forecastList.at(i).wind_dir;
        wind_sc += m_forecastList.at(i).wind_sc;
        wind_spd += m_forecastList.at(i).wind_spd;
        if (i != forecastCount-1) {
            forcast_date += ",";
            cond_code_d += ",";
            cond_code_n += ",";
            cond_txt_d += ",";
            cond_txt_n += ",";
            hum += ",";
            mr += ",";
            ms += ",";
            pcpn += ",";
            pop += ",";
            pres += ",";
            sr += ",";
            ss += ",";
            tmp_max += ",";
            tmp_min += ",";
            uv_index += ",";
            vis += ",";
            wind_deg += ",";
            wind_dir += ",";
            wind_sc += ",";
            wind_spd += ",";
        }
    }
    set->setValue("forcast_date", forcast_date);
    set->setValue("cond_code_d", cond_code_d);
    set->setValue("cond_txt_d", cond_txt_d);
    set->setValue("cond_code_n", cond_code_n);
    set->setValue("cond_txt_n", cond_txt_n);
    set->setValue("hum", hum);
//    set->setValue("mr_ms", mr_ms);
    set->setValue("mr", mr);
    set->setValue("ms", ms);
    set->setValue("pcpn", pcpn);
    set->setValue("pop", pop);
    set->setValue("pres", pres);
//    set->setValue("sr_ss", sr_ss);
    set->setValue("sr", sr);
    set->setValue("ss", ss);
    set->setValue("tmp_max", tmp_max);
    set->setValue("tmp_min", tmp_min);
    set->setValue("uv_index", uv_index);
    set->setValue("vis", vis);
    set->setValue("wind_deg", wind_deg);
    set->setValue("wind_dir", wind_dir);
    set->setValue("wind_sc", wind_sc);
    set->setValue("wind_spd", wind_spd);
    set->endGroup();


    set->beginGroup("Lifestyle");
    set->setValue("air_brf", m_lifestyle.air_brf);
    set->setValue("air_txt", m_lifestyle.air_txt);
    set->setValue("comf_brf", m_lifestyle.comf_brf);
    set->setValue("comf_txt", m_lifestyle.comf_txt);
    set->setValue("cw_brf", m_lifestyle.cw_brf);
    set->setValue("cw_txt", m_lifestyle.cw_txt);
    set->setValue("drsg_brf", m_lifestyle.drsg_brf);
    set->setValue("drsg_txt", m_lifestyle.drsg_txt);
    set->setValue("flu_brf", m_lifestyle.flu_brf);
    set->setValue("flu_txt", m_lifestyle.flu_txt);
    set->setValue("sport_brf", m_lifestyle.sport_brf);
    set->setValue("sport_txt", m_lifestyle.sport_txt);
    set->setValue("trav_brf", m_lifestyle.trav_brf);
    set->setValue("trav_txt", m_lifestyle.trav_txt);
    set->setValue("uv_brf", m_lifestyle.uv_brf);
    set->setValue("uv_txt", m_lifestyle.uv_txt);
    set->endGroup();

    set->sync();
}

void Preferences::load()
{
    QSettings *set = m_settings;

    set->beginGroup("City");
    m_currentCityId = set->value("current_city_id", m_currentCityId).toString();
    if (m_currentCityId.isEmpty()) {
        m_currentCityId = "101250101";
        m_currentCity = "长沙";

        City city;
        city.id = m_currentCityId;
        city.name = m_currentCity;
    }

    m_currentCity = set->value("current_city", m_currentCity).toString();

    int cityCount = set->beginReadArray("city_list");
    if (cityCount == 0) {
        set->setArrayIndex(0);
        City city;
        city.id = m_currentCityId;
        city.name = m_currentCity;
        m_cities.append(city);

        City city1;
        city1.id = "101010100";
        city1.name = "北京";
        m_cities.append(city1);

        City city2;
        city2.id = "101020100";
        city2.name = "上海";
        m_cities.append(city2);

        City city3;
        city3.id = "101280101";
        city3.name = "广州";
        m_cities.append(city3);

        City city4;
        city4.id = "101280601";
        city4.name = "深圳";
        m_cities.append(city4);
    }
    else {
        for (int i = 0; i < cityCount; ++i) {
            set->setArrayIndex(i);
            City city;
            city.id = set->value("id").toString();
            city.name = set->value("name").toString();
            m_cities.append(city);
        }
    }

    this->reloadCityList();
    set->endArray();

    m_updateFrequency = set->value("update_frequency", m_updateFrequency).toInt();
    if (m_updateFrequency > 60 || m_updateFrequency < 15) {
        m_updateFrequency = 30;
    }

    m_opacity = set->value("opacity", m_opacity).toInt();
    if (m_opacity > 100 || m_opacity < 60) {
        m_opacity = 100;
    }
    set->endGroup();


    set->beginGroup("Weather");
    m_observerWeather.updatetime = set->value("updatetime", m_observerWeather.updatetime).toString();
    m_observerWeather.air = set->value("air", m_observerWeather.air).toString();
    m_observerWeather.cloud = set->value("cloud", m_observerWeather.cloud).toString();
    m_observerWeather.cond_code = set->value("cond_code", m_observerWeather.cond_code).toString();
    m_observerWeather.cond_txt = set->value("cond_txt", m_observerWeather.cond_txt).toString();
    m_observerWeather.fl = set->value("fl", m_observerWeather.fl).toString();
    m_observerWeather.hum = set->value("hum", m_observerWeather.hum).toString();
    m_observerWeather.pcpn = set->value("pcpn", m_observerWeather.pcpn).toString();
    m_observerWeather.pres = set->value("pres", m_observerWeather.pres).toString();
    m_observerWeather.tmp = set->value("tmp", m_observerWeather.tmp).toString();
    m_observerWeather.vis = set->value("vis", m_observerWeather.vis).toString();
    m_observerWeather.wind_deg = set->value("wind_deg", m_observerWeather.wind_deg).toString();
    m_observerWeather.wind_dir = set->value("wind_dir", m_observerWeather.wind_dir).toString();
    m_observerWeather.wind_sc = set->value("wind_sc", m_observerWeather.wind_sc).toString();
    m_observerWeather.wind_spd = set->value("wind_spd", m_observerWeather.wind_spd).toString();
    set->endGroup();


    set->beginGroup("Forecast");
    QStringList forcast_date = set->value("forcast_date").toStringList();
    QStringList cond_code_d = set->value("cond_code_d").toStringList();
    QStringList cond_txt_d = set->value("cond_txt_d").toStringList();
    QStringList cond_code_n = set->value("cond_code_n").toStringList();
    QStringList cond_txt_n = set->value("cond_txt_n").toStringList();
    QStringList hum = set->value("hum").toStringList();
    QStringList mr = set->value("mr").toStringList();
    QStringList ms = set->value("ms").toStringList();
    QStringList pcpn = set->value("pcpn").toStringList();
    QStringList pop = set->value("pop").toStringList();
    QStringList pres = set->value("pres").toStringList();
    QStringList sr = set->value("sr").toStringList();
    QStringList ss = set->value("ss").toStringList();
    QStringList tmp_max = set->value("tmp_max").toStringList();
    QStringList tmp_min = set->value("tmp_min").toStringList();
    QStringList uv_index = set->value("uv_index").toStringList();
    QStringList vis = set->value("vis").toStringList();
    QStringList wind_deg = set->value("wind_deg").toStringList();
    QStringList wind_dir = set->value("wind_dir").toStringList();
    QStringList wind_sc = set->value("wind_sc").toStringList();
    QStringList wind_spd = set->value("wind_spd").toStringList();

    int forecastCount = forcast_date.length();
    if ((forecastCount == cond_code_d.length()) && (forecastCount == cond_txt_d.length()) &&
            (forecastCount == cond_code_n.length()) && (forecastCount == cond_txt_n.length()) &&
            (forecastCount == hum.length()) && (forecastCount == mr.length()) &&
            (forecastCount == ms.length()) && (forecastCount == pcpn.length()) &&
            (forecastCount == pop.length()) && (forecastCount == pres.length()) &&
            (forecastCount == sr.length()) && (forecastCount == ss.length()) &&
            (forecastCount == tmp_max.length()) && (forecastCount == tmp_min.length()) &&
            (forecastCount == uv_index.length()) && (forecastCount == vis.length()) &&
            (forecastCount == wind_deg.length()) && (forecastCount == wind_dir.length()) &&
            (forecastCount == wind_sc.length()) && (forecastCount == wind_spd.length())) {
        for (int i=0; i< forecastCount; i++) {
            ForecastWeather forecast;
            forecast.forcast_date = forcast_date.at(i);
            forecast.cond_code_d = cond_code_d.at(i);
            forecast.cond_txt_d = cond_txt_d.at(i);
            forecast.cond_code_n = cond_code_n.at(i);
            forecast.cond_txt_n = cond_txt_n.at(i);
            forecast.hum = hum.at(i);
            forecast.mr = mr.at(i);
            forecast.ms = ms.at(i);
            forecast.pcpn = pcpn.at(i);
            forecast.pop = pop.at(i);
            forecast.pres = pres.at(i);
            forecast.sr = sr.at(i);
            forecast.ss = ss.at(i);
            forecast.tmp_max = tmp_max.at(i);
            forecast.tmp_min = tmp_min.at(i);
            forecast.uv_index = uv_index.at(i);
            forecast.vis = vis.at(i);
            forecast.wind_deg = wind_deg.at(i);
            forecast.wind_dir = wind_dir.at(i);
            forecast.wind_sc = wind_sc.at(i);
            forecast.wind_spd = wind_spd.at(i);
            m_forecastList.append(forecast);
        }
    }
    else {
        m_forecastList.clear();
    }
    set->endGroup();


    set->beginGroup("Lifestyle");
    m_lifestyle.air_brf = set->value("air_brf", m_lifestyle.air_brf).toString();
    m_lifestyle.air_txt = set->value("air_txt", m_lifestyle.air_txt).toString();
    m_lifestyle.comf_brf = set->value("comf_brf", m_lifestyle.comf_brf).toString();
    m_lifestyle.comf_txt = set->value("comf_txt", m_lifestyle.comf_txt).toString();
    m_lifestyle.cw_brf = set->value("cw_brf", m_lifestyle.cw_brf).toString();
    m_lifestyle.cw_txt = set->value("cw_txt", m_lifestyle.cw_txt).toString();
    m_lifestyle.drsg_brf = set->value("drsg_brf", m_lifestyle.drsg_brf).toString();
    m_lifestyle.drsg_txt = set->value("drsg_txt", m_lifestyle.drsg_txt).toString();
    m_lifestyle.flu_brf = set->value("flu_brf", m_lifestyle.flu_brf).toString();
    m_lifestyle.flu_txt = set->value("flu_txt", m_lifestyle.flu_txt).toString();
    m_lifestyle.sport_brf = set->value("sport_brf", m_lifestyle.sport_brf).toString();
    m_lifestyle.sport_txt = set->value("sport_txt", m_lifestyle.sport_txt).toString();
    m_lifestyle.trav_brf = set->value("trav_brf", m_lifestyle.trav_brf).toString();
    m_lifestyle.trav_txt = set->value("trav_txt", m_lifestyle.trav_txt).toString();
    m_lifestyle.uv_brf = set->value("uv_brf", m_lifestyle.uv_brf).toString();
    m_lifestyle.uv_txt = set->value("uv_txt", m_lifestyle.uv_txt).toString();
    set->endGroup();
}

//void Preferences::addCityToStringList(QString name)
//{
//    int pos = m_cityList.indexOf(name);
//    if (pos != -1)
//        m_cityList.removeAt(pos);
//    m_cityList.prepend(name);

//    if (m_cityList.count() > m_maxCityItems)
//        m_cityList.removeLast();
//}

//void Preferences::removeCityFromStringList(QString name)
//{
//    int pos = m_cityList.indexOf(name);
//    if (pos != -1)
//        m_cityList.removeAt(pos);
//}

//QString Preferences::cityName(int index)
//{
//    QString name = m_cityList[index];

//    return name;
//}

//void Preferences::loadCityesToStringList(QStringList list)
//{
//    m_cityList.clear();

//    int max = list.count();
//    if (m_maxCityItems < max)
//        max = m_maxCityItems;

//    for (int n = 0; n < max; n++) {
//        m_cityList.append(list[n]);
//    }
//}

void Preferences::reloadCityList()
{
    m_cityList.clear();
    for (int i = 0; i < m_cities.size(); ++i) {
        m_cityList.append(m_cities.at(i).name);
    }
}

QStringList Preferences::getCitiesList()
{
    return m_cityList;
}

int Preferences::citiesCount()
{
    return m_cityList.count();
}

void Preferences::setCurrentCityIdAndName(const QString &cityName/*int index*/)
{
    this->m_currentCity = cityName;
    for (int i = 0; i < m_cities.size(); ++i) {
        if (m_cities.at(i).name == cityName) {
            this->m_currentCityId = m_cities.at(i).id;
            break;
        }
    }
}

void Preferences::setDefaultCity()
{
    if (m_cities.size() > 0) {
        this->m_currentCityId = m_cities.at(0).id;
        this->m_currentCity = m_cities.at(0).name;
    }
}

void Preferences::addCityInfoToPref(City city)
{
    if (m_cities.size() == 0) {
        m_cities.append(city);
    }
    else {
        int pos = m_cities.indexOf(city);
        if (pos != -1) {
            m_cities.removeAt(pos);
        }
        m_cities.prepend(city);
        if (m_cities.count() > m_maxCityItems)
            m_cities.removeLast();
    }

    this->reloadCityList();
}

void Preferences::removeCityInfoFromPref(const QString &id, bool isActive)
{
    for (int i = 0; i < m_cities.size(); ++i) {
        City city;
        city.id = m_cities.at(i).id;
        city.name = m_cities.at(i).name;
        int pos = m_cities.indexOf(city);
        if (pos != -1 && city.id == id) {
            m_cities.removeAt(pos);
            this->reloadCityList();
            break;
        }
    }

    if (isActive) {
        this->setDefaultCity();
    }
}

bool Preferences::isCityIdExist(const QString &id)
{
    bool b = false;

    for (int i = 0; i < m_cities.size(); ++i) {
        if (id == m_cities.at(i).id) {
            b = true;
            break;
        }
    }

    return b;
}

bool Preferences::isCitiesCountOverMax()
{
    bool b = false;
    if (m_cities.count() >= 10) {
        b = true;
        return b;
    }

    return b;
}

void Preferences::resetCurrentCityNameById(const QString &id)
{
    this->m_currentCityId = id;
    for (int i = 0; i < m_cities.size(); ++i) {
        if (m_cities.at(i).id == id) {
            this->m_currentCity = m_cities.at(i).name;
            break;
        }
    }
}
