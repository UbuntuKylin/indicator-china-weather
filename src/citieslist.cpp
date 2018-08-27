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

#include "citieslist.h"

CitiesList::CitiesList()
{
    m_cityList.clear();
    max_items = 10;
}

CitiesList::~CitiesList()
{

}

void CitiesList::clear()
{
    m_cityList.clear();
}

int CitiesList::count()
{
    return m_cityList.count();
}

void CitiesList::addCityToStringList(QString name)
{
    int pos = m_cityList.indexOf(name);
    if (pos != -1)
        m_cityList.removeAt(pos);
    m_cityList.prepend(name);

    if (m_cityList.count() > max_items)
        m_cityList.removeLast();
}

void CitiesList::removeCityFromStringList(QString name)
{
    int pos = m_cityList.indexOf(name);
    if (pos != -1)
        m_cityList.removeAt(pos);
}

QString CitiesList::cityName(int index)
{
    QString name = m_cityList[index];
    return name;
}

void CitiesList::loadCityesToStringList(QStringList list)
{
    m_cityList.clear();

    int max = list.count();
    if (max_items < max)
        max = max_items;

    for (int n = 0; n < max; n++) {
        m_cityList.append(list[n]);
    }
}

QStringList CitiesList::getCitiesList()
{
    return m_cityList;
}

