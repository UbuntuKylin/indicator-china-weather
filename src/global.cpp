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

#include "global.h"
#include "preferences.h"

#include <QApplication>
#include <QSettings>
#include <QFile>
#include <QDir>
#include <QDebug>

QSettings *Global::m_settings = 0;
Preferences *Global::m_preferences = 0;

using namespace Global;

void Global::global_init()
{
    QString filename = QDir::homePath() + "/.config/kylin-weather/kylin-weather.ini";
    m_settings = new QSettings(filename, QSettings::IniFormat);
    m_settings->setIniCodec("UTF-8");

    m_preferences = new Preferences;
}

void Global::global_end()
{
    delete m_preferences;
    m_preferences = 0;

    delete m_settings;
    m_settings = 0;
}
