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

#ifndef FORECAST_WEATHER_WIDGET_H
#define FORECAST_WEATHER_WIDGET_H

#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include "weatherworker.h"

class ForecastItemWidget;
class IndexItemWidget;
class HSeparator;
class VSeparator;
class TextTip;
class TipModule;

class ForecastWeatherWidget : public QFrame
{
    Q_OBJECT

public:
    explicit ForecastWeatherWidget(WeatherWorker *weatherWorker, QFrame *parent = 0);
    ~ForecastWeatherWidget();

    void initForecastWidget();
    void initIndexTitleWidget();
    void initIndexWidget();
    void initWidgets();
    void refershLifeIndexGridLayout();

    void refreshForecastData(const ForecastWeather &data, int index);
    void refreshLifestyleData(const LifeStyle &data);

    void setDayStyleSheets();
    void setNightStyleSheets();

    TextTip *setTipWidget(QWidget *w, const QString &txt);

public slots:
    void showLifeStyleIndex(const QString &name);

protected:
    void leaveEvent(QEvent *e);

private:
    QVBoxLayout *m_layout = nullptr;
    QHBoxLayout *m_forecastlayout = nullptr;
    QVBoxLayout *m_indexTitlelayout = nullptr;
    QVBoxLayout *m_indexlayout = nullptr;
    QGridLayout *m_indexGridLayout = nullptr;
    QStringList m_lifeIndexList;
    QStringList m_lifeIndexIconList;
    QLabel *lifeTextLabel = nullptr;
    ForecastItemWidget *m_firstDay = nullptr;
    ForecastItemWidget *m_secondDay = nullptr;
    ForecastItemWidget *m_thirdDay = nullptr;
    WeatherWorker *m_weatherWorker = nullptr;
    VSeparator *m_leftVSeparator = nullptr;
    VSeparator *m_rightVSeparator = nullptr;
    HSeparator *m_topHseperator = nullptr;
    HSeparator *m_bottomHseperator = nullptr;
    QList<IndexItemWidget *> m_lifeItems;
    QList<TextTip *> m_tips;
    TipModule *m_tipModule = nullptr;
};

#endif // FORECAST_WEATHER_WIDGET_H
