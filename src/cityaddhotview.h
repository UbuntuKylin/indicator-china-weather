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

#ifndef CITYADDHOTVIEW_H
#define CITYADDHOTVIEW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include "hotcity.h"
class CityAddHotView : public QWidget
{
    Q_OBJECT

public:
    explicit CityAddHotView(QWidget *parent = 0);
    void ThemeCityHotView(QString str);
public slots:

signals:
    void setHotCity(QString code);

private:
    QLabel *m_addCityhot;
//    QHBoxLayout *m_citylayout;
    HotCity *m_addcity11;
    HotCity *m_addcity12;
    HotCity *m_addcity13;
    HotCity *m_addcity14;
    HotCity *m_addcity15;
    HotCity *m_addcity16;
    HotCity *m_addcity17;
    HotCity *m_addcity18;
//    QHBoxLayout *m_citylayout1;
    HotCity *m_addcity21;
    HotCity *m_addcity22;
    HotCity *m_addcity23;
    HotCity *m_addcity24;
    HotCity *m_addcity25;
    HotCity *m_addcity26;
    HotCity *m_addcity27;
    HotCity *m_addcity28;
//    QHBoxLayout *m_citylayout2;
    HotCity *m_addcity31;
    HotCity *m_addcity32;
    HotCity *m_addcity33;
    HotCity *m_addcity34;
    HotCity *m_addcity35;
    HotCity *m_addcity36;
    HotCity *m_addcity37;

};

#endif // CITYADDHOTVIEW_H
