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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QSystemTrayIcon>

#include "menuactiongroup.h"

class TitleBar;
class ContentWidget;
class SettingDialog;
class WeatherWorker;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initMenuAndTray();
    void resetWeatherBackgroud(const QString &imgPath);
    void moveTopRight();
    void createSettingDialog();
    void refreshCityActions();

    void refreshUI();

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

public slots:
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void showSettingDialog();
    void applySettings();

private:
    QPoint m_dragPosition;
    bool m_mousePressed;

    QVBoxLayout *m_layout = nullptr;
    QWidget *m_centralWidget = nullptr;
    TitleBar *m_titleBar = nullptr;
    ContentWidget *m_contentWidget = nullptr;

    QMenu *m_mainMenu = nullptr;
    QMenu *m_cityMenu = nullptr;
    MenuActionGroup *m_cityActionGroup = nullptr;
    QAction *m_weatherAction = nullptr;
    QAction *m_temperatureAction = nullptr;
    QAction *m_sdAction = nullptr;
    QAction *m_aqiAction = nullptr;
    QAction *m_releaseTimeAction = nullptr;
    QAction *m_updateTimeAction = nullptr;
    QSystemTrayIcon *m_systemTray = nullptr;
    SettingDialog *m_setttingDialog = nullptr;

    //QStringList m_cityList;
    WeatherWorker *m_weatherWorker = nullptr;
};

#endif // MAINWINDOW_H
