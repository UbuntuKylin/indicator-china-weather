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

#ifndef HOTCITY_H
#define HOTCITY_H
#include <QPushButton>

class HotCity : public QPushButton
{
    Q_OBJECT

public:
    HotCity(QWidget *parent,QString name, QString code);
    void onThemeChanged(QString str);
signals:
    void hotCityClick(QString);

public slots:
    void onHotCityClick();

private:
    QString cityCode="";
};

#endif // HOTCITY_H
