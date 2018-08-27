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

#ifndef CITYITEMWIDGET_H
#define CITYITEMWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "data.h"

class CityItemWidget : public QWidget
{
    Q_OBJECT

public:
    CityItemWidget(CitySettingData info, QWidget *parent=0);

//    QListWidgetItem* getCityItem();

    QString getCityId();

    void setItemHovered();
    void unsetItemHovered();

signals:
    void requestDeleteCity(const QString &id);
    void enter();

protected:
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    bool m_mouseHover = false;
//    QListWidgetItem *item = nullptr;

    QString m_id;
    CitySettingData m_data;

    QLabel *m_iconLabel = nullptr;
    QLabel *m_cityLabel = nullptr;
    QLabel *m_temperatureLabel = nullptr;
    QPushButton *m_defaultBtn = nullptr;
    QPushButton *m_deleteBtn = nullptr;
    QHBoxLayout *m_layout = nullptr;
};

#endif // CITYITEMWIDGET_H
