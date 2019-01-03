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

#include "mainwindow.h"
#include "titlebar.h"
#include "contentwidget.h"
#include "settingdialog.h"
#include "aboutdialog.h"
#include "promptwidget.h"
#include "weatherworker.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QFileInfo>
#include <QMouseEvent>
#include <QMenu>
#include <QScreen>
#include <QShortcut>
#include <QDebug>
#include <QTimer>
#include <math.h>

#include "preferences.h"
#include "global.h"
using namespace Global;

inline QString convertCodeToBackgroud(int code)
{
    if (code == 100 || code == 900) {
        return ":/res/background/weather-clear.png";
    }
    else if (code <= 103 && code >= 101) {
        return ":/res/background/weather-few-clouds.png";
    }
    else if (code == 104 || code == 901) {
        return ":/res/background/weather-overcast.png";
    }
    else if (code <= 204 && code >= 200) {
        return ":/res/background/weather-clear.png";
    }
    else if (code <= 213 && code >= 205) {
        return ":/res/background/weather-overcast.png";
    }
    else if (code <= 399 && code >= 300) {
        return ":/res/background/weather-rain.png";
    }
    else if (code <= 499 && code >= 400) {
        return ":/res/background/weather-snow.png";
    }
    else if (code <= 502 && code >= 500) {
        return ":/res/background/weather-fog.png";
    }
    else if (code <= 508 && code >= 503) {
        return ":/res/background/weather-sandstorm.png";
    }
    else if (code <= 515 && code >= 509) {
        return ":/res/background/weather-fog.png";
    }
    else {
        return ":/res/background/weather-clear.png";
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
//    , m_mousePressed(false)
    , m_centralWidget(new QWidget(this))
    , m_titleBar(new TitleBar(this))
    , m_contentWidget(new ContentWidget(this))
    , m_pingbackTimer(new QTimer(this))
    , m_tipTimer(new QTimer(this))
    , m_autoRefreshTimer(new QTimer(this))
    , m_actualizationTime(0)
    , m_weatherWorker(new WeatherWorker(this))
{
    this->setFixedSize(355, 552);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint);
//    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setFocusPolicy(Qt::ClickFocus);//Qt::StrongFocus
    //this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);//ubuntu 16.04 可能需要加上Qt::WindowMinimizeButtonHint，否则showMinimized无效
    this->setWindowTitle(tr("Kylin Weather"));
    this->setWindowIcon(QIcon(":/res/indicator-china-weather.png"));
    this->setStyleSheet("QMainWindow{color:white;background-image:url(':/res/background/weather-clear.png');background-repeat:no-repeat;}");

    global_init();

    m_updateTimeStr = QString(tr("Refresh time"));

    m_layout = new QVBoxLayout(m_centralWidget);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);

    m_layout->addWidget(m_titleBar);//height:32
    m_layout->addWidget(m_contentWidget);//height:520
    this->setCentralWidget(m_centralWidget);

    this->initMenuAndTray();
    m_titleBar->setCityName(m_preferences->m_currentCity);

    m_currentDesktop = qgetenv("XDG_CURRENT_DESKTOP");
    if (m_currentDesktop.isEmpty()) {
        m_currentDesktop = qgetenv("XDG_SESSION_DESKTOP");
    }
    /*if (m_currentDesktop.isEmpty())
        this->moveTopRight();
    else {
        if (m_currentDesktop.toLower() == "ukui") {
            this->moveBottomRight();
        }
        else {
            this->moveTopRight();
        }
    }*/

    if (m_preferences->weather.cond_code.contains(QChar('n'))) {
        this->setStyleSheet("QMainWindow{color:white;background-image:url(':/res/background/weather-clear-night.png');background-repeat:no-repeat;}");
        m_contentWidget->setNightStyleSheets();
        m_titleBar->setNightStyleSheets();
    }
    else {
        QString styleSheetStr = QString("QMainWindow{color:white;background-image:url('%1');background-repeat:no-repeat;}").arg(convertCodeToBackgroud(m_preferences->weather.cond_code.toInt()));
        this->setStyleSheet(styleSheetStr);
        m_contentWidget->setDayStyleSheets();
        m_titleBar->setDayStyleSheets();
    }

    connect(m_titleBar, &TitleBar::requestShowSettingDialog, this, [=] {
        this->showSettingDialog();
    });

    m_hintWidget = new PromptWidget(this);
    m_hintWidget->setIconAndText(":/res/network_warn.png", tr("Network not connected"));
    m_hintWidget->move((this->width() - m_hintWidget->width())/2, (this->height() - m_hintWidget->height())/2);
    m_hintWidget->setVisible(false);

    m_movieWidget = new PromptWidget(tr("Getting data"), this, ":/res/link.gif", true);
    m_movieWidget->move((this->width() - m_hintWidget->width())/2, (this->height() - m_hintWidget->height())/2);
    m_movieWidget->setVisible(false);

    this->createSettingDialog();

    if (!m_weatherWorker->isNetWorkSettingsGood()) {
        m_contentWidget->setNetworkErrorPages();
        m_autoRefreshTimer->stop();
    }
    else {
        m_weatherWorker->netWorkOnlineOrNot();//ping www.baidu.com
//        m_weatherWorker->requestPostHostInfoToWeatherServer();
//        this->startGetWeather();
    }

    connect(m_contentWidget, &ContentWidget::requestRetryWeather, this, [=] {
        m_weatherWorker->requestPostHostInfoToWeatherServer();
        this->startGetWeather();
    });


    connect(m_weatherWorker, &WeatherWorker::nofityNetworkStatus, this, [=] (const QString &status) {
        if (status == "OK") {
            m_weatherWorker->requestPostHostInfoToWeatherServer();
            this->startGetWeather();
        }
        else {
            m_hintWidget->setIconAndText(":/res/network_warn.png", status);
            m_contentWidget->setNetworkErrorPages();
        }
    });

    connect(m_weatherWorker, &WeatherWorker::responseFailure, this, [=] (int code) {
        m_autoRefreshTimer->stop();
        m_movieWidget->setVisible(false);
        m_hintWidget->setVisible(true);
        if (code == 0) {
            m_hintWidget->setIconAndText(":/res/network_warn.png", tr("Incorrect access address"));
        }
        else {
            m_hintWidget->setIconAndText(":/res/network_warn.png", QString(tr("Network error code:%1")).arg(QString::number(code)));
        }
        m_contentWidget->setNetworkErrorPages();
    });

    connect(m_weatherWorker, &WeatherWorker::requestDiplayServerNotify, this, [=] (const QString &notifyInfo) {
        if (!notifyInfo.isEmpty() && m_preferences->m_serverNotify)
            m_contentWidget->showServerNotifyInfo(notifyInfo);
    });

    connect(m_weatherWorker, &WeatherWorker::observeDataRefreshed, this, [=] (const ObserveWeather &data) {
        m_autoRefreshTimer->start(m_preferences->m_updateFrequency * 1000 * 60);
        if (m_preferences->m_currentCity.isEmpty()) {
            m_preferences->m_currentCity = data.city;
        }
        m_movieWidget->setVisible(false);
        m_titleBar->setCityName(data.city);
        m_contentWidget->refreshObserveUI(data);
        this->refreshTrayMenuWeather(data);
        QString condCodeStr = data.cond_code;
        if (!condCodeStr.isEmpty()) {
            if (condCodeStr.contains(QChar('n'))) {
                this->setStyleSheet("QMainWindow{color:white;background-image:url(':/res/background/weather-clear-night.png');background-repeat:no-repeat;}");
                m_contentWidget->setNightStyleSheets();
                m_titleBar->setNightStyleSheets();
            }
            else {
                QString styleSheetStr = QString("QMainWindow{color:white;background-image:url('%1');background-repeat:no-repeat;}").arg(convertCodeToBackgroud(condCodeStr.toInt()));
                this->setStyleSheet(styleSheetStr);
                m_contentWidget->setDayStyleSheets();
                m_titleBar->setDayStyleSheets();
            }
        }
    });

    connect(m_weatherWorker, &WeatherWorker::forecastDataRefreshed, this, [=] (const QList<ForecastWeather> &datas, const LifeStyle &data) {
        if (!m_autoRefreshTimer->isActive()) {
            m_autoRefreshTimer->start(m_preferences->m_updateFrequency * 1000 * 60);
        }

        m_movieWidget->setVisible(false);

        int len = datas.size();
        if (len > 3) {
            len = 3;
        }
        for (int i = 0; i < len; ++i) {
            m_contentWidget->refreshForecastUI(datas[i], i);
        }

        m_contentWidget->refreshLifestyleUI(data);
    });

    m_tipTimer->setInterval(60*1000);
    m_tipTimer->setSingleShot(false);
    connect(m_tipTimer, &QTimer::timeout, this, static_cast<void (MainWindow::*)()>(&MainWindow::updateTimeTip));

    connect(m_autoRefreshTimer, &QTimer::timeout, this, [=] {
        this->startGetWeather();
    });


    double value = m_preferences->m_opacity*0.01;
    if (value < 0.6) {
        value = 0.60;
        m_preferences->m_opacity = 60;
    }
    this->setOpacity(value);

    this->setVisible(false);
}

MainWindow::~MainWindow()
{
    if (m_tipTimer->isActive()) {
        m_tipTimer->stop();
        delete m_tipTimer;
    }

    global_end();
}

void MainWindow::updateTimeTip()
{
    QDateTime time = QDateTime::currentDateTime();
    int timeIntValue = time.toTime_t();
    int ut = int((round(timeIntValue - m_actualizationTime)/60));
    if (ut == 0 || m_actualizationTime == 0) {
        m_updateTimeStr = QString(tr("Refresh time:Just updated"));
    }
    else {
        if (ut < 2) {
            m_updateTimeStr = QString(tr("Refresh time:%1 minute ago")).arg(QString::number(ut));
        }
        else {
            m_updateTimeStr = QString(tr("Refresh time:%1 minutes ago")).arg(QString::number(ut));
        }
    }

    if (QDateTime::currentDateTime().toTime_t()-m_actualizationTime > m_preferences->m_updateFrequency*1000*60) {
        m_actualizationTime = QDateTime::currentDateTime().toTime_t();
    }

    m_updateTimeAction->setText(m_updateTimeStr);

    m_weatherWorker->requestPingBackWeatherServer();
}

void MainWindow::setOpacity(double opacity)
{
    this->setWindowOpacity(opacity);
}

void MainWindow::startGetWeather()
{
    m_tipTimer->stop();
    m_movieWidget->setVisible(true);
    m_titleBar->setCityName(m_preferences->m_currentCity);
    m_weatherWorker->refreshObserveWeatherData(m_preferences->m_currentCityId);
    m_weatherWorker->refreshForecastWeatherData(m_preferences->m_currentCityId);
}

void MainWindow::initMenuAndTray()
{
    m_mainMenu = new QMenu(this);
    m_cityMenu = new QMenu(this);
    m_cityMenu->menuAction()->setText(tr("City"));
    m_cityActionGroup = new MenuActionGroup(this);
    connect(m_cityActionGroup, &MenuActionGroup::activated, this, [=] (int index) {
        QString cur_cityName = m_cityActionGroup->setCurrentCheckedIndex(index);
        if (m_preferences->citiesCount() > 0 && m_preferences->citiesCount() >= index) {
            m_cityMenu->menuAction()->setText(cur_cityName/*m_preferences->cityName(index)*/);

            m_preferences->setCurrentCityIdAndName(cur_cityName/*index*/);

            m_setttingDialog->refreshCityList(m_preferences->m_currentCity);
            this->startGetWeather();
        }
    });
    this->refreshCityActions();

    m_mainMenu->addMenu(m_cityMenu);
    //QAction *m_switchAciton = m_mainMenu->addAction(tr("Switch"));
    m_mainMenu->addSeparator();

    m_weatherAction = new QAction("N/A",this);
    m_temperatureAction = new QAction("N/A",this);
    m_sdAction = new QAction("N/A",this);
    m_aqiAction = new QAction("N/A",this);
    m_releaseTimeAction = new QAction(tr("Release time"),this);
    m_updateTimeAction = new QAction(m_updateTimeStr,this);
    m_mainMenu->addAction(m_weatherAction);
    m_mainMenu->addAction(m_temperatureAction);
    m_mainMenu->addAction(m_sdAction);
    m_mainMenu->addAction(m_aqiAction);
    m_mainMenu->addAction(m_releaseTimeAction);
    m_mainMenu->addAction(m_updateTimeAction);

    QAction *m_forecastAction = m_mainMenu->addAction(tr("Weather Forecast"));
    connect(m_forecastAction, &QAction::triggered, this, [=] {
        if (!this->isVisible()) {
            /*if (m_currentDesktop.isEmpty())
                this->moveTopRight();
            else {
                if (m_currentDesktop.toLower() == "ukui") {
                    this->moveBottomRight();
                }
                else {
                    this->moveTopRight();
                }
            }*/
            this->movePosition();
            //this->showNormal();
        }
        else {
            this->hide();
        }
    });

    m_mainMenu->addSeparator();
    QAction *m_settingAction = m_mainMenu->addAction(tr("Settings"));
    m_settingAction->setIcon(QIcon(":/res/prefs.png"));
    QAction *m_aboutAction = m_mainMenu->addAction(tr("Kylin Weather - About"));
    m_aboutAction->setIcon(QIcon(":/res/about_normal.png"));
    QAction *m_quitAction = m_mainMenu->addAction(tr("Exit"));
    m_quitAction->setIcon(QIcon(":/res/quit_normal.png"));

    //for test change weather background
    /*m_isDN = true;
    connect(m_switchAciton, &QAction::triggered, this, [=] {
        if (m_isDN) {
            m_isDN = false;
            QString currentBg = QString("QMainWindow{color:white;background-image:url('%1');background-repeat:no-repeat;}").arg(":/res/background/weather-clear-night.png");
            this->setStyleSheet(currentBg);
            m_contentWidget->setNightStyleSheets();
            m_titleBar->setNightStyleSheets();
        }
        else {
            m_isDN = true;
            QString currentBg = QString("QMainWindow{color:white;background-image:url('%1');background-repeat:no-repeat;}").arg(":/res/background/weather-clear.png");
            this->setStyleSheet(currentBg);
            m_contentWidget->setDayStyleSheets();
            m_titleBar->setDayStyleSheets();
        }
    });*/

    connect(m_settingAction, &QAction::triggered, this, [=] {
        this->showSettingDialog();
    });
    connect(m_aboutAction, &QAction::triggered, this, [=] {
        AboutDialog dlg;
        dlg.exec();
    });
    connect(m_quitAction, &QAction::triggered, qApp, &QApplication::quit);

    m_systemTray = new QSystemTrayIcon(this);
    m_systemTray->setToolTip(QString(tr("Kylin Weather")));
    m_systemTray->setIcon(QIcon(":/res/indicator-china-weather.png"));
    connect(m_systemTray,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
    m_systemTray->show();
    m_systemTray->setContextMenu(m_mainMenu);

    QShortcut *m_quitShortCut = new QShortcut(QKeySequence("Ctrl+Q"), this);
    connect(m_quitShortCut, SIGNAL(activated()), qApp, SLOT(quit()));
}

void MainWindow::showSettingDialog()
{
    int w_x, w_y;
    w_x = (width() - m_setttingDialog->width()) / 2 + mapToGlobal(QPoint(0, 0)).x();
    w_y = (window()->height() - m_setttingDialog->height()) / 2 + mapToGlobal(QPoint(0, 0)).y();

    if (w_x + m_setttingDialog->width() > qApp->primaryScreen()->geometry().width()) {
        w_x = qApp->primaryScreen()->geometry().width() - m_setttingDialog->width();//m_setttingDialog->move(qApp->primaryScreen()->geometry().width() - m_setttingDialog->width(), y);
    }
    if (w_y + m_setttingDialog->height() > qApp->primaryScreen()->availableGeometry().width()) {
        w_y = qApp->primaryScreen()->availableGeometry().width() - m_setttingDialog->height();
    }

    m_setttingDialog->move(w_x, w_y);
//    m_setttingDialog->move((width() - m_setttingDialog->width()) / 2 + mapToGlobal(QPoint(0, 0)).x(),
//                               (window()->height() - m_setttingDialog->height()) / 2 + mapToGlobal(QPoint(0, 0)).y());
    m_setttingDialog->show();
}

void MainWindow::createSettingDialog()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_setttingDialog = new SettingDialog;
    m_setttingDialog->setModal(false);
    connect(m_setttingDialog, SIGNAL(applied()), this, SLOT(applySettings()));
    connect(m_setttingDialog, &SettingDialog::requestRefreshCityMenu, this, [this] (bool removedDefault) {
        this->refreshCityActions();

        if (removedDefault) {//刪除了默认城市后，重新设置了列表中第一个城市为默认城市后，从服务端获取该城市的天气
            this->startGetWeather();
        }
    });
    connect(m_setttingDialog, &SettingDialog::requestRefreshWeatherById, this, [this] (const QString &id) {
        m_preferences->resetCurrentCityNameById(id);
        this->refreshCityActions();
        this->startGetWeather();
    });
    connect(m_setttingDialog, &SettingDialog::requestChangeOpacity, this, [this] (int opcatity) {
        double value = opcatity*0.01;
        if (value < 0.6) {
            value = 0.60;
            m_preferences->m_opacity = 60;
        }
        this->setOpacity(value);
    });



    /*connect(m_setttingDialog, &SettingDialog::requestSetDefaultCity, this, [=] {
        m_preferences->setDefaultCity();
        m_setttingDialog->refreshCityList(m_preferences->m_currentCityId);
        this->startGetWeather();
    });*/

    QApplication::restoreOverrideCursor();
}

void MainWindow::refreshCityActions()
{
    // clear orig action list
    m_cityActionGroup->clearAllActions();

    // add new action list
    int i = 0;
    int currentIndex = 0;
    foreach (QString city, m_preferences->getCitiesList()) {
//        m_cityMenu->addAction(city);
        if (city == m_preferences->m_currentCity) {
            currentIndex = i;
        }

        MenuActionGroupItem *cityAction = new MenuActionGroupItem(this, m_cityActionGroup, i);
        cityAction->setActionText(city);
        i++;
    }
    m_cityMenu->addActions(m_cityActionGroup->actions());
    m_cityMenu->menuAction()->setText(m_preferences->m_currentCity);
    m_cityActionGroup->setCurrentCheckedIndex(currentIndex);
}

void MainWindow::refreshTrayMenuWeather(const ObserveWeather &data)
{
    m_systemTray->setIcon(QIcon(QString(":/res/weather_icons/white/%1.png").arg(data.cond_code)));
    m_weatherAction->setText(data.cond_txt);
    m_temperatureAction->setText(QString(tr("Temperature:%1˚C")).arg(data.tmp));
    m_sdAction->setText(QString(tr("Relative humidity:%1")).arg(data.hum));
    if (data.air.isEmpty() || data.air.contains("Unknown")) {
        m_aqiAction->setText(QString(tr("Air quality:%1")).arg(QString(tr("Unknown"))));
    }
    else {
        m_aqiAction->setText(QString(tr("Air quality:%1")).arg(data.air));
    }
    m_releaseTimeAction->setText(QString(tr("Release time:%1")).arg(data.updatetime));

    m_actualizationTime = 0;
    this->updateTimeTip();
    m_tipTimer->start();
}

void MainWindow::applySettings()
{

}

void MainWindow::resetWeatherBackgroud(const QString &imgPath)
{
    QString weatherBg = imgPath;
    if(!QFileInfo(weatherBg).exists())
        weatherBg = ":/res/background/weather-clear.png";
    QString currentBg = QString("QMainWindow{color:white;background-image:url('%1');background-repeat:no-repeat;}").arg(weatherBg);
    this->setStyleSheet(currentBg);
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason) {
    /*case QSystemTrayIcon::DoubleClick:
    {
        QRect rect = m_systemTray->geometry();
        QMenu *currentMenu = m_systemTray->contextMenu();
        if (currentMenu->isHidden()) {
            currentMenu->popup(QPoint(rect.x()+8, rect.y()));
        }
    }
        break;*/
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::MiddleClick:
        if (this->isVisible()) {
            this->setVisible(false);
        }
        else {
            /*if (m_currentDesktop.isEmpty())
                this->moveTopRight();
            else {
                if (m_currentDesktop.toLower() == "ukui") {
                    this->moveBottomRight();
                }
                else {
                    this->moveTopRight();
                }
            }*/
            this->movePosition();
        }
        break;
    default:
        break;
    }
}

void MainWindow::movePosition()
{
    QPoint pos = QCursor::pos();
    /*qDebug() << "pos=" << pos;//QPoint(1720,236)
    qDebug() << "mapFromGlobal(pos)=" << mapFromGlobal(pos);//QPoint(1709,249)
    QPoint mousePos = mapToParent(mapFromGlobal(pos));
    qDebug() << "mousePos=" << mousePos;//QPoint(1709,249)*/
    QRect primaryGeometry = qApp->primaryScreen()->availableGeometry();
    qDebug() << "primaryGeometry=" << primaryGeometry;//QRect(65,24 1855x1056)
    qDebug() << "geometry=" << qApp->primaryScreen()->geometry();//QRect(0,0 1920x1080)
    /*for (QScreen *screen : qApp->screens()) {
        if (screen->geometry().contains(pos)) {
        }
    }*/
    if (primaryGeometry.contains(pos)) {
        this->move(primaryGeometry.x() + primaryGeometry.width() - this->width(), primaryGeometry.y());
        this->showNormal();//this->show();
        this->raise();
        this->activateWindow();
    }
}

void MainWindow::moveTopRight()
{
    /*QPoint pos = QCursor::pos();
    QRect primaryGeometry;
    for (QScreen *screen : qApp->screens()) {
        if (screen->geometry().contains(pos)) {
            primaryGeometry = screen->geometry();
        }
    }

    if (primaryGeometry.isEmpty()) {
        primaryGeometry = qApp->primaryScreen()->geometry();
    }

    this->move(primaryGeometry.x() + primaryGeometry.width() - this->width(), primaryGeometry.y());*/

    QRect primaryGeometry = qApp->primaryScreen()->availableGeometry();
    this->move(primaryGeometry.x() + primaryGeometry.width() - this->width(), primaryGeometry.y());
    this->showNormal();//this->show();
    this->raise();
    this->activateWindow();
}

void MainWindow::moveBottomRight()
{
    //QApplication::desktop()->availableGeometry();//桌面除去任务栏的区域
    //QApplication::desktop()->screenGeometry();//获取包括任务栏的区域
    /*QPoint pos = QCursor::pos();
    QRect primaryGeometry;
    for (QScreen *screen : qApp->screens()) {
//        if (screen->geometry().contains(pos)) {
//            primaryGeometry = screen->geometry();
//        }
        if (screen->geometry().contains(pos)) {//判断鼠标点击时，使用包括任务栏在内的整个屏幕区域
            primaryGeometry = screen->availableGeometry();//显示区域需要去掉任务栏的区域
        }
    }
    if (primaryGeometry.isEmpty()) {
        primaryGeometry = qApp->primaryScreen()->availableGeometry();//primaryGeometry = qApp->primaryScreen()->geometry();
    }*/


    /*QRect primaryGeometry = qApp->primaryScreen()->geometry();
    const qreal ratio = qApp->devicePixelRatio();
    const QScreen *screen;
    for (const auto *s : qApp->screens()) {
        const QRect &g(s->geometry());
        const QRect rect(g.topLeft()/ratio, g.size());
        if (rect.contains(primaryGeometry.center())) {
            screen = s;
            break;
        }
    }
    if  (screen) {
        const QRect screenRect = screen->geometry();
        primaryGeometry.moveTopLeft(screenRect.topLeft() + (primaryGeometry.topLeft() - screenRect.topLeft()) / screen->devicePixelRatio());
    }*/

    QRect primaryGeometry = qApp->primaryScreen()->availableGeometry();
    this->move(primaryGeometry.x() + primaryGeometry.width() - this->width(), primaryGeometry.height() - this->height());
    this->showNormal();//this->show();
    this->raise();
    this->activateWindow();
}

/*void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        this->m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        this->m_mousePressed = true;
    }

    if (m_hintWidget->isVisible())
        m_hintWidget->setVisible(false);

    QMainWindow::mousePressEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    this->m_mousePressed = false;

    QMainWindow::mouseReleaseEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (this->m_mousePressed) {
        move(event->globalPos() - this->m_dragPosition);
    }

    QMainWindow::mouseMoveEvent(event);
}

void MainWindow::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    //m_weatherWorker->refreshForecastWeatherData(m_preferences->m_currentCityId);
}*/

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        if (this->isVisible()) {
            this->setVisible(false);
        }
        event->accept();
    }

    QMainWindow::keyPressEvent(event);
}

void MainWindow::focusOutEvent(QFocusEvent *event)
{
    if (event->reason() == Qt::ActiveWindowFocusReason) {
        this->hide();
    }

    return QMainWindow::focusOutEvent(event);
}
