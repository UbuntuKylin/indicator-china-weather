/*
 * Copyright (C) 2013 ~ 2019 National University of Defense Technology(NUDT) & Tianjin Kylin Ltd.
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
    set->setValue("updatetime", weather.updatetime);
    set->setValue("air", weather.air);
    set->setValue("cloud", weather.cloud);
    set->setValue("cond_code", weather.cond_code);
    set->setValue("cond_txt", weather.cond_txt);
    set->setValue("fl", weather.fl);
    set->setValue("hum", weather.hum);
    set->setValue("pcpn", weather.pcpn);
    set->setValue("pres", weather.pres);
    set->setValue("tmp", weather.tmp);
    set->setValue("vis", weather.vis);
    set->setValue("wind_deg", weather.wind_deg);
    set->setValue("wind_dir", weather.wind_dir);
    set->setValue("wind_sc", weather.wind_sc);
    set->setValue("wind_spd", weather.wind_spd);
    set->endGroup();


    set->beginGroup("Forecast");
    QStringList result;
    result.append(forecast0.forcast_date);
    result.append(forecast1.forcast_date);
    result.append(forecast2.forcast_date);
    set->setValue("forcast_date", result);

    result.clear();
    result.append(forecast0.cond_code_d);
    result.append(forecast1.cond_code_d);
    result.append(forecast2.cond_code_d);
    set->setValue("cond_code_d", result);

    result.clear();
    result.append(forecast0.cond_txt_d);
    result.append(forecast1.cond_txt_d);
    result.append(forecast2.cond_txt_d);
    set->setValue("cond_txt_d", result);

    result.clear();
    result.append(forecast0.cond_code_n);
    result.append(forecast1.cond_code_n);
    result.append(forecast2.cond_code_n);
    set->setValue("cond_code_n", result);

    result.clear();
    result.append(forecast0.cond_txt_n);
    result.append(forecast1.cond_txt_n);
    result.append(forecast2.cond_txt_n);
    set->setValue("cond_txt_n", result);

    result.clear();
    result.append(forecast0.hum);
    result.append(forecast1.hum);
    result.append(forecast2.hum);
    set->setValue("hum", result);

    result.clear();
    result.append(forecast0.mr_ms);
    result.append(forecast1.mr_ms);
    result.append(forecast2.mr_ms);
    set->setValue("mr_ms", result);

    result.clear();
    result.append(forecast0.pcpn);
    result.append(forecast1.pcpn);
    result.append(forecast2.pcpn);
    set->setValue("pcpn", result);

    result.clear();
    result.append(forecast0.pop);
    result.append(forecast1.pop);
    result.append(forecast2.pop);
    set->setValue("pop", result);

    result.clear();
    result.append(forecast0.pres);
    result.append(forecast1.pres);
    result.append(forecast2.pres);
    set->setValue("pres", result);

    result.clear();
    result.append(forecast0.sr_ss);
    result.append(forecast1.sr_ss);
    result.append(forecast2.sr_ss);
    set->setValue("sr_ss", result);

    result.clear();
    result.append(forecast0.tmp_max);
    result.append(forecast1.tmp_max);
    result.append(forecast2.tmp_max);
    set->setValue("tmp_max", result);

    result.clear();
    result.append(forecast0.tmp_min);
    result.append(forecast1.tmp_min);
    result.append(forecast2.tmp_min);
    set->setValue("tmp_min", result);

    result.clear();
    result.append(forecast0.uv_index);
    result.append(forecast1.uv_index);
    result.append(forecast2.uv_index);
    set->setValue("uv_index", result);

    result.clear();
    result.append(forecast0.vis);
    result.append(forecast1.vis);
    result.append(forecast2.vis);
    set->setValue("vis", result);

    result.clear();
    result.append(forecast0.wind_deg);
    result.append(forecast1.wind_deg);
    result.append(forecast2.wind_deg);
    set->setValue("wind_deg", result);

    result.clear();
    result.append(forecast0.wind_dir);
    result.append(forecast1.wind_dir);
    result.append(forecast2.wind_dir);
    set->setValue("wind_dir", result);

    result.clear();
    result.append(forecast0.wind_sc);
    result.append(forecast1.wind_sc);
    result.append(forecast2.wind_sc);
    set->setValue("wind_sc", result);

    result.clear();
    result.append(forecast0.wind_spd);
    result.append(forecast1.wind_spd);
    result.append(forecast2.wind_spd);
    set->setValue("wind_spd", result);
    result.clear();
    set->endGroup();


    set->beginGroup("Lifestyle");
    set->setValue("air_brf", lifestyle.air_brf);
    set->setValue("air_txt", lifestyle.air_txt);
    set->setValue("comf_brf", lifestyle.comf_brf);
    set->setValue("comf_txt", lifestyle.comf_txt);
    set->setValue("cw_brf", lifestyle.cw_brf);
    set->setValue("cw_txt", lifestyle.cw_txt);
    set->setValue("drsg_brf", lifestyle.drsg_brf);
    set->setValue("drsg_txt", lifestyle.drsg_txt);
    set->setValue("flu_brf", lifestyle.flu_brf);
    set->setValue("flu_txt", lifestyle.flu_txt);
    set->setValue("sport_brf", lifestyle.sport_brf);
    set->setValue("sport_txt", lifestyle.sport_txt);
    set->setValue("trav_brf", lifestyle.trav_brf);
    set->setValue("trav_txt", lifestyle.trav_txt);
    set->setValue("uv_brf", lifestyle.uv_brf);
    set->setValue("uv_txt", lifestyle.uv_txt);
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
        //this->addCityToStringList("长沙");

        City city;
        city.id = m_currentCityId;
        city.name = m_currentCity;

        //this->addCityInfoToPref(city);
    }

    m_currentCity = set->value("current_city", m_currentCity).toString();

    //qDebug() << "init load:" << m_currentCityId << m_currentCity;
    //this->loadCityesToStringList(set->value("city_list2", this->getCitiesList()).toStringList());

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
    }
    else {
        for (int i = 0; i < cityCount; ++i) {
            set->setArrayIndex(i);
            City city;
            city.id = set->value("id").toString();
            city.name = set->value("name").toString();
            //qDebug() << "load:" <<city.id << city.name;
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
    weather.updatetime = set->value("updatetime", weather.updatetime).toString();
    weather.air = set->value("air", weather.air).toString();
    weather.cloud = set->value("cloud", weather.cloud).toString();
    weather.cond_code = set->value("cond_code", weather.cond_code).toString();
    weather.cond_txt = set->value("cond_txt", weather.cond_txt).toString();
    weather.fl = set->value("fl", weather.fl).toString();
    weather.hum = set->value("hum", weather.hum).toString();
    weather.pcpn = set->value("pcpn", weather.pcpn).toString();
    weather.pres = set->value("pres", weather.pres).toString();
    weather.tmp = set->value("tmp", weather.tmp).toString();
    weather.vis = set->value("vis", weather.vis).toString();
    weather.wind_deg = set->value("wind_deg", weather.wind_deg).toString();
    weather.wind_dir = set->value("wind_dir", weather.wind_dir).toString();
    weather.wind_sc = set->value("wind_sc", weather.wind_sc).toString();
    weather.wind_spd = set->value("wind_spd", weather.wind_spd).toString();
    set->endGroup();


    set->beginGroup("Forecast");
    QStringList resultlist = set->value("forcast_date").toStringList();
    if (resultlist.length() == 3) {
        forecast0.forcast_date = resultlist.at(0);
        forecast1.forcast_date = resultlist.at(1);
        forecast2.forcast_date = resultlist.at(2);
    }

    resultlist.clear();
    resultlist = set->value("cond_code_d").toStringList();
    if (resultlist.length() == 3) {
        forecast0.cond_code_d = resultlist.at(0);
        forecast1.cond_code_d = resultlist.at(1);
        forecast2.cond_code_d = resultlist.at(2);
    }

    resultlist.clear();
    resultlist = set->value("cond_txt_d").toStringList();
    if (resultlist.length() == 3) {
        forecast0.cond_txt_d = resultlist.at(0);
        forecast1.cond_txt_d = resultlist.at(1);
        forecast2.cond_txt_d = resultlist.at(2);
    }

    resultlist.clear();
    resultlist = set->value("cond_code_n").toStringList();
    if (resultlist.length() == 3) {
        forecast0.cond_code_n = resultlist.at(0);
        forecast1.cond_code_n = resultlist.at(1);
        forecast2.cond_code_n = resultlist.at(2);
    }

    resultlist.clear();
    resultlist = set->value("cond_txt_n").toStringList();
    if (resultlist.length() == 3) {
        forecast0.cond_txt_n = resultlist.at(0);
        forecast1.cond_txt_n = resultlist.at(1);
        forecast2.cond_txt_n = resultlist.at(2);
    }

    resultlist.clear();
    resultlist = set->value("hum").toStringList();
    if (resultlist.length() == 3) {
        forecast0.hum = resultlist.at(0);
        forecast1.hum = resultlist.at(1);
        forecast2.hum = resultlist.at(2);
    }

    resultlist.clear();
    resultlist = set->value("mr_ms").toStringList();
    if (resultlist.length() == 3) {
        forecast0.mr_ms = resultlist.at(0);
        forecast1.mr_ms = resultlist.at(1);
        forecast2.mr_ms = resultlist.at(2);
    }

    resultlist.clear();
    resultlist = set->value("pcpn").toStringList();
    if (resultlist.length() == 3) {
        forecast0.pcpn = resultlist.at(0);
        forecast1.pcpn = resultlist.at(1);
        forecast2.pcpn = resultlist.at(2);
    }

    resultlist.clear();
    resultlist = set->value("pop").toStringList();
    if (resultlist.length() == 3) {
        forecast0.pop = resultlist.at(0);
        forecast1.pop = resultlist.at(1);
        forecast2.pop = resultlist.at(2);
    }

    resultlist.clear();
    resultlist = set->value("pres").toStringList();
    if (resultlist.length() == 3) {
        forecast0.pres = resultlist.at(0);
        forecast1.pres = resultlist.at(1);
        forecast2.pres = resultlist.at(2);
    }

    resultlist.clear();
    resultlist = set->value("sr_ss").toStringList();
    if (resultlist.length() == 3) {
        forecast0.sr_ss = resultlist.at(0);
        forecast1.sr_ss = resultlist.at(1);
        forecast2.sr_ss = resultlist.at(2);
    }

    resultlist.clear();
    resultlist = set->value("tmp_max").toStringList();
    if (resultlist.length() == 3) {
        forecast0.tmp_max = resultlist.at(0);
        forecast1.tmp_max = resultlist.at(1);
        forecast2.tmp_max = resultlist.at(2);
    }

    resultlist.clear();
    resultlist = set->value("tmp_min").toStringList();
    if (resultlist.length() == 3) {
        forecast0.tmp_min = resultlist.at(0);
        forecast1.tmp_min = resultlist.at(1);
        forecast2.tmp_min = resultlist.at(2);
    }

    resultlist.clear();
    resultlist = set->value("uv_index").toStringList();
    if (resultlist.length() == 3) {
        forecast0.uv_index = resultlist.at(0);
        forecast1.uv_index = resultlist.at(1);
        forecast2.uv_index = resultlist.at(2);
    }

    resultlist.clear();
    resultlist = set->value("vis").toStringList();
    if (resultlist.length() == 3) {
        forecast0.vis = resultlist.at(0);
        forecast1.vis = resultlist.at(1);
        forecast2.vis = resultlist.at(2);
    }

    resultlist.clear();
    resultlist = set->value("wind_deg").toStringList();
    if (resultlist.length() == 3) {
        forecast0.wind_deg = resultlist.at(0);
        forecast1.wind_deg = resultlist.at(1);
        forecast2.wind_deg = resultlist.at(2);
    }

    resultlist.clear();
    resultlist = set->value("wind_dir").toStringList();
    if (resultlist.length() == 3) {
        forecast0.wind_dir = resultlist.at(0);
        forecast1.wind_dir = resultlist.at(1);
        forecast2.wind_dir = resultlist.at(2);
    }

    resultlist.clear();
    resultlist = set->value("wind_sc").toStringList();
    if (resultlist.length() == 3) {
        forecast0.wind_sc = resultlist.at(0);
        forecast1.wind_sc = resultlist.at(1);
        forecast2.wind_sc = resultlist.at(2);
    }

    resultlist.clear();
    resultlist = set->value("wind_spd").toStringList();
    if (resultlist.length() == 3) {
        forecast0.wind_spd = resultlist.at(0);
        forecast1.wind_spd = resultlist.at(1);
        forecast2.wind_spd = resultlist.at(2);
    }
    resultlist.clear();
    set->endGroup();


    set->beginGroup("Lifestyle");
    lifestyle.air_brf = set->value("air_brf", lifestyle.air_brf).toString();
    lifestyle.air_txt = set->value("air_txt", lifestyle.air_txt).toString();
    lifestyle.comf_brf = set->value("comf_brf", lifestyle.comf_brf).toString();
    lifestyle.comf_txt = set->value("comf_txt", lifestyle.comf_txt).toString();
    lifestyle.cw_brf = set->value("cw_brf", lifestyle.cw_brf).toString();
    lifestyle.cw_txt = set->value("cw_txt", lifestyle.cw_txt).toString();
    lifestyle.drsg_brf = set->value("drsg_brf", lifestyle.drsg_brf).toString();
    lifestyle.drsg_txt = set->value("drsg_txt", lifestyle.drsg_txt).toString();
    lifestyle.flu_brf = set->value("flu_brf", lifestyle.flu_brf).toString();
    lifestyle.flu_txt = set->value("flu_txt", lifestyle.flu_txt).toString();
    lifestyle.sport_brf = set->value("sport_brf", lifestyle.sport_brf).toString();
    lifestyle.sport_txt = set->value("sport_txt", lifestyle.sport_txt).toString();
    lifestyle.trav_brf = set->value("trav_brf", lifestyle.trav_brf).toString();
    lifestyle.trav_txt = set->value("trav_txt", lifestyle.trav_txt).toString();
    lifestyle.uv_brf = set->value("uv_brf", lifestyle.uv_brf).toString();
    lifestyle.uv_txt = set->value("uv_txt", lifestyle.uv_txt).toString();
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
//    return m_cities.count();
}

void Preferences::setCurrentCityIdAndName(const QString &cityName/*int index*/)
{
//    this->m_currentCityId = m_cities.at(index).id;
//    this->m_currentCity = m_cities.at(index).name;

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
