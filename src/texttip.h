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

#ifndef TEXTTIP_H
#define TEXTTIP_H

#include <QFrame>

class QLabel;

class TextTip : public QFrame
{
    Q_OBJECT
public:
    explicit TextTip(const QString &txt, QWidget *parent = 0);
    ~TextTip();

    void resetTipText(const QString &txt);

protected:
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    int m_radius;
    QBrush m_background;
    QColor m_borderColor;
    QLabel *m_textLabel = nullptr;
    QFrame *m_frame = nullptr;
};

#endif // TEXTTIP_H
