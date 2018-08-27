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

#ifndef CITYLISTWIDGET_H
#define CITYLISTWIDGET_H

#include "cityitemwidget.h"

#include <QListWidget>

class CitySettingData;

class CityListWidget : public QListWidget
{
    Q_OBJECT

public:
    CityListWidget(QWidget *parent=0);
    ~CityListWidget();

    void resetData();

    void loadCityItems();

    void clearUI();
    void loadItem(const CitySettingData &info);
    void removeItemById(const QString &id);

public slots:
    void onMouseEnter();
    void onRefreshUI();

private:
    QList<CitySettingData> m_dataList;
};

#endif // CITYLISTWIDGET_H
