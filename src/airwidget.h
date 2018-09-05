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

#ifndef AIRWIDGET_H
#define AIRWIDGET_H

#include <QFrame>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QVBoxLayout>

#include "data.h"

class AirWidget : public QFrame
{
    Q_OBJECT

public:
    explicit AirWidget(QWidget *parent = 0);
    ~AirWidget();

    void resetData(const Air &data);

public slots:
    void animationShow(const QString &styleSheet);
    void animationHide();

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    QGraphicsOpacityEffect *m_opacityEffect;
    QPropertyAnimation *m_animation;

    QLabel *m_aqiLabel = nullptr;
    QLabel *m_qltyLabel = nullptr;
    QLabel *m_mainLabel = nullptr;
    QLabel *m_pm25Label = nullptr;
    QLabel *m_pm10Label = nullptr;
    QLabel *m_no2Label = nullptr;
    QLabel *m_so2Label = nullptr;
    QLabel *m_coLabel = nullptr;
    QLabel *m_o3Label = nullptr;

    QTimer *m_timer = nullptr;

    QVBoxLayout *m_layout = nullptr;
};

#endif // AIRWIDGET_H
