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

#ifndef LEFTUPSEARCHBOX_H
#define LEFTUPSEARCHBOX_H

#include <QLineEdit>
#include <QMouseEvent>

class LeftUpSearchBox : public QLineEdit
{
    Q_OBJECT

public:
    explicit LeftUpSearchBox(QWidget* parent = 0);
    void ThemeLeftUpSearchBox(QString str);
signals:
    void lineEditKeyEvent(QString str);
protected:
    // set the display style of the searchbox.
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    bool event(QEvent *event) Q_DECL_OVERRIDE;

private:
    QString m_searchText;
    QPixmap m_searchPixmap;
    bool isFocus = false;
};

#endif // LEFTUPSEARCHBOX_H
