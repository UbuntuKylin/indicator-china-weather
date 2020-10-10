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

#include "cityaddsearchdelegate.h"
#include "data.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyledItemDelegate>
#include <QStyle>
#include <QEvent>
#include <QDebug>

CityAddSearchDelegate::CityAddSearchDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}

CityAddSearchDelegate::~CityAddSearchDelegate()
{

}

void CityAddSearchDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.isValid())
    {
        painter->save();

        QVariant variant = index.data(Qt::UserRole);
        ItemData data = variant.value<ItemData>();

        //draw a item in ui
        QStyleOptionViewItem viewOption(option);

        //set position and size of rect
        QRectF rect;
        rect.setX(option.rect.x());
        rect.setY(option.rect.y());
        rect.setWidth( option.rect.width()-1);
        rect.setHeight(option.rect.height()-1);

        //use QPainterPath to paint a rect with boder-radius
        const qreal radius = 4;
        QPainterPath path;
        path.moveTo(rect.topRight() - QPointF(radius, 0));
        path.lineTo(rect.topLeft() + QPointF(radius, 0));
        path.quadTo(rect.topLeft(), rect.topLeft() + QPointF(0, radius));
        path.lineTo(rect.bottomLeft() + QPointF(0, -radius));
        path.quadTo(rect.bottomLeft(), rect.bottomLeft() + QPointF(radius, 0));
        path.lineTo(rect.bottomRight() - QPointF(radius, 0));
        path.quadTo(rect.bottomRight(), rect.bottomRight() + QPointF(0, -radius));
        path.lineTo(rect.topRight() + QPointF(0, radius));
        path.quadTo(rect.topRight(), rect.topRight() + QPointF(-radius, -0));

        //handle different event type
        if(option.state.testFlag(QStyle::State_Selected)) {
            //painter->setPen(QPen(Qt::blue));
            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(61,107,229));
            painter->setOpacity(1);
            painter->drawPath(path);
        } else if(option.state.testFlag(QStyle::State_MouseOver)) {
            //painter->setPen(QPen(Qt::green));
            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(238,242,253));
            painter->setOpacity(1);
            painter->drawPath(path);
        } else {
            //painter->setPen(QPen(Qt::gray));
            painter->setPen(Qt::NoPen);
            painter->setBrush(Qt::NoBrush);
            painter->drawPath(path);
        }

        //set the position of the text
        QRect NameRect = QRect(rect.left()+8, rect.top(), rect.width()-30, 26);
        QRect telRect = QRect(rect.left()+8, rect.bottom()-22, rect.width()-10, 20);


        //painter->setPen(QPen(Qt::black));
        painter->setPen(QPen(QColor(68,68,68)));
        painter->setFont(QFont("", 13));
        painter->setOpacity(1);
        painter->drawText(NameRect,Qt::AlignLeft,data.cityName);

        //painter->setPen(QPen(Qt::black));
        painter->setPen(QPen(QColor(136,136,136)));
        painter->setFont(QFont("", 10));
        painter->setOpacity(1);
        painter->drawText(telRect,Qt::AlignLeft,data.cityProvince);

        painter->restore();
    }
}

QSize CityAddSearchDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(468, 45);
}
