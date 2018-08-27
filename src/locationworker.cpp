/*
 * Copyright (C) 2013 ~ 2018 National University of Defense Technology(NUDT) & Tianjin Kylin Ltd.
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

#include "locationworker.h"

#include <QDebug>
#include <QFile>

WorkerThread::WorkerThread(LocationWorker *parent) :
    QThread(parent)
{

}

void WorkerThread::run()
{
    LocationWorker *m_worker = qobject_cast<LocationWorker*>(parent());
    if (!m_worker)
        return;

    //TODO: read from DB, now test reading from txt file
    QFile file(":/data/data/locations.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString line = file.readLine();
        line = line.replace("\n", "");//北京,北京,北京:101010100:beijing
        while (!line.isEmpty()) {
            QStringList resultList = line.split(":");
            LocationData data;
            QString city = resultList.at(0);//北京,北京,北京
            data.id = resultList.at(1);//101010100
            data.pinyin = resultList.at(2);//beijing

            resultList.clear();
            resultList = city.split(",");
            data.province = resultList.at(0);
            data.city = resultList.at(1);
            data.county = resultList.at(1);
            data.country = "China";
            m_worker->m_locatonList << data;

            line = file.readLine();
            line = line.replace("\n", "");
        }
        file.close();
    }
}

//--------------------------------------------------------------
LocationWorker::LocationWorker(QObject *parent)
    : QObject(parent),
      m_workerThread(new WorkerThread(this))
{
    m_workerThread->start();
}

LocationWorker::~LocationWorker()
{
    m_workerThread->deleteLater();
}

QList<LocationData> LocationWorker::exactMatchCity(const QString &inputText) const
{
    QList<LocationData> searchedList;

    for (const LocationData line : m_locatonList) {
        if (line.id == inputText || line.city.contains(inputText) || line.county.contains(inputText) || line.pinyin.contains(inputText)) {
            searchedList.append(line);
        }
    }

    return searchedList;
}
