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

#ifndef AIRWIDGET_H
#define AIRWIDGET_H

#include <QFrame>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QVBoxLayout>

#include "data.h"

class QTimeLine;

class AirWidget : public QFrame
{
    Q_OBJECT

public:
    explicit AirWidget(QWidget *parent = 0);
    ~AirWidget();

    void resetData(const AqiAir &data);
    void showTooltip(const QPoint &position);
    int getWidth();

public slots:
    void animationShow(/*const QString &styleSheet*/);
    void animationHide();

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;

private:
    QTimer *m_showTimer = nullptr;

    QGraphicsOpacityEffect *m_opacityEffect = nullptr;
    QPropertyAnimation *m_animation = nullptr;

    QLabel *m_aqiLabel = nullptr;
    QLabel *m_qltyLabel = nullptr;
    QLabel *m_mainLabel = nullptr;
    QLabel *m_pm25Label = nullptr;
    QLabel *m_pm10Label = nullptr;
    QLabel *m_no2Label = nullptr;
    QLabel *m_so2Label = nullptr;
    QLabel *m_coLabel = nullptr;
    QLabel *m_o3Label = nullptr;

    QVBoxLayout *m_layout = nullptr;
    QBrush m_background;
    QColor m_borderColor;
    int m_radius;
};

#endif // AIRWIDGET_H
