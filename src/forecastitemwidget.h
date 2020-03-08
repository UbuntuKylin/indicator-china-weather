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

#ifndef FORECASTITEMWIDGET_H
#define FORECASTITEMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "data.h"

class ToolTip;

class ForecastItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ForecastItemWidget(QWidget *parent = 0);

    void resetForecastData(const ForecastWeather &data, int index);
    void setDefaultData();

    void setDayStyleSheets();
    void setNightStyleSheets();

protected:
    bool event(QEvent *event) Q_DECL_OVERRIDE;
//    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
//    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

private:
    QLabel *m_weekLabel = nullptr;
    QLabel *m_dateLabel = nullptr;
    QLabel *m_weatherLabel = nullptr;
    QLabel *m_iconLabel = nullptr;
    QLabel *m_tempLabel = nullptr;
    QVBoxLayout *m_layout = nullptr;
    ToolTip *m_toolTip = nullptr;
    bool m_isDayOrNight;
};

#endif // FORECASTITEMWIDGET_H
