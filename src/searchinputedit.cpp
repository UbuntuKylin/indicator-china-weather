/*
 * Copyright (C) 2013 ~ 2019 National University of Defense Technology(NUDT) & Tianjin Kylin Ltd.
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

#include "searchinputedit.h"

#include <QPainter>
#include <QGuiApplication>
#include <QDebug>

SearchInputEdit::SearchInputEdit(QWidget* parent)
    :QLineEdit(parent)
    , m_searchText(tr("Please input the city to search"))
    , m_searchPixmap(QPixmap(":/res/search.png"))
{
    this->setStyleSheet("QLineEdit{border:1px solid white;background-color:#3892eb;color:#ffffff;font-size:12px}");
    this->setFocusPolicy(Qt::ClickFocus);
    this->setFocus();
}

void SearchInputEdit::paintEvent(QPaintEvent *event)
{
    QLineEdit::paintEvent(event);
    //QPainter painter(this);
    //painter.setRenderHint(QPainter::Antialiasing);

    if (!this->hasFocus() && this->text().isEmpty()) {
        QRect rect = this->rect();

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setOpacity(0.6);
        painter.fillRect(this->rect(), QColor(56,146,235));//#3892eb

        QFont curFont = painter.font();//QGuiApplication::font();
        curFont.setPixelSize(12);//font.setPointSize(12);
        QFontMetrics fm(curFont);
        QPen pen(Qt::white);
        painter.setPen(pen);
        painter.setFont(curFont);

        int textWidth = fm.width(m_searchText);
        int iconWidth = m_searchPixmap.width();

        QRect textRect(5, 0, textWidth, rect.height());
        painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, m_searchText);

        QRect iconRect(QPoint(rect.width() - iconWidth/qApp->devicePixelRatio() - 5, rect.height()/2 - m_searchPixmap.height()/qApp->devicePixelRatio()/2), m_searchPixmap.size()/qApp->devicePixelRatio());
        painter.drawPixmap(iconRect, m_searchPixmap);
    }
    /*else {
        QRect rect = this->rect();
        painter.setOpacity(1);
        painter.fillRect(this->rect(), QColor(56,146,235));//#3892eb

        QFont curFont = painter.font();//QGuiApplication::font();
        curFont.setPixelSize(12);//font.setPointSize(12);
        QFontMetrics fm(curFont);
        QPen pen(Qt::white);
        painter.setPen(pen);
        painter.setFont(curFont);

        int textWidth = fm.width(this->text());
        QRect textRect(5, 0, textWidth, rect.height());
        painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, this->text());
    }*/
}
