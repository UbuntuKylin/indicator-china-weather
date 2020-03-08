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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QSystemTrayIcon>

#include "menuactiongroup.h"
#include "data.h"

class WeatherManager;
class TitleBar;
class ContentWidget;
class SettingDialog;
class PromptWidget;
class MaskWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    friend class DbusAdaptor;
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initMenuAndTray();
    void createSettingDialog();
    void resetWeatherBackgroud(const QString &imgPath);
    void refreshCityActions();
    void refreshTrayMenuWeather(const ObserveWeather &data);
    void startGetWeather();
    void startTrayFlashing(const QString &msg, bool restart);
    void stopTrayFlashing();
    void setOpacity(double opacity);
    void movePosition();

public slots:
    void changeTrayIcon();
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void showSettingDialog();
    void applySettings();
    void updateTimeTip();

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;

private:
    WeatherManager* m_weatherManager = nullptr;
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
    PromptWidget *m_hintWidget = nullptr;
    PromptWidget *m_movieWidget = nullptr;
    QTimer *m_trayTimer = nullptr;
    QTimer *m_tipTimer = nullptr;
    QTimer *m_autoRefreshTimer = nullptr;
    int m_actualizationTime;
    QString m_updateTimeStr;
    QString m_currentDesktop;
    MaskWidget *m_maskWidget = nullptr;
//    bool dataHadRefreshed;
};

#endif // MAINWINDOW_H
