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

#ifndef TOOLTIP_H
#define TOOLTIP_H

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "data.h"

class ToolTip : public QFrame
{
    Q_OBJECT
public:
    explicit ToolTip(QWidget *parent = 0);
    ~ToolTip();

    void resetData(const ForecastWeather &data, const QString &week);
    void popupTip(QPoint point);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    int m_radius;
    QBrush m_background;
    QColor m_borderColor;

    QFrame *m_frame = nullptr;
    QLabel *m_dateLabel = nullptr;
    QLabel *m_dWeatherLabel = nullptr;
    QLabel *m_nWeatherLabel = nullptr;
    QLabel *m_dIconLabel = nullptr;
    QLabel *m_nIconLabel = nullptr;
    QLabel *m_tempLabel = nullptr;
    QLabel *m_mrmsLabel = nullptr;
    QLabel *m_srssLabel = nullptr;
    QLabel *m_humLabel = nullptr;
    QLabel *m_pcpnLabel = nullptr;
    QLabel *m_popLabel = nullptr;
    QLabel *m_presLabel = nullptr;
    QLabel *m_uvIndexLabel = nullptr;
    QLabel *m_visLabel = nullptr;
    QLabel *m_windLabel = nullptr;

    QVBoxLayout *m_layout = nullptr;
};

#endif // TOOLTIP_H
