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

#include "leftupsearchmodel.h"

#include <QDebug>

LeftUpSearchModel::LeftUpSearchModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

LeftUpSearchModel::~LeftUpSearchModel()
{

}

int LeftUpSearchModel::rowCount(const QModelIndex &parent) const
{
    return m_locationDataList.length();
}

bool LeftUpSearchModel::insertColumns(int column, int count, const QModelIndex &)
{
    return true;
}

QVariant LeftUpSearchModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    const LocationData info = m_locationDataList.at(index.row());

    if (role == Qt::UserRole) {//for get LocationData from QList
        return QVariant(info.id);
    }

    if (role == Qt::ToolTipRole || role == Qt::WhatsThisRole) {
        return QVariant();
    }

    return QVariant(QString("%1, %2, %3").arg(info.city).arg(info.admin_district).arg(info.province));
}

QList<LocationData> LeftUpSearchModel::locationList() const
{
    return m_locationDataList;
}

void LeftUpSearchModel::setLocationData(const QList<LocationData> &results)
{
    beginResetModel();
    m_locationDataList = results;
    endResetModel();
}
