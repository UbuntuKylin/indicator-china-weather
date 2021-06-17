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

#include "leftupsearchbox.h"

#include <QPainter>
#include <QGuiApplication>
#include <QDebug>

LeftUpSearchBox::LeftUpSearchBox(QWidget* parent)
    :QLineEdit(parent)
    , m_searchText(tr("search"))
    , m_searchPixmap(QPixmap(":/res/control_icons/search.png"))
{
    this->setFixedSize(180, 28);
    this->setStyleSheet("QLineEdit{padding-left:31px;background:rgba(255,255,255,0.2);border-radius:4px;color:rgba(255,255,255,0.5);}");
//    this->setStyleSheet("QLineEdit{padding-left:31px;"
//                        "background:rgba(255,255,255,0.2);"
//                        "border:3px solid rgba(0,255,0,1);"
//                        "border-radius:4px;"
//                        "color:rgba(255,255,255,0.5);}");
    this->setFocusPolicy(Qt::ClickFocus);
//    preferences-system-search-symbolic
}

void LeftUpSearchBox::paintEvent(QPaintEvent *event)
{
    QLineEdit::paintEvent(event);

    if (!this->hasFocus() && this->text().isEmpty()) {
        // show icon and tips when the focus is not on searchbox
        QPainter painter(this);
        painter.setOpacity(0.5);

        QFont curFont = painter.font(); //QGuiApplication::font();
        curFont.setPixelSize(14); //font.setPointSize(14);
        QPen pen(Qt::white);
        painter.setPen(pen);
        painter.setFont(curFont);

        bool noChinese=(QLocale::system().name() != "zh_CN");

        //2020.12.22调整右上角搜索框里的间距
//        QRect iconRect(QPoint(40-13*noChinese, 7), m_searchPixmap.size()/qApp->devicePixelRatio());
        QRect iconRect(QPoint(40-13*noChinese, 7), m_searchPixmap.size());
        painter.drawPixmap(iconRect, m_searchPixmap); //add icon in searchbox

        QRect textRect(62-13*noChinese, 3, 43, 28);//2020.12.22调整右上角搜索框里的间距
        painter.drawText(textRect, m_searchText); //add text in searchbox
    } else {
        // show cursor when the focus is on searchbox, and set the icon left
        QPainter painter(this);
        painter.setOpacity(0.5);

//        QRect iconRect(QPoint(9, 6), m_searchPixmap.size()/qApp->devicePixelRatio());
        QRect iconRect(QPoint(9, 6), m_searchPixmap.size());
        painter.drawPixmap(iconRect, m_searchPixmap);
    }
}//主题适配
void LeftUpSearchBox::ThemeLeftUpSearchBox(QString str)
{
    if("ukui-dark" == str || "ukui-black" == str)
    {
        // 改搜索框属性
//        this->setStyleSheet("QLineEdit{padding-left:31px;background:rgba(0,0,0,0.2);border-radius:4px;color:rgba(255,255,255,0.5);border:1px solid rgba(0,0,255,1)}");
        this->setStyleSheet("QLineEdit{padding-left:31px;background:rgba(0,0,0,0.2);border-radius:6px;color:rgba(255,255,255,1);}"
                            "QLineEdit:hover{padding-left:31px;background:rgba(0,0,0,0.2);border-radius:6px;color:rgba(255,255,255,1);border:2px solid rgba(55,144,250,1);}"
                            "QLineEdit:focus{padding-left:31px;background:rgba(0,0,0,0,0.2);border-radius:6px;color:rgba(255,255,255,1);border:2px solid rgba(55,144,250,1);}"
                            );
//        this->setPlaceholderText("搜索城市");
    }
    else if("ukui-default" == str || "ukui-white" == str || "ukui-light" == str)
    {
        // 改搜索框属性
//       this->setStyleSheet("QLineEdit{padding-left:31px;background:rgba(255,255,255,0.2);border-radius:4px;color:rgba(255,255,255,0.5);border:1px solid rgba(0,0,255,1)}");
        this->setStyleSheet("QLineEdit{padding-left:31px;background:rgba(255,255,255,0.2);border-radius:6px;color:rgba(255,255,255,1);}"
                            "QLineEdit:hover{padding-left:31px;background:rgba(255,255,255,0.2);border-radius:6px;color:rgba(255,255,255,0.5);border:2px solid rgba(55,144,250,1);}"
                            "QLineEdit:focus{padding-left:31px;background:rgba(255,255,255,0.2);border-radius:6px;color:rgba(255,255,255,0.5);border:2px solid rgba(55,144,250,1);}"
                            );
    }
}

bool LeftUpSearchBox::event(QEvent *e){
    if(e->type() == QEvent::FocusOut){
        isFocus = false;
        return QLineEdit::event(e);
    }else if(e->type() == QEvent::FocusIn)
    {
        isFocus = true;
        return QLineEdit::event(e);
    }

    if(e->type() == QEvent::KeyPress && isFocus){
        QKeyEvent *k = static_cast<QKeyEvent *>(e);
        switch(k->key()){
        case Qt::Key_Up:{
            emit lineEditKeyEvent("up");
            break;
        }
        case Qt::Key_Down:{
            emit lineEditKeyEvent("down");
            break;
        }
        case Qt::Key_Return:{
            emit lineEditKeyEvent("enter");
            break;
        }
        case Qt::Key_Enter:{
            emit lineEditKeyEvent("enter");
            break;
        }
        }
    }
    return QLineEdit::event(e);
}
