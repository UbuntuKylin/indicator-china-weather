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

#include "searchdelegate.h"

#include <QPainter>
#include <QTextOption>

SearchDelegate::SearchDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}

SearchDelegate::~SearchDelegate()
{

}

void SearchDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString cityName = index.data(Qt::DisplayRole).toString();
    QRect rect(option.rect.adjusted(0, 0, 0, 0) );
    painter->fillRect(rect, QColor::fromRgbF(1, 1, 1, 0.15));

    QFont font = painter->font();
    font.setPixelSize(12);

    QTextOption textOption;
    textOption.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    painter->setPen(QColor("#808080"));
    painter->setFont(font);
    rect = rect.adjusted(15, 0, 15, 0);
    painter->drawText(rect, cityName, textOption);
}

QSize SearchDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
//    Q_UNUSED(option)

//    QVariant value = index.data(Qt::SizeHintRole);
//    if(value.isValid())
//        return qvariant_cast<QSize>(value);

    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(32);

    return size;
}
