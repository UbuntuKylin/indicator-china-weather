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

#ifndef CITYADDSEARCHBOX_H
#define CITYADDSEARCHBOX_H

#include <QLineEdit>
#include <QKeyEvent>

class CityAddSearchBox : public QLineEdit
{
    Q_OBJECT

public:
    explicit CityAddSearchBox(QWidget* parent = 0);
    void ThemeCitySearchBox(QString str);
signals:
    void lineEditKeyEvent(QString);
protected:
    // set the display style of the searchbox.
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    bool event(QEvent *) Q_DECL_OVERRIDE;
private:
    QString m_searchText;
    QPixmap m_searchPixmap;
    QString m_ThemeStyle;
    void drawbackground();
};

#endif // CITYADDSEARCHBOX_H
