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

#include "weathermanager.h"
#include "weatherworker.h"
#include "data.h"

#include <QDebug>
#include <QThread>
#include <QFile>
#include <QTimer>

WeatherManager::WeatherManager(QObject *parent) : QObject(parent)
{
    m_weatherWorker = new WeatherWorker();

    connect(m_weatherWorker, &WeatherWorker::nofityNetworkStatus, this, &WeatherManager::nofityNetworkStatus);
}

WeatherManager::~WeatherManager()
{

}

void WeatherManager::startTestNetwork()
{
    emit m_weatherWorker->requestTestNetwork();
}
