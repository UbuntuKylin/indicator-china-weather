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
