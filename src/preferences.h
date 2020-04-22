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
    bool isCitiesCountOverMax(int maxNum);
    void setDefaultCity();

    QString m_currentCity;
    QString m_currentCityId;
    int m_updateFrequency;
    int m_opacity;
    bool m_serverNotify;

    /*struct Weather_settings {
        QString id;
        QString city;
        QString updatetime;
        QString air;//空气质量
        QString cloud;//云量
        QString cond_code;//实况天气状况代码 	100
        QString cond_txt;//实况天气状况描述 晴
        QString fl;//体感温度，默认单位：摄氏度 	23
        QString hum;//相对湿度 	40
        QString pcpn;//降水量(毫米 mm) 	0
        QString pres;//大气压强(百帕 hPa) 	1020
        QString tmp;//温度，默认单位：摄氏度℃ 	21
        QString vis;//能见度，默认单位：公里 km 	10
        QString wind_deg;//风向360角度 	305
        QString wind_dir;//风向 	西北
        QString wind_sc;//风力 	3 (0:静风  1:1级风)
        QString wind_spd;//风速，公里/小时 km/h 	15
    } weather;*/

    /*struct Forecast_settings {
        QString cond_code_d;//白天天气状况代码 	100
        QString cond_code_n;//晚间天气状况代码 	100
        QString cond_txt_d;//白天天气状况描述 	晴
        QString cond_txt_n;//晚间天气状况描述 	晴
        QString forcast_date;//预报日期 	2013-12-30
        QString hum;//相对湿度 	37
        QString mr_ms;//月升时间 	04:47   月落时间 	14:59
        QString pcpn;//降水量 	0
        QString pop;//降水概率 	0
        QString pres;//大气压强 	1018
        QString sr_ss;//日出时间 	07:36   日落时间 	16:58
        QString tmp_max;//最高温度 	4
        QString tmp_min;//最低温度 	-5
        QString uv_index;//紫外线强度指数 	3
        QString vis;//能见度，单位：公里 	10
        QString wind_deg;//风向360角度 	310
        QString wind_dir;//风向 	西北风
        QString wind_sc;//风力 	1-2 (级风)
        QString wind_spd;//风速，公里/小时 	14
    } forecast0, forecast1, forecast2;*/

    /*struct LifeStyle_settings {
        QString air_brf;
        QString air_txt;
        QString comf_brf;
        QString comf_txt;
        QString cw_brf;
        QString cw_txt;
        QString drsg_brf;
        QString drsg_txt;
        QString flu_brf;
        QString flu_txt;
        QString sport_brf;
        QString sport_txt;
        QString trav_brf;
        QString trav_txt;
        QString uv_brf;
        QString uv_txt;
    } lifestyle;*/

    ObserveWeather weather;
    ForecastWeather forecast0, forecast1, forecast2;
    LifeStyle lifestyle;
    Air air;

    int m_maxCityItems;
    QStringList m_cityList;
    //QStringList m_cityIdList;
    QList<City> m_cities;
};

#endif // PREFERENCES_H
