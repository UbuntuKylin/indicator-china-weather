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

#include "cityaddsearchbox.h"

#include <QPainter>
#include <QGuiApplication>
#include <QDebug>

CityAddSearchBox::CityAddSearchBox(QWidget* parent)
    :QLineEdit(parent)
    , m_searchText("搜索")
    , m_searchPixmap(QPixmap(":/res/control_icons/search_city.png"))
{
    this->setFixedSize(470, 30);
    this->setStyleSheet("QLineEdit{padding-left:31px;"
                        "background:rgba(248,248,248,1);"
                        "border:1px solid rgba(61,107,229,1);"
                        "border-radius:4px;"
                        "font-size:14px;"
                        "color:rgba(0,0,0,1);}");
    this->setFocusPolicy(Qt::ClickFocus);
}

void CityAddSearchBox::paintEvent(QPaintEvent *event)
{
    QLineEdit::paintEvent(event);

    if (!this->hasFocus() && this->text().isEmpty()) {
        // show icon and tips when the focus is not on searchbox
        QPainter painter(this);
        painter.setOpacity(0.5);

        QFont curFont = painter.font(); //QGuiApplication::font();
        curFont.setPixelSize(14); //font.setPointSize(14);
        QPen pen(Qt::black);
        painter.setPen(pen);
        painter.setFont(curFont);

        QRect iconRect(QPoint(210, 7), m_searchPixmap.size()/qApp->devicePixelRatio());
        painter.drawPixmap(iconRect, m_searchPixmap); //add icon in searchbox

        QRect textRect(235, 3, 30, 28);
        painter.drawText(textRect, m_searchText); //add text in searchbox
    } else {
        // show cursor when the focus is on searchbox, and set the icon left
        QPainter painter(this);
        painter.setOpacity(0.5);

        QRect iconRect(QPoint(9, 7), m_searchPixmap.size()/qApp->devicePixelRatio());
        painter.drawPixmap(iconRect, m_searchPixmap);
    }
}
//主题适配
void CityAddSearchBox::ThemeCitySearchBox(QString str)
{
    if("ukui-dark" == str || "ukui-black" == str)
    {
   this->setStyleSheet("QLineEdit{padding-left:31px;"
                       "background:rgba(72,72,76,1);"
                       "border:1px solid rgba(61,107,229,1);"
                       "border-radius:4px;"
                       "font-size:14px;"
                       "color:rgba(0,0,0,1);}");
}
    else if("ukui-default" == str || "ukui-white" == str || "ukui-light" == str)
    {
        this->setStyleSheet("QLineEdit{padding-left:31px;"
                            "background:rgba(248,248,248,1);"
                            "border:1px solid rgba(61,107,229,1);"
                            "border-radius:4px;"
                            "font-size:14px;"
                            "color:rgba(0,0,0,1);}");
    }
}
