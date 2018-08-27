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

#ifndef TOOLTIP_H
#define TOOLTIP_H

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

class ToolTip : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(int radius READ radius WRITE setRadius)
    Q_PROPERTY(QBrush background READ background WRITE setBackground)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)

public:
    explicit ToolTip(QWidget *parent = 0);

    int radius() const { return this->_radius; }
    QBrush background() const { return this->_background; }
    QColor borderColor() const { return this->_borderColor; }

    void setRadius(int radius);
    void setBackground(QBrush background);
    void setBorderColor(QColor borderColor);

    void resetData();
    void popupTip(QPoint point);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    int _radius;
    QBrush _background;
    QColor _borderColor;

    QFrame *m_frame = nullptr;
    QLabel *m_weekLabel = nullptr;
    QLabel *m_dateLabel = nullptr;
    QLabel *m_weatherLabel = nullptr;
    QLabel *m_iconLabel = nullptr;
    QLabel *m_tempLabel = nullptr;
    QVBoxLayout *m_layout = nullptr;
};

#endif // TOOLTIP_H
