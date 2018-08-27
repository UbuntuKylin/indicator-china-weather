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

#ifndef DATA_H
#define DATA_H

#include <QObject>

struct LocationData {
    QString id;
    QString country;
    QString province;
    QString city;
    QString county;
    QString pinyin;
};

inline bool operator ==(const LocationData &data, const LocationData &that)
{
    return data.id == that.id || (data.county == that.county && data.city == that.city);
}

class CitySettingData
{
public:
    bool active;

    QString id;
    QString name;
    QString temperature;
    QString icon;

public:
    void setActive(bool b) { this->active = b; }
};

Q_DECLARE_METATYPE(LocationData)
Q_DECLARE_METATYPE(CitySettingData)

#endif // DATA_H
