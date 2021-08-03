/*
 * Copyright (C) 2020, KylinSoft Co., Ltd.
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

#ifndef LEFTUPSEARCHVIEW_H
#define LEFTUPSEARCHVIEW_H

#include <QListView>
#include <QDebug>
#include "data.h"
class WeatherWorker;


class LeftUpSearchView : public QListView
{
    Q_OBJECT
public:
    explicit LeftUpSearchView(QWidget *parent = 0);
    ~LeftUpSearchView();

    void requestWeatherData(QString cityId);
    void ThemeLeftUpSearchView(QString str);

protected:
    // handle click event of searchview, send signal when mouse release.
    void mouseReleaseEvent(QMouseEvent *e) override;
public slots:
    void dealSearchBoxKeyPress(QString str);
signals:
    void requestSetCityName(QString cityName); //点击选中列表中某一个城市后发出，通知更改左上角城市名
    //void requestGetWeatherData(QString id); //发送出去用于获取数据并在主界面显示
    void requestSetNewCityWeather(QString id); //发送出去用于获取数据并在主界面显示

private:
    QString m_cityid;
    void send();
};

#endif // LEFTUPSEARCHVIEW_H
