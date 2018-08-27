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

#ifndef CITYWIDGET_H
#define CITYWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>

class CityListWidget;
class CitySettingData;
class QPushButton;

class CityWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CityWidget(QWidget *parent = 0);
    ~CityWidget();

    void loadCityItems();

public slots:
    void addCityItem(const CitySettingData &info);
    void removeCityItemById(const QString &id);
//    void onMouseEnter();

signals:
    void requestAddCity();
    void requestRemoveCityFromMenu(const QString &name);

private:
    bool event(QEvent *event);

private:
    CityListWidget *m_cityListWidget = nullptr;
    QPushButton *m_addBtn = nullptr;
    QVBoxLayout *m_layout = nullptr;
    QList<CitySettingData> m_dataList;
    QScrollArea *m_scrollArea = nullptr;
    QWidget *m_widget = nullptr;
};

#endif // CITYWIDGET_H
