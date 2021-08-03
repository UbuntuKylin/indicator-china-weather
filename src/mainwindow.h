/*
 * Copyright (C) 2020, KylinSoft Co., Ltd.
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

#define CHINAWEATHERDATA "org.china-weather-data.settings"
#define FITTHEMEWINDOW "org.ukui.style"

#include "mainwindow.h"
#include "leftupcitybtn.h"
#include "leftupsearchbox.h"
#include "leftupsearchview.h"
#include "leftupsearchdelegate.h"
#include "informationwidget.h"
#include "locationworker.h"
#include "weathermanager.h"
#include "promptwidget.h"
#include "data.h"
#include "citycollectionwidget.h"//需要

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <xcb/xcb.h>

#include <QMainWindow>
#include <QStandardPaths>
#include <QMouseEvent>
#include <QDebug>
#include <QButtonGroup>
#include <QObjectUserData>
#include <QSortFilterProxyModel>
#include <QScrollArea>
#include <QScrollBar>
#include <QWidget>
#include <QSystemTrayIcon>
#include <QRect>
#include <QScreen>
#include <QDesktopWidget>
#include <QTime>
#include <QTimer>
#include <QFileInfo>
#include <QLocale>
#include <QPainterPath>
#include <QMenu>
#include <QAction>
#include <QToolButton>

#include <QPushButton>
#include <QLabel>

#include <QGSettings>
#include "daemondbus.h"
#include "menumodule.h"
#include "xatom-helper.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void createTrayIcon();
    void handleIconClicked();
    void handleIconClickedSub();

    void onRefreshMainWindowWeather();
    void onHandelAbnormalSituation(QString abnormalText);

    void onSetForecastWeather(ForecastWeather m_forecastweather);
    void onSetObserveWeather(ObserveWeather m_observeweather);
    void onSetLifeStyle(LifeStyle m_lifestyle);

private slots:
    void on_btnMinimize_clicked();

    void on_btnCancel_clicked();

    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void closeActivated();

private:

    // 用户手册功能
    DaemonDbus *mDaemonIpcDbus;

    Ui::MainWindow *ui;
    QScrollArea *m_scrollarea = nullptr;
    QWidget *m_scrollwidget = nullptr;
    LeftUpCityBtn *m_leftupcitybtn = nullptr;
    LeftUpSearchBox *m_leftupsearchbox = nullptr;
    WeatherManager* m_weatherManager = nullptr;
    PromptWidget *m_hintWidget = nullptr;
    PromptWidget *m_movieWidget = nullptr;
    LeftUpSearchView *m_searchView = nullptr;
    LeftUpSearchDelegate *m_delegate = nullptr;
    QSortFilterProxyModel* m_proxyModel = nullptr;
    QStandardItemModel *m_model = nullptr;
    LocationWorker *m_locationWorker = nullptr;
    Information *m_information;
    QSystemTrayIcon *m_trayIcon = nullptr;
    QTimer *m_refreshweather;
    QMenu *m_mainMenu = nullptr;
    QAction *m_openAction;
    QAction *m_quitAction;

    //*****2020.12.19增加
    QPushButton *logoBtn;
    QLabel *logolb;

    QWidget *titleWid;
    QHBoxLayout *titleLayout;

    QPushButton *setBtn;
    QMenu *menu ;
//    AddCityAction *addCityAction;
    QAction *aboutAction;
    QList<QAction *> actions ;
    //*****2020.12.19增加
    menuModule *m_menu = nullptr;

    void judgeSystemLanguage();

    void onSearchBoxEdited();
    void searchCityName();

    void initControlQss();
    void initConnections();

    void setAbnormalMainWindow();

    QString convertCodeToBackgroud(int code);
    void convertCodeToTrayIcon(QString code);

    // 键盘响应事件
    void keyPressEvent(QKeyEvent *event);

//    void mousePressEvent(QMouseEvent *event);
//    void mouseReleaseEvent(QMouseEvent *event);
//    void mouseMoveEvent(QMouseEvent *event);

    bool event(QEvent *event);
    bool isPress;
    QPoint winPos;
    QPoint dragPos;

    // getstting初始化、值获取、 设置getsetting值
    void initGsetting();
    QString getCityList();
    void setCityList(QString str);
    void setThemeStyle();

    QGSettings  *m_pWeatherData= nullptr;
    QGSettings  *m_pThemeStyle= nullptr;
    QString firstGetCityList="";

    QString nowThemeStyle;

    QLabel *cityLabel;
    bool is_open_city_collect_widget = false;
    CityCollectionWidget *m_citycollectionwidget;
signals:
    void sendCurrentCityId(QString id);//发送到主界面更新主界面天气
    void requestShowCollCityWeather(); //显示收藏城市列表天气
    void requestSetCityWeather(QString weather_data); //发送出去显示主界面城市天气
    void updatecity();
    void requestSetCityName(QString cityName);//在搜索列表中选中一个城市后，左上角城市名需要更改



};

#endif // MAINWINDOW_H
