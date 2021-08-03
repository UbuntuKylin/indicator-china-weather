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

#ifndef CITYADDSEARCHVIEW_H
#define CITYADDSEARCHVIEW_H

#include <QListView>
#include <QDebug>
#include "data.h"


class CityAddSearchView : public QListView
{
    Q_OBJECT
public:
    explicit CityAddSearchView(QWidget *parent = 0);
    ~CityAddSearchView();
    void ThemeCitySearchView(QString str);
protected:
    // handle click event of searchview, send signal when mouse release.
    void mouseReleaseEvent(QMouseEvent *e) override;
public slots:
    void dealSearchBoxKeyPress(QString);

signals:
    void requestClearLineEdit(); //set original state after mouse release
    void requestAddNewCity(QString cityId); //send this signal to mainwindow, show new city weather
private:
    void send();
};

#endif // CITYADDSEARCHVIEW_H
