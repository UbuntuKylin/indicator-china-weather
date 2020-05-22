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

#ifndef LOCATIONWORKER_H
#define LOCATIONWORKER_H

#include <QList>
#include <QObject>
#include <QThread>

#include "data.h"

class LocationWorker;

class WorkerThread : public QThread
{
    Q_OBJECT

public:
    explicit WorkerThread(LocationWorker *parent);
    ~WorkerThread() {}

    //从文件china-city-list.csv中查找子串，并将城市数据保存在LocationData结构体中
    void run() Q_DECL_OVERRIDE;
};


class LocationWorker : public QObject
{
    Q_OBJECT
public:
    explicit LocationWorker(QObject *parent = 0);
    ~LocationWorker();

    //将匹配到的每个城市加入一个列表中
    QList<LocationData> exactMatchCity(const QString &inputText) const;

private:
    friend class WorkerThread;
    QList<LocationData> m_locatonList;
    WorkerThread *m_workerThread = nullptr;
};

#endif // LOCATIONWORKER_H
