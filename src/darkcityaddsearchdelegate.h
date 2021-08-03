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

#ifndef DARKCITYADDSEARCHDELEGATE_H
#define DARKCITYADDSEARCHDELEGATE_H

#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QStandardItemModel>

class DarkCityAddSearchDelegate : public QStyledItemDelegate
{
    Q_OBJECT
signals:

public:
    explicit DarkCityAddSearchDelegate(QObject *parent = 0);
    ~DarkCityAddSearchDelegate();

    // set the drop down list style of the searchview widget.
    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;

    // set the size of drop down item of the searchview widget.
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:

};


#endif // DARKCITYADDSEARCHDELEGATE_H
